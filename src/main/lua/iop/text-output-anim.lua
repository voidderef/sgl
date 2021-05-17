function iop_text_output_anim_create(display_size, cycle_time_ms)
    self = {
        display_size = display_size,
        cycle_time_ms = cycle_time_ms,
        pos = 0,
        start_time_ms = 0,
        text = "",
        display_text = "",
    }

    return self
end

function iop_text_output_anim_set_text(self, text)
    self.text = text
end

function iop_text_output_anim_start(self)
    self.pos = 0
    self.start_time_ms = sgl_time_now_ms()
end

function iop_text_output_anim_update(self, output_state, id)
    local time_ms = sgl_time_now_ms() - self.start_time_ms
    local text_len = string.len(self.text)

    if text_len > 0 then
        if time_ms >= self.cycle_time_ms then
            self.start_time_ms = sgl_time_now_ms()

            self.pos = (self.pos + 1) % string.len(self.text)
        end

        self.display_text = ""

        for i = 0, self.display_size do
            local pos = (self.pos + i) % text_len
            self.display_text = self.display_text .. string.sub(self.text, pos, pos)
        end
    else
        for i = 0, self.display_size do
            self.display_text = self.display_text .. " "
        end
    end

    output_state.text[id] = self.display_text
end
