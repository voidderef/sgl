package.path = './data/lua/?.lua;' .. package.path

require("sgl")

local __mov_title_volume = 1.0
local __mov_trans_volume = 1.0

local __id_mov_title = SGL_MOV_ID_INVALID
local __id_mov_trans = SGL_MOV_ID_INVALID

local __id_sprite_title = SGL_SPRITE_ID_INVALID
local __id_sprite_trans = SGL_SPRITE_ID_INVALID

local __STATE_PLAYING_NONE = 0
local __STATE_PLAYING_TITLE = 1
local __STATE_PLAYING_TRANS = 2

local __state_playing = __STATE_PLAYING_TITLE

local __confirmation_in_trans = false

local function __play(state)
    -- stop current playing video first
    if __state_playing == __STATE_PLAYING_TITLE then
        sgl_sprite_activate(__id_sprite_title, false)
        sgl_mov_stop(__id_mov_title)
    elseif __state_playing == __STATE_PLAYING_TRANS then
        sgl_sprite_activate(__id_sprite_trans, false)
        sgl_mov_stop(__id_mov_trans)
    end

    __state_playing = state

    if __state_playing == __STATE_PLAYING_TITLE then
        sgl_mov_play(__id_mov_title)
        sgl_sprite_activate(__id_sprite_title, true)
    elseif __state_playing == __STATE_PLAYING_TRANS then
        sgl_mov_play(__id_mov_trans)
        sgl_sprite_activate(__id_sprite_trans, true)
    end
end

local function __load(table_resources, table_args)
    if not (table_args["mov_title_volume"] == nil) then
        __mov_title_volume = table_args["mov_title_volume"]
    end

    if not (table_args["mov_trans_volume"] == nil) then
        __mov_trans_volume = table_args["mov_trans_volume"]
    end

    local res_mov_title = table_resources["mov_title"]
    local res_mov_trans = table_resources["mov_trans"]

    if res_mov_title == nil then
        print("Missing mov_title resource entry")
        return false
    end

    if res_mov_trans == nil then
        print("Missing mov_trans resource entry")
        return false
    end

    local asset_paths = sgl_asset_paths()

    res_mov_title = asset_paths["mov"] .. "/" .. res_mov_title
    res_mov_trans = asset_paths["mov"] .. "/" .. res_mov_trans

    -- title video
    __id_mov_title = sgl_mov_create(res_mov_title)
    sgl_mov_loop(__id_mov_title, -1)
    sgl_mov_volume(__id_mov_title, __mov_title_volume)

    __id_sprite_title = sgl_sprite_create(sgl_mov_texture(__id_mov_title))
    sgl_sprite_pos(__id_sprite_title, 0, 0)
    sgl_sprite_dim(__id_sprite_title, sgl_ren_screen_width(), sgl_ren_screen_height())

    -- transition video
    __id_mov_trans = sgl_mov_create(res_mov_trans)
    sgl_mov_loop(__id_mov_trans, 0)
    sgl_mov_volume(__id_mov_trans, __mov_trans_volume)

    __id_sprite_trans = sgl_sprite_create(sgl_mov_texture(__id_mov_trans))
    sgl_sprite_pos(__id_sprite_trans, 0, 0)
    sgl_sprite_dim(__id_sprite_trans, sgl_ren_screen_width(), sgl_ren_screen_height())

    return true
end

local function __unload()
    -- title
    sgl_sprite_destroy(__id_sprite_title)
    sgl_mov_stop(__id_mov_title)
    sgl_mov_destroy(__id_mov_title)

    -- transition
    sgl_sprite_destroy(__id_sprite_trans)
    sgl_mov_stop(__id_mov_trans)
    sgl_mov_destroy(__id_mov_trans)
end

local function __update(prev_delta_sec, input_state)
    if __confirmation_in_trans == true then
        if sgl_mov_eof(__id_mov_trans) then
            __confirmation_in_trans = false
        end
    else
        -- don't process inputs on transition
        if input_state.digital[SCREEN_ATTRACT_DI_DEBUG_0] then
            __play(__STATE_PLAYING_TITLE)
        elseif input_state.digital[SCREEN_ATTRACT_DI_DEBUG_1] then
            __play(__STATE_PLAYING_TRANS)
        end

        if __state_playing == __STATE_PLAYING_TRANS and sgl_mov_eof(__id_mov_trans) then
            __play(__STATE_PLAYING_TITLE)
        end
    end
end

local function __on_enable()
    __play(__STATE_PLAYING_TITLE)
end

local function __on_disable()
    __play(__STATE_PLAYING_NONE)
end

local function __on_confirm()
    __play(__STATE_PLAYING_TRANS)
    __confirmation_in_trans = true

    return true
end

local function __on_transition()
    -- return true to execute the selected game once the transition is finished
    return not __confirmation_in_trans
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
