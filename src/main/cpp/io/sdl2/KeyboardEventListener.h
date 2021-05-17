#pragma once

#include <SDL2/SDL.h>

namespace io {
namespace sdl2 {

class KeyboardEventListener
{
public:
    virtual void ProcessKeyboardEvent(const SDL_KeyboardEvent& event) = 0;

protected:
    KeyboardEventListener() {};
    virtual ~KeyboardEventListener() {};
};

}
}