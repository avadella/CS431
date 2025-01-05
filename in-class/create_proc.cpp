#include <sys/wait.h>
#include <iostream>
#include <unistd.h>

int main() {

    pid_t pid;

    /* fork a child process */
    pid = fork();

    if (pid < 0) { /* error occurred */
        std::cerr << "fork() failed\n";
        return 1;
    }
    else if (pid == 0) { /* child process */
        execlp("/bin/ls", "ls", NULL);
    }
    else { /* parent process */
        /* parent will wait for the child to complete */
        wait(NULL);
        std::cout << "child complete\n";
    }

    return 0;
}