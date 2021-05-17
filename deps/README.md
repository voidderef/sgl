# Third party direct dependencies of SGL
To ensure maintainability, less platform/distribution and version dependability, fixed versions of the main dependencies
are built by a series of makefiles and scripts which handle importing/downloading and building them.

## Build system
The "build system" is kept simple: Makefiles contain the commands to retrieve the files to build and how to build them
for Linux and Windows as platforms.

Importing is handled by separate scripts from the `script` folder. For git/hg imports, a `import.conf` file determines
where to get the source and which version/commit id of it to checkout for building.

Building is executed using `autotools` and `make` or `cmake` depending on what's available on each project.

Cross dependencies are resolved within this build system if possible, e.g. `sdl2-image` requires `libjpeg` and `libpng`
for loading these image formats.

All compiled output is placed in a separate `build` sub-folder inside the `deps` subdirectory of the project (i.e.
the directory this readme is in).

## Build
The root `Makefile` is the entry point to import and build everything. Simply run `make import` to import all
dependencies followed by `make build` to build everything for Linux and Windows.

For Windows, all dependencies are cross-compiled using `mingw`.

The output will be located in `build`.

### Docker build
You can also build all dependencies inside a docker container. This ensures consistent builds, a stable environment
and building stuff on different host platforms.

Simply run `make docker-build` to start building the dependencies inside a docker container. Naturally, you must have
`dockerd` installed on your system.

## Dependencies
A few notes regarding building these.

### FFmpeg
Nothing particular to mention here. Standard builds for both platforms.

### FMODEX
Nothing to build, because these ship as pre-built binaries, only. Also, including these with the repository because
they are super difficult to find. These were removed from the website for some reason (discontinued?).

### FreeType
Build without `harfbuzz` (font smoothing) as a dependency for now.

### libjpeg
Nothing particular to mention here. Standard builds for both platforms.

### libpng
* Disable `libpng compat` building because that will be named `libpng.so` which messes with linking of other projects.
* On Windows build: A symlink is missing which causes other projects using `libpng` to fail on linking.

### SDL2
* On Linux build: Disable wayland, avoid dependency
* On Windows build: Disable D3D renderers, use OpenGL. 

### SDL2 image
For both platforms: Disable a bunch of image formats that are not used a lot/at all to reduce dependencies.

### SDL2 TTF
* Both platforms: The build system seems to be broken if it comes to locating the PKG config path. This needs to be
set explicitly, otherwise it cannot find the freetype lib.
* Pre-fixes for freetype and sdl2 also needed to be added to make it find the dependencies.

### zlib
On Windows build: Naming messed up, needs to be fixed post build. This is not the case on the Linux build.