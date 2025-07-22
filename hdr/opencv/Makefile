.PHONY: all packages ubuntu debian clean

all: ubuntu bookworm bookworm-slim bullseye

ubuntu: target=ubuntu
ubuntu: packages

bookworm: target=bookworm
bookworm: packages

bookworm-slim: target=bookworm-slim
bookworm-slim: packages

bullseye: target=bullseye
bullseye: packages

clean:
	rm -rf packages

packages:
	mkdir -p $@/$(target) \
	&& docker build -t opencv-build-$(target) . -f Dockerfile.$(target)\
	&& docker run -v $(shell pwd)/$@/$(target):/output opencv-build-$(target)