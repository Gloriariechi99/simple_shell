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
