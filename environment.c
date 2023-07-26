#include "simpleshell.h"

/**
 * _getenv - gets environment variable value
 * @name: variable searched
 *
 * Return: pointer to value
 */
char *_getenv(const char *name)
{
	char **my_env = environ, *ret = NULL, temp_ret[1024] = {0}, *real_ret = NULL;
	int i = 0, j;

	if (name == NULL)
		return (NULL);
	while (my_env[0] != NULL)/*search for var*/
	{
		if (!(_strcmp(my_env[0], name)))
			break;
		my_env++;
	}
	if (my_env[0] != NULL)
	{
		ret = (char *) malloc(sizeof(char) * (_strlen(my_env[0]) + 1));
		if (ret == NULL)
			exerror("Malloc");
		else if (my_env[0] != NULL)
		{
			if (_strcpy(ret, my_env[0]) != NULL)/*Successful copy*/
			{
				while (ret[i] != '=')/*increment up to '='*/
					i++;
				++i;/*next char after '='*/
				j = 0;
				while (ret[i] != '\0')/*copy cont from '='*/
				{
					temp_ret[j] = ret[i];
					++i;
					++j;
				}
				free(ret);
				temp_ret[j] = '\0';
				real_ret = (char *) malloc(sizeof(char) * (_strlen(temp_ret) + 1));
				_strcpy(real_ret, temp_ret);
			}
		}
	}
	return (real_ret);
}

/**
 * _setenv - sets value of environment variable
 * @name: variable
 * @value: variable value to be set
 * @overwrite: indicator of how to handle setting
 *
 * Return: 1 for SUCCESS, 0 otherwise
 */
int _setenv(const char *name, const char *value, int overwrite)
{
	char **my_env = environ;
	int ret = -1;

	(void)overwrite, (void)ret;
	if (name == NULL || value == NULL)
		return (-1);
	else if (name != NULL)
	{
		for (; *my_env; my_env++)
		{
			/*cmp = _strcmp(*my_env, name);*/
			/*if (cmp == 0)name exists as a variable*/
				/*{if (overwrite == 0)}*/
		}
	}
	return (0);
}
/**
 * _env - prints environment variables
 *
 * Return: nothing
 */
void _env(void)
{
	size_t u, v;
	char **envo = environ;

	for (u = 0; envo[u] != NULL && envo[u][0] != '\0'; u++)
	{
		v = 0;

		while (envo[u][v] != '\0')
			v++;
		write(STDOUT_FILENO, envo[u], v);
		write(STDOUT_FILENO, "\n", 1);
	}
}

/**
 * set_joint - prepare joint container and save alias_name='value
 * @joint: alias container
 * @index: joint index
 * @cmd: alias list
 * @i: cmd index
 *
 * Return: nothing
 */
void set_joint(char ***joint, int index, char ***cmd, int i)
{
	joint[0][index] = (char *) malloc(sizeof(char) * 50);/*enough space to contain alias like ll='ls -l -h'*/
	if (joint[0][index] == NULL)
		exerror("Malloc Error");
	_strcpy(joint[0][index], cmd[0][i]);
}

/**
 * concat_joint - append other alias values
 * @joint: alias container
 * @index: joint index
 * @cmd: alias list
 * @i: cmd index
 *
 * Return: nothing
 */
void concat_joint(char ***joint, int index, char ***cmd, int i)
{
	_strcat(joint[0][index], " ");/*restore space removed by interpret*/
	_strcat(joint[0][index], cmd[0][i]);
}
