#include "io/LibIODevice.h"

#include "PiuBtn.h"

extern "C" {

io::IODevice* io_device_instance(const io::IOManagerCtx& ioManagerCtx)
{
    return new io::piubtn::PiuBtn();
}

}