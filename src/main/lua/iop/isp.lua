-- Example: input state structure/table
--[[
input_state = {
    digital = {
        true, false, true, false
    }

    analog = {
        0.1, 0.55, 0
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

local function __process(src, src_prev)
    local dest = {
        digital = {},
        analog = {},
    }

    local out = {
        digital = {},
        analog = {},
        analog_u8 = {},
        text = {}
    }

     -- stub

    return dest, out
end

return {
    device = "<io device name>",
    screen = "<screen name/identifier>",
    f_init = __init,
    f_activate = __activate,
    f_deactivate = __deactivate,
    f_process = __process,
}
