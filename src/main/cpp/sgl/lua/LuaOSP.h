#pragma once

#include "io/OutputStateProcessor.h"

#include "sgl/lua/LuaOSPBindings.h"

namespace sgl {

class LuaOSP : public io::OutputStateProcessor
{
public:
    explicit LuaOSP(LuaOSPBindings* bindings) :
        OutputStateProcessor(bindings->GetScreenIdentifier(), bindings->GetDeviceIdentifier()),
        m_bindings(bindings)
    {
        m_bindings->LInit();
    }

    ~LuaOSP() override
    {
        m_bindings->Unload();
        delete m_bindings;
    }

    void Activate() override
    {
        m_bindings->LActivate();
    }

    void Process(const io::OutputState& src, io::OutputState& dest) override
    {
        m_bindings->LProcess(src, dest);
    }

    void Deactivate() override
    {
        m_bindings->LDeactivate();
    }

private:
    LuaOSPBindings* m_bindings;
};

}
