#pragma once

#include <SDL2/SDL.h>

#include "io/InputState.h"
#include "io/OutputState.h"

#include "EngineProxy.h"

namespace sgl {

class Screen
{
public:
    explicit Screen(const std::string& name) :
        m_name(name),
        m_refEngineProxy(nullptr),
        m_exitApplication(false),
        m_nextScreen("")
    {};

    virtual ~Screen() {};

    void SetEngineProxy(EngineProxy* refEngineProxy) {
        m_refEngineProxy = refEngineProxy;
    }

    const std::string& GetName() const {
        return m_name;
    }

    const std::string& NextScreen() const {
        return m_nextScreen;
    }

    bool ExitApplication() const {
        return m_exitApplication;
    }

    virtual bool Init() = 0;

    virtual void Cleanup() = 0;

    virtual void Update(double prevDeltaSec, const io::InputState& inputState, io::OutputState& outputState) = 0;

protected:
    EngineProxy* _GetEngineProxy() const {
        return m_refEngineProxy;
    }

    void _SetNextScreen(const std::string& name) {
        m_nextScreen = name;
    }

    void _ExitApplication() {
        m_exitApplication = true;
    }

protected:
    bool m_exitApplication;
    std::string m_nextScreen;

private:
    const std::string m_name;
    EngineProxy* m_refEngineProxy;
};

}
