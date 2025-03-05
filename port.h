/**
 * @file port.h
 * @brief This header file defines classes for interacting with I/O ports.
 *
 * This file provides the declaration of the Port class and its derived classes
 * for 8-bit, 16-bit, and 32-bit I/O port operations. These classes provide methods
 * for reading from and writing to I/O ports, which are essential for hardware communication.
 */

 #ifndef __PORT_H
 #define __PORT_H
 
 #include "types.h"
 
     class Port
     {
     protected:
         uint16_t portnumber;
         Port(uint16_t portnumber);
         ~Port();
     };
 
     class Port8Bit : public Port
     {
     public:
         Port8Bit(uint16_t portnumber);
         ~Port8Bit();
         virtual void Write(uint8_t data);
         virtual uint8_t Read();
     };
 
     class Port8BitSlow : public Port8Bit
     {
     public:
         Port8BitSlow(uint16_t portnumber);
         ~Port8BitSlow();
         virtual void Write(uint8_t data);
     };
 
     class Port16Bit : public Port
     {
     public:
         Port16Bit(uint16_t portnumber);
         ~Port16Bit();
         virtual void Write(uint16_t data);
         virtual uint16_t Read();
     };
 
     class Port32Bit : public Port
     {
     public:
         Port32Bit(uint16_t portnumber);
         ~Port32Bit();
         virtual void Write(uint32_t data);
         virtual uint32_t Read();
     };
 
 
 #endif