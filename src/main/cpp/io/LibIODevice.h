#pragma once

#include "IODevice.h"
#include "IOManagerCtx.h"

static const char* io_device_instance_func_name = "io_device_instance";
typedef io::IODevice* (*io_device_instance_t)(const io::IOManagerCtx& ioManagerCtx);

extern "C" {
    io::IODevice* io_device_instance(const io::IOManagerCtx& ioManagerCtx);
};
