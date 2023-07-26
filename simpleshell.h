#ifndef SIMPLESHELL_H
#define SIMPLESHELL_H

#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>

#ifndef alice
#define alice
/**
 * struct alias - structure of aliases
 * @name: alias name
 * @command: actual command
 */
typedef struct alias
	{
		char name[10];
		char command[50];
	} Alias;
#endif

extern char **environ;
void read_error(int wads);
int pip_exit(char ***cmd);
void exit_proc(char ***cmds, char **lines, int *ret);
void buf_init(char **buf, int *fd, int count, char *argv);
void init_sep(char ***sep, char **cmd_path, int ct, char ***cmd);
void in_builts(char ***cs, int *ct, char **pd, char *av, Alias **A, char **cp);
void mallerror(char **str, char *small_str);
void register_elias(Alias **Alice, int i, char **joint);
void edit_elias(Alias **Alice, int i, int *fo, char **jo, int x, char **alia);
void set_joint(char ***joint, int index, char ***cmd, int i);
void concat_joint(char ***joint, int index, char ***cmd, int i);
int is_elias(char ***cmd, char *cmd_path, Alias *elias, char *argv);
void show_alice(Alias **Alice, int *found, const char *cmp);
char **joint_aliases(char ***cmd, char ***alias_name);
void to_exec(int *execute, char ***cmd, int ct, char ***sep);
void _alias(char ***cmd, Alias **Alice, char *argv);
void package(char ***cmd, char **cmd_path, char *argv);
void no_path(char ***cmd, int *ct, char ***sep, int *argument, char *argv);
int next_not_and(char ***cmd, int ct);
void command_separator(char ***cmd, char **cmd_path, char *argv);
int has_separator(char ***cmd);
size_t _getline(char **buf, size_t *size, int fd);
void _changed(char *dir, char **prevdir);
int my_atoi(char *str);
void stitch(char **cmd, char *real_path);
int file_found(char **cmd);
void path_error(char *argv, char *cmd, const char *elias, const char *msg);
void one_command(char ***temp_cmds, char **cmd_path, char *argv);
void command_file(char **cmd_path, char **argv);
void option_folder_file_etc(char ***cmd, char **cmd_path, char *argv);
int is_in(char *String, char T);
void exerror(char *String);
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
void free_cmd(char ***cmd);
void free_1(char **cmd);
void _cd(int *count, char **prevdir, char ***cmds);
char *_getenv(const char *name);
void _env(void);
int _exec(char ***str);
char **interpret(char *T);
int loop_shell(char *argv, char **cmd_path, Alias **alice, char *pd, int *ct);
int _strcmp(const char *s1, const char *s2);
int _strlen(const char *s);
char *_strcpy(char *dest, const char *src);
char *_strcat(char *dest, const char *src);

#endif
