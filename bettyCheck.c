#include "shell.h"

/**
 * main - program
 * @ac: command line arguments
 * @av: argument variables
 * @env: envrionment variables
 * Return: 0
 */
int main(int ac, char **av, char **env)
{
	if (ac == 1)
		prompt(av, env);

	return (0);
}
