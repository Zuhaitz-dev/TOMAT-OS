#include <hardwarecommunication/pci.h>

using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;

// PCI -> Peripheral Component Interconnect
//
// Grosso modo:
//
//              Bus (8)                     Functions (8)
//              3 bits                        3 bits
//
//             +---- ...                      +----/
//   +-----+   | 1                            | 1
//   | PCI |---|---- ...    Devices (32)      |----/
//   +-----+   | 2             5 bits         | 2
//             |-----------[+*+*+*+*+*+]------|---/
//             | 3                            | 3
//             ...                            ...



PeripheralComponentInterconnectController::PeripheralComponentInterconnectController()
: dataPort(0xCFC),
commandPort(0xCF8)
{
    
}



PeripheralComponentInterconnectController::~PeripheralComponentInterconnectController()
{
    
}



PeripheralComponentInterconnectDeviceDescriptor::PeripheralComponentInterconnectDeviceDescriptor()
{

}



PeripheralComponentInterconnectDeviceDescriptor::~PeripheralComponentInterconnectDeviceDescriptor()
{

}



uint32_t
PeripheralComponentInterconnectController::Read(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset)
{
    uint32_t id = 0x1 << 31
                | ((bus & 0xFF) << 16)
                | ((device & 0x1F) << 11)
                | ((function & 0x07) << 8)
                | (registeroffset & 0xFC);
    
    commandPort.Write(id);
    uint32_t result = dataPort.Read();

    return result >> (8 * (registeroffset % 4));
}



void
PeripheralComponentInterconnectController::Write(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset, uint32_t value)
{
    uint32_t id = 0x1 << 31
                | ((bus & 0xFF) << 16)
                | ((device & 0x1F) << 11)
                | ((function & 0x07) << 8)
                | (registeroffset & 0xFC);

    commandPort.Write(id);
    dataPort.Write(value);
}



bool
PeripheralComponentInterconnectController::DeviceHasFunctions(uint16_t bus, uint16_t device)
{
    return Read(bus, device, 0, 0x0E) & (1 << 7);
}



void
printf(char* str);

void
printfHex(uint8_t);



void
PeripheralComponentInterconnectController::SelectDrivers(DriverManager* driverManager)
{
   for (int bus = 0; bus < 8; bus++)
   {
    for (int device = 0; device < 32; device++)
    {
        int numFunctions = DeviceHasFunctions(bus, device) ? 8 : 1;
        
        for (int function = 0; function < numFunctions; function++)
        {
            PeripheralComponentInterconnectDeviceDescriptor dev = GetDeviceDescriptor(bus, device, function);

            if (dev.vendor_id == 0x0000 || dev.vendor_id == 0xFFFF)
                break;

            printf("PCI BUS ");
            printfHex(bus & 0xFF);     
            
            printf(", DEVICE ");
            printfHex(device & 0xFF);    

            printf(", FUNCTION ");
            printfHex(function & 0xFF);    

            printf(" = VENDOR ");
            printfHex((dev.vendor_id & 0xFF00) >> 8);  
            printfHex(dev.vendor_id & 0xFF);  
            
            printf(", DEVICE ");
            printfHex((dev.device_id & 0xFF00) >> 8);  
            printfHex(dev.device_id & 0xFF);   

            printf("\n");
        }

    }
   }
}



PeripheralComponentInterconnectDeviceDescriptor
PeripheralComponentInterconnectController::GetDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function)
{
    PeripheralComponentInterconnectDeviceDescriptor result;

    result.bus = bus;
    result.device = device;
    result.function = function;

    result.vendor_id = Read(bus, device, function, 0x00);
    result.device_id = Read(bus, device, function, 0x02);

    result.class_id = Read(bus, device, function, 0x0B);
    result.subclass_id = Read(bus, device, function, 0x0A);
    result.interface_id = Read(bus, device, function, 0x09);

    result.revision = Read(bus, device, function, 0x08);
    result.Interrupt = Read(bus, device, function, 0x3C);


    return result;
}