#include "io/keyboard/sdl2/Mappings.h"

namespace io {
namespace keyboard {
namespace sdl2 {

const Mappings::SDL2InputToDigitalInputMap Mappings::ms_mappings = __CreateMappings();

DigitalInputs Mappings::Get(const SDL_Keysym& keysym)
{
    return ms_mappings.at(keysym.sym);
}

const Mappings::SDL2InputToDigitalInputMap Mappings::__CreateMappings()
{
    auto map = std::unordered_map<int32_t, DigitalInputs>();

    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_1, e_DI_1));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_2, e_DI_2));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_3, e_DI_3));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_4, e_DI_4));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_5, e_DI_5));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_6, e_DI_6));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_7, e_DI_7));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_8, e_DI_8));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_9, e_DI_9));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_0, e_DI_0));

    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_BACKSPACE, e_DI_Back));

    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_MINUS, e_DI_Minus));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_EQUALS, e_DI_Equals));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_SPACE, e_DI_Space));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_COMMA, e_DI_Comma));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_PERIOD, e_DI_Period));

    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_BACKSLASH, e_DI_Backslash));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_SLASH, e_DI_Slash));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_LEFTBRACKET, e_DI_LBracket));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_RIGHTBRACKET, e_DI_RBracket));

    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_ESCAPE, e_DI_Escape));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_CAPSLOCK, e_DI_Capital));

    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_TAB, e_DI_Tab));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_RETURN, e_DI_Return));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_LCTRL, e_DI_LControl));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_RCTRL, e_DI_RControl));

    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_COLON, e_DI_Colon));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_SEMICOLON, e_DI_Semicolon));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_QUOTEDBL, e_DI_Apostrophe));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_BACKQUOTE, e_DI_Grave));

    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_a, e_DI_A));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_b, e_DI_B));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_c, e_DI_C));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_d, e_DI_D));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_e, e_DI_E));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_f, e_DI_F));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_h, e_DI_G));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_h, e_DI_H));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_i, e_DI_I));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_j, e_DI_J));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_k, e_DI_K));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_l, e_DI_L));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_m, e_DI_M));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_n, e_DI_N));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_o, e_DI_O));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_p, e_DI_P));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_q, e_DI_Q));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_r, e_DI_R));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_s, e_DI_S));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_t, e_DI_T));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_u, e_DI_U));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_v, e_DI_V));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_w, e_DI_W));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_x, e_DI_X));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_y, e_DI_Y));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_z, e_DI_Z));

    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_F1, e_DI_F1));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_F2, e_DI_F2));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_F3, e_DI_F3));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_F4, e_DI_F4));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_F5, e_DI_F5));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_F6, e_DI_F6));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_F7, e_DI_F7));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_F8, e_DI_F8));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_F9, e_DI_F9));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_F10, e_DI_F10));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_F11, e_DI_F11));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_F12, e_DI_F12));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_F13, e_DI_F13));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_F14, e_DI_F14));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_F15, e_DI_F15));

    //Keypad
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_KP_0, e_DI_Numpad0));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_KP_1, e_DI_Numpad1));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_KP_2, e_DI_Numpad2));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_KP_3, e_DI_Numpad3));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_KP_4, e_DI_Numpad4));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_KP_5, e_DI_Numpad5));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_KP_6, e_DI_Numpad6));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_KP_7, e_DI_Numpad7));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_KP_8, e_DI_Numpad8));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_KP_9, e_DI_Numpad9));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_KP_PLUS, e_DI_Add));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_KP_MINUS, e_DI_Subtract));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_KP_PERIOD, e_DI_Decimal));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_KP_DIVIDE, e_DI_Divide));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_KP_MULTIPLY, e_DI_Multiply));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_KP_ENTER, e_DI_NumpadEnter));

    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_UP, e_DI_Up));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_DOWN, e_DI_Down));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_LEFT, e_DI_Left));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_RIGHT, e_DI_Right));

    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_PAGEUP, e_DI_PgUp));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_PAGEDOWN, e_DI_PgDown));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_HOME, e_DI_Home));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_END, e_DI_End));

    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_NUMLOCKCLEAR, e_DI_Numlock));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_PRINTSCREEN, e_DI_SysRQ));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_SCROLLLOCK, e_DI_Scroll));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_PAUSE, e_DI_Pause));

    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_RSHIFT, e_DI_RShift));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_LSHIFT, e_DI_LShift));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_RALT, e_DI_RMenu));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_LALT, e_DI_LMenu));

    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_INSERT, e_DI_Insert));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_DELETE, e_DI_Delete));

    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_LGUI, e_DI_LWin));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_RGUI, e_DI_RWin));
    map.insert(SDL2InputToDigitalInputMap::value_type(SDLK_APPLICATION, e_DI_Apps));

    return map;
}

}
}
}