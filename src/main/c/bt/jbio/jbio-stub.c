#include "jbio.h"

void jb_io_set_loggers(log_formatter_t misc, log_formatter_t info,
        log_formatter_t warning, log_formatter_t fatal)
{
    // stub
}

bool jb_io_init(thread_create_t thread_create, thread_join_t thread_join,
        thread_destroy_t thread_destroy)
{
    // stub
    return true;
}

void jb_io_fini(void)
{
    // stub
}

bool jb_io_read_inputs(void)
{
    // stub
    return true;
}

bool jb_io_write_outputs(void)
{
    // stub
    return true;
}

uint8_t jb_io_get_sys_inputs(void)
{
    // stub
    return 0;
}

uint16_t jb_io_get_panel_inputs(void)
{
    // stub
    return 0;
}

void jb_io_set_rgb_led(enum jb_io_rgb_led unit, uint8_t r, uint8_t g, uint8_t b)
{
    // stub
}