package.path = './data/lua/?.lua;' .. package.path

require("io/piumk6itg/defs")
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

    dest.digital[SCREEN_ATTRACT_DI_SELECT_NEXT] =
        iop_input_state_is_pushed(IO_PIUMK6ITG_DI_P1_CABINET_RIGHT, src, src_prev) or
        iop_input_state_is_pushed(IO_PIUMK6ITG_DI_P2_CABINET_RIGHT, src, src_prev)
    dest.digital[SCREEN_ATTRACT_DI_SELECT_PREV] =
        iop_input_state_is_pushed(IO_PIUMK6ITG_DI_P1_CABINET_LEFT, src, src_prev) or
        iop_input_state_is_pushed(IO_PIUMK6ITG_DI_P2_CABINET_LEFT, src, src_prev)
    dest.digital[SCREEN_ATTRACT_DI_SELECTION_CONFIRMED] =
        iop_input_state_is_pushed(IO_PIUMK6ITG_DI_P1_CABINET_START, src, src_prev) or
        iop_input_state_is_pushed(IO_PIUMK6ITG_DI_P2_CABINET_START, src, src_prev)

    dest.digital[SCREEN_ATTRACT_DI_EXIT_APPLICATION] =
        iop_input_state_is_held(IO_PIUMK6ITG_DI_OP_TEST, src, src_prev) and
        iop_input_state_is_held(IO_PIUMK6ITG_DI_OP_SERVICE, src, src_prev)

    dest.digital[SCREEN_ATTRACT_DI_OPERATOR_SCREEN] =
        iop_input_state_is_pushed(IO_PIUMK6ITG_DI_OP_TEST, src, src_prev)

    out.digital[IO_PIUMK6ITG_DO_P1_LEFT] = src.digital[IO_PIUMK6ITG_DO_P1_LEFT]
    out.digital[IO_PIUMK6ITG_DO_P1_RIGHT] = src.digital[IO_PIUMK6ITG_DO_P1_RIGHT]
    out.digital[IO_PIUMK6ITG_DO_P1_UP] = src.digital[IO_PIUMK6ITG_DO_P1_UP]
    out.digital[IO_PIUMK6ITG_DO_P1_DOWN] = src.digital[IO_PIUMK6ITG_DO_P1_DOWN]

    out.digital[IO_PIUMK6ITG_DO_P2_LEFT] = src.digital[IO_PIUMK6ITG_DO_P2_LEFT]
    out.digital[IO_PIUMK6ITG_DO_P2_RIGHT] = src.digital[IO_PIUMK6ITG_DO_P2_RIGHT]
    out.digital[IO_PIUMK6ITG_DO_P2_UP] = src.digital[IO_PIUMK6ITG_DO_P2_UP]
    out.digital[IO_PIUMK6ITG_DO_P2_DOWN] = src.digital[IO_PIUMK6ITG_DO_P2_DOWN]

    return dest, out
end

return {
    device = IO_PIUMK6ITG_IDENTIFIER,
    screen = SCREEN_ATTRACT_IDENTIFIER,
    f_init = __init,
    f_activate = __activate,
    f_deactivate = __deactivate,
    f_process = __process,
}
