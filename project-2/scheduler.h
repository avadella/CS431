#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <iostream>
#include <vector>
#include <algorithm>        //for sorting, using built-in swap
#include "process.h"

#include <cstdlib>          //for rand() and srand()
#include <ctime>            //for time()

class scheduler
{
    public:
        scheduler(const std::vector<process>& processes);                           //default constructor
        void stats(std::vector<process>& processes);                                //print stats
        void schedule(std::vector<process>& currentProcesses);                      //run all policies
        void fifo(std::vector<process>& currentProcesses, bool SJF);                //also runs SJF with sorted inputs
        void stride(std::vector<process>& currentProcesses);      
        void round_robin(std::vector<process>& currentProcesses, int quantum);
        void random_robin(std::vector<process>& currentProcesses);

        int quantum = 10;               //default time quantum
};

#endif