#include "mmu.h"

mmu::mmu()
{
    Memory ram_;
    PageTable pt_;
}

uint8_t mmu::translate(uint8_t virtual_address)
{
    uint8_t vpn = (virtual_address & VPN_MASK) >> SHIFT;                    //bitwise AND, shifting to the right (to make it match psuedocode)
    // PTEAddr = PageTableBaseRegister + (VPN*sizeof(PTE));
	// we'll magically store our page table rather than place it in RAM
    uint8_t pfn = pt_.get(vpn);                                             //physical frame number
    uint8_t offset   = virtual_address & OFFSET_MASK;                       //get offset using offset mask
    uint8_t physical_address = (pfn << SHIFT) | offset;
    return physical_address;
}

void mmu::store(uint8_t virtual_address, uint8_t data)
{
    if (virtual_address >= memory_size_in_bytes)
        throw std::out_of_range("Address out of range");
    uint8_t physical_address = translate(virtual_address);                  //taking virtual address and translating into a physical address
    ram_.store(physical_address, data);
}

uint8_t mmu::load(uint8_t virtual_address)
{
    if (virtual_address >= memory_size_in_bytes)
        throw std::out_of_range("Address out of range");
    uint8_t physical_address = translate(virtual_address);                  //taking virtual address and translating into a physical address
    return ram_.load(physical_address);
}

void mmu::dump()
{
	ram_.dump();
}
