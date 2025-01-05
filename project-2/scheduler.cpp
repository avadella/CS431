#include "scheduler.h"
   
scheduler::scheduler(const std::vector<process>& processes)                     //default constructor
{
    
}


void scheduler::stats(std::vector<process>& processes)                          //function to calculate stats for each policy
{
    double averageTurnaround = 0;
    double averageResponse = 0;
    double averageWait = 0;
    double size = processes.size();

    //calculate sums
    for(process&p : processes)
    {
        averageTurnaround += p.turnaround;
        averageResponse += p.response;
        averageWait += p.wait;
    }

    //calculate averages
    averageTurnaround = averageTurnaround/size;
    averageResponse = averageResponse/size;
    averageWait = averageWait/size;

    std::cout << "    Average turnaround time: " << averageTurnaround << " ms" << std::endl; 
    std::cout << "    Average response time: " << averageResponse << " ms" << std::endl; 
    std::cout << "    Average wait time: " << averageWait << " ms" << std::endl; 
    std::cout << std::endl;
}    


void scheduler::schedule(std::vector<process>& currentProcesses)                //function to run all policies
{
    //std::cout << "scheduler is running! yay!!!" << std::endl;

    std::cout << "Round Robin: " << std::endl;
    round_robin(currentProcesses, 10);

    std::cout << "Stride: " << std::endl;
    stride(currentProcesses);

    std::cout << "FIFO: " << std::endl;
    fifo(currentProcesses, false);

    std::cout << "SJF: " << std::endl;
    fifo(currentProcesses, true);

    std::cout << "Random Robin: " << std::endl;
    random_robin(currentProcesses);
}


void scheduler::fifo(std::vector<process>& currentProcesses, bool SJF)          //function to run FIFO and SJF scheduling
{
    //sort based on arrival time if false (FIFO), sort based on burst time if true (SJF)
    if(SJF == false)
    {
        for(int i = 0; i < currentProcesses.size(); i++)
        {
            for(int j = 0; j < currentProcesses.size() - i - 1; j++)
            {
                if(currentProcesses[j].arrival > currentProcesses[j + 1].arrival)
                    std::swap(currentProcesses[j], currentProcesses[j + 1]);
            }
        }
    }
    else                           //sorts based on burst time, and then by arrival time if burst times are equal
    {
        for(int i = 0; i < currentProcesses.size(); i++)
        {
            for(int j = 0; j < currentProcesses.size() - i - 1; j++)
            {
                //if a.burst != b.burst, return a.burst, otherwise return a.arrival > b.arrival
                if(currentProcesses[j].burst > currentProcesses[j + 1].burst || (currentProcesses[j].burst == currentProcesses[j + 1].burst && currentProcesses[j].arrival > currentProcesses[j + 1].arrival))
                    std::swap(currentProcesses[j], currentProcesses[j + 1]);
            }
        }
    }
    
    int time = 0;

    //calculate wait, response, and turnaround times for each process
    for (process& p : currentProcesses) 
    {
        if (time < p.arrival)                  //if current time is less than arrival time, update time
            time = p.arrival;

        p.wait = time - p.arrival;             //calculate wait time          
        p.response = time - p.arrival;         //calculate response time
        time += p.burst;                       //increment time by burst time
        p.turnaround = time - p.arrival;       //calculate turnaround time
    }

    stats(currentProcesses);
}


//deterministic, divide stride_numerator by burst num to get stride; when choosing if all at 0 choose a run for a complete stride, move on to whatever has made the least progress
void scheduler::stride(std::vector<process>& currentProcesses)                  //function to run stride scheduling
{
    int time = 0;       
    int pass = 0;                                                       //pass variable to determine if process should be passed for scheduling 
    const int stride_numerator = 10000;
    //const int stride_numerator = 100;                                   //use smaller numerator for clearer output
    bool work_remaining = true;

    std::vector<process> processes = currentProcesses;                  //copy of original processes
    std::vector<int> remaining_burst(processes.size());
    std::vector<int> total_executed(processes.size(), 0);

    //calculate stride for each process and initialize times
    for (int i = 0; i < processes.size(); i++) 
    {
        remaining_burst[i] = processes[i].burst;                       //remaining burst will keep track of how much burst each process has left
        processes[i].stride = stride_numerator/processes[i].burst;     //calculate stride for each process
        processes[i].wait = 0;
        processes[i].response = -1;                                     //-1 means response time hasn't been set yet
        processes[i].turnaround = 0;
        //std::cout << "Process " << i << " - Burst: " << processes[i].burst << ", Stride: " << processes[i].stride << ", Arrival: " << processes[i].arrival << "\n";
    }

    //find the earliest arrival time
    int earliest_arrival = processes[0].arrival;
    for (int i = 1; i < processes.size(); ++i) 
        earliest_arrival = std::min(earliest_arrival, processes[i].arrival);

    time = earliest_arrival;
    //std::cout << "Earliest arrival time: " << earliest_arrival << "\n";

    do 
    {
        work_remaining = false;
        int selected_process = -1;
        int least_progress = -1;                                        //initalize to -1 to indicate no progress

        //first pass: find the first eligible process
        for (int i = 0; i < processes.size(); i++) 
        {
            if (remaining_burst[i] > 0 && processes[i].arrival <= time) 
            {
                selected_process = i;
                least_progress = total_executed[i];
                work_remaining = true;
                break;
            }
        }

        //second pass: compare with other eligible processes
        if (selected_process != -1)
        {
            for (int i = selected_process + 1; i < processes.size(); i++) 
            {
                if (remaining_burst[i] > 0 && processes[i].arrival <= time) 
                {
                    if (total_executed[i] < least_progress)
                    {
                        selected_process = i;
                        least_progress = total_executed[i];
                    }
                }
            }
        }

        if (selected_process != -1)            
        {   
            //std::cout << "Selected process: " << selected_process << "\n";

            //set response time if not already set
            if (processes[selected_process].response == -1) 
            {
                processes[selected_process].response = time - processes[selected_process].arrival;
                //std::cout << "Set response time for process " << selected_process << " to " << processes[selected_process].response << "\n";
            }

            int execute_time = processes[selected_process].stride;
            if (remaining_burst[selected_process] < execute_time) 
                execute_time = remaining_burst[selected_process];

            time += execute_time;
            remaining_burst[selected_process] -= execute_time;
            total_executed[selected_process] += execute_time;

            //std::cout << "Executed process " << selected_process << " for " << execute_time << " units. Remaining burst: " << remaining_burst[selected_process] << ", Total executed: " << total_executed[selected_process] << "\n";

            //update pass value
            pass += processes[selected_process].stride;
            //std::cout << "Updated pass to " << pass << "\n";

            //update wait times for other processes
            for (int i = 0; i < processes.size(); i++) 
            {
                if (i != selected_process && remaining_burst[i] > 0 && processes[i].arrival <= time) 
                {
                    processes[i].wait += execute_time;
                    //std::cout << "Updated wait time for process " << i << " to " << processes[i].wait << "\n";
                }
            }

            //check if process is complete
            if (total_executed[selected_process] == processes[selected_process].burst) 
            { 
                processes[selected_process].turnaround = time - processes[selected_process].arrival;
                //std::cout << "Process " << selected_process << " completed. Turnaround time: " << processes[selected_process].turnaround << "\n";  
            }
        } 
        else 
        {
            //no process is ready, move time to the next arrival
            int next_arrival = -1;
            for (int i = 0; i < processes.size(); i++) 
            {
                if (processes[i].arrival > time) 
                {
                    if (next_arrival == -1 || processes[i].arrival < next_arrival) 
                        next_arrival = processes[i].arrival;
                }
            }
            if (next_arrival != -1) 
                time = next_arrival;
            else 
                break;                                                              //no more processes left to run
        }
    } while (work_remaining);

    //debugging print statement to show each process' response time, wait time, and turnaround time before calculating averages
    /*for (int i = 0; i < processes.size(); i++)
    {
        std::cout << "Process " << i << ":\n";
        std::cout << "  Response Time: " << processes[i].response << " ms\n";
        std::cout << "  Wait Time: " << processes[i].wait << " ms\n";
        std::cout << "  Turnaround Time: " << processes[i].turnaround << " ms\n";
        std::cout << std::endl;
    }*/

    //update the original process list with the calculated values
    for (int i = 0; i < currentProcesses.size(); i++) 
    {
        currentProcesses[i].wait = processes[i].wait;
        currentProcesses[i].response = processes[i].response;
        currentProcesses[i].turnaround = processes[i].turnaround;
    }

    stats(currentProcesses);
} 


void scheduler::round_robin(std::vector<process>& currentProcesses, int quantum)    //function to run round robin scheduling
{
    int time = 0;
    bool work_remaining;                                                            //true if any process has remaining burst time, loop continues as long as check is true, meaning still processes to execute
    unsigned int first = 0;                                                         

    std::vector<process> processes = currentProcesses;                            //copy of original processes
    std::vector<int> remaining_burst(processes.size());       

    //initialize times
    for(int i = 0; i < processes.size(); i++) 
    {
        remaining_burst[i] = processes[i].burst;
        processes[i].wait = 0;
        processes[i].response = -1;                           //-1 --> response time hasn't been set yet
        processes[i].turnaround = 0;
    }

    do
    {
        work_remaining = false;                                          //reset flag for every "round"
        for (int i = 0; i < processes.size(); i++)
        {
            if (remaining_burst[i] > 0) 
            {
                work_remaining = true;                                   //at least one process is still running

                //set response time if not set yet
                if (processes[i].response < 0)                                     
                    processes[i].response = time - processes[i].arrival;

                //calculate execution time for quantum -- std::min returns smaller of two values
                int execute_time = std::min(quantum, remaining_burst[i]);               
                time += execute_time;
                remaining_burst[i] -= execute_time;

                //update wait time for all other processes that have arrived and aren't completed yet
                for (int j = 0; j < processes.size(); j++) 
                {
                    if(j != i && remaining_burst[j] > 0) 
                    {
                        if(processes[j].arrival <= time - execute_time)                //process j already waiting at start of execution
                            processes[j].wait += execute_time;
                        else                                                            //process j only waiting part of time
                            processes[j].wait += time - processes[j].arrival;
                    }
                }

                //if completed, calculate turnaround time
                if (remaining_burst[i] == 0) 
                    processes[i].turnaround = time - processes[i].arrival;
            }
        }
    } while(work_remaining);

    //update the original process list with the calculated values
    for(int i = 0; i < processes.size(); i++)                       
    {
        currentProcesses[i].wait = processes[i].wait;
        currentProcesses[i].response = processes[i].response;
        currentProcesses[i].turnaround = processes[i].turnaround;
    }

    stats(currentProcesses);
}


//non-deterministic, use random to select which process to run, after each time quantum randomly choose from running process which to run next, otherwise, same rules as round robin
void scheduler::random_robin(std::vector<process>& currentProcesses)            //function to run random robin scheduling
{
    //seed random num generator
    srand(time(NULL));

    bool work_remaining;                             //set to false if any ready processes found, loop continues as long as check is false, meaning still processes to execute 
    int time = 0;

    std::vector<process> processes = currentProcesses;        
    std::vector<int> remaining_burst(processes.size());

    //initialize remaining burst times
    for (int i = 0; i < processes.size(); i++) 
    {
        remaining_burst[i] = processes[i].burst;
        processes[i].wait = 0;
        processes[i].response = -1;
        processes[i].turnaround = 0;
    }

    do 
    {
        work_remaining = true;
        std::vector<int> readyProcesses;

        //find ready processes (ready process = has arrived, still has remaining execution time, i.e, burst time has not been fully consumed))
        for (int i = 0; i < processes.size(); i++) 
        {
            if (remaining_burst[i] > 0 && processes[i].arrival <= time) 
            {
                readyProcesses.push_back(i);
                work_remaining = false;
            }
        }

        if (!readyProcesses.empty()) 
        {
            //randomly select a process
            int selectedIndex = readyProcesses[rand() % readyProcesses.size()];

            //execute the selected process
            if (processes[selectedIndex].response < 0) 
                processes[selectedIndex].response = time - processes[selectedIndex].arrival;

            //std::min: returns smaller of two values --> used to determine execution time in each quantum
            int execute_time = std::min(quantum, remaining_burst[selectedIndex]);
            time += execute_time;
            remaining_burst[selectedIndex] -= execute_time;

            //update wait time for other processes
            for (int i = 0; i < processes.size(); i++) 
            {
                if (i != selectedIndex && processes[i].arrival <= time && remaining_burst[i] > 0) 
                    processes[i].wait += execute_time;
            }

            //if process is completed, calculate turnaround time
            if (remaining_burst[selectedIndex] == 0) 
                processes[selectedIndex].turnaround = time - processes[selectedIndex].arrival;
        } 
        else 
        {
            time++;             //no process is ready, so just increment time
        }
            
    } while (!work_remaining);

    //update the original process list with the calculated values
    for (int i = 0; i < processes.size(); i++) 
    {
        currentProcesses[i].wait = processes[i].wait;
        currentProcesses[i].response = processes[i].response;
        currentProcesses[i].turnaround = processes[i].turnaround;
    }

    stats(currentProcesses);
}