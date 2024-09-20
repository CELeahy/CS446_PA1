//Canon Leahy
//9/8/24
//CS 446.1001
//Programming Assignment 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <errno.h> 
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_IN 1024
#define MAX_ARG 64

void parseInput(char *input, char **args);
int executeCommand(char **args);
int changeDirectory(char *dir);

int main()
{
    char input[MAX_IN];
    char dir[MAX_IN];
    char *args[MAX_ARG];
    int status = 1;
    while(status)
    {
        printf("canonl:~%s$ ", getcwd(dir, MAX_IN)); //1."Print a Prompt and wait for input."
        fgets(input, MAX_IN, stdin); //2."Get the command line input."
        input[strlen(input) - 1] = '\0';//replace /n with /0 for parsing
        parseInput(input, args); //3."Parse the command line input into an array of C-strings.""
        if(strcmp(args[0], "exit") == 0)//4."Inspect the resulting parsed command line input array to see whether input/output redirection is desired, and set it up by using the appropriate OS system call (dup2)."
        {
            status = 0;
        }
        else if(strcmp(args[0], "cd") == 0) //Interpret the remaining parsed command line input array elements into:a. a command C-string b. if applicable its additional arguments C-strings and pass them all to the appropriate OS system call (exec) that will execute this command.
        {
            if(args[2] != NULL)
            {
                printf("Error: Too many arguments\n");
            }
            else
            { 
            printf("%s\n", args[1]);
            changeDirectory(args[1]);
            }
        }
        else
        {
            executeCommand(args);
        }
    }
    /*for(int i = 0; i < strlen(*args)-1; i++)
    {
        printf("%s ", args[i]);
    }*/
    
    return 0;
}

void parseInput(char *input, char **args)
{
    char *command = strtok(input, " ");
    int i = 0;
    while(command != NULL)
    {
        args[i] = command;
        i++;
        command = strtok(NULL, " ");
    }
    args[i] = NULL;
}

int executeCommand(char **args)
{
    pid_t p = fork(); //launch child
    if(p<0)
    {
        printf("fork failed : %s\n", strerror(errno));
        return -1;
    }else if(p == 0)
    {
        execvp(args[0], args); //assign command to child
        return 1;
    }else
    {
        wait(NULL); //wait for child to finish
    }  
    printf("Child finished with status : %s\n", strerror(errno));
    return 0;
}

int changeDirectory(char *dir)
{
    if(dir == NULL)
    {
        printf("Path Not Formatted Correctly! \n");
        return -1;
    }
    if(chdir(dir) != 0)
    {
        printf("Error: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}