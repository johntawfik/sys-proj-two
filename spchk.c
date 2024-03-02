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


int is_txt_file(const char *filename) {
    const char *dot = strrchr(filename, '.');
    return dot && strcmp(dot, ".txt") == 0;
}

void check_spelling(const char *filepath) {
    printf("Checking spelling in: %s\n", filepath);
}

void traverse_dir(const char *dir_path) {
    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        perror("Unable to open directory");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') {
            continue;
        }

        char full_path[PATH_MAX];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

        struct stat entry_stat;
        if (stat(full_path, &entry_stat) == -1) {
            perror("stat");
            continue;
        }

        if (S_ISDIR(entry_stat.st_mode)) {
            traverse_dir(full_path);
        } else if (S_ISREG(entry_stat.st_mode) && is_txt_file(entry->d_name)) {
            check_spelling(full_path);
        }
    }

    closedir(dir);
}

int binary_search(char **arr, const char *string){
    int low = 0;
    int high = (sizeof(arr) / sizeof(arr[0])) - 1;

    while (low <= high){
        int mid = low + (high - low) / 2;
        printf("%s \n", arr[mid]);
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

void add_string(char **array, int index, const char *string) {
    if (index < 0 || index >= MAX_STRINGS) {
        fprintf(stderr, "Index out of bounds %d \n", index);
        return;
    }
    array[index] = (char *)malloc(MAX_LENGTH * sizeof(char));
    if (array[index] == NULL) {
        perror("Memory allocation failed");
        return;
    }
    strncpy(array[index], string, MAX_LENGTH - 1);
    array[index][MAX_LENGTH - 1] = '\0';  
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



int main() {
    
    char **stringDict = (char **)calloc(MAX_STRINGS, sizeof(char*));

    create_dictionary(stringDict);
    traverse_dir("test_dir");

    return EXIT_SUCCESS;
}
