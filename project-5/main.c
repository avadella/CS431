//CS431: Persistence Assignment - Anna Vadella
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "zip.h"

int main( int argc, char *argv[] ) 
{
    if (argc != 2) 
    {
        printf("usage: %s <zip filename>\n", argv[0]);
        exit(0);
    }

    char* zipname = argv[1];
    char* numfiles = readline("How many files do you wish to include? ");
    char* files[atoi(numfiles)];
    
    for(int i = 0; i < atoi(numfiles); i++) 
    {
        files[i] = readline("filename: ");
    }

    printf("creating zip file: %s\n", zipname);
    create_file(zipname, files, atoi(numfiles));
    return EXIT_SUCCESS;
}