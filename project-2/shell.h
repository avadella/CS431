#ifndef SHELL_H
#define SHELL_H

#include <string>
#include <iostream>
#include <unistd.h>

void cd(const char * path);
void echo(const char * echo);
void help();

#endif