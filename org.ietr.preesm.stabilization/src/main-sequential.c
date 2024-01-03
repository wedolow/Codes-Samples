#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "applicationParameters.h"
#ifdef DISPLAY
#include "yuvDisplay.h"
#endif
#include "yuvRead.h"
#include "yuvWrite.h"
#include "stabilization.h"
#include "clock.h"

// #define PREESM_VERBOSE
#ifdef PREESM_VERBOSE
#include <stdio.h>
#endif

#define FPS_INTERVAL 10

int preesmStopThreads = 0;

void processImage(const int width, const int height,
						const int blockWidth, const int blockHeight,
						const int maxDeltaX, const int maxDeltaY,
						const unsigned char * const frame, 
						const unsigned char * const previousFrame,
						coord * const motionVectors, coordf* dominatingMotionVector){
	// Compute motion vectors
	computeBlockMotionVectors(width, height,
								blockWidth, blockHeight,
								maxDeltaX, maxDeltaY,
								frame, previousFrame,
								motionVectors);

	// Find dominating motion vector
	const int nbVectors = (height / blockHeight)*(width / blockWidth);
	findDominatingMotionVector(nbVectors,
								motionVectors, dominatingMotionVector);
	#ifdef PREESM_VERBOSE
	// Print motion vector
	printf("Frame %3d: %2.2f, %2.2f\n", frameIndex,
			dominatingMotionVector.x, dominatingMotionVector.y);
	#endif
}

int main(int argc, char** argv)
{
	// Declarations
	static unsigned char y[HEIGHT*WIDTH], u[HEIGHT*WIDTH / 4], v[HEIGHT*WIDTH / 4];
	static unsigned char yPrevious[HEIGHT*WIDTH];
	static unsigned char yDisp[DISPLAY_W*DISPLAY_H], uDisp[DISPLAY_W*DISPLAY_H / 4], vDisp[DISPLAY_W*DISPLAY_H / 4];
	static unsigned char yDispPrev[DISPLAY_W*DISPLAY_H], uDispPrev[DISPLAY_W*DISPLAY_H / 4], vDispPrev[DISPLAY_W*DISPLAY_H / 4];
	static coord motionVectors[(HEIGHT / BLOCK_HEIGHT)*(WIDTH / BLOCK_WIDTH)];
	static coordf dominatingMotionVector;
	static coordf accumulatedMotion = { 0.0f, 0.0f };
	static coordf filteredMotion;

	#ifdef DISPLAY
	// Init display
	yuvDisplayInit(0, DISPLAY_W, DISPLAY_H);
	#endif

	// Open files
	initReadYUV(WIDTH, HEIGHT);
	initYUVWrite();
	startTiming(0);

	unsigned int frameIndex = 1;
	unsigned int time = 0;
	while (!preesmStopThreads)
	{
		// Backup previous frame
		memcpy(yPrevious, y, HEIGHT*WIDTH);

		// Read a frame
		readYUV(WIDTH, HEIGHT, y, u, v);

		time -= getTiming(0);

		// Process it
		processImage(WIDTH, HEIGHT, BLOCK_WIDTH, BLOCK_HEIGHT, MAX_DELTA_X, MAX_DELTA_Y, y, yPrevious, motionVectors, &dominatingMotionVector);

		time += getTiming(0);
		frameIndex++;
		if (frameIndex % FPS_INTERVAL == 0) {
			float fps = (float)frameIndex / ((float)time / 1000000.f);
			printf("\nMain: %d frames in avg %d us - %f fps\n", frameIndex, time, fps);
		}

		// Accumulate motion
		accumulateMotion(&dominatingMotionVector, &accumulatedMotion, &filteredMotion, &filteredMotion, &accumulatedMotion);

		// Render the motion compensated frame
		renderFrame(WIDTH, HEIGHT, DISPLAY_W, DISPLAY_H, &accumulatedMotion, &filteredMotion, y, u, v, yDispPrev, uDispPrev, vDispPrev, yDisp, uDisp, vDisp);

		// Backup for future ghosting
		memcpy(yDispPrev, yDisp, DISPLAY_W*DISPLAY_H);
		memcpy(uDispPrev, uDisp, DISPLAY_W/2*DISPLAY_H/2);
		memcpy(vDispPrev, vDisp, DISPLAY_W/2*DISPLAY_H/2);

		#ifdef DISPLAY
		// Display it
		yuvDisplay(0, yDisp, uDisp, vDisp);
		#endif

		// Save it
		yuvWrite(DISPLAY_W, DISPLAY_H, yDisp, uDisp, vDisp);

		// Exit ?
		if (frameIndex == NB_FRAME){
			preesmStopThreads = 1;
		}
	}

	#ifdef PREESM_VERBOSE
	printf("Exit program\n");
	#endif
	#ifdef DISPLAY
	yuvFinalize(0);
	#endif
	endYUVRead();
	endYUVWrite();

	return 0;
}
