package.path = './data/lua/?.lua;' .. package.path

require("sgl")
require("screen/attract/defs")

local __aud_title_volume = 1.0

local __fade_out_time_sec = 1.5

local __id_snd_title = SGL_SOUND_ID_INVALID

local __id_mov_title_start = SGL_MOV_ID_INVALID
local __id_mov_title_loop = SGL_MOV_ID_INVALID

local __id_sprite_title_start = SGL_SPRITE_ID_INVALID
local __id_sprite_title_loop = SGL_SPRITE_ID_INVALID

local __STATE_PLAYING_NONE = 0
local __STATE_PLAYING_TITLE_START = 1
local __STATE_PLAYING_TITLE_LOOP = 2

local __state_playing = __STATE_PLAYING_TITLE_START

local __confirmation_fade_out = false
local __confirmation_fade_out_timer = 0

local function __play(state)
    -- stop current playing video first
    if __state_playing == __STATE_PLAYING_TITLE_START then
        sgl_sprite_activate(__id_sprite_title_start, false)
        sgl_mov_stop(__id_mov_title_start)
    elseif __state_playing == __STATE_PLAYING_TITLE_LOOP then
        sgl_sprite_activate(__id_sprite_title_loop, false)
        sgl_mov_stop(__id_mov_title_loop)
    end

    __state_playing = state

    if __state_playing == __STATE_PLAYING_TITLE_START then
        sgl_mov_play(__id_mov_title_start)
        sgl_sprite_activate(__id_sprite_title_start, true)
        sgl_snd_play(__id_snd_title)
    elseif __state_playing == __STATE_PLAYING_TITLE_LOOP then
        sgl_mov_play(__id_mov_title_loop)
        sgl_sprite_activate(__id_sprite_title_loop, true)
    elseif  __state_playing == __STATE_PLAYING_TITLE_LOOP then
        sgl_snd_stop(__id_snd_title)
    end
end

local function __load(table_resources, table_args)
    if not (table_args["aud_title_volume"] == nil) then
        __aud_title_volume = table_args["aud_title_volume"]
    end

    if not (table_args["fade_out_time_sec"] == nil) then
        __fade_out_time_sec = table_args["fade_out_time_sec"]
    end

    local res_aud_title = table_resources["aud_title"]
    local res_mov_title_start = table_resources["mov_title_start"]
    local res_mov_title_loop = table_resources["mov_title_loop"]

    if res_aud_title == nil then
        print("Missing aud_title resource entry")
        return false
    end

    if res_mov_title_start == nil then
        print("Missing mov_title_start resource entry")
        return false
    end

    if res_mov_title_loop == nil then
        print("Missing mov_title_loop resource entry")
        return false
    end

    local asset_paths = sgl_asset_paths()

    res_aud_title = asset_paths["aud"] .. "/" .. res_aud_title
    res_mov_title_start = asset_paths["mov"] .. "/" .. res_mov_title_start
    res_mov_title_loop = asset_paths["mov"] .. "/" .. res_mov_title_loop

    -- load audio for title
    __id_snd_title = sgl_snd_create(res_aud_title)
    sgl_snd_loop(__id_snd_title, -1)
    sgl_snd_volume(__id_snd_title, __aud_title_volume)

    -- load the start video which preceeds the loop
    -- Use external sound file, not sound from video
    __id_mov_title_start = sgl_mov_create(res_mov_title_start, false)
    -- Don't loop, followed by loop section
    sgl_mov_loop(__id_mov_title_start, 0)
    sgl_mov_volume(__id_mov_title_start, 0.0)

    __id_sprite_title_start = sgl_sprite_create(sgl_mov_texture(__id_mov_title_start))
    sgl_sprite_pos(__id_sprite_title_start, 0, 0)
    sgl_sprite_dim(__id_sprite_title_start, sgl_ren_screen_width(), sgl_ren_screen_height())

    -- load loop video which follows the start part
    -- Use external sound file, not sound from video
    __id_mov_title_loop = sgl_mov_create(res_mov_title_loop, false)
    -- Endless loop, sound file determines loop count and end
    sgl_mov_loop(__id_mov_title_loop, -1)
    sgl_mov_volume(__id_mov_title_loop, 0.0)

    __id_sprite_title_loop = sgl_sprite_create(sgl_mov_texture(__id_mov_title_loop))
    sgl_sprite_pos(__id_sprite_title_loop, 0, 0)
    sgl_sprite_dim(__id_sprite_title_loop, sgl_ren_screen_width(), sgl_ren_screen_height())

    return true
end

local function __unload()
    -- title audio
    sgl_snd_stop(__id_snd_title)
    sgl_snd_destroy(__id_snd_title)

    -- title start
    sgl_sprite_destroy(__id_sprite_title_start)
    sgl_mov_stop(__id_mov_title_start)
    sgl_mov_destroy(__id_mov_title_start)

    -- title loop
    sgl_sprite_destroy(__id_sprite_title_loop)
    sgl_mov_stop(__id_mov_title_loop)
    sgl_mov_destroy(__id_mov_title_loop)
end

local function __update(prev_delta_sec, input_state)
    if __confirmation_fade_out then
        __confirmation_fade_out_timer = __confirmation_fade_out_timer + prev_delta_sec

        if __confirmation_fade_out_timer > __fade_out_time_sec then
            -- fade out finished
            __confirmation_fade_out = false
        else
            local fade_perc = (__fade_out_time_sec - __confirmation_fade_out_timer) / __fade_out_time_sec

            -- fade out only possible when playing title
            sgl_mov_volume(__id_mov_title_loop, fade_perc)
            sgl_sprite_alpha(__id_sprite_title_loop, math.floor(fade_perc * 0xFF))
        end
    else
        -- don't process inputs on transition
        if input_state.digital[SCREEN_ATTRACT_DI_DEBUG_0] then
            __play(__STATE_PLAYING_TITLE_START)
        elseif input_state.digital[SCREEN_ATTRACT_DI_DEBUG_1] then
            __play(__STATE_PLAYING_TITLE_LOOP)
        end
    end

    if __state_playing == __STATE_PLAYING_TITLE_START and sgl_mov_eof(__id_mov_title_start) then
        __play(__STATE_PLAYING_TITLE_LOOP)
    elseif __state_playing == __STATE_PLAYING_TITLE_LOOP and sgl_mov_eof(__id_mov_title_loop) then
        __play(__STATE_PLAYING_TITLE_LOOP)
    end
end

local function __on_enable()
    __play(__STATE_PLAYING_TITLE_START)
end

local function __on_disable()
    __play(__STATE_PLAYING_NONE)
end

local function __on_confirm()
    if __state_playing == __STATE_PLAYING_TITLE_START then
        __play(__STATE_PLAYING_TITLE_LOOP)
        return false
    else
        __confirmation_fade_out = true
        return true
    end
end

local function __on_transition()
    return not __confirmation_fade_out
end

return {
    f_load = __load,
    f_unload = __unload,
    f_update = __update,
    f_on_enable = __on_enable,
    f_on_disable = __on_disable,
    f_on_confirm = __on_confirm,
    f_on_transition = __on_transition,
}
