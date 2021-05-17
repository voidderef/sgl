package.path = './data/lua/?.lua;' .. package.path

require("io/jbio/defs")
require("iop/output-anim")
require("screen/attract/defs")

local function __init()
    -- stub
end

local function __activate()
    -- stub
end

local function __deactivate()
    -- stub
end

local function __process(src)
    local dest = {
        digital = {},
        analog = {},
        analog_u8 = {},
        text = {}
    }

    -- stub

    return dest
end

return {
    device = IO_JBIO_IDENTIFIER,
    screen = SCREEN_ATTRACT_IDENTIFIER,
    f_init = __init,
    f_activate = __activate,
    f_deactivate = __deactivate,
    f_process = __process,
}
