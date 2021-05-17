#pragma once

#include <string>
#include <utility>

#include "ks/Settings.h"

#include "InputState.h"
#include "OutputState.h"

namespace io {

class IODevice
{
public:
    explicit IODevice(const std::string& identifier) :
        m_identifier(std::move(identifier))
    {};

    virtual ~IODevice() = default;

    const std::string& GetIdentifier() const {
        return m_identifier;
    }

    virtual void Open(const ks::Settings& settings) = 0;

    virtual void Close() = 0;

    virtual void Update() = 0;

    virtual void ProcessInputStates(InputState& inputStates) {
        // stub
    }

    virtual void ProcessOutputStates(const OutputState& outputStates) {
        // stub
    }

private:
    const std::string m_identifier;
};

}
