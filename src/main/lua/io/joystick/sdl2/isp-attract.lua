package.path = './data/lua/?.lua;' .. package.path

require("io/joystick/sdl2/defs")
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

    dest.digital[SCREEN_ATTRACT_DI_SELECT_NEXT] = iop_input_state_is_pushed(IO_JOYSTICK_SDL2_DI_JOY1_BUTTON_1, src, src_prev)
    dest.digital[SCREEN_ATTRACT_DI_SELECT_PREV] = iop_input_state_is_pushed(IO_JOYSTICK_SDL2_DI_JOY1_BUTTON_2, src, src_prev)
    dest.digital[SCREEN_ATTRACT_DI_SELECTION_CONFIRMED] = iop_input_state_is_pushed(IO_JOYSTICK_SDL2_DI_JOY1_BUTTON_3, src,
        src_prev)
    dest.digital[SCREEN_ATTRACT_DI_EXIT_APPLICATION] = iop_input_state_is_pushed(IO_JOYSTICK_SDL2_DI_JOY1_BUTTON_4, src, src_prev)
    dest.digital[SCREEN_ATTRACT_DI_OPERATOR_SCREEN] = iop_input_state_is_pushed(IO_JOYSTICK_SDL2_DI_JOY1_BUTTON_5, src, src_prev)

    return dest, out
end

return {
    device = IO_JOYSTICK_SDL2_IDENTIFIER,
    screen = SCREEN_ATTRACT_IDENTIFIER,
    f_init = __init,
    f_activate = __activate,
    f_deactivate = __deactivate,
    f_process = __process,
}
