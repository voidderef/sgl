#pragma once

#include <unordered_map>

#include <SDL2/SDL.h>

#include "io/keyboard/DigitalInputs.h"

namespace io {
namespace keyboard {
namespace sdl2 {

class Mappings
{
public:
    static DigitalInputs Get(const SDL_Keysym& keysym);

private:
    Mappings() {};
    ~Mappings() {};

    typedef std::unordered_map<int32_t, DigitalInputs> SDL2InputToDigitalInputMap;
    static const SDL2InputToDigitalInputMap ms_mappings;

    static const SDL2InputToDigitalInputMap __CreateMappings();
};

}
}
}