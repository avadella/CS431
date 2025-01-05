/*#ifndef MEMORY_H
#define MEMORY_H

#include <array>
#include <cstdint>         //for uint8_t; for c-headers (instead of .h) we can prefix the include with a 'c'
#include <stdexcept>

#define memory_size_in_bytes 256

//a class to represent main memory as an array of bytes
class memory
{
    private:
        std::array<uint8_t, memory_size_in_bytes> ram_ = {0};     //uint8_t --> unsigned 8-bit int

    public:
        memory();       //default constructor
        void store(uint8_t address, uint8_t data);      //function for storing something in mem, need to know where to put it (address) and the data
        uint8_t load(uint8_t address);
};

#endif8*/

#ifndef MEMORY_H
#define MEMORY_H

#include <array>
#include <cstdint>
#include <stdexcept>
#include <iostream>
#include <iomanip>

#define memory_size_in_bytes 256

// a class to represent main memory as an array of bytes
class Memory
{
    private:
        std::array<uint8_t, memory_size_in_bytes> ram_ = {0};

    public:
        Memory();
        void store(uint8_t address, uint8_t data);
        uint8_t load(uint8_t address);
		void dump();
};

#endif
