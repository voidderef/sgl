package.path = './data/lua/?.lua;' .. package.path

require("io/iidxio/defs")
require("iop/output-anim")
require("iop/text-output-anim")
require("screen/attract/defs")

local ANIM_CYCLE_TIME_MS = 2000
local ANIM_ENTRY_CYCLE_TIME_MS = 700
local ANIM_TEXT_MOVE_TIME_MS = 200
local TEXT_16SEG_SIZE = 9

local __anim_light
local __anim_entry_light
local __anim_text

local STATE_IDLE = 0
local STATE_ENTRY = 1
local STATE_EXIT = 2

local __state = STATE_IDLE

local function __init()
    __anim_light = iop_output_anim_create(ANIM_CYCLE_TIME_MS)
    __anim_entry_light = iop_output_anim_create(ANIM_ENTRY_CYCLE_TIME_MS)
    __anim_text = iop_text_output_anim_create(TEXT_16SEG_SIZE, ANIM_TEXT_MOVE_TIME_MS)

    -- neons
    iop_output_anim_add_event_digital(__anim_light, IO_IIDXIO_DO_NEONS, 0, ANIM_CYCLE_TIME_MS / 2)

    -- deck keys
    for i = IO_IIDXIO_DO_P1_KEY1, IO_IIDXIO_DO_P2_KEY7 do
        iop_output_anim_add_event_digital(__anim_light, i, 0, ANIM_CYCLE_TIME_MS / 2)
    end

    -- start buttons
    for i = 0, 1 do
        iop_output_anim_add_event_digital(__anim_light, IO_IIDXIO_DO_P1_START,
            i * ANIM_CYCLE_TIME_MS / 2, ANIM_CYCLE_TIME_MS / 4)
        iop_output_anim_add_event_digital(__anim_light, IO_IIDXIO_DO_P2_START,
            i * ANIM_CYCLE_TIME_MS / 2, ANIM_CYCLE_TIME_MS / 4)
    end

    -- top lamps
    for i = 0, 15 do
        local lamp_left
        local lamp_right

        if i % 4 == 0 then
            lamp_left = IO_IIDXIO_DO_HALO_LEFT_RED
            lamp_right = IO_IIDXIO_DO_HALO_RIGHT_RED
        elseif i % 4 == 1 then
            lamp_left = IO_IIDXIO_DO_HALO_LEFT_YELLOW
            lamp_right = IO_IIDXIO_DO_HALO_RIGHT_YELLOW
        elseif i % 4 == 2 then
            lamp_left = IO_IIDXIO_DO_HALO_LEFT_GREEN
            lamp_right = IO_IIDXIO_DO_HALO_RIGHT_GREEN
        else
            lamp_left = IO_IIDXIO_DO_HALO_LEFT_BLUE
            lamp_right = IO_IIDXIO_DO_HALO_RIGHT_BLUE
        end

        iop_output_anim_add_event_digital(__anim_light, lamp_left, i * ANIM_CYCLE_TIME_MS / 16, ANIM_CYCLE_TIME_MS / 8)
        iop_output_anim_add_event_digital(__anim_light, lamp_right, i * ANIM_CYCLE_TIME_MS / 16, ANIM_CYCLE_TIME_MS / 8)
    end

    -- for entry animation
    -- all top lamps on
    for i = IO_IIDXIO_DO_HALO_LEFT_RED, IO_IIDXIO_DO_HALO_RIGHT_RED do
        iop_output_anim_add_event_digital(__anim_entry_light, i, 0, ANIM_ENTRY_CYCLE_TIME_MS)
    end

    -- neons on
    iop_output_anim_add_event_digital(__anim_entry_light, IO_IIDXIO_DO_NEONS, 0, ANIM_ENTRY_CYCLE_TIME_MS)

    -- start button animation
    for i = 0, 3 do
        iop_output_anim_add_event_digital(__anim_entry_light, IO_IIDXIO_DO_P1_START,
            i * ANIM_ENTRY_CYCLE_TIME_MS / 2, ANIM_ENTRY_CYCLE_TIME_MS / 8)
        iop_output_anim_add_event_digital(__anim_entry_light, IO_IIDXIO_DO_P2_START,
            i * ANIM_ENTRY_CYCLE_TIME_MS / 2, ANIM_ENTRY_CYCLE_TIME_MS / 8)
    end

    -- deck keys animation
    for i = IO_IIDXIO_DO_P1_KEY1, IO_IIDXIO_DO_P1_KEY7 do
        iop_output_anim_add_event_digital(__anim_entry_light, i,
            i * ANIM_ENTRY_CYCLE_TIME_MS / 14, ANIM_ENTRY_CYCLE_TIME_MS / 14)
        iop_output_anim_add_event_digital(__anim_entry_light, IO_IIDXIO_DO_P2_KEY7 - i,
            i * ANIM_ENTRY_CYCLE_TIME_MS / 14, ANIM_ENTRY_CYCLE_TIME_MS / 14)
    end

    for i = IO_IIDXIO_DO_P1_KEY1, IO_IIDXIO_DO_P1_KEY7 do
        iop_output_anim_add_event_digital(__anim_entry_light, 6 - i,
            ANIM_ENTRY_CYCLE_TIME_MS / 2 + i * ANIM_ENTRY_CYCLE_TIME_MS / 14, ANIM_ENTRY_CYCLE_TIME_MS / 14)
        iop_output_anim_add_event_digital(__anim_entry_light, 7 + i,
            ANIM_ENTRY_CYCLE_TIME_MS / 2 + i * ANIM_ENTRY_CYCLE_TIME_MS / 14, ANIM_ENTRY_CYCLE_TIME_MS / 14)
    end
end

local function __activate()
    iop_output_anim_start(__anim_light)
    iop_text_output_anim_start(__anim_text)

    __state = STATE_IDLE
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

    -- new entry selected event
    if src.digital[SCREEN_ATTRACT_DO_NEW_ENTRY_SELECTED] then
        -- restart text animation
        -- 16seg supports upper case, only
        iop_text_output_anim_set_text(__anim_text,
            string.upper(src.text[SCREEN_ATTRACT_TO_GAME_NAME_SELECTED]) .. "         ")
        iop_text_output_anim_start(__anim_text)
    end

    -- entry selected, decision event
    if src.digital[SCREEN_ATTRACT_DO_ENTRY_SELECTION_CONFIRMED] then
        iop_output_anim_start(__anim_entry_light)
        __state = STATE_ENTRY
    end

    if src.digital[SCREEN_ATTRACT_DO_EXIT_SCREEN] then
        for i = IO_IIDXIO_DO_P1_KEY1, IO_IIDXIO_DO_NEONS do
            dest.digital[i] = false
        end

        dest.text[IO_IIDX_FX_TO_16SEG] = "         "

        __state = STATE_EXIT
    end

    -- state machine for playing different animations
    if __state == STATE_IDLE then
        iop_output_anim_update(__anim_light, dest)
        iop_text_output_anim_update(__anim_text, dest, IO_IIDX_FX_TO_16SEG)
    elseif __state == STATE_ENTRY then
        dest.text[IO_IIDX_FX_TO_16SEG] = " DECIDE! "

        iop_output_anim_update(__anim_entry_light, dest)
    elseif __state == STATE_EXIT then
        -- do nothing
    end

    return dest
end

return {
    device = IO_IIDXIO_IDENTIFIER,
    screen = SCREEN_ATTRACT_IDENTIFIER,
    f_init = __init,
    f_activate = __activate,
    f_deactivate = __deactivate,
    f_process = __process,
}
