package.path = './data/lua/?.lua;' .. package.path

require("io/piubtn/defs")
require("iop/output-anim")
require("screen/attract/defs")

local ANIM_CYCLE_TIME_MS = 2000

local __anim_light

local __selection_confirmed = false

local function __init()
    __anim_light = iop_output_anim_create(ANIM_CYCLE_TIME_MS)

    for i = IO_PIUBTN_DO_P1_LEFT, IO_PIUBTN_DO_P2_SELECT do
        iop_output_anim_add_event_digital(__anim_light, i, 0, ANIM_CYCLE_TIME_MS / 2)
    end
end

local function __activate()
    iop_output_anim_start(__anim_light)

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
        for i = IO_PIUBTN_DO_P1_LEFT, IO_PIUBTN_DO_P2_SELECT do
            dest.digital[i] = false
        end
    elseif __selection_confirmed == true or src.digital[SCREEN_ATTRACT_DO_ENTRY_SELECTION_CONFIRMED] == true then
        -- all lights on
        for i = IO_PIUBTN_DO_P1_LEFT, IO_PIUBTN_DO_P2_SELECT do
            dest.digital[i] = true
        end

        __selection_confirmed = true
    else
        local light_state = src.analog_u8[SCREEN_ATTRACT_AO_LIGHT_ANIMATION_STATE]

        if light_state == SCREEN_ATTRACT_LA_ATTRACT then
            iop_output_anim_update(__anim_light, dest)
        elseif light_state == SCREEN_ATTRACT_LA_ENTRY then
            iop_output_anim_update(__anim_light, dest)
        elseif light_state == SCREEN_ATTRACT_LA_OFF then
            -- execution, turn everything off
            -- restart animation if application can't be started
            iop_output_anim_start(__anim_light)

            for i = IO_PIUBTN_DO_P1_LEFT, IO_PIUBTN_DO_P2_SELECT do
                dest.digital[i] = false
            end
        end
    end

    return dest
end

return {
    device = IO_PIUBTN_IDENTIFIER,
    screen = SCREEN_ATTRACT_IDENTIFIER,
    f_init = __init,
    f_activate = __activate,
    f_deactivate = __deactivate,
    f_process = __process,
}
