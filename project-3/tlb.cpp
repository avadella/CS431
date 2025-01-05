#include "tlb.h"
#include <string>
#include <limits>

//TLB ENTRY FUNCTIONS
tlb_entry::tlb_entry(): page_(0), frame_(0), accessed_(0)                               //default constructor
{

}
tlb_entry::tlb_entry(int page, int frame): page_(page), frame_(frame), accessed_(0)     //parameterized constructor
{

}

void tlb_entry::access(int time) 
{ 
    this->accessed_ = time; 
}

std::string tlb_entry::to_string()
{
	return std::to_string(page_) + ":" + std::to_string(frame_);
}

int tlb_entry::getPage() 
{
    return page_;
}
int tlb_entry::getFrame()
{
    return frame_;
}

int tlb_entry::getAccess()
{
    return accessed_;
}

void tlb_entry::setPage(int page)
{
    page_ = page;
}

void tlb_entry::setFrame(int frame)
{
    frame_ = frame;
}

void tlb_entry::setAccess(int time)
{
    accessed_ = time;
}

void tlb_entry::setEntry(tlb_entry entry, int index)
{
    setPage(entry.getPage());
	setFrame(entry.getFrame());
	setAccess(entry.getAccess());
}

//TLB FUNCTIONS
TLB::TLB()
{
    size_ = DEFAULT_TLB_SIZE;
    int current_entry_num = 0;
    tlb_ = new tlb_entry[size_];
}

TLB::TLB(int size)
{
    size_ = size;
}

bool TLB::contains(int page, int time) 
{ 
    //std::cout << page << std::endl;
	for(int i = 0; i < current_entry_num; i++)
    {
        if(tlb_[i].getPage() == page)
		{
			tlb_[i].setAccess(time);
            //tlb_[i].access(time);
			return true;
		}
    }
    
    return false; 
}

void TLB::print()
{
	std::cout << "TLB size: " << current_entry_num << std::endl;
	std::cout << "TLB Entries (entry, page, frame):" << std::endl;
    for(int i = 0; i < current_entry_num; i++)
        std::cout << "  [" << i << "]: " << tlb_[i].to_string() << std::endl;
}

int TLB::getSize()
{
    return size_;
}

//for generating random memory references
int TLB::getRandom(const int min, const int max)
{
  static std::random_device dev;
  static std::mt19937 twister(dev());
  static std::uniform_int_distribution<int> dist;

  dist.param(std::uniform_int_distribution<int>::param_type(min, max));
  return dist(twister);
}

void TLB::clearTable()
{
    current_entry_num = 0;
    /*for(int i = 0; i < size_; ++i) 
         tlb_[i] = tlb_entry();*/
}

void TLB::replaceFIFO(tlb_entry entry, int time)        //First in, First out - selects oldest page in memory to replace when page needs to be removed to make room for new page
{
    for (int i = 0; i < (size_ - 1); i++)
        tlb_[i] = tlb_[(i + 1)];                  
    tlb_[size_ - 1].setEntry(entry, time);              //set new entry at the last position
}

void TLB::replaceRandom(tlb_entry entry, int time)      //randomly select a page to replace when a page needs to be removed to make room for a new page
{
    int random = getRandom(0, current_entry_num - 1);
    tlb_[random].setEntry(entry, time);
}

void TLB::replaceLRU(tlb_entry entry, int time)         //Least Recently Used - track order in which pages are accessed over a set time, whoever has smallest access time gets removed
{
    int max = std::numeric_limits<int>::max();          //initialize max to a large number so first one is always smaller
    int index = -1;                                     //initialize index to an invalid value
    for(int i = 0; i < current_entry_num; i++)
    {
        if(tlb_[i].getAccess() < max)
        {
            max = tlb_[i].getAccess();
            index = i;
        }
    }

    if (index != -1)                                    //ensure that a valid index was found before replacing
	    tlb_[index].setEntry(entry, time);
}

void TLB::insert(tlb_entry entry, int time)
{
    if(current_entry_num < size_)
    {
        tlb_[current_entry_num].setPage(entry.getPage());
        tlb_[current_entry_num].setFrame(entry.getFrame());
        tlb_[current_entry_num].setAccess(time);
        current_entry_num++;
    }
    else
    {
        replaceLRU(entry, time);                                    //if TLB is full, replace the least recently used entry
    }
}

//add method to update access time, search tlb and update one that matches - contains: if it contains it, it updates it, if it doesnt, it doesn't
void TLB::updateAccess(int page, int time) 
{
    for(int i = 0; i < current_entry_num; i++)
    {
        if(tlb_[i].getPage() == page)
        {
            //tlb_[i].access(time);                               //update access time
            tlb_[i].setAccess(time);
            return;
        }
    }
}