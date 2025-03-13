#ifndef __TOMATOS__GDT_H
#define __TOMATOS__GDT_H

#include <common/types.h>

namespace TomatOS
{
    
    class GlobalDescriptorTable
    {
        public:

            class SegmentDescriptor
            {
                private:
                    TomatOS::common::uint16_t limit_lo;
                    TomatOS::common::uint16_t base_lo;
                    TomatOS::common::uint8_t base_hi;
                    TomatOS::common::uint8_t type;
                    TomatOS::common::uint8_t limit_hi;
                    TomatOS::common::uint8_t base_vhi;

                public:
                    SegmentDescriptor(TomatOS::common::uint32_t base, TomatOS::common::uint32_t limit, TomatOS::common::uint8_t type);
                    TomatOS::common::uint32_t Base();
                    TomatOS::common::uint32_t Limit();
            } __attribute__((packed));

        private:
            SegmentDescriptor nullSegmentSelector;
            SegmentDescriptor unusedSegmentSelector;
            SegmentDescriptor codeSegmentSelector;
            SegmentDescriptor dataSegmentSelector;

        public:

            GlobalDescriptorTable();
            ~GlobalDescriptorTable();

            TomatOS::common::uint16_t CodeSegmentSelector();
            TomatOS::common::uint16_t DataSegmentSelector();
    };

}
    
#endif