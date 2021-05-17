FROM ubuntu:16.04

LABEL description="Build SGL dependencies"

RUN apt-get update && apt-get install -y \
    make \
    cmake \
    zip \
    g++ \
    autoconf \
    automake \
    pkg-config \
    libtool \
    mingw-w64-i686-dev \
    g++-mingw-w64-i686 \
    git \
    mercurial \
    libdw-dev \
    libelf-dev \
    libusb-1.0-0-dev \
    nasm \
    libgl1-mesa-dev

RUN mkdir -p /sgl/deps
WORKDIR /sgl/deps

COPY script script
COPY src src
COPY Makefile Makefile

# Layer them to allow docker caching
RUN make import-ffmpeg
RUN make import-freetype
RUN make import-libjpeg
RUN make import-libpng
RUN make import-sdl2
RUN make import-sdl2-image
RUN make import-sdl2-ttf
RUN make import-zlib

RUN make build-ffmpeg
RUN make build-fmodex
RUN make build-zlib
RUN make build-libjpeg
RUN make build-libpng
RUN make build-freetype
RUN make build-sdl2
RUN make build-sdl2-image
RUN make build-sdl2-ttf

# Libs required by windows build, copy to include it for extraction from docker container
WORKDIR /
RUN cp /usr/lib/gcc/i686-w64-mingw32/5.3-posix/libgcc_s_sjlj-1.dll /sgl/deps/build/win/bin/libgcc_s_sjlj-1.dll
RUN cp /usr/lib/gcc/i686-w64-mingw32/5.3-posix/libstdc++-6.dll /sgl/deps/build/win/bin/libstdc++-6.dll
RUN cp /usr/i686-w64-mingw32/lib/libwinpthread-1.dll /sgl/deps/build/win/bin/libwinpthread-1.dll