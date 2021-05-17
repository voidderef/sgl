#include "iidxio.h"

void iidx_io_set_loggers(log_formatter_t misc, log_formatter_t info,
        log_formatter_t warning, log_formatter_t fatal)
{
    // stub
}

bool iidx_io_init(thread_create_t thread_create, thread_join_t thread_join,
        thread_destroy_t thread_destroy)
{
    // stub
    return true;
}

void iidx_io_fini(void)
{
    // stub
}

void iidx_io_ep1_set_deck_lights(uint16_t deck_lights)
{
    // stub
}

void iidx_io_ep1_set_panel_lights(uint8_t panel_lights)
{
    // stub
}

void iidx_io_ep1_set_top_lamps(uint8_t top_lamps)
{
    // stub
}

void iidx_io_ep1_set_top_neons(bool top_neons)
{
    // stub
}

bool iidx_io_ep1_send(void)
{
    // stub
    return true;
}

bool iidx_io_ep2_recv(void)
{
    // stub
    return true;
}

uint8_t iidx_io_ep2_get_turntable(uint8_t player_no)
{
    // stub
    return 0;
}

uint8_t iidx_io_ep2_get_slider(uint8_t slider_no)
{
    // stub
    return 0;
}

uint8_t iidx_io_ep2_get_sys(void)
{
    // stub
    return 0;
}

uint8_t iidx_io_ep2_get_panel(void)
{
    // stub
    return 0;
}

uint16_t iidx_io_ep2_get_keys(void)
{
    // stub
    return 0;
}

bool iidx_io_ep3_write_16seg(const char *text)
{
    // stub
    return true;
}
