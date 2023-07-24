#include "simpleshell.h"

/**
 * _strcpy - copies a string
 * @dest: pointer to buffer
 * @src: pointer to string
 *
 * Return: character
 */
char *_strcpy(char *dest, const char *src)
{
	int u = 0, len = 0;

	len = _strlen(src) + 1;
	while (u < len)
	{
		dest[u] = src[u];
		u++;
	}
	printf("u is %d\n", u);
	dest[u] = '\0';
	return (dest);
}

/**
 * _strlen - returns length of string
 * @s: checked string
 *
 * Return: nothing.
 */
int _strlen(const char *s)
{
	int u = 0;

	while (s[u] != '\0')
		u++;
	return (u);
}

/**
 * _strcmp - compares two strings
 * @s1: first string
 * @s2: second string
 *
 * Return: 0 for success, any other for fail.
 */
int _strcmp(const char *s1, const char *s2)
{
	int i = 0, ret = 0;

	if (_strlen(s1) < _strlen(s2))
		return (-1);
	else if (_strlen(s1) > _strlen(s2))
		return (1);
	while (s1[i] != '\0')
	{
		if (s1[i] < s2[i])
		{
			ret = -1;
			break;
		}
		else if (s1[i] > s2[i])
		{
			ret = 1;
			break;
		}
		i++;
	}
	return (ret);
}

/**
 * _strcat - put strings together
 * @dest: destination point
 * @src: source string
 *
 * Return: added string
 */
char *_strcat(char *dest, const char *src)
{
	int countd = 0, counts = 0;

	while (dest[countd] != '\0')
		countd++;
	while (src[counts] != '\0')
	{
		dest[countd] = src[counts];
		countd++;
		counts++;
	}
	dest[countd] = '\0';
	return (dest);
}

/**
 * _strncpy - copies strings
 * @dest: destination point
 * @src: source of string
 * @n: number of times to copy
 *
 * Return: cpoied string.
 */
char *_strncpy(char *dest, const char *src, int n)
{
	int i;

	for (i = 0; i < n && src[i] != '\0'; i++)
		dest[i] = src[i];
	dest[i] = '\0';/**just one nul will do **/
	return (dest);
}
