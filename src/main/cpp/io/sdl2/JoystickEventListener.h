#pragma once

#include <SDL2/SDL.h>

namespace io {
namespace sdl2 {

class JoystickEventListener
{
public:
    virtual void ProcessJoystickButtonEvent(const SDL_JoyButtonEvent & event) = 0;

protected:
    JoystickEventListener() {};
    virtual ~JoystickEventListener() {};
};

}
}