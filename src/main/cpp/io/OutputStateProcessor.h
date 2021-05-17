#pragma once

#include "OutputState.h"

namespace io {

class OutputStateProcessor
{
public:
    OutputStateProcessor(const std::string& srcIdent, const std::string& destDeviceIdent) :
        m_srcIdent(srcIdent),
        m_destDeviceIdent(destDeviceIdent)
    {};

    virtual ~OutputStateProcessor() = default;

    const std::string& GetSrcIdent() const {
        return m_srcIdent;
    }

    const std::string GetDestDeviceIdent() const {
        return m_destDeviceIdent;
    }

    virtual void Activate() {
        // stub
    }

    virtual void Process(const OutputState& src, OutputState& dest) = 0;

    virtual void Deactivate() {
        // stub
    }

private:
    const std::string m_srcIdent {};
    const std::string m_destDeviceIdent {};
};

}
