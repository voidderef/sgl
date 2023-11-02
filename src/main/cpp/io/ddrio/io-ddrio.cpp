#include "io/LibIODevice.h"

#include "DDRIO.h"

extern "C" {

io::IODevice* io_device_instance(const io::IOManagerCtx& ioManagerCtx)
{
    return new io::ddrio::DDRIO();
}

}