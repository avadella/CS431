#ifndef PROCESS_H
#define PROCESS_H

class process
{
    public:
        int arrival;      //arrival time
        int burst;
        int wait;          //wait time (wt)
        int response;      //response time (rt)
        int turnaround;    //turnaround time (tt)
        int stride;

        process();
        process(int arrival, int burst);        //burst = amount of work a process is able to do "right now"
};

#endif