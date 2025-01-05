#include "page_table.h"


pte::pte(): pfn_(0) {}
pte::pte(uint32_t pfn): pfn_(pfn) {}


PageTable::PageTable()
{
    for(uint32_t i=0; i<static_cast<uint32_t>(NUMBER_OF_PAGES); i++)
        table_.at(i) = pte(i); // initialize in order; for all of the pages, need to initialize them

	shuffle();
}

void PageTable::shuffle()
{
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle (table_.begin(), table_.end(), std::default_random_engine(seed));      //series of swaps, will be somewhere in memory, likely not zero (but could be!)
}

uint8_t PageTable::get(uint8_t vpn)
{
    return table_.at(vpn).pfn_;
}

void PageTable::put(uint8_t vpn, uint8_t pfn)
{
    table_.at(vpn) = pfn;
}

//to map to physical memory:
//take each page table entry and put it in a page/at an address
//a pte object is how large? 
//Have 32 pages, 8B each, using 8bit addresses -- we could use 1 byte (1B) per pte
    //tracking 32 ptes requires 32B (32 bytes)
    //fill 4 pages with 8 ptes each (linear page table vs. multi-level page table slide for reference!) -- for multi-level, likely would want to make space larger
//How do we know where the page table is? 
    //want to estbalish a PageTableBaseRegister, which will tell us where the page table is in memory
//Note that size of page table is not a problem on it's own, it's a problem because there are many page tables (multi-level page tables help solve this problem!)
//Add a TLB and swapping, implement replacement policies and effective access time (EAT)