#include "io/LibIODevice.h"

#include "io/joystick/sdl2/Joystick.h"

extern "C" {

io::IODevice* io_device_instance(const io::IOManagerCtx& ioManagerCtx)
{
    return new io::joystick::sdl2::Joystick(ioManagerCtx);
}

}