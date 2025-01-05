//CS431, Project #2 - CPU Scheduling Simulator: Written by Anna Vadella
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>      //for istringstream
#include <sys/wait.h>
#include <unistd.h>

#include "shell.h"
#include "scheduler.h"   

int main()
{
    std::string prompt = "cshell$ ";
    std::string input;

    while(1)                //while true
    {
        std::cout << prompt; 
        std::getline(std::cin, input);
        std::string remainingInput;

        //separate input into tokens - tokenizing
        std::vector<std::string> tokens;
        std::istringstream iss(input);
        std::string token;
        while(iss >> token)             //as long as there are words, will do this
            tokens.push_back(token);

        //parsing through all the tokens
        for(int i = 0; i < tokens.size(); i++)
        {
            if(tokens[i] == "exit")                     //exit the shell
            {
                return EXIT_SUCCESS;
            }
            else if(tokens[i] == "cd")                  //change directories using chdir()
            {
                if (i + 1 < tokens.size())
                    cd(tokens[i + 1].c_str());

                break;
            }
            else if(tokens[i] == "echo")                //echo everything but tokens[0] and put spaces back into the output
            {
                for (size_t j = i + 1; j < tokens.size(); j++) //starting from the next token
                    remainingInput += tokens[j] + " ";

                echo(remainingInput.c_str());
                break;
            }
            else if(tokens[i] == "help")                //print a help message
            {
                help();
                break; 
            }
            else if(tokens[i] == "sched")                
            {
                std::vector<process> sched_data;
                int burstTime = 0;
                int arrivalTime = 0;

                //fill with data from file, then pass as a param for creating schedulers
                std::ifstream input("testing.txt");
                if(input.is_open())
                {
                    while(!input.eof())
                    {
                        input >> arrivalTime >> burstTime;
                        process p(arrivalTime, burstTime);
                        sched_data.push_back(p);
                    }
                input.close();
                }

                scheduler s(sched_data);
                s.schedule(sched_data);
            }
            else                                        
            {
                pid_t pid;

                /* fork a child process */
                pid = fork();

                if (pid < 0) { /* error occurred */
                    std::cerr << "fork() failed\n";
                    return 1;
                }
                else if (pid == 0) { /* child process */ 
                    std::vector<char*> args;
                    for (const std::string& token : tokens) 
                        args.push_back(const_cast<char*>(token.c_str()));
                    
                    args.push_back(NULL); //NULL in the last spot

                    execvp(args[0], args.data());   

                    std::cerr << "execvp failed\n"; 
                    exit(1); //exit if execvp fails
                }
                else { /* parent process */
                    /* parent will wait for the child to complete */
                    wait(NULL);
                }
            }
        }
    }

    return EXIT_SUCCESS;
}