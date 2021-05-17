package.path = './data/lua/?.lua;' .. package.path

require("io/piumk6/defs")
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

    dest.digital[SCREEN_ATTRACT_DI_SELECT_NEXT] = iop_input_state_is_pushed(IO_PIUMK6_DI_P1_RD, src, src_prev) or
        iop_input_state_is_pushed(IO_PIUMK6_DI_P2_RD, src, src_prev)
    dest.digital[SCREEN_ATTRACT_DI_SELECT_PREV] = iop_input_state_is_pushed(IO_PIUMK6_DI_P1_LD, src, src_prev) or
        iop_input_state_is_pushed(IO_PIUMK6_DI_P2_LD, src, src_prev)
    dest.digital[SCREEN_ATTRACT_DI_SELECTION_CONFIRMED] = iop_input_state_is_pushed(IO_PIUMK6_DI_P1_CN, src,
        src_prev) or iop_input_state_is_pushed(IO_PIUMK6_DI_P2_CN, src, src_prev)

    dest.digital[SCREEN_ATTRACT_DI_EXIT_APPLICATION] = iop_input_state_is_held(IO_PIUMK6_DI_OP_TEST, src,
        src_prev) and iop_input_state_is_held(IO_PIUMK6_DI_OP_SERVICE, src, src_prev)

    dest.digital[SCREEN_ATTRACT_DI_OPERATOR_SCREEN] = iop_input_state_is_pushed(IO_PIUMK6_DI_OP_TEST, src, src_prev)

    out.digital[IO_PIUMK6_DO_P1_LU] = src.digital[IO_PIUMK6_DO_P1_LU]
    out.digital[IO_PIUMK6_DI_P1_RU] = src.digital[IO_PIUMK6_DO_P1_RU]
    out.digital[IO_PIUMK6_DI_P1_CN] = src.digital[IO_PIUMK6_DO_P1_CN]
    out.digital[IO_PIUMK6_DI_P1_LD] = src.digital[IO_PIUMK6_DO_P1_LD]
    out.digital[IO_PIUMK6_DI_P1_RD] = src.digital[IO_PIUMK6_DO_P1_RD]

    out.digital[IO_PIUMK6_DO_P2_LU] = src.digital[IO_PIUMK6_DO_P2_LU]
    out.digital[IO_PIUMK6_DI_P2_RU] = src.digital[IO_PIUMK6_DO_P2_RU]
    out.digital[IO_PIUMK6_DI_P2_CN] = src.digital[IO_PIUMK6_DO_P2_CN]
    out.digital[IO_PIUMK6_DI_P2_LD] = src.digital[IO_PIUMK6_DO_P2_LD]
    out.digital[IO_PIUMK6_DI_P2_RD] = src.digital[IO_PIUMK6_DO_P2_RD]

    return dest, out
end

return {
    device = IO_PIUMK6_IDENTIFIER,
    screen = SCREEN_ATTRACT_IDENTIFIER,
    f_init = __init,
    f_activate = __activate,
    f_deactivate = __deactivate,
    f_process = __process,
}
