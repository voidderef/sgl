#include "io/LibIODevice.h"

#include "PiuMk6.h"

extern "C" {

io::IODevice* io_device_instance(const io::IOManagerCtx& ioManagerCtx)
{
    return new io::piumk6::PiuMk6Itg();
}

}