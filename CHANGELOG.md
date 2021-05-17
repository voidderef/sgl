# Release history
## 1.04
* Bugfix lua function exposure was missing

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