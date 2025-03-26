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

// +-------------------------------------------------------+
// | Just checked. It works, but I found a fun Easter Egg. |
// | The vendor 80EE is Oracle VirtualBox and they... We   |
// | could say they use interesting device_IDs. Like, BEEF |
// | or CAFE. Not that remarkable, but it was cute.        |
// +-------------------------------------------------------+

// BAR -> Base Address Register
//
// We will focus on this one:
//                                Port Number                               Type
//         (-----------------------------------------------------------)     ↓
//         |            |              |               |               |   |   |
//   I/O   |            |              |               |               | x | 1 |
//         |            |              |               |               |   |   |
//                                                                       ↑   
//                                                                    Reserved
// There is also this one:
//                                Port Number
//         (---------------------------------------------------)     
//         |            |              |               |       |   |   |   |   |                  | 00 -> 32 Bit
//   Mem   |            |              |               |       |   | Type  | 0 |  Possible types -+ 01 -> 20 Bit
//         |            |              |               |       |   |   |   |   |                  | 10 -> 64 Bit
//                                                               ↑
//                                                          Prefetchable
//
// Many times, when dealing with Hardware, last bits are reserved and cannot be used.
// Would not forget that, just in case.


// | Command to remember (Debian/Ubuntu):
// |     
// |    ```bash
// |    lspci
// |    ```
// |
// |    · Some options that I think are interesting:
// |
// |    -n      Show numeric ID's
// |    -x      Show hex-dump of the standard part of the config space



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
PeripheralComponentInterconnectController::SelectDrivers(DriverManager* driverManager, InterruptManager* interrupts)
{
    printf("\n");
    
    for (int bus = 0; bus < 8; bus++)
    {
        for (int device = 0; device < 32; device++)
        {
            int numFunctions = DeviceHasFunctions(bus, device) ? 8 : 1;
        
            for (int function = 0; function < numFunctions; function++)
            {
                PeripheralComponentInterconnectDeviceDescriptor dev = GetDeviceDescriptor(bus, device, function);

                // This will not work well, as functions do not necessarily go in order. Therefore we are not checking everything.
                //
                // if (dev.vendor_id == 0x0000 || dev.vendor_id == 0xFFFF)
                //      break;

                if (dev.vendor_id == 0x0000 || dev.vendor_id == 0xFFFF)
                    continue;

                for (int barNum = 0; barNum < 6; barNum++)
                {
                    BaseAddressRegister bar = GetBaseAddressRegister(bus, device, function, barNum);
                    if (bar.address && (bar.type == InputOutput))
                        dev.portBase = (uint32_t)bar.address;

                    Driver* driver = GetDriver(dev, interrupts);

                    if (driver != 0)
                        driverManager->AddDriver(driver);
                }


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


BaseAddressRegister
PeripheralComponentInterconnectController::GetBaseAddressRegister(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar)
{
    BaseAddressRegister result;
    
    uint32_t headertype = Read(bus, device, function, 0x0E) & 0x7F;
    
    // 6 or 2, it depends
    int maxBARs = 6 -(4 * headertype);

    if (bar >= maxBARs)
        return result;

    uint32_t bar_value = Read(bus, device, function, 0x10 + 4 * bar);
    result.type = (bar_value & 0x1) ? InputOutput : MemoryMapping;
    uint32_t temp;

    if (result.type == MemoryMapping)
    {
        // We will figure out this later

        switch ((bar_value >> 1) & 0x3)
        {
            case 0: // 32 Bit Mode
            case 1: // 20 Bit Mode
            case 2: // 64 Bit Mode
                break;
        }

        result.prefetchable = ((bar_value >> 3) & 0x1) == 0x1;
    } 
    else    // InputOutput
    {
        result.address = (uint8_t*)(bar_value & ~0x3);
        result.prefetchable = false;
    }

    return result;
}



Driver*
PeripheralComponentInterconnectController::GetDriver(PeripheralComponentInterconnectDeviceDescriptor dev, InterruptManager* interrupts)
{
    // Hard-coding. We do not have access to the hard drive yet so, it is what it is.

    // Btw, I don't think I need to mention this, but... obviously this doesn't do anything *yet*
    Driver *driver = 0;
    switch (dev.vendor_id)
    {
        case 0x1022: // AMD
            switch(dev.device_id)
            {
                case 0x2000: // am79c973
                    /*
                    driver = (amd_am79c973*)MemoryManager::activeMemoryManager -> malloc(sizeof(amd_am79c973));
                    if (driver != 0)
                        new (driver) amd_am79c973(...);
                    */
                    // printf("AMD am79c973 ");
                    break;
            }
            break;
        
        case 0x8086: // Intel
            break;
    }

    switch (dev.class_id)
    {
        case 0x03: // graphics
            switch(dev.subclass_id)
            {
                case 0x00:  // VGA
                    // printf("VGA ");
                    break;
            }
            break;
    }

    return driver;
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