.PHONY: deps all clean bookworm-deps ubuntu-deps bullseye-deps deps-clean all-deps

hdr: hdr.cpp
	g++ $< -o $@ $(shell pkg-config --cflags --libs opencv4) -I/usr/include/opencv4

configure:
	apt update && \
	apt install -y ./opencv/packages/$(os)/OpenCV-4.12.0-x86_64-dev.deb ./opencv/packages/$(os)/OpenCV-4.12.0-x86_64-libs.deb

clean:
	rm -rf ldr*.jpg
	rm -rf hdr
	rm -rf *.hdr

deps-clean:
	$(MAKE) -C opencv clean

all-deps: bookworm-deps ubuntu-deps bookworm-slim-deps bullseye-deps

bookworm-deps: target=bookworm
bookworm-deps: deps

bullseye-deps: target=bullseye
bullseye-deps: deps

bookworm-slim-deps: target=bookworm-slim
bookworm-slim-deps: deps

ubuntu-deps: target=ubuntu
ubuntu-deps: deps

deps:
	$(MAKE) -C opencv $(target)