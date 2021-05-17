#pragma once

#include "io/InputState.h"
#include "io/OutputState.h"

#include "sgl/lua/LuaState.h"

namespace sgl {

class LuaOSPBindings
{
public:
    explicit LuaOSPBindings(const std::string& scriptPath);

    ~LuaOSPBindings() = default;

    void Load();

    void Unload();

    const std::string& GetDeviceIdentifier() const {
        return m_device;
    }

    const std::string& GetScreenIdentifier() const {
        return m_screen;
    }

    void LInit();

    void LActivate();

    void LDeactivate();

    void LProcess(const io::OutputState& src, io::OutputState& dest);

private:
    const std::string m_scriptPath;
    LuaState* m_state;

    std::string m_device;
    std::string m_screen;
    int m_funcInit;
    int m_funcActivate;
    int m_funcDeactivate;
    int m_funcProcess;

    void __ThrowCallException(const std::string& function);
};

}
