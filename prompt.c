#include "simpleshell.h"

/**
 * loop_shell - provides shell prompt
 * @argv: prog name
 * @cmd_path: executable path
 * @Alice: struct alias
 *
 * Return: exit value
 */
int loop_shell(char *argv, char **cmd_path, Alias **Alice)
{
	ssize_t wads = 0;
	size_t size = 0;
	static int count, ret;
	char **cmds, *lines = NULL, *prevdir = NULL;

	while (1)
	{
		write(STDOUT_FILENO, "$ ", 3);
		fflush(stdout);
		wads = _getline(&lines, &size, STDIN_FILENO);
		if (wads == -1)
		{
			write(STDOUT_FILENO, "\n", 1);
				break;
		}
		cmds = interpret(lines);
		if (cmds != NULL)
		{
			if (has_separator(&cmds))
				command_separator(&cmds, cmd_path, argv);
			else
			{
				if (!is_elias(&cmds, cmd_path[0], Alice[0], argv))/*if not an alias continue below*/
				{
					if (_strcmp(cmds[0], "exit") == 0)
					{
						if (cmds[1] != NULL)
							ret = my_atoi(cmds[1]);
						free_cmd(&cmds);
						free_1(&lines);
						break;
					}
					else if (_strcmp(cmds[0], "env") == 0)
						_env();
					else if (_strcmp(cmds[0], "cd") == 0)
						_cd(&count, &prevdir, &cmds);
					else if (_strcmp(cmds[0], "alias") == 0)
						_alias(&cmds, Alice, argv);
					else/*possible executable commands*/
						option_folder_file_etc(&cmds, cmd_path, argv);
				}
			}
		}
		free_1(&lines);
		free_cmd(&cmds);
	}
	free_1(&prevdir);
	return (ret);
}

/**
* _cd - builtin cd
* @count: cd status
* @prevdir: holds previous directory string
* @cmds: commands
*
* Return: nothing
*/
void _cd(int *count, char **prevdir, char ***cmds)
{
	if (count[0] == 0)
	{
		prevdir[0] = (char *) malloc(sizeof(char) * 256);
		if (prevdir[0] == NULL)
			exerror("Malloc");
		if (getcwd(prevdir[0], 256) == NULL)
			exerror("Getcwd Error");
		count[0]++;
	}
	_changed(cmds[0][1], prevdir);
}

/**
 * free_cmd - frees double char pointer
 * @cmd: triple char pointer
 *
 * Return: nothing
 */
void free_cmd(char ***cmd)
{
	int i = 0;

	if (*cmd == NULL)
		return;
	while (cmd[0][i] != NULL)
	{
		free(cmd[0][i]);
		cmd[0][i] = NULL;
		i++;
	}
	free(*cmd);
	*cmd = NULL;
}

/**
 * _getline - reads line from input stream
 * @buf: string buffer
 * @size: string size
 * @fd: file descriptor
 *
 * Return: new pointer
 */
size_t _getline(char **buf, size_t *size, int fd)
{
	ssize_t wads, linesize = 0;
	*buf = NULL;

	if (size[0] == 0)/*incase input size is zero*/
		size[0] = 1024;
	*buf = (char *) malloc(sizeof(char) * size[0]);
	if (*buf == NULL)
		exerror("Malloc Error");
	do {
		wads = read(fd, *buf, size[0]);/*reading stdin cannot be seek, it is always begins from start*/
		if (wads == -1)
			return (-1);
		linesize += (ssize_t)wads;/*file size so far, becomes actual file size on exit*/
		if (wads == (ssize_t)size[0])/*extra unread data may exist*/
		{
			size[0] += 1024;/*increase size to read*/
			*buf = _realloc(*buf, (size[0] - 1024), size[0]);
		}
	} while (wads == (ssize_t)size[0]);
	buf[0][linesize - 1] = '\0';/*terminate string c style*/
	return ((size_t)linesize);
}

/**
 * free_1 - frees single pointer
 * @cmd: double char pointer
 *
 * Return: nothing
 */
void free_1(char **cmd)
{
	if (cmd[0] != NULL)
	{
		free(cmd[0]);
		cmd[0] = 0;
	}
}
