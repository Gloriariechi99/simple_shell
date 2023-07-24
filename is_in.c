#include "simpleshell.h"

/**
 * is_in - checks for allowed chars
 * @String: string checked for chars
 * @T: allowed char
 *
 * Return: 1 for success
 */
int is_in(char *String, char T)
{
	int ret = 0, count = 0;

	while (String[count] != '\0')
	{
		if (String[count] == T)
		{
			ret = 1;
			break;
		}
		count++;
	}
	return (ret);
}

/**
 * exerror - prints execution error
 * @String: name of command executed
 *
 * Return: nothing
 */
void exerror(char *String)
{
	perror(String);
	exit(1);
}

/**
 * stitch - join path to executable
 * @cmd: commands
 * @real_path: path
 *
 * Return: nothing
 */
void stitch(char **cmd, char *real_path)
{
	char temp[256] = {0};

	_strcpy(temp, *cmd);
	_strcpy(*cmd, real_path);
	_strcat(*cmd, "/");
	_strcat(*cmd, temp);
}

/**
 * my_atoi - converts string to integer
 * @str: string to be converted
 *
 * Return: integer
 */
int my_atoi(char *str)
{
	int result = 0, i = 1;

	result = str[0] - '0';
	while (str[i] != '\0')
	{
		result = (result * 10) + (str[i] - '0');
		i++;
	}
	return (result);
}

/**
 * _changed - changes directory
 * @dir: directory
 * @prevdir: previous directory
 *
 * Return: nothing
 */
void _changed(char *dir, char **prevdir)
{
	char *home;
	const char curdir[256] = {0};
	size_t size = 256;

	home = getenv("HOME");
	if (getcwd((char *)curdir, size) == NULL)
	{
		perror("Getcwd error");
		return;
	}
	if (dir == NULL)
	{
		if (home == NULL)
		{
			perror("No home found");
			return;
		}
		chdir(home);
		setenv("PWD", home, 0);
	}
	else if (dir[0] == '-')
	{
		chdir(*prevdir);
		setenv("PWD", *prevdir, 0);
	}
	else
	{
		chdir(dir);
		setenv("PWD", dir, 0);
	}
	_strcpy(*prevdir, curdir);
}
