package.path = './data/lua/?.lua;' .. package.path

require("io/ddrio/defs")
require("iop/output-anim")
require("screen/attract/defs")

local function __init()
   
end

local function __activate()
  
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

    return dest
end

return {
    device = IO_DDRIO_IDENTIFIER,
    screen = SCREEN_ATTRACT_IDENTIFIER,
    f_init = __init,
    f_activate = __activate,
    f_deactivate = __deactivate,
    f_process = __process,
}
