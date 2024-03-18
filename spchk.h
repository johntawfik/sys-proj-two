#ifndef SPCHK_H
#define SPCHK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <ctype.h>
#include <stdbool.h>

#define PATH "/usr/share/dict/words" 
#define MAX_LENGTH 100

extern bool error_found;
extern int curr_size;

int is_txt_file(const char *filename);
int is_trailing_punctuation(char c);
void fix_word(char *word);
int binary_search(char **arr, const char *string);
void capitalize_initials(char *str);
void check_spelling(const char *filepath, char **dict);
void add_string(char **array, int index, const char *string);
char **create_dictionary(char **dict, char *dict_path);
void traverse_dir(const char *dir_path, char **dict);
int compare_strings(const void *a, const void *b);

#endif 
