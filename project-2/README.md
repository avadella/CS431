## Project 2: CPU Scheduling Simulator
# Description, Details and Deliverables:
**Write a CPU scheduling simulator which will compute:**
- *Average response time* - time until the process is first scheduled
- *Average turnaround time* - time until the process completes work requested
- *Average wait time* - time the process spends waiting for other processes

**For each of the scheduling policies:**
- *FIFO*
- *Shortest Job First*
- *Round Robin* (time quantum default = 10, may be user specified)
- *Stride*
- *Random*\
The input may be read from a file which contains a list of arrival time and CPU burst requirements in\
milliseconds, one process' requirement per line. See *Testing.txt* for an example.

# Requirements
The scheduling simulator will be added as a built-in to your shell assignment.\
You should create appropriate separate files, e.g. a Scheduler.h and Scheduler.cpp where your scheduler\
functions and code are written, maybe a Process.cpp and Process.h to define a process object, etc.
This program should be completed in C++ and must compile and run on thomas.butler.edu.
You should turn in your program, in electronic format, as source files or as a github repository.