## Project 3: Cache Management
# Description and Requirements:
This assignment must be completed in C++. It should be initiated as a command from your existing shell assignment and added to the help menu.\
1. implement a data structure which simulates adding a TLB which holds up to four (4) TLB entries
2. implement replacement policies FIFO, LRU, and RANDOM for the TLB
    - for optional extra credit, implement OPT and / or CLOCK
3. read or generate memory references from a file / randomly (there's a random generator in the code sample)
4. compute the TLB hit rate and effective access time (EAT) for each run\

Code from class ( a good starting point - modify / use as needed ): https://github.butler.edu/npartenh/ctlb\
Use `references.txt` as a sample input.\

# Sample Output
Sample output for the sample input:\
```
----- fifo -----
Number of hits: 809
Number of references: 1000
0.809
EAT: 619.32 us
----- lru ------
Number of hits: 868
Number of references: 1000
0.868
EAT: 588.64 us
----- rand -----
Number of hits: 793
Number of references: 1000
0.793
EAT: 627.64 us
```

# Rubric
Points for this assignment will be awarded as follows:\
- program organization, compiles and runs = 10
- Page Table and TLB implementation = 15
- FIFO = 5
- Random = 5
- LRU = 15
- EC = up to 10