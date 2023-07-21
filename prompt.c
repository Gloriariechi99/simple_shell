#include "shell.h"

/**
 * prompt - Displays a prompt and wait for the user to type a command
 * @av: pointer to an array of strings used to
 * represent command line arguments
 * @env: Pointer to an array of strings that
 * represent environment variables
 */

void prompt(char **av, char **env)
{
	char *string = NULL;
	int i, status;
	size_t n = 0;
	ssize_t char_count;
	char *argv[] = {NULL, NULL};
	pid_t pid;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(2, "CISFUN$ ", 8);
		char_count = getline(&string, &n, stdin);
		if (char_count == -1)
			exit(EXIT_FAILURE);
		i = 0;
		while (string[i])
		{
			if (string[i] == '\n')
				string[i] = 0;
			i++;
		}
		argv[0] = string;
		pid = fork();
		if (pid == -1)
			exit(EXIT_FAILURE);
		if (pid == 0)
		{
			if (execve(argv[0], argv, env) == -1)
			{
				const char *error = ": No such file or directory\n";

				write(2, av[0], strlen(av[0]));
				write(2, error, strlen(error));
			}
		}
		else
			wait(&status);
	}
}
