//CS431, Project #1 - Interactive Shell Project: Written by Anna Vadella
#include <iostream>
#include <string>
#include <vector>
#include <sstream>      //for istringstream
#include <sys/wait.h>
#include <unistd.h>

#include "shell.h"

int main()
{
    std::string prompt = "cshell$ ";
    std::string input;

    while(1)                            //while true
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

        //checking if previous code block works -- tester print statement
        /*for(auto t : tokens)
            std::cout << t << std::endl;*/

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
            else                                        
            {
                pid_t pid;

                /* fork a child process */
                pid = fork();

                if (pid < 0) { /* error occurred */
                    std::cerr << "fork() failed\n";
                    return 1;
                }
                //NOTE: execvp runs things like g++ and nano!
                else if (pid == 0) { /* child process */ 
                /*for loop, convert standard string tokens to char * tokens using .c_str and const_cast, NULL in 
                last spot, bin becomes args[0] and ls becomes args[]*/
                    std::vector<char*> args;
                    for (const std::string& token : tokens) 
                        args.push_back(const_cast<char*>(token.c_str()));
                    
                    args.push_back(NULL); //NULL in the last spot

                    execvp(args[0], args.data());   
                    //args.data b/c it ensures you get a pointer to the first element of the vector, regular array inside vector that you get access to (c array, not std)

                    std::cerr << "execvp failed\n"; 
                    exit(1); //exit if execvp fails
                    
                    //execvp("/bin/ls", "ls", NULL);
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