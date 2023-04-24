#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

/**
 * error_msg - prints error messages to stderr
 * @av: the name of the program
 * @msg: the error message
 * @errnum: the error number
 */
void error_msg(char *av, char *msg, int errnum)
{
    fprintf(stderr, "%s: %s", av, msg);
    if (errnum)
        fprintf(stderr, ": %s", strerror(errnum));
    fprintf(stderr, "\n");
}

/**
 * main - simple shell implementation
 *
 * Return: always 0
 */
int main(void)
{
    char *line = NULL, *token = NULL, *av = NULL, *path = NULL, **args = NULL;
    size_t n = 0;
    int i = 0, status = 0, interactive = isatty(STDIN_FILENO);
    pid_t pid;

    while (1)
    {
        if (interactive)
            printf("$ ");
        if (getline(&line, &n, stdin) == -1)
        {
            if (interactive)
                putchar('\n');
            break;
        }
        if (!interactive)
            line[strlen(line) - 1] = '\0';
        token = strtok(line, " \t\n");
        if (!token)
            continue;
        args = malloc(sizeof(char *) * 2);
        if (!args)
            continue;
        args[0] = token;
        args[1] = NULL;
        pid = fork();
        if (pid == -1)
        {
            error_msg(av, "fork", errno);
            free(args);
            continue;
        }
        if (pid == 0)
        {
            path = strtok(getenv("PATH"), ":");
            while (path)
            {
                av = malloc(strlen(path) + strlen(args[0]) + 2);
                if (!av)
                {
                    error_msg(av, "malloc", errno);
                    exit(EXIT_FAILURE);
                }
                sprintf(av, "%s/%s", path, args[0]);
                if (access(av, X_OK) == 0)
                {
                    if (execve(av, args, NULL) == -1)
                    {
                        error_msg(av, "execve", errno);
                        exit(EXIT_FAILURE);
                    }
                }
                free(av);
                path = strtok(NULL, ":");
            }
            error_msg(args[0], "No such file or directory", 0);
            exit(EXIT_FAILURE);
        }
        else
        {
            waitpid(pid, &status, 0);
            free(args);
        }
    }
    free(line);
    return (0);
}

