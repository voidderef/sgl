#include "Trace.h"

namespace ks {

void Trace::PrintStackTrace()
{
    backward::StackTrace trace;
    trace.load_here(32);

    backward::Printer p;
    p.print(trace);
}

}