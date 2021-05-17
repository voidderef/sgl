#pragma once

#include <string>
#include <utility>

#include "InputState.h"
#include "OutputState.h"

namespace io {

class InputStateProcessor
{
public:
    InputStateProcessor(const std::string& srcDeviceIdent, const std::string& destIdent) :
        m_srcDeviceIdent(std::move(srcDeviceIdent)),
        m_destIdent(std::move(destIdent))
    {};

    virtual ~InputStateProcessor() = default;

    const std::string& GetSrcDeviceIdent() const {
        return m_srcDeviceIdent;
    }

    const std::string GetDestIdent() const {
        return m_destIdent;
    }

    virtual void Activate() {
        // stub
    }

    virtual void Process(const InputState& src, const InputState& srcPrev, InputState& dest, OutputState& out) = 0;

    virtual void Deactivate() {
        // stub
    }

private:
    const std::string m_srcDeviceIdent;
    const std::string m_destIdent;
};

}
