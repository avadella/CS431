#ifndef TLB_H
#define TLB_H

#include <vector>
#include <iostream>
#include <random>

#define DEFAULT_TLB_SIZE 4

class tlb_entry 
{
  int page_;
  int frame_;
  int accessed_;

public:
    tlb_entry();
    tlb_entry(int page, int frame);

    void access(int time);
    std::string to_string();

    //getters
    int getPage();
    int getFrame();
    int getAccess();

    //setters
    void setPage(int page);
    void setFrame(int frame);
    void setAccess(int time);
    void setEntry(tlb_entry entry, int index);
};

class TLB 
{
  private:
    int size_;
	//std::vector<tlb_entry> tlb_;

  public:
	TLB();
	TLB(int size);
	bool contains(int page, int time);
	void print();

  int getSize();
  int getRandom(const int min, const int max);

  void clearTable();
  void replaceFIFO(tlb_entry entry, int time);
  void replaceRandom(tlb_entry entry, int time);
  void replaceLRU(tlb_entry entry, int time);

  void insert(tlb_entry entry, int time);
  void updateAccess(int page, int time);

  int current_entry_num; 
  tlb_entry * tlb_; 
};

#endif