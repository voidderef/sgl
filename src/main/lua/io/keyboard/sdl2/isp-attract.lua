package.path = './data/lua/?.lua;' .. package.path

require("io/keyboard/sdl2/defs")
require("iop/input-state")
require("screen/attract/defs")

local function __init()
   -- stub
end

local function __activate()
     -- stub
end

local function __deactivate()
     -- stub
end

local function __process(src, src_prev)
    local dest = {
        digital = {},
        analog = {},
    }

    local out = {
        digital = {},
        analog = {},
        analog_u8 = {},
        text = {}
    }

    dest.digital[SCREEN_ATTRACT_DI_SELECT_NEXT] = iop_input_state_is_pushed(IO_KEYBOARD_SDL2_DI_RIGHT, src, src_prev)
    dest.digital[SCREEN_ATTRACT_DI_SELECT_PREV] = iop_input_state_is_pushed(IO_KEYBOARD_SDL2_DI_LEFT, src, src_prev)
    dest.digital[SCREEN_ATTRACT_DI_SELECTION_CONFIRMED] = iop_input_state_is_pushed(IO_KEYBOARD_SDL2_DI_RETURN, src,
        src_prev)
    dest.digital[SCREEN_ATTRACT_DI_EXIT_APPLICATION] = iop_input_state_is_pushed(IO_KEYBOARD_SDL2_DI_ESCAPE, src, src_prev)
    dest.digital[SCREEN_ATTRACT_DI_OPERATOR_SCREEN] = iop_input_state_is_pushed(IO_KEYBOARD_SDL2_DI_F1, src, src_prev)

    dest.digital[SCREEN_ATTRACT_DI_DEBUG_0] = iop_input_state_is_pushed(IO_KEYBOARD_SDL2_DI_1, src, src_prev)
    dest.digital[SCREEN_ATTRACT_DI_DEBUG_1] = iop_input_state_is_pushed(IO_KEYBOARD_SDL2_DI_2, src, src_prev)
    dest.digital[SCREEN_ATTRACT_DI_DEBUG_2] = iop_input_state_is_pushed(IO_KEYBOARD_SDL2_DI_3, src, src_prev)
    dest.digital[SCREEN_ATTRACT_DI_DEBUG_3] = iop_input_state_is_pushed(IO_KEYBOARD_SDL2_DI_4, src, src_prev)
    dest.digital[SCREEN_ATTRACT_DI_DEBUG_4] = iop_input_state_is_pushed(IO_KEYBOARD_SDL2_DI_5, src, src_prev)
    dest.digital[SCREEN_ATTRACT_DI_DEBUG_5] = iop_input_state_is_pushed(IO_KEYBOARD_SDL2_DI_6, src, src_prev)
    dest.digital[SCREEN_ATTRACT_DI_DEBUG_6] = iop_input_state_is_pushed(IO_KEYBOARD_SDL2_DI_7, src, src_prev)
    dest.digital[SCREEN_ATTRACT_DI_DEBUG_7] = iop_input_state_is_pushed(IO_KEYBOARD_SDL2_DI_8, src, src_prev)

    return dest, out
end

return {
    device = IO_KEYBOARD_SDL2_IDENTIFIER,
    screen = SCREEN_ATTRACT_IDENTIFIER,
    f_init = __init,
    f_activate = __activate,
    f_deactivate = __deactivate,
    f_process = __process,
}
