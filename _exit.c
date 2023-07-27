#include "simpleshell.h"
/**
 * exit_proc - exiting
 * @cmds: commands
 * @lines: input string
 * @ret: exit value
 *
 * Return: nothing
 */
void exit_proc(char ***cmds, char **lines, int *ret)
{
	if (cmds[0][1] != NULL)
		*ret = my_atoi(cmds[0][1]);
	free_cmd(cmds);
	free_1(lines);
}

/**
 * pip_exit - exit for piped
 * @cmd: commands
 *
 * Return: exit condition
 */
int pip_exit(char ***cmd)
{
	int ret = 0;

	if (cmd[0][1] != NULL)
	{
		ret = my_atoi(cmd[0][1]);
		return (ret);
	}
	return (ret);
}

/**
 * read_error - read error
 * @wads: return for read
 *
 * Return: nothing
 */
void read_error(int wads)
{
	if (wads == -1)
		exerror("Error while reading file");
}
