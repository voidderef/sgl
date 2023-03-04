FROM ubuntu:16.04

LABEL description="Build environment for SGL dependencies"

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

ENTRYPOINT [ \
    "/bin/bash", \
    "-c" , \
    "cd /sgl/deps && \
    make import && \
    make build" ]