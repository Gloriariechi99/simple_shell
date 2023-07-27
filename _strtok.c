#include "simpleshell.h"

/**
 * interpret - reads the commands and stores as strings
 * @T: string read from input
 * Return: commands / arguments in new memory space
 */
char **interpret(char *T)
{
	int  c = 0, argdx = 0, str_no = 0;
	char **cmdargs, q[] = "abcdefghijklmnopqrstuvwxyz.-/_01234567890ABCDEFG";
	char r[] = "OHIJKLMNPQRSTUVWXYZ;&|='";

	cmdargs = (char **) malloc(sizeof(char *) * (20));
	mallerror(cmdargs, "not");
	do {
		if (is_in(q, T[c]) || is_in(r, T[c]))
		{
			if (argdx == 0) /*malloc at the start of string*/
				cmdargs[str_no] = (char *) malloc(sizeof(char) * 20);
			mallerror(&cmdargs[str_no], cmdargs[str_no]);
			cmdargs[str_no][argdx] = T[c];/*copy character into string*/
			argdx++;
		}
		else if ((T[c] == '\n' || T[c] == '\r' || T[c] == ' ') && argdx > 0)
		{
			if (argdx != 0)/*no string being processed*/
			{
				cmdargs[str_no][argdx] = '\0';
				argdx = 0;
				str_no++;
			}
		}
		else if (T[c] == '\0' || T[c] == '#')
		{
			if (argdx > 0)
			{
				cmdargs[str_no][argdx] = '\0';
				argdx = 0;
				str_no++;
			}
			break;
		}
		c++;
	} while (1);
	cmdargs[str_no] = 0; /*last string is NULL, required by execve*/
	if (cmdargs[0] == NULL)
		free_cmd(&cmdargs);
	return (cmdargs);
}

/**
 * show_alice - display aliases
 * @Alice: Alias structure list
 * @found: Alias index
 * @cmp: comparing aliases
 *
 * Return: nothing
 */
void show_alice(Alias **Alice, int *found, const char *cmp)
{
	char CMP[50]; /* determine if display all or matching aliases*/
	int i, j = *found;

	for (i = 0; _strcmp(Alice[0][i].name, "end") != 0; i++)
	{
		if (_strcmp(cmp, "by my self") == 0)
			_strcpy(CMP, Alice[0][i].name);/* match all*/
		else
			_strcpy(CMP, cmp);
		if (strcmp(CMP, Alice[0][i].name) == 0)/* if named alias matches then print*/
		{
			write(STDOUT_FILENO, Alice[0][i].name, _strlen(Alice[0][i].name));
			write(STDOUT_FILENO, "=", 1);
			write(STDOUT_FILENO, Alice[0][i].command, _strlen(Alice[0][i].command));
			write(STDOUT_FILENO, "\n", 1);
			j++;
		}
	}
	*found = j;
}

/**
 * register_elias - add alias to stucture
 * @Alice: Alias structure
 * @i: index
 * @joint: container for alias
 *
 * Return: nothing
 */
void register_elias(Alias **Alice, int i, char **joint)
{
	int x;
	char strtak[50], *nam, *com;

	for (x = 0; joint[x] != NULL; x++) /*registering*/
	{
		_strcpy(strtak, joint[x]);/*coz strtok messes the 1st argument*/
		nam = strtok(strtak, "=");
		com = strtok(NULL, "=");
		_strcpy(Alice[0][i].name, nam);
		_strcpy(Alice[0][i].command, com);
		i++;
	}
	_strcpy(Alice[0][i].name, "end");
}

/**
 * edit_elias - edit alias values
 * @Alice: alias structure
 * @i: alias index
 * @sta: edit status
 * @jo: container for alias
 * @x: alias_names index
 * @alias: list of aliases
 *
 * Return: nothing
 */
void edit_elias(Alias **Alice, int i, int *sta, char **jo, int x, char **alias)
{
	char strtak[50], *com;
	int j = *sta;

	if (_strcmp(Alice[0][i].name, alias[x]) == 0)
	{
		j++;/*alias required editing, not adding*/
		_strcpy(strtak, jo[x]);/*coz strtok messes the 1st argument*/
		strtok(strtak, "=");
		com = strtok(NULL, "="); /*capture NEW value for given alias*/
		_strcpy(Alice[0][i].command, com); /*edit current value to new one*/
	}
	*sta = j;
}

/**
 * is_elias - execute aliases
 * @cmd: alias command
 * @cmd_path: current alias name
 * @elias: Alias structure
 * @argv: prog name
 *
 * Return: 1 if executed, else 0
 */
int is_elias(char ***cmd, char *cmd_path, Alias *elias, char *argv)
{
	int ret = 0, i = 0, j = 0, k;
	char strtak[50], *com = NULL, **alsa = NULL, **alsb = NULL;

	while (_strcmp(elias[i].name, "end") != 0)/*till last position*/
	{
		if (_strcmp(elias[i].name, cmd[0][0]) == 0) /*command is an alias*/
		{
			_strcpy(strtak, elias[i].command);/*coz strtok messes string*/
			com = strtok(strtak, "'");
			alsa = interpret(com); /*capture alias command and arguments*/
			alsb = (char **) malloc(sizeof(char *) * 20);
			if (alsb == NULL)
				exerror("Malloc Error");
			while (alsa[j] != NULL)
			{
				alsb[j] = (char *) malloc(sizeof(char) * (_strlen(alsa[j]) + 1));
				if (alsb[j] == NULL)
					exerror("Malloc Error");
				_strcpy(alsb[j], alsa[j]);/*copy alias value e.g. ls -l -h to alsb*/
				j++;
			}
			free_cmd(&alsa);
			k = 1;/*index 0 is the alias name*/
			while (cmd[0][k] != NULL)/*Append options following alias e.g. /var*/
			{
				alsb[j] = (char *) malloc(sizeof(char) * (_strlen(cmd[0][k]) + 1));
				if (alsb[j] == NULL)
					exerror("Malloc Error");
				_strcpy(alsb[j], cmd[0][k]);
				j++;
				k++;
			}
			alsb[j] = NULL;/*last string needed by execve*/
			package(&alsb, &cmd_path, argv);
			ret = 1;
			break;
		}
		i++;
	}
	return (ret);
}
