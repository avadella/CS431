## Project 1: Interactive Shell Project
# Description, Details and Deliverables:
You will build a simple command line shell similar to shells commonly used in UNIX and Linux OSes (e.g. bash or zsh). 
The shell accepts typed instructions and either responds directly or runs programs as specified by the user. 
When the command entered is a program, the shell will fork a new process, invoke the external command, and then 
wait for the command to finish.

# Requirements
This shell program should be completed in C or C++ and must compile and run on thomas.butler.edu.\
You should turn in your program, in electronic format, as source files or as a github repository.\
This shell program will accept a filename as a parameter and interpret the shell script and run it when provided.\
Your shell should implement, at minimum, the following commands:\
- *cd* – use the system call chdir() to implement a change directory (cd) command
- *echo* – echo will print all of the tokens from the command entered except the word "echo", i.e. `echo hello world` would 
print "hello world" on a line by itself
- *exit* – exit the shell (EXIT_SUCCESS or 0)
- *help* – help will display a short help message including built in functions and the program author(s)\
Additionally, any other input should be handled as a request to execute a program, with or without arguments, and 
execute the program as specified.