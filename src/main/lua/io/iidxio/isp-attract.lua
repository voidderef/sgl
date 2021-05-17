package.path = './data/lua/?.lua;' .. package.path

require("io/iidxio/defs")
require("iop/input-state")
require("screen/attract/defs")

local BUFFER_COUNT = 10

local __buffer_tt = 0

local function __init()

end

local function __activate()

end

local function __deactivate()

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

    if iop_input_state_is_incremented(IO_IIDXIO_AI_TT_P1, 0.003, src, src_prev) or
            iop_input_state_is_incremented(IO_IIDXIO_AI_TT_P2, 0.003, src, src_prev) then
        if __buffer_tt >= 0 then
            __buffer_tt = __buffer_tt + 1
        else
            __buffer_tt = 0
        end
    elseif iop_input_state_is_decremented(IO_IIDXIO_AI_TT_P1, 0.003, src, src_prev) or
            iop_input_state_is_decremented(IO_IIDXIO_AI_TT_P2, 0.003, src, src_prev) then
        if __buffer_tt <= 0 then
            __buffer_tt = __buffer_tt - 1
        else
            __buffer_tt = 0
        end
    end

    if __buffer_tt >= BUFFER_COUNT then
        dest.digital[SCREEN_ATTRACT_DI_SELECT_NEXT] = true
        __buffer_tt = 0
    elseif __buffer_tt <= -BUFFER_COUNT then
        dest.digital[SCREEN_ATTRACT_DI_SELECT_PREV] = true
        __buffer_tt = 0
    end

    if iop_input_state_is_pushed(IO_IIDXIO_DI_P1_START, src, src_prev) then
        out.digital[IO_IIDXIO_DO_P1_START] = true
        dest.digital[SCREEN_ATTRACT_DI_SELECTION_CONFIRMED] = true
    end

    if iop_input_state_is_pushed(IO_IIDXIO_DI_P2_START, src, src_prev) then
        out.digital[IO_IIDXIO_DO_P2_START] = true
        dest.digital[SCREEN_ATTRACT_DI_SELECTION_CONFIRMED] = true
    end

    for i = IO_IIDXIO_DO_P1_KEY1, IO_IIDXIO_DO_P2_KEY7 do
        out.digital[i] = src.digital[i]
    end

    dest.digital[SCREEN_ATTRACT_DI_OPERATOR_SCREEN] = iop_input_state_is_pushed(IO_IIDXIO_DI_TEST, src, src_prev)

    return dest, out
end

return {
    device = IO_IIDXIO_IDENTIFIER,
    screen = SCREEN_ATTRACT_IDENTIFIER,
    f_init = __init,
    f_activate = __activate,
    f_deactivate = __deactivate,
    f_process = __process,
}
