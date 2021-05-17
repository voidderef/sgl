#pragma once

#include <backwards/backward.hpp>

namespace ks {

class KitchenSink
{
public:
    static void Init(const std::string& logFile = "");

    static void Shutdown();

private:
    KitchenSink() = default;

    ~KitchenSink() = default;

    static backward::SignalHandling* ms_signalHandler;
};

}
