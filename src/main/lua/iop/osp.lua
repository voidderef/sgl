-- Example: input state structure/table
--[[
output_state = {
    digital = {
        true, false, true, false
    }

    analog = {
        0.1, 0.22
    }

    analog_u8 = {
        5, 255, 0
    }

    text = {
        "asdf"
    }
}
]]--


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
    device = "<io device name>",
    screen = "<screen name/identifier>",
    f_init = __init,
    f_activate = __activate,
    f_deactivate = __deactivate,
    f_process = __process,
}
