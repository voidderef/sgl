#include "ddrio.h"

void ddr_io_set_loggers(
    log_formatter_t misc,
    log_formatter_t info,
    log_formatter_t warning,
    log_formatter_t fatal)
{
    // stub
}

bool ddr_io_init(
    thread_create_t thread_create,
    thread_join_t thread_join,
    thread_destroy_t thread_destroy)
{
    // stub
    return true;
}

uint32_t ddr_io_read_pad(void)
{
    // stub
    return 0;
}

void ddr_io_set_lights_extio(uint32_t extio_lights)
{
    // stub
}

void ddr_io_set_lights_p3io(uint32_t p3io_lights)
{
    // stub
}

void ddr_io_set_lights_hdxs_panel(uint32_t hdxs_lights)
{
    // stub
}

void ddr_io_set_lights_hdxs_rgb(uint8_t idx, uint8_t r, uint8_t g, uint8_t b)
{
    // stub
}

void ddr_io_fini(void)
{
    // stub
}