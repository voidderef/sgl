package.path = './data/lua/?.lua;' .. package.path

require("sgl")
require("screen/attract/defs")

local __mov_title_volume = 1.0
local __mov_intro_volume = 1.0

local __mov_title_loop = 0
local __fade_out_time_sec = 1.5

local __id_mov_title = SGL_MOV_ID_INVALID
local __id_mov_intro = SGL_MOV_ID_INVALID

local __id_sprite_title = SGL_SPRITE_ID_INVALID
local __id_sprite_intro = SGL_SPRITE_ID_INVALID

local __STATE_PLAYING_NONE = 0
local __STATE_PLAYING_TITLE = 1
local __STATE_PLAYING_INTRO = 2

local __state_playing = __STATE_PLAYING_TITLE

local __confirmation_fade_out = false
local __confirmation_fade_out_timer = 0

local function __play(state)
    -- stop current playing video first
    if __state_playing == __STATE_PLAYING_TITLE then
        sgl_sprite_activate(__id_sprite_title, false)
        sgl_mov_stop(__id_mov_title)
    elseif __state_playing == __STATE_PLAYING_INTRO then
        sgl_sprite_activate(__id_sprite_intro, false)
        sgl_mov_stop(__id_mov_intro)
    end

    __state_playing = state

    if __state_playing == __STATE_PLAYING_TITLE then
        sgl_mov_play(__id_mov_title)
        sgl_sprite_activate(__id_sprite_title, true)
    elseif __state_playing == __STATE_PLAYING_INTRO then
        sgl_mov_play(__id_mov_intro)
        sgl_sprite_activate(__id_sprite_intro, true)
    end
end

local function __load(table_resources, table_args)
    if not (table_args["mov_title_volume"] == nil) then
        __mov_title_volume = table_args["mov_title_volume"]
    end

    if not (table_args["mov_intro_volume"] == nil) then
        __mov_intro_volume = table_args["mov_intro_volume"]
    end

    if not (table_args["mov_title_loop"] == nil) then
        __mov_title_loop = table_args["mov_title_loop"]
    end

    if not (table_args["fade_out_time_sec"] == nil) then
        __fade_out_time_sec = table_args["fade_out_time_sec"]
    end

    local res_mov_title = table_resources["mov_title"]
    local res_mov_intro = table_resources["mov_intro"]

    if res_mov_title == nil then
        print("Missing mov_title resource entry")
        return false
    end

    if res_mov_intro == nil then
        print("Missing mov_intro resource entry")
        return false
    end

    local asset_paths = sgl_asset_paths()

    res_mov_title = asset_paths["mov"] .. "/" .. res_mov_title
    res_mov_intro = asset_paths["mov"] .. "/" .. res_mov_intro

    -- load title
    __id_mov_title = sgl_mov_create(res_mov_title)
    sgl_mov_loop(__id_mov_title, __mov_title_loop)
    sgl_mov_volume(__id_mov_title, __mov_title_volume)

    __id_sprite_title = sgl_sprite_create(sgl_mov_texture(__id_mov_title))
    sgl_sprite_pos(__id_sprite_title, 0, 0)
    sgl_sprite_dim(__id_sprite_title, sgl_ren_screen_width(), sgl_ren_screen_height())

    -- load intro
    __id_mov_intro = sgl_mov_create(res_mov_intro)
    sgl_mov_loop(__id_mov_intro, 0)
    sgl_mov_volume(__id_mov_intro, __mov_intro_volume)

    __id_sprite_intro = sgl_sprite_create(sgl_mov_texture(__id_mov_intro))
    sgl_sprite_pos(__id_sprite_intro, 0, 0)
    sgl_sprite_dim(__id_sprite_intro, sgl_ren_screen_width(), sgl_ren_screen_height())

    return true
end

local function __unload()
    -- title
    sgl_sprite_destroy(__id_sprite_title)
    sgl_mov_stop(__id_mov_title)
    sgl_mov_destroy(__id_mov_title)

    -- intro
    sgl_sprite_destroy(__id_sprite_intro)
    sgl_mov_stop(__id_mov_intro)
    sgl_mov_destroy(__id_mov_intro)
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
            sgl_mov_volume(__id_mov_title, fade_perc)
            sgl_sprite_alpha(__id_sprite_title, math.floor(fade_perc * 0xFF))
        end
    else
        -- don't process inputs on transition
        if input_state.digital[SCREEN_ATTRACT_DI_DEBUG_0] then
            __play(__STATE_PLAYING_TITLE)
        elseif input_state.digital[SCREEN_ATTRACT_DI_DEBUG_1] then
            __play(__STATE_PLAYING_INTRO)
        end
    end

    if __state_playing == __STATE_PLAYING_TITLE and sgl_mov_eof(__id_mov_title) then
        __play(__STATE_PLAYING_INTRO)
    elseif __state_playing == __STATE_PLAYING_INTRO and sgl_mov_eof(__id_mov_intro) then
        __play(__STATE_PLAYING_TITLE)
    end
end

local function __on_enable()
    __play(__STATE_PLAYING_TITLE)
end

local function __on_disable()
    __play(__STATE_PLAYING_NONE)
end

local function __on_confirm()
    if __state_playing == __STATE_PLAYING_INTRO then
        __play(__STATE_PLAYING_TITLE)
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
