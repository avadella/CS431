#include "process.h"

process::process() : arrival(0), burst(0), wait(0), response(0), turnaround(0), stride(0)
{

}

process::process(int arrival, int burst) : arrival(arrival), burst(burst), wait(0), response(0), turnaround(0), stride(0)
{

}