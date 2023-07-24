#include "simpleshell.h"

/**
 * main - entry point
 * @argc: number of arguments
 * @argv: actual arguments
 *
 * Return: exit value
 */
int main(int argc, char **argv)
{
	char *buf, **cmd, *cmd_path;
	size_t size = 0;
	int ret = 0;
	ssize_t wads = 0;
	Alias *_alias;

	_alias = (Alias *) malloc(sizeof(Alias) * 20);
	if (_alias == NULL)
		exerror("Malloc Error");
	_strcpy(_alias[0].name, "end");
	cmd_path = (char *) malloc(sizeof(char) * 256);
	if (cmd_path == NULL)
		exerror("Malloc error");
	if (isatty(STDIN_FILENO) && argc == 1)/*not piped*/
		ret = loop_shell(argv[0], &cmd_path, &_alias);
	else if (isatty(STDIN_FILENO) == 0) /*piped*/
	{
		wads = _getline(&buf, &size, STDIN_FILENO);
		if (wads == -1)
			perror("No words read");
		cmd = interpret(buf);
		if (has_separator(&cmd)) /* commands are separated by ||, && or ; */
			command_separator(&cmd, &cmd_path, argv[0]);/* handles ; || and &&*/
		else
			option_folder_file_etc(&cmd, &cmd_path, argv[0]);/* handles everything else*/
		free_1(&buf);
	}
	if (argc == 2)/*input File with commands*/
		command_file(&cmd_path, argv);/*read and execute commands from file*/
	free_1(&cmd_path);
	free(_alias);
	return (ret);
}

/**
* package - prep commands for execution
* @cmd: commands
* @cmd_path: executable path
* @argv: prog name
*
* Return: nothing
*/
void package(char ***cmd, char **cmd_path, char *argv)
{
	char **pkg;
	int i = 0, cnt = 0;

	while (cmd[0][cnt] != 0)/*find no of strings*/
		cnt++;
	while (cmd[0][i] != NULL)
	{
		if (i == 0)/* exec name*/
		{
			pkg = (char **) malloc(sizeof(char *) * (cnt + 1));
			if (pkg == NULL)
				exerror("Malloc Error");
			_strcpy(cmd_path[i], cmd[0][i]);
			if (!file_found(cmd_path))/*executable not found*/
			{
				path_error(argv, cmd[0][0], "non", "not found");
				free_cmd(cmd);
				return;
			}
			else/*executable exists*/
			{
				pkg[i] = (char *) malloc(sizeof(char) * (_strlen(cmd[0][i]) + 1));
				if (pkg[i] == NULL)
					exerror("Malloc Error");
				_strcpy(pkg[i], cmd_path[0]);
			}
		}
		else
		{
			pkg[i] = (char *) malloc(sizeof(char) * (_strlen(cmd[0][i] + 1)));
			if (pkg[i] == NULL)
				exerror("Malloc Error");
			_strcpy(pkg[i], cmd[0][i]);
		}
		i++;
	}
	pkg[i] = NULL;
	_exec(&pkg);
	free_cmd(cmd);
}

/**
* option_folder_file_etc - determine command format
* @cmd: commands
* @cmd_path: executable path
* @argv: prog name
*
* Return: nothing
*/

void option_folder_file_etc(char ***cmd, char **cmd_path, char *argv)
{
	int stat_val;
	struct stat mode;

	if (cmd[0][1] == NULL)/*single string e.g ls*/
	{
		package(cmd, cmd_path, argv);
		return;
	}
	else if (cmd[0][1][0] == '-')/*options e.g ls -lh */
	{
		package(cmd, cmd_path, argv);
		return;
	}
	_strcpy(cmd_path[0], cmd[0][1]);
	stat_val = stat(cmd_path[0], &mode);
	if (stat_val == 0 && (mode.st_mode & S_IFMT) == S_IFDIR)/* status ok and is folder*/
	{
		package(cmd, cmd_path, argv);
		return;
	}
	else
	{
		if (file_found(cmd_path))/*executable or text file*/
		{
			if (access(cmd_path[0], X_OK) == 0)
				one_command(cmd, cmd_path, argv);/*instead of command_loop*/
			else
				package(cmd, cmd_path, argv);
		}
		else /*probably string eg echo gap*/
			package(cmd, cmd_path, argv);
	}
}

/**
 * command_separator - process separator & logical operators
 * @cmd: commands
 * @cmd_path: executable path
 * @argv: prog name
 *
 * Return: nothing
 */
void command_separator(char ***cmd, char **cmd_path, char *argv)
{
	char **sep, sepa[] = "|;&";
	int ct = 0, execute = 0, argument = 0;

	while (cmd[0][ct] != NULL)
	{
		if (argument == 0)/*e.g. ls*/
		{
			sep = (char **) malloc(sizeof(char *) * 4); /*command list e.g. ls -lh /var NULL*/
			if (sep == NULL)
				exerror("Malloc Error");
			_strcpy(cmd_path[0], cmd[0][ct]);
			if (file_found(cmd_path))
			{
				sep[argument] = (char *) malloc(sizeof(char) * (_strlen(cmd_path[0]) + 1));
				if (sep[argument] == NULL)
					exerror("Malloc Error");
				_strcpy(sep[argument], cmd_path[0]);
				if (cmd[0][ct + 1] == NULL) /*next string is NULL*/
					to_exec(&execute, cmd, ct, &sep);
			}
			else
				no_path(cmd, &ct, &sep, &argument, argv);
		}
		else /*options e.g. -lh*/
		{
			if (is_in(sepa, cmd[0][ct][0]))
			{
				sep[argument] = NULL; /*last string for execve*/
				argument = -1;/*will become 0 below to allocate next double pointer*/
				to_exec(&execute, cmd, ct, &sep);
			}
			else
			{
				sep[argument] = (char *) malloc(sizeof(char) * (_strlen(cmd[0][ct]) + 1));
				if (sep[argument] == NULL)
					exerror("Malloc Error");
				_strcpy(sep[argument], cmd[0][ct]);
				if (cmd[0][ct + 1] == NULL) /*next string is NULL*/
					to_exec(&execute, cmd, ct, &sep);
			}
		}
		argument++;
		ct++;
	}
}

/**
 * next_not_and - process logical operators
 * @cmd: commands
 * @ct: command index
 *
 * Return: logical status
 */
int next_not_and(char ***cmd, int ct)
{
	int ret = 1;

	while (cmd[0][ct] != NULL)
	{
		if (cmd[0][ct][0] == '&')
			ret = 0;
		ct++;
	}
	return (ret);
}
