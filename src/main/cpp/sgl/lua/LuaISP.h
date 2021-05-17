#pragma once

#include "io/InputStateProcessor.h"

#include "sgl/lua/LuaISPBindings.h"

namespace sgl {

class LuaISP : public io::InputStateProcessor
{
public:
    explicit LuaISP(LuaISPBindings* bindings) :
        InputStateProcessor(bindings->GetDeviceIdentifier(), bindings->GetScreenIdentifier()),
        m_bindings(bindings)
    {
        m_bindings->LInit();
    }

    ~LuaISP() override
    {
        m_bindings->Unload();
        delete m_bindings;
    }

    void Activate() override
    {
        m_bindings->LActivate();
    }

    void Process(const io::InputState& src, const io::InputState& srcPrev, io::InputState& dest,
            io::OutputState& out) override
    {
        m_bindings->LProcess(src, srcPrev, dest, out);
    }

    void Deactivate() override
    {
        m_bindings->LDeactivate();
    }

private:
    LuaISPBindings* m_bindings;
};

}
