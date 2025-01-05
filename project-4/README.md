## Project 4: Concurrency Assignment
# Description, Details and Deliverables:
Students in the dining hall invoke dine() and then leave(). After invoking dine and before invoking leave a student is considered "ready to leave".

In order to maintain social norms, a student may never sit at a table alone. A student is considered to be sitting alone if everyone else who has invoked dine() invokes leave() before she has finished dine().

Additionally,the dining hall should enforce a maximum capacity. Write code that enforces this constraint.

Write a program to coordinate the students - you may wish to use this code as a starting point: https://github.butler.edu/npartenh/dining-hall

Use semaphores for any required locks or ordering. At least two (2) semaphores will be required.  One or more will protect shared data, e.g. the number of students in the dining hall. One or more semaphores are also needed to communicate between threads when a student is waiting to leave.