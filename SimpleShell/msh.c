/*  Author: WANG Chen
    BUPT number: 2015213086
    Date: 14 May, 2019
    Brief: A simple shell for coursework, named msh.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#define MSH_ARG_BUFSIZE 16

int msh_run_back = 0;        // Indicator of running in background
int msh_re_in = 0;           // Indicator of input redirection
int msh_re_out = 0;          // Indicator of output redirection
char *msh_in_fn = NULL;      // file name of input redirection
char *msh_out_fn = NULL;     // file name of output redirection
char *command_prompt = NULL; // Command prompt string

/* BUILTIN FUNCTION */
/* Buildin functions declaration */
int msh_cd(char **args);
int msh_help(char **args);
int msh_exit(char **args);

/* List of builtin functions */
char *msh_builtin_func_str[] = {"cd", "help", "exit"};
int (*msh_builtin_func[])(char **) = {&msh_cd, &msh_help, &msh_exit};

int msh_builtin_func_num(void)
{
    return sizeof(msh_builtin_func_str) / sizeof(char *);
}

int msh_cd(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "msh: cannot reach\n");
    }
    else if (chdir(args[1]) != 0)
    {
        perror("msh");
    }

    return 1;
}

int msh_help(char **args)
{
    if (args[1] == NULL)
    {
        printf("This is a simple shell program for coursework, named msh, coded by WANG Chen.\n");
        printf("You can use \"help <function name>\" to get information about following builtin function:\n");
        printf("cd, exit.\n");
    }
    else if (strcmp(args[1], "cd") == 0)
    {
        printf("You can use \"cd <dir>\" to change the direct.\n");
    }
    else if (strcmp(args[1], "exit") == 0)
    {
        printf("You can use \"exit\" to close this shell.\n");
    }
    else
    {
        printf("There is not this function.\n");
    }

    return 1;
}

int msh_exit(char **args)
{
    return 0;
}
/* BUILTIN FUNCTION OVER */

int msh_execute(char **args)
{
    int i, status, fd;
    pid_t pid;

    if (args[0] == NULL) // No command was entered
    {
        return 1;
    }

    for (i = 0; i < msh_builtin_func_num(); i++)
    {
        if (strcmp(args[0], msh_builtin_func_str[i]) == 0)
        {
            return (*msh_builtin_func[i])(args);
        }
    }

    pid = fork();
    if (pid == 0) // Child
    {
        if (msh_re_in)
        {
            fd = open(msh_in_fn, O_RDONLY);
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        if (msh_re_out)
        {
            fd = creat(msh_out_fn, 0644);
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        if (execvp(args[0], args) == -1)
        {
            perror("msh");
        }
        exit(1);
    }
    else if (pid < 0) // Error forking
    {
        perror("msh");
    }
    else // Parent
    {
        if (msh_run_back == 0)
        {
            if ((pid = waitpid(pid, &status, 0)) < 0)
            {
                perror("waitpid error");
            }
        }
    }

    return 1;
}

char **msh_split_line(char *line)
{
    int bufsize = MSH_ARG_BUFSIZE;
    int cur_arg_pos = 0;
    char **args = malloc(sizeof(char *) * bufsize);
    char *delim = " \n";
    char *arg;

    if (!args)
    {
        fprintf(stderr, "msh: allocation error for split line\n");
        exit(1);
    }

    arg = strsep(&line, delim); // First argument
    while (arg != NULL)
    {
        if (strcmp(arg, "") != 0) // If not a "" string
        {
            if (strcmp(arg, ">") == 0) // If output redirection
            {
                msh_re_out = 1;             // Set indicator
                arg = strsep(&line, delim); // Next argument
                msh_out_fn = arg;           // file name
            }
            else if (strcmp(arg, "<") == 0) // If input redirection
            {
                msh_re_in = 1;              // Set indicator
                arg = strsep(&line, delim); // Next argument
                msh_in_fn = arg;            // file name
            }
            else
            {
                args[cur_arg_pos++] = arg; // Store argument
            }
        }

        if (cur_arg_pos >= bufsize) // If over buffer
        {
            bufsize += MSH_ARG_BUFSIZE;
            args = realloc(args, sizeof(char *) * bufsize); // Reallocate memory to buffer
            if (!args)
            {
                fprintf(stderr, "msh: reallocation error for split line\n");
                exit(1);
            }
        }

        arg = strsep(&line, delim); // Next argument
    }

    if (strcmp(args[cur_arg_pos - 1], "&") == 0) // If run in background
    {
        msh_run_back = 1; // Indicator of running in background
    }

    args[cur_arg_pos] = NULL;

    return args;
}

char *msh_read_line(void)
{
    char *line = NULL;
    size_t n = 0;

    if (getline(&line, &n, stdin) == -1)
    {
        printf("\n"); // For beauty
        exit(1);
    }

    return line;
}

void msh_run_for_command(void)
{
    char *line = NULL;  // Input line
    char **args = NULL; // Arguments in input line
    int status = 1;     // Running status of shell, 1-continue running, 0-exit

    do
    {
        /* Reset option */
        msh_re_in = 0;
        msh_re_out = 0;
        msh_run_back = 0;

        /* Running loop */
        printf("%s ", command_prompt);
        line = msh_read_line();      // Read the input line
        args = msh_split_line(line); // Get the arguments
        status = msh_execute(args);  // Execute the command

        free(line); // Release allocated memory
        free(args); // Release allocated memory
    } while (status);
}

void msh_load_config(void)
{
    if ((command_prompt = getenv("msh_cp")) == NULL)
    {
        command_prompt = "->";
    }
}

int main(int args, char **argv)
{
    msh_load_config(); // Load the running configuration of msh, it named ".mshrc"

    msh_run_for_command(); // Run for command

    return 0;
}
