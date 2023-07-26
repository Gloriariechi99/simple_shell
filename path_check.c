#include "simpleshell.h"

/**
 * one_command - process multiple commands, 1 command at a time
 * @cmd: list of commands
 * @cmd_path: executable path
 * @argv: prog name
 *
 * Return: nothing
 */
void one_command(char ***cmd, char **cmd_path, char *argv)
{
	char **new_cmds = NULL;
	int i = 0;

	while (cmd[0][i] != NULL)
	{
		_strcpy(cmd_path[0], cmd[0][i]);
		if (has_separator(cmd))
			command_separator(cmd, cmd_path, argv);
		else
		{
			if (_strcmp(cmd_path[0], "exit") == 0)
			{
				free_cmd(cmd);
				break;
			}
			else if (_strcmp(cmd_path[0], "env") == 0)
				_env();
			else
			{
					if (file_found(cmd_path)) /*probable executable found*/
					{
						new_cmds = (char **) malloc(sizeof(char *) * 2);
						mallerror(new_cmds, "no");
						new_cmds[1] = NULL;/*needed by execve*/
						new_cmds[0] = (char *) malloc(sizeof(char) * (_strlen(cmd_path[0]) + 1));
						mallerror(&new_cmds[0], new_cmds[0]);
						_strcpy(new_cmds[0], cmd_path[0]);
						_exec(&new_cmds);
					}
					else
						path_error(argv, cmd[0][i], "non", "not found");
			}
		}
		i++;
	}
	free_cmd(cmd);
}

/**
 * file_found - file existence
 * @cmd: referenced file_name
 *
 * Return: 1 for SUCCESS, 0 otherwise
 */
int file_found(char **cmd)
{
	char *path, cpy_path[256], *real_path, curdir[250];
	size_t size_dir = 250;

	path = getenv("PATH");
	_strcpy(cpy_path, path);
	getcwd(curdir, size_dir);
	if (cmd[0] == NULL)
		return (0);
	if (access(cmd[0], F_OK) == 0)/*if in local*/
		return (1);
	real_path = strtok(cpy_path, ":");/*split first path*/
	if (real_path == NULL)
		return (0);
	chdir(real_path);
	if (access(cmd[0], F_OK) == 0)
	{
		stitch(cmd, real_path);
		chdir(curdir);
		return (1);
	}
	while (real_path != NULL)
	{
		real_path = strtok(NULL, ":");
		if (real_path == NULL)
		{
			chdir(curdir);
			return (0);
		}
		chdir(real_path);
		if (access(cmd[0], F_OK) == 0)
		{
			chdir(curdir);
			stitch(cmd, real_path);
			return (1);
		}
	}
	return (0);
}

/**
 * command_file - runs commands from file
 * @cmd_path: executable path
 * @argv: prog name and arguments
 *
 * Return: nothing
 */
void command_file(char **cmd_path, char **argv)
{
	int fd;
	ssize_t wads;
	size_t count = 1024, new_size = 0, resize = 0;
	char *buf, **cmd;

	buf_init(&buf, &fd, count, argv[1]);
	if (fd == -1)
	{
		free_1(&buf);
		path_error(argv[0], argv[1], "non", "Can't open");
		return;
	}
	wads = read(fd, buf, count);
	if (wads == -1)
		exerror("Error while reading file");
	else
	{
		new_size = count;
		while (wads == (ssize_t)count) /*possible remaining data to read*/
		{
			wads = read(fd, buf, count); /*find file size*/
			read_error(wads);
			new_size += wads;
			resize++;
		}
		if (resize > 0)
		{
			lseek(fd, 0, SEEK_SET);/*goto begining of file*/
			buf[wads - 1] = '\0';
			buf = _realloc(buf, count, new_size);
			wads = read(fd, buf, new_size); /*read entire file*/
			if (wads == -1)
				perror("Error reading whole file");
		}
		close(fd);
		buf[wads - 1] = '\0';
		cmd = interpret(buf);
		free_1(&buf);
		one_command(&cmd, cmd_path, argv[0]);
	}
}

/**
 * path_error - error message for command execution
 * @argv: prog name
 * @cmd: executable
 * @msg: message
 * @elias: alias
 *
 * Return: nothing
 */
void path_error(char *argv, char *cmd, const char *elias, const char *msg)
{
	if (_strcmp(elias, "non") == 0) /*not alias */
	{
		write(STDERR_FILENO, argv, _strlen(argv));
		write(STDERR_FILENO, ": 1: ", 6);
		if ((_strcmp("not found", msg)) == 0)
		{
			write(STDERR_FILENO, cmd, _strlen(cmd));
			write(STDERR_FILENO, ": ", 2);
			write(STDERR_FILENO, msg, _strlen(msg));
		}
		else
		{
			write(STDERR_FILENO, msg, _strlen(msg));
			write(STDERR_FILENO, " ", 1);
			write(STDERR_FILENO, cmd, _strlen(cmd));
		}
	}
	else
	{
		write(STDOUT_FILENO, argv, _strlen(argv));
		write(STDOUT_FILENO, ": ", 2);
		write(STDOUT_FILENO, elias, _strlen(elias));
		write(STDOUT_FILENO, ": ", 2);
		write(STDOUT_FILENO, cmd, _strlen(cmd));
		write(STDOUT_FILENO, ": ", 2);
		write(STDERR_FILENO, msg, _strlen(msg));
	}
	write(STDERR_FILENO, "\n", 1);
}

/**
 * has_separator - determine if separators or logical operators exists
 * @cmd: commands
 *
 * Return: 1 for success, 0 otherwise
 */
int has_separator(char ***cmd)
{
	int ret = 0, i = 0;
	char sep[] = "&;|";

	while (cmd[0][i] != NULL)
	{
		if (is_in(sep, cmd[0][i][0]))
		{
			ret = 1;
			break;
		}
		i++;
	}
	return (ret);
}
