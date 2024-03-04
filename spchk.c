#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <ctype.h>

#define PATH "/usr/share/dict/words"
#define MAX_STRINGS 313291
#define MAX_LENGTH 100


int is_txt_file(const char *filename)
{
    const char *dot = strrchr(filename, '.');
    return dot && strcmp(dot, ".txt") == 0;
}

int is_trailing_punctuation(char c)
{
    return c == '.' || c == ',' || c == '!' || c == '?' || c == ':' || c == ';' || c == ')';
}

void fix_word(char *word)
{
    int len = strlen(word);
    int i = 0, j = 0;

    // Skip leading punctuation marks
    while (i < len && (word[i] == '\'' || word[i] == '\"' || word[i] == '(' || word[i] == '[' || word[i] == '{'))
    {
        i++;
    }

    // Copy word characters until a trailing punctuation mark or whitespace is encountered
    while (i < len && !is_trailing_punctuation(word[i]) && !isspace(word[i]))
    {
        word[j++] = word[i++];
    }

    // Null-terminate the modified word
    word[j] = '\0';
}




int binary_search(char **arr, const char *string){
    int low = 0;
    int high = MAX_STRINGS - 1;

    while (low <= high){
        int mid = low + (high - low) / 2;
        int res = strcmp(arr[mid], string);
        if (res == 0) return 1;
        else if (res < 0){
            low = mid + 1;
        }
        else{
            high  = mid - 1;
        }
    }

    printf("FINAL COMP: %s\n", arr[low + (high - low) / 2]);
    return 0;

}

void capitalize_initials(char *str)
{
    int capitalize_next = 1;

    for (int i = 0; str[i] != '\0'; i++)
    {
        if (isspace(str[i]) || ispunct(str[i]))
        {
            capitalize_next = 1;
        }
        else if (isalpha(str[i]) && capitalize_next)
        {
            str[i] = toupper(str[i]);
            capitalize_next = 0;
        }
        else
        {
            str[i] = tolower(str[i]); // Ensure all non-initial letters are lowercase
        }
    }
}

void check_spelling(const char *filepath, char **dict)
{
    FILE *file = fopen(filepath, "r");
    if (file == NULL)
    {
        printf("Error opening file at: %s\n", filepath);
        return;
    }

    char word[512];

    while (fscanf(file, "%s", word) == 1)
    {
        fix_word(word);
        // Trim leading and trailing whitespace
        char *trimmed_word = strtok(word, " \t\n\r");
        if (trimmed_word && binary_search(dict, trimmed_word) != 1) 
        {
            printf("%s : %s\n", filepath, trimmed_word);
        }
    }

    fclose(file);
}


void add_string(char **array, int index, const char *string) {
    if (index < 0 || index >= MAX_STRINGS) {
        fprintf(stderr, "Index out of bounds %d \n", index);
        return;
    }
    array[index] = (char *)malloc((strlen(string) + 1) * sizeof(char)); // Allocate memory for the string
    if (array[index] == NULL) {
        perror("Memory allocation failed");
        return;
    }
    strcpy(array[index], string); 
    array[index][strlen(string)] = '\0'; 
}



void create_dictionary(char **dict)
{

    FILE *file = fopen(PATH, "r");

    char word[512];
    int idx = 0;

    while (fgets(word, sizeof(word), file) != NULL)
    {
        if(strcmp(word, "A") == 0) printf("COMP WORKED");
        char lower[MAX_LENGTH];
        char caps[MAX_LENGTH];

        strcpy(lower, word);
        strcpy(caps, word);

        for (int i = 0; lower[i]; i++)
        {
            lower[i] = tolower(lower[i]);
        }
        capitalize_initials(caps);

        if (strcmp(word, lower) != 0)
        {
            add_string(dict, idx, lower);
            idx++;
        }

        if (strcmp(word, caps) != 0)
        {
            add_string(dict, idx, caps);
            idx++;
        }

        add_string(dict, idx, word);
        idx++;

        capitalize_initials(lower);

        if (strcmp(word, lower) != 0)
        {
            add_string(dict, idx, lower);
            idx++;
        }

        
    }

    return;
}

void traverse_dir(const char *dir_path, char **dict)
{
    DIR *dir = opendir(dir_path);
    if (dir == NULL)
    {
        perror("Unable to open directory");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] == '.')
        {
            continue;
        }

        char full_path[PATH_MAX];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

        struct stat entry_stat;
        if (stat(full_path, &entry_stat) == -1)
        {
            perror("stat");
            continue;
        }

        if (S_ISDIR(entry_stat.st_mode))
        {
            traverse_dir(full_path, dict);
        }
        else if (S_ISREG(entry_stat.st_mode) && is_txt_file(entry->d_name))
        {
            check_spelling(full_path, dict);
        }
    }

    closedir(dir);
}

int compare_strings(const void *a, const void *b)
{
    const char *str1 = *(const char **)a;
    const char *str2 = *(const char **)b;
    return strcmp(str1, str2);
}

int main()
{
    char **stringDict = (char **)calloc(MAX_STRINGS, sizeof(char *));
    create_dictionary(stringDict);

    //ASCII SORTING DICT FOR BINARY SEARCH, DO NOT REMOVE
    qsort(stringDict, MAX_STRINGS, sizeof(char *), compare_strings);

    traverse_dir("test_dir", stringDict);
    
    return EXIT_SUCCESS;
}
