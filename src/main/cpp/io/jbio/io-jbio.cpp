#include "io/LibIODevice.h"

#include "JBIO.h"

extern "C" {

io::IODevice* io_device_instance(const io::IOManagerCtx& ioManagerCtx)
{
    return new io::jbio::JBIO();
}

}