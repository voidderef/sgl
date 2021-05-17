function iop_input_state_is_pushed(id, cur, prev)
    return cur.digital[id] and not prev.digital[id]
end

function iop_input_state_is_released(id, cur, prev)
    return not cur.digital[id] and prev.digital[id]
end

function iop_input_state_is_held(id, cur, prev)
    return cur.digital[id] and prev.digital[id]
end

function iop_input_state_is_incremented(id, delta, cur, prev)
    local prevVal = prev.analog[id]
    local curVal = cur.analog[id]

    -- detect overflow
    return (prevVal - curVal > 0.5) or (curVal - prevVal >= delta)
end

function iop_input_state_is_decremented(id, delta, cur, prev)
    local prevVal = prev.analog[id]
    local curVal = cur.analog[id]

    -- detect underflow
    return (curVal - prevVal > 0.5) or (prevVal - curVal >= delta)
end
