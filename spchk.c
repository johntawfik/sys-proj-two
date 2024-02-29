#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#define PATH "/usr/share/dict/words"
#define MAX_STRINGS 104334
#define MAX_LENGTH 100


// Function to check if the file name ends with ".txt"
int is_txt_file(const char *filename) {
    const char *dot = strrchr(filename, '.');
    return dot && strcmp(dot, ".txt") == 0;
}

// Function to check spelling in a file (to be implemented)
void check_spelling(const char *filepath) {
    printf("Checking spelling in: %s\n", filepath);
}

// Recursive function to traverse directories
void traverse_dir(const char *dir_path) {
    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        perror("Unable to open directory");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        // Skip '.' and '..' entries and files/directories starting with '.'
        if (entry->d_name[0] == '.') {
            continue;
        }

        // Construct full path of the entry
        char full_path[PATH_MAX];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

        // Check if entry is a directory or a file
        struct stat entry_stat;
        if (stat(full_path, &entry_stat) == -1) {
            perror("stat");
            continue;
        }

        if (S_ISDIR(entry_stat.st_mode)) {
            // Recursively traverse the directory
            traverse_dir(full_path);
        } else if (S_ISREG(entry_stat.st_mode) && is_txt_file(entry->d_name)) {
            // If it's a regular file and ends with '.txt', check its spelling
            check_spelling(full_path);
        }
    }

    closedir(dir);
}

void add_string(char **array, int index, const char *string) {
    if (index < 0 || index >= MAX_STRINGS) {
        fprintf(stderr, "Index out of bounds\n");
        return;
    }
    array[index] = (char *)malloc(MAX_LENGTH * sizeof(char));
    if (array[index] == NULL) {
        perror("Memory allocation failed");
        return;
    }
    strncpy(array[index], string, MAX_LENGTH - 1);
    array[index][MAX_LENGTH - 1] = '\0';  // Ensure null-termination
}

void create_dictionary(char **dict){

        FILE *file = fopen(PATH, "r");
        

        char word[512];
        int idx = 0;
        

        while (fgets(word, sizeof(word), file) != NULL && idx <= MAX_STRINGS) {
            add_string(dict, idx, word);
            idx++;
        }

    return;

}

// Function to add a string to the array at the specified index



int main() {
    
    char **stringDict = (char **)calloc(MAX_STRINGS, sizeof(char*));

    create_dictionary(stringDict);
    traverse_dir("test_dir");


    return EXIT_SUCCESS;
}
