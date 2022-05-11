#include "io/LibIODevice.h"

#include "PiuLxio.h"

extern "C" {

io::IODevice* io_device_instance(const io::IOManagerCtx& ioManagerCtx)
{
    return new io::piulxio::PiuLxio();
}

}