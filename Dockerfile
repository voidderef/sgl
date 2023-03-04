FROM ubuntu:16.04

LABEL description="Build environment for SGL"

RUN apt-get update && apt-get install -y \
    make \
    cmake \
    zip \
    git \
    g++ \
    libtool \
    mingw-w64-i686-dev \
    g++-mingw-w64-i686 \
    libdw-dev \
    libelf-dev \
    libusb-1.0-0-dev \
    libgl1-mesa-dev

RUN mkdir /sgl
WORKDIR /sgl

ENTRYPOINT [ \
    "/bin/bash", \
    "-c" , \
    "cd /sgl && \
    make build && \
    make package" ]