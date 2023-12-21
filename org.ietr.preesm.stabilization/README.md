# Stabilization

This is a project forked from https://github.com/preesm/preesm-apps

## Try it

### Linux

#### Install dependencies

The following libraries are required:
* SDL2
* SDL2 TTF

```bash
sudo apt install libsdl2-ttf-dev libsdl2-dev
```

#### Compile

To enable displaying the video, you need to set the env variable `DISPLAY`

```bash
cmake -B build -D DISPLAY=on .
cmake --build ./build --target stabilization-sequential
```

#### Launch

The executable is located at `build/Release/stabilization-sequential`

```bash
./build/Release/stabilization-sequential
```

#### Clean

To clean the project:

```bash
cmake --build build --target clean
```