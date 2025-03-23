#ifndef __MYOS__HARDWARECOMMUNICATION__PCI_H
#define __MYOS__HARDWARECOMMUNICATION__PCI_H

#include <hardwarecommunication/port.h>
#include <drivers/driver.h>
#include <common/types.h>
#include <hardwarecommunication/interrupts.h>


namespace myos
{
    namespace hardwarecommunication
    {
        // For last bit
        enum BaseAddressRegisterType
        {
            MemoryMapping = 0,
            InputOutput = 1
        };


        class BaseAddressRegister
        {
         public:
            // 4th bit Mem mapping
            bool prefetchable;
            common::uint8_t* address;
            common::int32_t size;
            BaseAddressRegisterType type;
        };



        class PeripheralComponentInterconnectDeviceDescriptor
        {
         public:
            common::uint32_t portBase;
            common::uint32_t Interrupt;

            common::uint16_t bus;
            common::uint16_t device;
            common::uint16_t function;

            common::uint16_t vendor_id;
            common::uint16_t device_id;

            common::uint8_t class_id;
            common::uint8_t subclass_id;
            common::uint8_t interface_id;

            common::uint8_t revision;

            PeripheralComponentInterconnectDeviceDescriptor();
            ~PeripheralComponentInterconnectDeviceDescriptor();
        };



        class PeripheralComponentInterconnectController
        {
            Port32Bit dataPort;
            Port32Bit commandPort;

        public:
            PeripheralComponentInterconnectController();
            ~PeripheralComponentInterconnectController();

            common::uint32_t Read(common::uint16_t bus, common::uint16_t device, common::uint16_t function, common::uint32_t registeroffset);
            void Write(common::uint16_t bus, common::uint16_t device, common::uint16_t function, common::uint32_t registeroffset, common::uint32_t value);
            bool DeviceHasFunctions(common::uint16_t bus, common::uint16_t device);

            void SelectDrivers(drivers::DriverManager* driverManager, hardwarecommunication::InterruptManager* interrupts);
            drivers::Driver* GetDriver(PeripheralComponentInterconnectDeviceDescriptor dev, hardwarecommunication::InterruptManager* interrupts);
            
            PeripheralComponentInterconnectDeviceDescriptor GetDeviceDescriptor(common::uint16_t bus, common::uint16_t device, common::uint16_t function);
            BaseAddressRegister GetBaseAddressRegister(common::uint16_t bus, common::uint16_t device, common::uint16_t function, common::uint16_t bar);
        };
    }
}

#endif