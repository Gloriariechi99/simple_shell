#include "shell.h"
#define MAX_CHAR 1024

/**
 * prompt - Displays a prompt and waits for the
 * user to type a command
 * @av: Pointer to an array of strings used to type a command
 * that represents command line arguments
 * @env: Pointer to an array of strings that represents
 * environment variables
*/

void prompt(char **av, char **env)
{
	char *string = NULL;
	int i, m, status;
	size_t n = 0;
	ssize_t com;
	char *argv[MAX_CHAR];
	pid_t pid;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "CISFUN$ ", 8);
		com = getline(&string, &n, stdin);
		if (com == -1)
			write(STDOUT_FILENO, "\n", 1);
		i = 0;
		while (string[i])
		{
			if (string[i] == '\n')
				string[i] = 0;
			i++;
		}
		argv[m = 0] = strtok(string, " ");
		while (argv[m] && m < MAX_CHAR - 1)
			argv[++m] = strtok(NULL, " ");
		pid = fork();
		if (pid == -1)
			exit(EXIT_FAILURE);
		if (pid == 0)
		{
			if (execve(argv[0], argv, env) == -1)
			{
				perror(":No such file or directory");

				write(2, av[0], strlen(av[0]));
				exit(EXIT_FAILURE);
			}
		}
		else
			wait(&status);
	}
}
