/*#include "memory.h"

memory::memory() {}

 void memory::store(uint8_t address, uint8_t data)
 {
    if (address >= memory_size_in_bytes || address <= 64)   //64 = os_boundary, reserved for os and cannot be used by user
        throw std::out_of_range("Address out of range.");
    ram_[address] = data;
 }
        
uint8_t memory::load(uint8_t address)
{
    if (address >= memory_size_in_bytes)
        throw std::out_of_range("Address out of range.");
    return ram_[address];
}*/

#include "memory.h"

Memory::Memory() {}

void Memory::store(uint8_t address, uint8_t data)
{
    if (address >= memory_size_in_bytes)
        throw std::out_of_range("Address out of range");
    ram_[address] = data;
}

uint8_t Memory::load(uint8_t address)
{
    if (address >= memory_size_in_bytes)
        throw std::out_of_range("Address out of range");
    return ram_[address];
}

void Memory::dump()
{
    for( uint32_t i = 0; i < memory_size_in_bytes; i++)
	{
		if (ram_[i] == 0)
		    std::cout << " ";
		else
		    std::cout << ram_[i];
		    // std::cout << std::hex << std::setw(2) << std::setfill('0') << ram_[i] << std::endl;

		if (i%32 == 0 && i >0) { std::cout << "." << std::endl; }
	}
	std::cout << std::endl;
}
