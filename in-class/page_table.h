#ifndef PAGETABLE_H
#define PAGETABLE_H

#include <array>
#include <cstdint>
#include <algorithm>    // std::shuffle
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

#define PAGE_SIZE_IN_BYTES 8
#define NUMBER_OF_PAGES 32

//pte and pageTable class together, because they "kind of go together"
class pte
{
    public:
      uint32_t pfn_;                //pte has only physical frame number, get virtual page number from array index b/c storing pfn_ in an array
      pte();
      pte(uint32_t pfn);
};

class PageTable
{
    private:
      std::array<pte, static_cast<uint32_t>(NUMBER_OF_PAGES)> table_;           //casting number of pages b/c when decided it's an unsigned long, not a uint32_t

    public:
      PageTable();
      void shuffle();   
      uint8_t get(uint8_t vpn);                     //similar to load
      void put(uint8_t vpn, uint8_t pfn);           //similar to store

};


#endif