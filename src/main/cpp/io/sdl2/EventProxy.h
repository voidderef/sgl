#pragma once

#include <vector>

#include <SDL2/SDL.h>

#include "io/sdl2/JoystickEventListener.h"
#include "io/sdl2/KeyboardEventListener.h"

namespace io {
namespace sdl2 {

class EventProxy
{
public:
    EventProxy() {};
    ~EventProxy() {};

    void AddKeyboardEventListener(KeyboardEventListener* refListener)
    {
        m_refKeyboardEventListeners.push_back(refListener);
    }

    void ProcessKeyboardEvent(const SDL_KeyboardEvent& event)
    {
        for (auto& listener : m_refKeyboardEventListeners) {
            listener->ProcessKeyboardEvent(event);
        }
    }

    void AddJoystickEventListener(JoystickEventListener* refListener)
    {
        m_refJoystickEventListeners.push_back(refListener);
    }

    void ProcessJoystickButtonEvent(const SDL_JoyButtonEvent & event)
    {
        for (auto& listener : m_refJoystickEventListeners) {
            listener->ProcessJoystickButtonEvent(event);
        }
    }

private:
    std::vector<KeyboardEventListener*> m_refKeyboardEventListeners;
    std::vector<JoystickEventListener*> m_refJoystickEventListeners;
};

}
}