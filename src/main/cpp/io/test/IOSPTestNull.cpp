#include "IOSPTestNull.h"

#include "io/null/NullDefs.h"

namespace io {

IOSPTestNull::IOSPTestNull() :
    InputStateProcessor(null::DEVICE_IDENT, "test")
{

}

void IOSPTestNull::Process(const InputState& src, const InputState& srcPrev, InputState& dest, OutputState& out)
{

}

}