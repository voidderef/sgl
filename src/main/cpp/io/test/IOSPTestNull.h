#pragma once

#include "InputStateProcessor.h"

namespace io {

class IOSPTestNull : public InputStateProcessor
{
public:
    IOSPTestNull();

    ~IOSPTestNull() override = default;

    void Process(const InputState& src, const InputState& srcPrev, InputState& dest, OutputState& out) override;
};

}
