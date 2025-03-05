/**
 * @file types.h
 * @brief This header file defines fixed-width integer types.
 *
 * This file provides typedefs for integer types with specific widths.
 * These typedefs ensure that the integers have the same size on any platform,
 * which is crucial for portability and consistency in systems programming.
 */

 #ifndef __TYPES_H
 #define __TYPES_H
 
 
 
     typedef char int8_t;
     typedef unsigned char uint8_t;
 
     typedef short int16_t;
     typedef unsigned short uint16_t;
     
     typedef int int32_t;
     typedef unsigned int uint32_t;
     
     typedef long long int int64_t;
     typedef unsigned long long int uint64_t;
     
 #endif