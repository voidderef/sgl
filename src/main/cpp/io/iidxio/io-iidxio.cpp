#include "io/LibIODevice.h"

#include "IIDXIO.h"

extern "C" {

io::IODevice* io_device_instance(const io::IOManagerCtx& ioManagerCtx)
{
    return new io::iidxio::IIDXIO();
}

}