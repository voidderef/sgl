package.path = './data/lua/?.lua;' .. package.path

require("sgl")

local __id_font_title = SGL_FONT_ID_INVALID
local __id_tex_title = SGL_TEX_ID_INVALID
local __id_sprite_title = SGL_SPRITE_ID_INVALID

local function __load(table_resources, table_args)
    local res_font_title = table_resources["font_title"]

    if res_font_title == nil then
        print("Missing font_title resource entry")
        return false
    end

    local asset_paths = sgl_asset_paths()

    local text = table_args["text"]
    local size = table_args["size"]
    local pos_x = table_args["pos_x"]
    local pos_y = table_args["pos_y"]
    local color_r = table_args["color_r"]
    local color_g = table_args["color_g"]
    local color_b = table_args["color_b"]
    local color_a = table_args["color_a"]

    -- all parameters except text optional
    if text == nil then
        print("Missing text args entry")
        return false
    end

    -- assign default values if no values specified
    size = size == nil and 32 or size
    pos_x = pos_x == nil and 20 or pos_x
    pos_y = pos_y == nil and 20 or pos_y
    color_r = color_r == nil and 0xFF or color_r
    color_g = color_g == nil and 0xFF or color_g
    color_b = color_b == nil and 0xFF or color_b
    color_a = color_a == nil and 0xFF or color_a

    res_font_title = asset_paths["font"] .. "/" .. res_font_title

    __id_font_title = sgl_font_create(res_font_title, size)

    __id_tex_title = sgl_tex_create_text(text, color_r, color_g, color_b, color_a, __id_font_title)
    __id_sprite_title = sgl_sprite_create(__id_tex_title)
    sgl_sprite_pos(__id_sprite_title, pos_x, pos_y)

    return true
end

local function __unload()
    sgl_sprite_destroy(__id_sprite_title)
    sgl_tex_destroy(__id_tex_title)
    sgl_font_destroy(__id_font_title)
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
    -- return true to execute the selected game once the transition is finished
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
