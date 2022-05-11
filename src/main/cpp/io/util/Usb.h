#pragma once

#include <cstdint>

#ifdef IO_PLATFORM_LINUX
#include <libusb-1.0/libusb.h>
#else
#error "Unsupported platform"
#endif

namespace io {

class Usb
{
public:
    Usb(uint16_t vid, uint16_t pid, uint16_t config, uint16_t interface);

    ~Usb() = default;

    uint16_t GetVid() const {
        return m_vid;
    }

    uint16_t GetPid() const {
        return m_pid;
    }

    uint16_t GetConfig() const {
        return m_config;
    }

    uint16_t GetInterface() const {
        return m_interface;
    }

    void Open();

    void ControlTransfer(uint8_t requestType, uint8_t request, uint16_t value, uint16_t index, uint8_t* data,
        uint16_t len, uint32_t timeoutMs);

    void IntTransfer(uint8_t request, uint8_t* data, uint16_t len, uint32_t timeoutMs);


    void Close();

private:
    uint16_t m_vid;
    uint16_t m_pid;
    uint16_t m_config;
    uint16_t m_interface;

private:
    libusb_context* m_ctx;
    libusb_device_handle* m_dev;

private:
    libusb_device_handle* __GetDeviceHandle(libusb_context* ctx, uint16_t vid,
        uint16_t pid);
};

}
