package.path = './data/lua/?.lua;' .. package.path

require("sgl")

local __id_tex_title = SGL_TEX_ID_INVALID
local __id_sprite_title = SGL_SPRITE_ID_INVALID

local function __load(table_resources, table_args)
    local res_tex_title = table_resources["tex_title"]

    if res_tex_title == nil then
        print("Missing tex_title resource entry")
        return false
    end

    local asset_paths = sgl_asset_paths()

    res_tex_title = asset_paths["title"] .. "/" .. res_tex_title

    -- load title
    __id_tex_title = sgl_tex_create(res_tex_title)
    __id_sprite_title = sgl_sprite_create(__id_tex_title)
    sgl_sprite_pos(__id_sprite_title, 0, 0)
    sgl_sprite_dim(__id_sprite_title, sgl_ren_screen_width(), sgl_ren_screen_height())

    return true
end

local function __unload()
    sgl_sprite_destroy(__id_sprite_title)
    sgl_tex_destroy(__id_tex_title)
end

local function __update(prev_delta_sec, input_state)

end

local function __on_enable()
    sgl_sprite_activate(__id_sprite_title, true)
end

local function __on_disable()
    sgl_sprite_activate(__id_sprite_title, false)
end

local function __on_confirm()
    return true
end

local function __on_transition()
    return true
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