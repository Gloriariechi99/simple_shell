#include "simpleshell.h"

/**
 * _realloc - reallocated a mem space
 * @ptr: old memory space
 * @old_size: old memory size
 * @new_size: new memory size
 *
 * Return: new reallocated memory space
 */
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	void *new_ptr;

	if (new_size == 0 && ptr != NULL)
	{
		free(ptr);
		return (NULL);
	}
	else if (ptr == NULL)
		return (malloc(new_size * sizeof(char)));
	else if (new_size == old_size)
		return (ptr);
	else if (new_size > old_size)
	{
		new_ptr = malloc(new_size * sizeof(char));
		if (new_ptr == NULL)
			exerror("Malloc error");
		else
		{
			_strcpy(new_ptr, (char *)ptr);
			free(ptr);
		}
	}
	return (new_ptr);
}

/**
 * mallerror - malloc error
 * @str: double pointer
 * @small_str: single pointer
 *
 * Return: nothing
 */
void mallerror(char **str, char *small_str)
{
	if (str == NULL)
		exerror("Malloc Error");
	else if (small_str == NULL)
		perror("Malloc Error");

}

/**
 * in_builts - in built commands
 * @cs: commands
 * @ct: for cd
 * @pd: previous directory
 * @av: prog name
 * @Al: struct for aliases
 * @cp: executable path
 *
 * Return: nothing
 */
void in_builts(char ***cs, int *ct, char **pd, char *av, Alias **Al, char **cp)
{
	if (_strcmp(cs[0][0], "env") == 0)
		_env();
	else if (_strcmp(cs[0][0], "cd") == 0)
		_cd(ct, pd, cs);
	else if (_strcmp(cs[0][0], "alias") == 0)
		_alias(cs, Al, av);
	else/*possible executable commands*/
		option_folder_file_etc(cs, cp, av);
}

/**
 * init_sep - initialize sep
 * @sep: double pointer
 * @cmd_path: command path
 * @ct: cmd index
 * @cmd: commands
 *
 * Return: nothing
 */
void init_sep(char ***sep, char **cmd_path, int ct, char ***cmd)
{
	sep[0] = (char **) malloc(sizeof(char *) * 4);
	mallerror(sep[0], "no");
	_strcpy(cmd_path[0], cmd[0][ct]);
}

/**
 * buf_init - initialize buf
 * @buf: buffer to initialize
 * @fd: file descriptor
 * @count: buffer size
 * @argv: file name
 *
 * Return: nothing
 */
void buf_init(char **buf, int *fd, int count, char *argv)
{
	*buf = (char *) malloc(sizeof(char) * count);
	mallerror(buf, *buf);
	*fd = open(argv, O_RDONLY);
}
