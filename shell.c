#include "shell.h"

/**
 * main - start of the program
 * @ac: argument count
 * @av: argument variable
 * @env: envrionment variable
 * Return: 0
 */

int main(int ac, char **av, char **env)
{
	if (ac == 1)
		prompt(av, env);
	return (0);
}
