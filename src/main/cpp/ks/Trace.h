#pragma once

#include <backwards/backward.hpp>

namespace ks {

class Trace
{
public:
    static void PrintStackTrace();

private:
    Trace() = default;
    ~Trace() = default;
};

}
