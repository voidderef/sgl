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

RUN mkdir -p /sgl/deps/build
COPY deps/build/docker /sgl/deps/build

# Also copy to binary output dir of linux build because setting rpath is broken on some cmake versions. This results
# in builds failing on linking the sgl executable because the local libraries to use cannot be found.
RUN mkdir -p /sgl/deps/build/linux
COPY deps/build/docker/linux/lib /sgl/build/sgl/linux/bin

COPY cmake cmake
COPY dist dist
COPY src src
COPY Common.mk Common.mk
COPY CMakeLists.txt CMakeLists.txt
COPY Makefile Makefile
COPY Package.mk Package.mk
# .git folder required or building fails when version is generated
COPY .git .git

RUN make build
RUN make package