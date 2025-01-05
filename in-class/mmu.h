#ifndef MMU_H
#define MMU_H

#include <array>
#include <cstdint>          //for unint
#include <stdexcept>

#include "memory.h"
#include "page_table.h"

#define memory_size_in_bytes 256
#define VPN_MASK 0xF8
#define SHIFT 3
#define OFFSET_MASK 0x07

//0xF8 = 1111 1000
//0x07 = 0000 0111
//32 pages, 8 bits each

// a class to represent an MMU
class mmu
{
    private:
        // std::array<uint8_t, memory_size_in_bytes> ram_ = {0};
		Memory ram_;
        PageTable pt_;      //not in pysical memory, yet

    public:
        mmu();
    	uint8_t translate(uint8_t address);             //trnalstaes a virtual address to a physical address
        void store(uint8_t address, uint8_t data);
        uint8_t load(uint8_t address);
		void dump();
};

#endif
