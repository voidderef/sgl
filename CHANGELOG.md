# Release history
## 1.05
### Features

### Fixes

## 1.04

### Features
* Add support for bemanitools ddrio interface
* `log_file` and `log_level` as configuration parameters, see `settings.lua` example
* Command line argument configuration overrides, e.g. `--gfx.fullscreen false` following the
  configuration path nesting in the `settings.lua` file for any configuration parameter

### Fixes
* Broken parameter logging on BemanitoolsGlue code. Fixes logging in general for bemanitools API
  log output
* Lua function exposure missing

## 1.03
* Automatic default game start when a timer runs out and no user input triggered
* Joystick support with SDL2
* piuiomk6itg IO device. Support ITG2 dedicated cabinets
* Various bugfixes

## 1.02
* Fix windows build and packaging
* Force enable vsync for renderer
* Code cleanup
* Enhance log output, include environment and OS info (Linux only right now)
* Various bugfixes

## 1.01
* Fix exception handling, stack trace printed on application crash
* Sound: Configure sound driver and device explicitly in `settings.lua`. No configuration defaults to auto detection.
* Bugfix: Specify io modules in `settings.lua` without extension now

## 1.00
* Initial release