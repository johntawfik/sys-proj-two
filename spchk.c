#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_LENGTH 100

bool error_found = false;
int curr_size = 100000;

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

    while (i < len && (word[i] == '\'' || word[i] == '\"' || word[i] == '(' || word[i] == '[' || word[i] == '{'))
    {
        i++;
    }

    while (i < len && !is_trailing_punctuation(word[i]) && !isspace(word[i]))
    {
        word[j++] = word[i++];
    }

    word[j] = '\0';
}


int binary_search(char **arr, const char *string){
    int low = 0;
    int high = curr_size - 1;

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

    return 0;

}

void capitalize_initials(char *str)
{
    int capitalize_next = 1;

    for (int i = 0; str[i] != '\0'; i++)
    {
        if ((isspace(str[i]) || (ispunct(str[i]) && str[i] != '\'')))
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
            str[i] = tolower(str[i]); 
        }
    }
}

void check_spelling(const char *filepath, char **dict) {
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        printf("Error opening file at: %s\n", filepath);
        exit(EXIT_FAILURE);
    }

    char line[512];
    int line_number = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        line_number++;
        int column_number = 0;
        char *context;
        char *word = strtok_r(line, " \t\n\r", &context);

        while (word != NULL) {
            fix_word(word); // Modify the word to remove leading/trailing punctuation, etc.
            char *hyphen = strchr(word, '-');
            if (hyphen) {
                char* original_word = malloc(strlen(word) + 1);

                if (original_word == NULL) {
                    fprintf(stderr, "Failed to allocate memory.\n");
                    exit(EXIT_FAILURE);
                }

                strcpy(original_word, word);


                bool is_correct = true;
                char *part = strtok(word, "-");
                while (part != NULL && is_correct) {
                    if (binary_search(dict, part) != 1) {
                        is_correct = false;
                    }
                    part = strtok(NULL, "-");
                }

                if (!is_correct) {
                    printf("%s (%d,%d): %s\n", filepath, line_number, column_number + 1, original_word);
                    error_found = true;
                }
                free(original_word);
            } else {
                if (binary_search(dict, word) != 1) {
                    printf("%s (%d,%d): %s\n", filepath, line_number, column_number + 1, word);
                    error_found = true;
                }
            }
            column_number += strlen(word) + 1;
            word = strtok_r(NULL, " \t\n\r", &context);
        }
    }

    fclose(file);
}



void add_string(char **array, int index, const char *string) {
    if (index < 0 || index >= curr_size) {
        fprintf(stderr, "Index out of bounds %d \n", index);
        return;
    }
    array[index] = (char *)malloc((strlen(string) + 1) * sizeof(char)); 
    if (array[index] == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    strcpy(array[index], string); 
    array[index][strlen(string)] = '\0'; 
}



char **create_dictionary(char **dict, char *dict_path) {
    FILE *file = fopen(dict_path, "r");
    if (file == NULL) {
        perror("Failed to open dictionary file");
        exit(EXIT_FAILURE);
    }

    char word[512];
    int idx = 0;

    while (fgets(word, sizeof(word), file) != NULL)
    {
        if (idx >= curr_size - 6) {
            char **tempDict = (char **)realloc(dict, curr_size * 2 * sizeof(char *));
            if (tempDict == NULL) {
                printf("Memory reallocation failed.\n");
                free(dict);
                exit(EXIT_FAILURE);
            } else {
                dict = tempDict;
                curr_size *= 2;
            }
        }

        size_t len = strlen(word);
        if (len > 0 && word[len - 1] == '\n') {
            word[len - 1] = '\0'; 
        }

        char lower[MAX_LENGTH];
        char caps[MAX_LENGTH];

        strcpy(lower, word);
        strcpy(caps, word);

        for (int i = 0; lower[i]; i++) {
            lower[i] = tolower(lower[i]);
        }

        for (int i = 0; caps[i]; i++) {
            caps[i] = toupper(caps[i]);
        }
        

        if (strcmp(word, lower) != 0) {
            add_string(dict, idx, lower);
            idx++;
        }

        if (strcmp(word, caps) != 0) {
            add_string(dict, idx, caps);
            idx++;
        }

        add_string(dict, idx, word);
        idx++;

        capitalize_initials(lower);

        if (strcmp(word, lower) != 0) {
            add_string(dict, idx, lower);
            idx++;
        }
    }


    fclose(file);
    return dict;
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
        // Skip the current and parent directory entries "." and ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        char full_path[PATH_MAX];

        if (snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name) >= PATH_MAX) {
            fprintf(stderr, "Path length has exceeded the limit: %s/%s\n", dir_path, entry->d_name);
            continue;
        }

        struct stat entry_stat;
        if (stat(full_path, &entry_stat) == -1)
        {
            perror("Failed to get file status");
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

int main(int argc, char *argv[])
{
    //ARG FORMAT: ./spchk ../dict ../testfile
    if (argc < 3) {
        fprintf(stderr, "Improper Argument Usage: %s <directory_path>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    char **stringDict = (char **)calloc(curr_size, sizeof(char *));
    if (stringDict == NULL) {
        perror("Failed to allocate memory for the dictionary");
        exit(EXIT_FAILURE);
    }
    stringDict = create_dictionary(stringDict, argv[1]);

    for(int i = 0; i < curr_size; i++){
        if(stringDict[i] == NULL){
            char **tempDict = (char **)realloc(stringDict, (i * sizeof(char *)));
            if (tempDict == NULL) {
                printf("Memory reallocation failed.\n");
                free(stringDict);
                exit(EXIT_FAILURE);
            } else {
                stringDict = tempDict;
                curr_size = i;
                break;
            }
        }
    }

    //ASCII SORTING DICT FOR BINARY SEARCH, DO NOT REMOVE
    qsort(stringDict, curr_size, sizeof(char *), compare_strings);

    for(int i = 2; i < argc; i++){
        if(is_txt_file(argv[i])){
            check_spelling(argv[i], stringDict);
        }else{
            traverse_dir(argv[i], stringDict);
        }
    }
    
    
    if (error_found) {
        printf("Incorrect words found, exiting with EXIT_FAILURE \n");
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}
