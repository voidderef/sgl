local function __sort_func_event(a, b)
    return a.timestamp_ms < b.timestamp_ms
end

function iop_output_anim_create(cycle_time_ms)
    self = {
        cycle_time_ms = cycle_time_ms,
        cycle_counter = 0,
        start_time_ms = 0,
        events = {

        },
    }

    return self
end

function iop_output_anim_add_event_digital(self, id, timestamp_ms, duration_ms)
    table.insert(self.events, {
        id = id,
        timestamp_ms = timestamp_ms,
        duration_ms = duration_ms,
    })

    table.sort(self.events, __sort_func_event)
end

function iop_output_anim_start(self)
    self.cycle_counter = 0
    self.start_time_ms = sgl_time_now_ms()
end

function iop_output_anim_update(self, output_state)
    local time_ms = (sgl_time_now_ms() - self.start_time_ms) % self.cycle_time_ms

    for i = 1, #self.events do
        if time_ms >= self.events[i].timestamp_ms and time_ms < self.events[i].timestamp_ms + self.events[i].duration_ms then
            output_state.digital[self.events[i].id] = true
        end
    end
end
