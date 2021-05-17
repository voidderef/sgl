#include "io/LibIODevice.h"

#include "io/keyboard/sdl2/Keyboard.h"

extern "C" {

io::IODevice* io_device_instance(const io::IOManagerCtx& ioManagerCtx)
{
    return new io::keyboard::sdl2::Keyboard(ioManagerCtx);
}

}