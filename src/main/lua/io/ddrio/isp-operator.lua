package.path = './data/lua/?.lua;' .. package.path

require("io/ddrio/defs")
require("iop/input-state")
require("screen/operator/defs")

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

    dest.digital[SCREEN_OPERATOR_DI_SELECT_NEXT] = iop_input_state_is_pushed(IO_DDRIO_DI_P1_SELECT_RIGHT, src, src_prev)
    dest.digital[SCREEN_OPERATOR_DI_SELECT_PREV] = iop_input_state_is_pushed(IO_DDRIO_DI_P1_SELECT_LEFT, src, src_prev)
    dest.digital[SCREEN_OPERATOR_DI_SELECTED] = iop_input_state_is_pushed(IO_DDRIO_DI_P1_START, src,
        src_prev)
    dest.digital[SCREEN_OPERATOR_DI_NOISE_BACK] = iop_input_state_is_pushed(IO_DDRIO_DI_P1_START, src, src_prev)
    dest.digital[SCREEN_OPERATOR_DI_EXIT_APPLICATION] = iop_input_state_is_held(IO_DDRIO_DI_TEST, src, src_prev) and
        iop_input_state_is_held(IO_DDRIO_DI_SERVICE, src, src_prev)

    return dest, out
end

return {
    device = IO_DDRIO_IDENTIFIER,
    screen = SCREEN_OPERATOR_IDENTIFIER,
    f_init = __init,
    f_activate = __activate,
    f_deactivate = __deactivate,
    f_process = __process,
}
