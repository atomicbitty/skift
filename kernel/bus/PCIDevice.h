#pragma once

#include <libsystem/Common.h>

struct PCIDevice
{
    int vendor;
    int device;

    int bus;
    int slot;
    int func;
} ;

static inline uint32_t pci_device_get_address(PCIDevice device, int field)
{
    return 0x80000000 |
           (device.bus << 16) |
           (device.slot << 11) |
           (device.func << 8) |
           ((field)&0xFC);
}

void pci_device_write(PCIDevice device, int field, int size, uint32_t value);

uint32_t pci_device_read(PCIDevice device, int field, int size);

uint32_t pci_device_read_bar(PCIDevice device, int bar);

void pci_device_write_bar(PCIDevice device, int bar, uint32_t value);

uint16_t pci_device_type(PCIDevice device);
