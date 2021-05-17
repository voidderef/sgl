package.path = './data/lua/?.lua;' .. package.path

require("io/jbio/defs")
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

    dest.digital[SCREEN_ATTRACT_DI_SELECT_NEXT] = iop_input_state_is_pushed(IO_JBIO_DI_PANEL1, src, src_prev)
    dest.digital[SCREEN_ATTRACT_DI_SELECT_PREV] = iop_input_state_is_pushed(IO_JBIO_DI_PANEL2, src, src_prev)
    dest.digital[SCREEN_ATTRACT_DI_SELECTION_CONFIRMED] = iop_input_state_is_pushed(IO_JBIO_DI_PANEL3, src, src_prev)

    dest.digital[SCREEN_ATTRACT_DI_OPERATOR_SCREEN] = iop_input_state_is_pushed(IO_JBIO_DI_TEST, src, src_prev)

    return dest, out
end

return {
    device = IO_JBIO_IDENTIFIER,
    screen = SCREEN_ATTRACT_IDENTIFIER,
    f_init = __init,
    f_activate = __activate,
    f_deactivate = __deactivate,
    f_process = __process,
}
