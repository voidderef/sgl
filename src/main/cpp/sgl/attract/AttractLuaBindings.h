#pragma once

#include "io/InputState.h"

#include "sgl/lua/LuaState.h"

namespace sgl {

class AttractLuaBindings
{
public:
    explicit AttractLuaBindings(const std::string& scriptPath);

    ~AttractLuaBindings() = default;

    void Load(LuaState* refLuaState);

    void Unload();

    bool LLoad(int luaRefTableResources, int luaRefTableArgs);

    void LUnload();

    void LUpdate(double delta, const io::InputState& input);

    void LOnEnable();

    void LOnDisable();

    bool LOnConfirm();

    bool LOnTransition();

    friend std::ostream& operator<<(std::ostream& os, const AttractLuaBindings& o)
    {
        return os << o.m_scriptPath;
    }

private:
    const std::string m_scriptPath;
    LuaState* m_refState;

    int m_funcLoad;
    int m_funcUnload;
    int m_funcUpdate;
    int m_funcOnEnable;
    int m_funcOnDisable;
    int m_funcOnConfirm;
    int m_funcOnTransition;

    void __ThrowCallException(const std::string& function);
};

}
