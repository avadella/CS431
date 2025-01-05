#include "shell.h"

void cd(const char * path)
{
    if(chdir(path) == 0)
		std::cout<<"Changed directory to: " << path << std::endl;
	else
		perror("chdir");        //perror -->  prints an error message to stderr
}

void echo(const char * echo)
{
    std::cout << echo << std::endl;
}

void help()
{
    std::cout << "Help:"  << std::endl;
    std::cout << "- cd will change the current directory." << std::endl;
    std::cout << "- echo will repeat what you typed in." << std::endl;
    std::cout << "- exit will exit the shell." << std::endl;
    std::cout << "- sched will run a scheduling simulator using testing.txt as input." << std::endl;            //Project #2 Addiiton           
    std::cout << "- program written by Anna Vadella." << std::endl;
}