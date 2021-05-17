#include "Usb.h"

#include "IOException.h"

namespace io {

Usb::Usb(uint16_t vid, uint16_t pid, uint16_t config, uint16_t interface) :
    m_vid(vid),
    m_pid(pid),
    m_config(config),
    m_interface(interface)
{
    m_ctx = nullptr;
    m_dev = nullptr;
}

void Usb::Open()
{
    int ret;

    ret = libusb_init(&m_ctx);

    if (ret != LIBUSB_SUCCESS) {
        throw IOException("(%04X:%04X) Initializing libusb context failed: %s", m_vid, m_pid, libusb_error_name(ret));
    }

    // dont print any error messages to stderr, change this for debugging
    libusb_set_debug(m_ctx, 0);

    try {
        m_dev = __GetDeviceHandle(m_ctx, m_vid, m_pid);
    } catch (...) {
        libusb_exit(m_ctx);
        throw;
    }

    ret = libusb_set_configuration(m_dev, m_config);

    if (ret != LIBUSB_SUCCESS) {
        libusb_close(m_dev);
        libusb_exit(m_ctx);

        throw IOException("(%04X:%04X) Setting configuration failed", m_vid, m_pid, libusb_error_name(ret));
    }

    // check if the device is attached to the kernel, detach it first then
    if (libusb_kernel_driver_active(m_dev, m_interface) == 1) {
        ret = libusb_detach_kernel_driver(m_dev, m_interface);

        if (ret != LIBUSB_SUCCESS) {
            libusb_close(m_dev);
            libusb_exit(m_ctx);

            throw IOException("(%04X:%04X) Detaching interface %d from kernel failed: %s", m_vid, m_pid, m_interface,
                libusb_error_name(ret));
        }
    }

    ret = libusb_claim_interface(m_dev, m_interface);

    if (ret != LIBUSB_SUCCESS) {
        libusb_close(m_dev);
        libusb_exit(m_ctx);

        throw IOException("(%04X:%04X) Claiming interface %d failed: %s", m_vid, m_pid, m_interface,
            libusb_error_name(ret));
    }
}

void Usb::ControlTransfer(uint8_t requestType, uint8_t request, uint16_t value, uint16_t index, uint8_t* data,
        uint16_t len, uint32_t timeoutMs)
{
    int32_t ret;

    ret = libusb_control_transfer(m_dev, requestType, request, value, index, data, len, timeoutMs);

    if (ret != len) {
        throw IOException("Control transfer, failed: %s", libusb_error_name(ret));
    }
}

void Usb::Close()
{
    libusb_close(m_dev);
    libusb_exit(m_ctx);

    m_dev = nullptr;
    m_ctx = nullptr;
}

libusb_device_handle* Usb::__GetDeviceHandle(libusb_context* ctx, uint16_t vid, uint16_t pid)
{
    int ret;
    struct libusb_device** devList;
    struct libusb_device* device;
    struct libusb_device_handle* handle;

    devList = nullptr;
    device = nullptr;

    ssize_t numDev = libusb_get_device_list(ctx, &devList);

    // find device
    for (ssize_t i = 0; i < numDev; i++) {
        libusb_device_descriptor devDescTmp {};
        libusb_device* devTmp;

        devTmp = devList[i];

        libusb_get_device_descriptor(devTmp, &devDescTmp);

        // for handling multiple devices with the same pid and vid
        // device_id_tmp = ((libusb_get_bus_number(dev_tmp) << 8) |
        //         libusb_get_port_number(dev_tmp));

        if (devDescTmp.idVendor == vid && devDescTmp.idProduct == pid) {
            device = devTmp;
            break;
        }
    }

    // not found
    if (!device) {
        libusb_free_device_list(devList, 1);
        throw IOException("Could not find device %04X:%04X", vid, pid);
    }

    ret = libusb_open(device, &handle);
    if (ret != LIBUSB_SUCCESS) {
        libusb_free_device_list(devList, 1);

        throw IOException("Could not open device %04X:%04X: %s", vid, pid, libusb_error_name(ret));
    }

    libusb_free_device_list(devList, 1);

    return handle;
}

}