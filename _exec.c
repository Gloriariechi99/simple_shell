#include "simpleshell.h"

/**
 * _exec - executes functions
 * @str: strings with command/s
 *
 * Return: 0 for SUCCESS, -1 for failure
 */
int _exec(char ***str)
{
	int ret = -1, val = 0;
	pid_t cld, child_t;

	if (*str[0] != NULL)
	{
		cld = fork();
		if (cld == -1)
			perror("No child created");
		else if (cld == 0)
		{
			ret = execve(*str[0], *str, NULL);
			if (ret == -1)/*error, exit*/
				perror("Error in execution");
		}
		else
		{
			child_t = wait(NULL);
			kill(child_t, SIGTERM);
		}
		free_cmd(str);
	}
	return (val);
}

/**
 * no_path - shows not found msg then set to next command
 * @cmd: commands
 * @ct: index of current command
 * @sep: processed commands
 * @argument: command process status
 * @argv: progname
 */
void no_path(char ***cmd, int *ct, char ***sep, int *argument, char *argv)
{
	int index;
	char sepa[] = "&;|";

	path_error(argv, cmd[0][*ct], "non", "not found");
	free_cmd(sep); /*free here coz it misses being freed ans _exec*/
	index  = *ct + 1;
	while (cmd[0][index] != NULL)/*ump options like -l /var in "hal -l /var" to NEXT INSTRUCTION*/
	{
		if (is_in(sepa, cmd[0][index][0])) /* aseparator found, thus exit loop to process it*/
			break;
		index++;
	}
	*ct = index; /*set index after options*/
	*argument = -1;/*prep argument to process next instruction if available*/
}

/**
 * to_exec - if conditions exist to execute
 * @execute: execution status
 * @cmd: commands
 * @ct: comand index
 * @sep: cnew commands
 *
 * Return: nothing
 */
void to_exec(int *execute, char ***cmd, int ct, char ***sep)
{
	if (*execute == 0 || (*execute != 0 && next_not_and(cmd, ct)))
		*execute = _exec(sep);
}

/**
 * _alias - process alias built-in
 * @cmd: commands
 * @Alice: alias structure
 * @argv: progname
 *
 * Return: nothing
 */
void _alias(char ***cmd, Alias **Alice, char *argv)
{
	int i = 0, x, found = 0;
	char **joint, **alias_names, strtak[50];

	if (cmd[0][1] == NULL)/*alias without options, display all aliases*/
		show_alice(Alice, &found, "by my self"); /*match everything*/
	else /* has options e.g alias dfh='df -h' [...] or alias dfh*/
	{
		_strcpy(strtak, cmd[0][1]);/*coz strtok messes the 1st argument*/
		if (strtok(strtak, "=") != NULL)/*is assignment e.g. alias ll='ls -l' OR reassignment*/
		{
			joint = joint_aliases(cmd, &alias_names);/*packs aliases back to name='value' per string, was split by interpret*/
			for (x = 0; alias_names[x] != NULL; x++)/*run through aliases to process*/
			{
				i = 0;/*initialize Alice structure to start from beginning*/
				while (_strcmp(Alice[0][i].name, "end") != 0)/*go to the end*/
				{
					edit_elias(Alice, i, &found, joint, x, alias_names);/*edit selection to new value*/
					i++;
				}
				if (found == 0)/*no existing aliases modified, add instead*/
					register_elias(Alice, i, joint);/*new add*/
			}
			free_cmd(&joint);
			free_cmd(&alias_names);
		}
		else /*display named aliases*/
		{
			for (x = 0; cmd[0][x] != NULL; x++)
			{
				found = 0;
				show_alice(Alice, &found, cmd[0][x]); /*display selected aliases*/
				if (found == 0)/*alias name not found*/
					path_error(argv, cmd[0][x], "alias", "not found");
			}
		}
	}
}

/**
 * joint_aliases - pack alias assignments to ouble pointer
 * @cmd: alias statements
 * @alias_name: alias name container
 *
 * Return: nothing
 */
char **joint_aliases(char ***cmd, char ***alias_name)
{
	char strtak[50], **joint, *name;
	int i = 1, index = 0, found[10] = {-1};/*initialize to unfound status*/

	alias_name[0] = (char **) malloc(sizeof(char *) * 5);
	if (alias_name[0] == NULL)
		exerror("Malloc Error");
	while (cmd[0][i] != NULL)/*find indices where alias begins and store in found*/
	{
		_strcpy(strtak, cmd[0][i]);/*coz strtok messes the 1st argument*/
		name = strtok(strtak, "=");
		if (_strcmp(name, cmd[0][i]) != 0)/*equal sign exists, is aliasname=..*/
		{
			found[index] = i;
			alias_name[0][index] = (char *) malloc(sizeof(char) * (_strlen(name) + 1));
			if (alias_name[0][index] == NULL)
				exerror("Malloc Error");
			_strcpy(alias_name[0][index], name);	/*store typed alias names*/
			index++;
		}
		i++;
	} /*index contain no of strings - 1*/
	joint = (char **) malloc(sizeof(char *) * (index + 2));
	if (joint == NULL)
		exerror("Malloc Error");
	i = 1;/*fisrt alias name index on cmd, 0 contain 'alias'*/
	index = 0;/*first alias position*/
	while (cmd[0][i] != NULL)
	{
		if (found[index] == i)
			set_joint(&joint, index, cmd, i);	/*create joint doule pointer write "alias_name='value"*/
		else
			concat_joint(&joint, index, cmd, i); /*append other values if any*/
		i++;
		if (found[index + 1] == i)
			index++;/*if next alias index pos matches current joint index*/
	}
	index++;
	joint[index] = NULL;/*last string*/
	return (joint);
}
