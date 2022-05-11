package.path = './data/lua/?.lua;' .. package.path

require("io/piulxio/defs")
require("iop/output-anim")
require("screen/attract/defs")

local ANIM_CAB_CYCLE_TIME_MS = 3000
local ANIM_PAD_CYCLE_TIME_MS = 2800

local ANIM_TIME_CAB_MS = 300
local ANIM_TIME_PAD_MS = 200

local __anim_cab
local __anim_pad
local __anim_button

local __selection_confirmed = false

local function __init()
    __anim_cab = iop_output_anim_create(ANIM_CAB_CYCLE_TIME_MS)
    __anim_pad = iop_output_anim_create(ANIM_PAD_CYCLE_TIME_MS)
    __anim_button = iop_output_anim_create(ANIM_CAB_CYCLE_TIME_MS)

    -- similar to nx title screen

    -- cabinet

    iop_output_anim_add_event_digital(__anim_cab, IO_PIUIOLXIO_DO_HALOL2, ANIM_TIME_CAB_MS * 0, ANIM_TIME_CAB_MS)
    iop_output_anim_add_event_digital(__anim_cab, IO_PIUIOLXIO_DO_HALOL1, ANIM_TIME_CAB_MS * 0, ANIM_TIME_CAB_MS)
    iop_output_anim_add_event_digital(__anim_cab, IO_PIUIOLXIO_DO_HALOR2, ANIM_TIME_CAB_MS * 1, ANIM_TIME_CAB_MS)
    iop_output_anim_add_event_digital(__anim_cab, IO_PIUIOLXIO_DO_HALOR1, ANIM_TIME_CAB_MS * 1, ANIM_TIME_CAB_MS)
    iop_output_anim_add_event_digital(__anim_cab, IO_PIUIOLXIO_DO_BASS, ANIM_TIME_CAB_MS * 2, ANIM_TIME_CAB_MS)

    iop_output_anim_add_event_digital(__anim_cab, IO_PIUIOLXIO_DO_HALOR2, ANIM_TIME_CAB_MS * 4, ANIM_TIME_CAB_MS)
    iop_output_anim_add_event_digital(__anim_cab, IO_PIUIOLXIO_DO_HALOR1, ANIM_TIME_CAB_MS * 4, ANIM_TIME_CAB_MS)
    iop_output_anim_add_event_digital(__anim_cab, IO_PIUIOLXIO_DO_HALOL2, ANIM_TIME_CAB_MS * 5, ANIM_TIME_CAB_MS)
    iop_output_anim_add_event_digital(__anim_cab, IO_PIUIOLXIO_DO_HALOL1, ANIM_TIME_CAB_MS * 5, ANIM_TIME_CAB_MS)
    iop_output_anim_add_event_digital(__anim_cab, IO_PIUIOLXIO_DO_BASS, ANIM_TIME_CAB_MS * 6, ANIM_TIME_CAB_MS)

    iop_output_anim_add_event_digital(__anim_cab, IO_PIUIOLXIO_DO_HALOL2, ANIM_TIME_CAB_MS * 8, ANIM_TIME_CAB_MS)
    iop_output_anim_add_event_digital(__anim_cab, IO_PIUIOLXIO_DO_HALOL1, ANIM_TIME_CAB_MS * 8, ANIM_TIME_CAB_MS)
    iop_output_anim_add_event_digital(__anim_cab, IO_PIUIOLXIO_DO_HALOR2, ANIM_TIME_CAB_MS * 8, ANIM_TIME_CAB_MS)
    iop_output_anim_add_event_digital(__anim_cab, IO_PIUIOLXIO_DO_HALOR1, ANIM_TIME_CAB_MS * 8, ANIM_TIME_CAB_MS)
    iop_output_anim_add_event_digital(__anim_cab, IO_PIUIOLXIO_DO_BASS, ANIM_TIME_CAB_MS * 8, ANIM_TIME_CAB_MS)

    -- pad

    iop_output_anim_add_event_digital(__anim_pad, IO_PIUIOLXIO_DO_P1_LU, ANIM_TIME_PAD_MS * 0, ANIM_TIME_PAD_MS)
    iop_output_anim_add_event_digital(__anim_pad, IO_PIUIOLXIO_DO_P1_RU, ANIM_TIME_PAD_MS * 1, ANIM_TIME_PAD_MS)
    iop_output_anim_add_event_digital(__anim_pad, IO_PIUIOLXIO_DO_P1_CN, ANIM_TIME_PAD_MS * 2, ANIM_TIME_PAD_MS)
    iop_output_anim_add_event_digital(__anim_pad, IO_PIUIOLXIO_DO_P1_LD, ANIM_TIME_PAD_MS * 3, ANIM_TIME_PAD_MS)
    iop_output_anim_add_event_digital(__anim_pad, IO_PIUIOLXIO_DO_P1_RD, ANIM_TIME_PAD_MS * 4, ANIM_TIME_PAD_MS)
    iop_output_anim_add_event_digital(__anim_pad, IO_PIUIOLXIO_DO_P1_RU, ANIM_TIME_PAD_MS * 5, ANIM_TIME_PAD_MS)
    iop_output_anim_add_event_digital(__anim_pad, IO_PIUIOLXIO_DO_P1_LU, ANIM_TIME_PAD_MS * 6, ANIM_TIME_PAD_MS)
    iop_output_anim_add_event_digital(__anim_pad, IO_PIUIOLXIO_DO_P1_CN, ANIM_TIME_PAD_MS * 7, ANIM_TIME_PAD_MS)
    iop_output_anim_add_event_digital(__anim_pad, IO_PIUIOLXIO_DO_P1_RD, ANIM_TIME_PAD_MS * 8, ANIM_TIME_PAD_MS)
    iop_output_anim_add_event_digital(__anim_pad, IO_PIUIOLXIO_DO_P1_LD, ANIM_TIME_PAD_MS * 9, ANIM_TIME_PAD_MS)

    iop_output_anim_add_event_digital(__anim_pad, IO_PIUIOLXIO_DO_P1_LU, 2200, 400)
    iop_output_anim_add_event_digital(__anim_pad, IO_PIUIOLXIO_DO_P1_RU, 2200, 400)
    iop_output_anim_add_event_digital(__anim_pad, IO_PIUIOLXIO_DO_P1_CN, 2200, 400)
    iop_output_anim_add_event_digital(__anim_pad, IO_PIUIOLXIO_DO_P1_LD, 2200, 400)
    iop_output_anim_add_event_digital(__anim_pad, IO_PIUIOLXIO_DO_P1_RD, 2200, 400)

    iop_output_anim_add_event_digital(__anim_pad, IO_PIUIOLXIO_DO_P2_RU, ANIM_TIME_PAD_MS * 0, ANIM_TIME_PAD_MS)
    iop_output_anim_add_event_digital(__anim_pad, IO_PIUIOLXIO_DO_P2_LU, ANIM_TIME_PAD_MS * 1, ANIM_TIME_PAD_MS)
    iop_output_anim_add_event_digital(__anim_pad, IO_PIUIOLXIO_DO_P2_CN, ANIM_TIME_PAD_MS * 2, ANIM_TIME_PAD_MS)
    iop_output_anim_add_event_digital(__anim_pad, IO_PIUIOLXIO_DO_P2_RD, ANIM_TIME_PAD_MS * 3, ANIM_TIME_PAD_MS)
    iop_output_anim_add_event_digital(__anim_pad, IO_PIUIOLXIO_DO_P2_LD, ANIM_TIME_PAD_MS * 4, ANIM_TIME_PAD_MS)
    iop_output_anim_add_event_digital(__anim_pad, IO_PIUIOLXIO_DO_P2_LU, ANIM_TIME_PAD_MS * 5, ANIM_TIME_PAD_MS)
    iop_output_anim_add_event_digital(__anim_pad, IO_PIUIOLXIO_DO_P2_RU, ANIM_TIME_PAD_MS * 6, ANIM_TIME_PAD_MS)
    iop_output_anim_add_event_digital(__anim_pad, IO_PIUIOLXIO_DO_P2_CN, ANIM_TIME_PAD_MS * 7, ANIM_TIME_PAD_MS)
    iop_output_anim_add_event_digital(__anim_pad, IO_PIUIOLXIO_DO_P2_LD, ANIM_TIME_PAD_MS * 8, ANIM_TIME_PAD_MS)
    iop_output_anim_add_event_digital(__anim_pad, IO_PIUIOLXIO_DO_P2_RD, ANIM_TIME_PAD_MS * 9, ANIM_TIME_PAD_MS)

    iop_output_anim_add_event_digital(__anim_pad, IO_PIUIOLXIO_DO_P2_LU, 2200, 400)
    iop_output_anim_add_event_digital(__anim_pad, IO_PIUIOLXIO_DO_P2_RU, 2200, 400)
    iop_output_anim_add_event_digital(__anim_pad, IO_PIUIOLXIO_DO_P2_CN, 2200, 400)
    iop_output_anim_add_event_digital(__anim_pad, IO_PIUIOLXIO_DO_P2_LD, 2200, 400)
    iop_output_anim_add_event_digital(__anim_pad, IO_PIUIOLXIO_DO_P2_RD, 2200, 400)


    -- buttons
    iop_output_anim_add_event_digital(__anim_button, IO_PIUIOLXIO_DO_P1_LD_MENU, ANIM_TIME_CAB_MS * 1, ANIM_TIME_CAB_MS)
    iop_output_anim_add_event_digital(__anim_button, IO_PIUIOLXIO_DO_P1_LD_MENU, ANIM_TIME_CAB_MS * 3, ANIM_TIME_CAB_MS)
    iop_output_anim_add_event_digital(__anim_button, IO_PIUIOLXIO_DO_P1_RD_MENU, ANIM_TIME_CAB_MS * 5, ANIM_TIME_CAB_MS)
    iop_output_anim_add_event_digital(__anim_button, IO_PIUIOLXIO_DO_P1_RD_MENU, ANIM_TIME_CAB_MS * 7, ANIM_TIME_CAB_MS)
    iop_output_anim_add_event_digital(__anim_button, IO_PIUIOLXIO_DO_P1_CN_MENU, ANIM_TIME_CAB_MS * 9, ANIM_TIME_CAB_MS * 2)

    iop_output_anim_add_event_digital(__anim_button, IO_PIUIOLXIO_DO_P2_RD_MENU, ANIM_TIME_CAB_MS * 1, ANIM_TIME_CAB_MS)
    iop_output_anim_add_event_digital(__anim_button, IO_PIUIOLXIO_DO_P2_RD_MENU, ANIM_TIME_CAB_MS * 3, ANIM_TIME_CAB_MS)
    iop_output_anim_add_event_digital(__anim_button, IO_PIUIOLXIO_DO_P2_LD_MENU, ANIM_TIME_CAB_MS * 5, ANIM_TIME_CAB_MS)
    iop_output_anim_add_event_digital(__anim_button, IO_PIUIOLXIO_DO_P2_LD_MENU, ANIM_TIME_CAB_MS * 7, ANIM_TIME_CAB_MS)
    iop_output_anim_add_event_digital(__anim_button, IO_PIUIOLXIO_DO_P2_CN_MENU, ANIM_TIME_CAB_MS * 9, ANIM_TIME_CAB_MS * 2)

end

local function __activate()
    iop_output_anim_start(__anim_cab)
    iop_output_anim_start(__anim_pad)
    iop_output_anim_start(__anim_button)

    __selection_confirmed = false
end

local function __deactivate()

end

local function __process(src)
    local dest = {
        digital = {},
        analog = {},
        analog_u8 = {},
        text = {}
    }

    if src.digital[SCREEN_ATTRACT_DO_EXIT_SCREEN] == true then
        -- turn all lights off
        for i = IO_PIUIOLXIO_DO_P1_LU, IO_PIUIOLXIO_DO_HALOL1 do
            dest.digital[i] = false
        end
    elseif __selection_confirmed == true or src.digital[SCREEN_ATTRACT_DO_ENTRY_SELECTION_CONFIRMED] == true then
        -- all lights on
        for i = IO_PIUIOLXIO_DO_P1_LU, IO_PIUIOLXIO_DO_HALOL1 do
            dest.digital[i] = true
        end

        __selection_confirmed = true
    else
        local light_state = src.analog_u8[SCREEN_ATTRACT_AO_LIGHT_ANIMATION_STATE]

        if light_state == SCREEN_ATTRACT_LA_ATTRACT then
            iop_output_anim_update(__anim_cab, dest)
            iop_output_anim_update(__anim_pad, dest)
            iop_output_anim_update(__anim_button, dest)
        elseif light_state == SCREEN_ATTRACT_LA_ENTRY then
            iop_output_anim_update(__anim_cab, dest)
            iop_output_anim_update(__anim_pad, dest)
            iop_output_anim_update(__anim_button, dest)
        elseif light_state == SCREEN_ATTRACT_LA_OFF then
            -- execution, turn everything off
            -- restart animation if application can't be started
            iop_output_anim_start(__anim_cab)
            iop_output_anim_start(__anim_pad)
            iop_output_anim_start(__anim_button)

            for i = IO_PIUIOLXIO_DO_P1_LU, IO_PIUIOLXIO_DO_HALOL1 do
                dest.digital[i] = false
            end
        end
    end

    return dest
end

return {
    device = IO_PIULXIO_IDENTIFIER,
    screen = SCREEN_ATTRACT_IDENTIFIER,
    f_init = __init,
    f_activate = __activate,
    f_deactivate = __deactivate,
    f_process = __process,
}
