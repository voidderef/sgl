#include "io/LibIODevice.h"

#include "PiuMk6Itg.h"

extern "C" {

io::IODevice* io_device_instance(const io::IOManagerCtx& ioManagerCtx)
{
    return new io::piumk6itg::PiuMk6Itg();
}

}