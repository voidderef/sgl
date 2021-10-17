# Simple Game Loader (SGL)
Version 1.03

[Release history](CHANGELOG.md)

Until I got a proper CI pipeline set up, you can find binary distributions [here](https://drive.google.com/drive/folders/1-9gBaQCclefLNzQwuOYRPtj4qzw1lSCe?usp=sharing).

SGL is a cross-platform scriptable front-end for selecting programs/games on a dedicated machine that doesn't
necessarily have standard input devices like a keyboard or mouse attached.

One of the key features is a flexible IO subsystem which allows implementing devices and handlers for custom IO
hardware. You can also script entire output sequences to make your cabinets look awesome even when just bootstrapping
and starting different games.

## Demo
Note: To save space, the demo videos have a lower quality than the actual assets available.

### Pump It Up
Various versions of Pump It Up selectable in the loader. The title sequences are pre-rendered videos and scripted to run
the intro attract sequences of each game without any game HUD elements. When using a dedicated cabinet with real IO
hardware, the loader plays a light attract sequence. 
![Piu demo](doc/demo/piu.mp4)

### Beatmania IIDX
Various versions of Beatmania IIDX selectable in the loader. The intros of each games attract sequences are pre-recorded
videos without any HUD elements and loop as long as the current entry is highlighted. Once selection is confirmed, a
pre-rendered video of the title entry sequence is played. Some games don't have the proper assets created, yet, and
therefore contain simple place holder images of the title screen. When using a dedicated cabinet with real IO
hardware, the loader plays a light attract sequence.<br/>
![IIDX demo](doc/demo/iidx.mp4)

## Why not use Hyperspin or some other MAME front end?
That depends on your requirements. If you need custom IO support and some other special features SGL offers, other
solutions might not be suitable or very uncomfortable to get tweaked and working for your target setup. SGL provides
some niche features I haven't seen on similar front-ends, yet. However, SGL does not aim to replace any of the existing
front end solutions.

## Features
* Runs on Windows (minimum XP) and Linux
* Extensible IO interface to support standard (e.g. keyboard, joystick) and custom/non standard IO hardware
* Lua scripting interface to customize IO handling and mappings. You can also drive outputs of your hardware to create
light shows etc.
* Lua scripting for selectable program/game entries: a single entry can be as simple as a white text on a black
background, a static image with some music or a sequence of moving images and/or videos.
* Programs/Games are started by calling shell scripts which gives you full control over the startup process

## System requirements
* Linux: 64-bit distribution of your choice. Anything that's Ubuntu 16.04 or newer should be fine. However, docker builds
are on Ubuntu 16.04 and tested on the same platform, only.
* Windows: Minimum XP 32-bit with SP3

## Dependencies
The following **direct** dependencies are used by SGL:
* FFMPEG (pre 4.0 for XP support): More precisely libav from the FFMPEG project, for decoding videos
* SDL2: For general rendering
* SDL2_image: For supporting rendering of commonly used image formats like png or jpeg
* SDL_ttf: For text rendering
* FMODEX: Low level API for handling audio data and outputting sound
* Lua (source already included with project): Making the loader interface script-able without having to re-compile it

Further indirect dependencies are:
* FreeType: Required by SDL_ttf
* libjpeg: Required by SDL2_image
* libpng: Required by SDL2_image
* zlib: Required by libpng

Some tooling is also required to build everything:
* makefile
* cmake
* autoconf
* automake
* zip
* git
* g++ (For Linux)
* mingw-w64-i686 (For Windows)
* g++-mingw-w64-i686 (For Windows)
* dockerd: For building in docker containers (optional)

## Official assets for different games
Checkout the [sgldata repository](https://github.com/voidderef/sgldata) for official assets for different kinds of games
like Pump It Up or BeatmaniaIIDX. If you have assets that are not part of the repository and want to share them, please
see the contributors documentation in the sgldata repository.

## Build
The build infrastructure is triggered and controlled via makefiles. There are two ways to build the project:
* Local/dev build: Using the environment of the current host you are on
* Docker: Build everything inside docker containers. Therefore, you don't have to have everything installed on your host

All makefiles output usage information if you call them without a target: `make`

### Local build
```shell script
# First, build all dependencies SGL requires
make build-deps

# Build SGL, all platforms
make build

# Create distribution packages, all platforms
make package
```

The final output is located in `build/package`.

### Docker build
```shell script
# First, build all dependencies SGL requires within its own docker container
make build-docker-deps

# Build SGL, all platforms, in a docker container. Built dependencies are pulled from the deps container
make build-docker
```

The final output is located in `build/docker/package`.

## How to run
Once built and packaged, there are two distribution packages per platform (located in `build/package` or
`build/docker/package`:
* Linux: `sgl-linux.zip`, `sgl-linux-example.zip`
* Windows: `sgl-win.zip`, `sgl-win-example.zip`

Each contains a full runtime, i.e. the SGL executable, all dependencies and (base) game assets to get started.

The `sgl-<platform>.zip` contains a bare skeleton to get started configuring SGL from scratch. 
`sgl-<platform>-example.zip` showcases different ways to configure and script SGL on how to use its features. If you
haven't worked with SGL so far, the example package is recommended to get started.

By default, keyboard controls are configured in `data/settings.lua`:
```lua
-- ...
        -- List of IO modules to load, list of filenames without extension (!)
        modules = {
            -- Default IO is keyboard using SDL2
            "libio-keyboard-sdl2",
        },
-- ...
```

Further modules can be added as well which actives them in the application:
```lua
-- ...
        -- List of IO modules to load, list of filenames without extension (!)
        modules = {
            -- Default IO is keyboard using SDL2
            "libio-keyboard-sdl2",
            "libio-piuio",
        },
-- ...
```

Just unpack the package to a target installation destination of your choice and run either `sgl.bat` on Windows or
`sgl.sh` on Linux.

Note: `sgl-trap.sh` bootstrap SGL in a new X session using `xinit`. Run this only on systems without an already active
X session, e.g. dedicated setups, and provide an xorg configuration file as an argument:
```shell script
./sgl-trap.sh my-xorg.conf
```

Once the loader is booted, you can select entries using the `left` and `right` arrow keys on your keyboard (default
configuration). `Enter` confirms the currently displayed game and launches a shell script attached to that entry. 
`Escape` exits the loader. `F1` opens the operator menu which is navigated using `left` and `right`, and `Enter` to
confirm the current selection. 

## How to customize
### Asset folder structure
The `data` folder contains the following sub-folders and files:
* `aud`: Folder for audio files like background music playing with a game entry.
* `font`: Fonts used to display text.
* `io`: Put all compiled IO libraries here. Keyboard support is provided by `libio-keyboard-sdl2.so/.dll` for Linux
and Windows. The loader will load them on boot. Enable each library you want to use by adding the filename without
extension as an entry in the settings file under `io/modules`:
```lua
return {
    -- ... [Removed to shorten example]
    -- Settings for IO
    io = {
        -- ... [Removed to shorten example]

        -- List of IO modules to load, list of filenames without extension (!)
        modules = {
            -- Default IO is keyboard using SDL2
            "libio-keyboard-sdl2",
        },
    }
    -- ... [Removed to shorten example]
}
```
* `lua`: This folder contains all the lua tools which are either distributed with SGL or your own custom tools you create.
* `mov`: Put any video files used for game entries here.
* `sfx`: Sound effects used by the loader.
* `shell`: Put all your shell tools (sh/bat) for bootstrapping your games here.
* `title`: Put all your images used for game entries here.
* `games.lua`: Defines the list of games for the loader.
* `settings.lua`: Settings file for the loader.

### Customization of game titles with lua scripts
There are two main files to take a look at first: One is the `settings.lua` file in the data folder which contains
various tweakable parameters (the most relevant ones should be self explanatory). The second file is `games.lua`, also
in the data directory. It provides a list of games with configuration parameters and tools to use for the loader.

The examples included are displaying a game entry as a simple white text on black background and a static image. How an
entry is displayed by the loader is defined by the configured lua script. A single script can be re-used for multiple
entries. The lua scripts handling very simple and minimal screen processing flows are:
* [Display some text on a black background](src/main/lua/screen/attract/std/text.lua)
* [Display a "title" image full screen](src/main/lua/screen/attract/std/title.lua)
* [Display a "title" image full screen and play some background music](src/main/lua/screen/attract/std/title-bgm.lua)
* [Play and display a video (with sound)](src/main/lua/screen/attract/std/video.lua)

More common use-case scripts might follow in the future. Based on these, you can already build very simple loaders, e.g.
for a single game series, to run on dedicated setups.

Have an in-depth look into the lua scripts and how they are configured for game entries in `games.lua`. The shell script
provided as parameter in `games.lua` for an entry gets launched once the entry is selected. Additional parameters,
such as resources or arguments for the script depend on the script used.

More complex screen flows are implements by lua sources in use with very specific assets for
[iidx](src/main/lua/screen/attract/iidx) and [piu](src/main/lua/screen/attract/piu). The lua scripts are handling
multiple video sequences with transitions or fade-in/out.

Feel free to use any of these as an inspiration to build even fancier title screens presenting the games available for
selection!

### Default game boot on timeout
If you want the loader to boot into a default game on no user input after a defined number of seconds, change the
values accordingly in the `settings.lua` file:

```lua
-- ...
    -- Settings for different "screens" of the loader
    screen = {
        attract = {
            font = "OpenSans-Regular.ttf",
            -- ID of game to boot automatically if no input is pressed and the defined timeout
            -- is exceeded
            default_game_timeout_id = 0,
            -- Set this to a value greater 0 to activate
            default_game_timeout_duration_sec = 0,
        },
    -- ...
    }
-- ...
```

Activate this feature by setting `default_game_timeout_duration_sec` to any value greater than 0.
`default_game_timeout_id` defines which game to run when the game timeouts. The ID must match an existing ID defined
for a game in `games.lua`.

### Change IO input/output mappings
Any IO is supported by a collection of [lua scripts](src/main/lua) which are located in `data/lua` when the SGL is
deployed. Mappings of any supported device to translate button/keys on the IO device to game specific inputs, e.g.
next menu item, are defined in such scripts.

For keyboard controls for example, `lua/io/keyboad/sdl2/defs.lua` outlines the available inputs of a keyboard as well
as it's device identifier. `lua/io/keyboad/sdl2/isp-attract.lua` is the input state processor for the main attract
screen (the screen you select your game). In the `__process` function, you can see a block of code that maps keyboard
controls to game side inputs, for example:
```lua
dest.digital[SCREEN_ATTRACT_DI_SELECT_NEXT] = iop_input_state_is_pushed(IO_KEYBOARD_SDL2_DI_RIGHT, src, src_prev)
```

The above line maps the keyboard "arrow key right" to the game side input "select next". Available game side input
states for the attract screen can be found in `lua/screen/attract/defs`. For the operator screen, have a look at
`lua/screen/operator/defs`.

In order to process the inputs correctly, you have to define these mappings for **EVERY** IO device you want to use.

#### Joysticks
Joysticks work very similar to the keyboard as described in the previous section. However, there is a limitation of 4
joystick devices and 32 buttons each that are currently supported by SGL.

Furthermore, it is not possible to map joysticks to specific IDs. They get detected according to their enumeration in
`/dev/input`, e.g. `/dev/input/js0` -> Joystick 0 in SGL.

In order to enable joystick, add the `libio-joystick-sdl2` to the module section as well as the input state processor
scripts to the sections in the `settings.lua` file. The following excerpt depicts part of the `settings.lua` file 
configured for both, keyboard and joystick:
```lua
-- ...
        -- List of IO modules to load, list of filenames without extension (!)
        modules = {
            -- Default IO is keyboard using SDL2
            "libio-keyboard-sdl2",
            "libio-joystick-sdl2",
        },

        -- Input state processing tools. There must be
        -- one for each device which processes the inputs
        -- of the device and maps it to loader specific
        -- inputs/states
        isp_scripts = {
            -- For keyboard
            "io/keyboard/sdl2/isp-attract.lua",
            "io/keyboard/sdl2/isp-operator.lua",
            -- For joystick
            "io/joystick/sdl2/isp-attract.lua",
            "io/joystick/sdl2/isp-operator.lua",
        },
-- ...
```

To configure mappings, e.g. different button mappings or when using multiple joysticks, the same steps as described
[here](#change-io-inputoutput-mappings) for the keyboard apply to the joystick. Make sure to take the button definitions
from the file `lua/io/joystick/sdl2/defs.lua`.

If you want to map the same game input to multiple joystick inputs, use a logical or to combine the inputs, for example:
```lua
dest.digital[SCREEN_ATTRACT_DI_SELECT_NEXT] = 
    iop_input_state_is_pushed(IO_JOYSTICK_SDL2_DI_JOY1_BUTTON_1, src, src_prev) or
    iop_input_state_is_pushed(IO_JOYSTICK_SDL2_DI_JOY2_BUTTON_1, src, src_prev) 
```

This triggers the "select next" input when either button 1 on joystick 1 or 2 is defined.

### Custom IO hardware
If you are using special hardware that supports output, i.e. lights, you can also script light sequences while the
currently highlighted title is showing. Checkout the respective sub-folders for the different IO hardware SGL has
drivers for:
* [iidxio](src/main/lua/io/iidxio)
* [jbio](src/main/lua/io/jbio)

SGL's IO sub-system is extensible with drivers for hardware implemented in separate libraries. The IO sub-systems 
provides an abstraction layer to abstract inputs and outputs and exposes them to lua scripts. These scripts, 
`input-state-processor` for inputs and `output-state-processor` for outputs, implement mappings for inputs/outputs,
and how the they are handled on different screens in SGL.

Furthermore, the IO sub-system supports bemanitool's IO API for `iidxio` and `jbio`. Further APIs can be supported when
there is demand. This allows you to use any IO drivers you already implemented with BT5's API with SGL without
any additional coding effort.

#### Use IO libraries implementing Bemanitools 5 API
SGL provides glue libraries that implement SGL's IO interface and glue the BT5 API of specific games to it. This allows
you to use IO libs from BT5 with SGL without having to implement another IO interface for your IO hardware. SGL
implements the BT5 IO APIs as stub libraries (e.g. `libbt-iidxio.dll`) to allow linking it to the SGL IO lib
(e.g. `libio-iidxio.dll`). Thus, you have to rename the BT5 IO dlls accordingly (e.g. `iidxio.dll` ->
`libbt-iidxio.dll`), that these get picked up when they are loaded by the SGL IO lib.

The following BT5 APIs are currently supported using the specified IO libs from SGL:
* `iidxio.dll` -> `libio-iidxio.dll`, rename `iidxio.dll` to `libbt-iidxio.dll`

In order to use these, put both libraries, the SGL IO a BT5 IO one, into the `data/io` folder. If the BT5 IO lib does
have further dependencies, put these also into `data/io`. Make sure to run SGL using the `sgl.sh`/`sgl.bat` which sets
the PATH variable that the process is able to find all dependencies correctly.

## FAQ
### SoundException: Initializing FMOD system failed: Error initializing output device
If you get the following exception when starting SGL:
```text
ALSA lib pcm.c:2495:(snd_pcm_open_noupdate) Unknown PCM cards.pcm.dmix
terminate called after throwing an instance of 'sgl::SoundException'
  what():  Initializing FMOD system failed: Error initializing output device. 
Stack trace (most recent call last):
#12   Object "", at 0xffffffffffffffff, in 
#11   Object "/piu/sgl/sgl", at 0x433e78, in _start
#10   Source "../csu/libc-start.c", line 310, in __libc_start_main [0x7fc5b306fb96]
#9    Source "/sgl/src/main/cpp/sgl/main.cpp", line 10, in main [0x42f7ba]
#8  | Source "/sgl/src/main/cpp/sgl/SGL.cpp", line 55, in __Init
    | Source "/sgl/src/main/cpp/sgl/SGL.cpp", line 82, in __InitSfx
      Source "/sgl/src/main/cpp/sgl/SGL.cpp", line 317, in Main [0x46fc45]
#7    Source "/sgl/src/main/cpp/sgl/SoundRenderer.cpp", line 35, in Init [0x474534]
#6    Source "/sgl/src/main/cpp/sgl/SoundRenderer.cpp", line 376, in __FmodInit [0x472800]
#5    Object "/usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.25", at 0x7fc5b3a87d53, in __cxa_throw
#4    Object "/usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.25", at 0x7fc5b3a87b20, in std::terminate()
#3    Object "/usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.25", at 0x7fc5b3a87ae5, in 
#2    Object "/usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.25", at 0x7fc5b3a81956, in 
#1    Source "/build/glibc-OTsEL5/glibc-2.27/stdlib/abort.c", line 79, in abort [0x7fc5b308e800]
#0    Source "../sysdeps/unix/sysv/linux/raise.c", line 51, in raise [0x7fc5b308ce97]
Aborted (Signal sent by tkill() 808 0)
...
```

That means that FMOD cannot use the selected (or default) sound device for some reason. Further error information is not given at
this point unless SGL is run with a debug version of FMOD.

However, right above that error, SGL should output a list of available devices that FMOD detected (if debug log level
is set). This could like like follows (naturally, the list will look different on different hardware):
```text
[I][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:330] Num available devices 49
[D][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:339] Device 0: default
[D][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:339] Device 1: null
[D][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:339] Device 2: default:CARD=PCH
[D][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:339] Device 3: sysdefault:CARD=PCH
[D][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:339] Device 4: front:CARD=PCH,DEV=0
[D][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:339] Device 5: surround21:CARD=PCH,DEV=0
[D][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:339] Device 6: surround40:CARD=PCH,DEV=0
[D][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:339] Device 7: surround41:CARD=PCH,DEV=0
[D][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:339] Device 8: surround50:CARD=PCH,DEV=0
[D][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:339] Device 9: surround51:CARD=PCH,DEV=0
[D][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:339] Device 10: surround71:CARD=PCH,DEV=0
[D][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:339] Device 11: iec958:CARD=PCH,DEV=0
[D][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:339] Device 12: dsnoop:CARD=PCH,DEV=0
[D][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:339] Device 13: dsnoop:CARD=PCH,DEV=1
[D][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:339] Device 14: dsnoop:CARD=PCH,DEV=2
[D][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:339] Device 15: hw:CARD=PCH,DEV=0
[D][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:339] Device 16: hw:CARD=PCH,DEV=1
[D][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:339] Device 17: hw:CARD=PCH,DEV=2
[D][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:339] Device 18: plughw:CARD=PCH,DEV=0
[D][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:339] Device 19: plughw:CARD=PCH,DEV=1
[D][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:339] Device 20: plughw:CARD=PCH,DEV=2
[D][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:339] Device 21: hdmi:CARD=NVidia,DEV=0
[D][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:339] Device 22: hdmi:CARD=NVidia,DEV=1
[D][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:339] Device 23: hdmi:CARD=NVidia,DEV=2
[D][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:339] Device 24: hdmi:CARD=NVidia,DEV=3
[D][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:339] Device 25: dsnoop:CARD=NVidia,DEV=3
[D][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:339] Device 26: dsnoop:CARD=NVidia,DEV=7
[D][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:339] Device 27: dsnoop:CARD=NVidia,DEV=8
[D][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:339] Device 28: dsnoop:CARD=NVidia,DEV=9
[D][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:339] Device 29: hw:CARD=NVidia,DEV=3
[D][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:339] Device 30: hw:CARD=NVidia,DEV=7
[D][10/22/20][16:18:20.937][thread-767][SoundRenderer.cpp:339] Device 31: hw:CARD=NVidia,DEV=8
[D][10/22/20][16:18:20.938][thread-767][SoundRenderer.cpp:339] Device 32: hw:CARD=NVidia,DEV=9
[D][10/22/20][16:18:20.938][thread-767][SoundRenderer.cpp:339] Device 33: plughw:CARD=NVidia,DEV=3
[D][10/22/20][16:18:20.938][thread-767][SoundRenderer.cpp:339] Device 34: plughw:CARD=NVidia,DEV=7
[D][10/22/20][16:18:20.938][thread-767][SoundRenderer.cpp:339] Device 35: plughw:CARD=NVidia,DEV=8
[D][10/22/20][16:18:20.938][thread-767][SoundRenderer.cpp:339] Device 36: plughw:CARD=NVidia,DEV=9
[D][10/22/20][16:18:20.938][thread-767][SoundRenderer.cpp:339] Device 37: cards
[D][10/22/20][16:18:20.938][thread-767][SoundRenderer.cpp:339] Device 38: rear
[D][10/22/20][16:18:20.938][thread-767][SoundRenderer.cpp:339] Device 39: center_lfe
[D][10/22/20][16:18:20.938][thread-767][SoundRenderer.cpp:339] Device 40: side
[D][10/22/20][16:18:20.938][thread-767][SoundRenderer.cpp:339] Device 41: spdif
[D][10/22/20][16:18:20.938][thread-767][SoundRenderer.cpp:339] Device 42: dmix
[D][10/22/20][16:18:20.938][thread-767][SoundRenderer.cpp:339] Device 43: modem
[D][10/22/20][16:18:20.938][thread-767][SoundRenderer.cpp:339] Device 44: phoneline
[D][10/22/20][16:18:20.938][thread-767][SoundRenderer.cpp:339] Device 45: plug
[D][10/22/20][16:18:20.938][thread-767][SoundRenderer.cpp:339] Device 46: shm
[D][10/22/20][16:18:20.938][thread-767][SoundRenderer.cpp:339] Device 47: tee
[D][10/22/20][16:18:20.938][thread-767][SoundRenderer.cpp:339] Device 48: file
[I][10/22/20][16:18:20.938][thread-767][SoundRenderer.cpp:348] Selecting default device (0) because either the selected driver was not found or no driver was selected

```

In this case, no explicit audio device configuration was given, so SGL selects the default audio device. This can be
easily overridden by defining a device from the list in the `data/settings.lua` configuration file. For example, if you
want to configure the device `Device 42: dmix`, you set the device name like this:

```lua
    -- ...
    sound = {
        -- Other entries removed for this example
        -- ...
        --
        -- Explicitly specify the audio device to use. Leave empty for auto.
        -- SGL prints a list of available devices and their names to use here to the console on startup.
        device = "dmix",
        -- ...
    },
    -- ...
```

SGL will look for the named device (case-sensitive!) and pick it up if it can find it. This is reflected in the log
output once you restarted SGL, e.g.
```text
[I][10/22/20][16:18:52.445][thread-808][SoundRenderer.cpp:351] Found selected driver 'dmix', id 42
```

Depending on your setup and the available sound cards/output method, you might have to try a few different ones. It
also depends on how your local audio manager, e.g. pulse, is configured.

For example, output to the standard 3.5 audio jack, which should be available on any standard soundcard/motherboard,
might work with one of the following devices (if available):
```text
default
default:CARD=PCH
sysdefault:CARD=PCH
front:CARD=PCH,DEV=0
hw:CARD=PCH,DEV=0
dmix
```

## TODOs
A list of things that are currently not implemented/supported but should be in the near future:
* VideoDecoder: Converting different audio rates, if necessary, e.g. 44100khz -> 48000khz and vice versa
* SGL Main Loop: Frameskipping
* IOManager: Device hotplugging, frameskipping (?), proper merging of input states

## Future ideas
A list of things that might be implemented in the future:
* Add support for a "virtual file system" for the game entries: support directory hierarchies with subdirs

## License
Source code license is the Unlicense; you are permitted to do with this as thou wilt. For details, please refer to the
[LICENSE file](LICENSE) included with the source code.
