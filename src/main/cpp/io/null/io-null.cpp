#include "io/LibIODevice.h"

#include "Null.h"

extern "C" {

io::IODevice *io_device_instance(const io::IOManagerCtx& ioManagerCtx)
{
    return new io::null::Null();
}

}