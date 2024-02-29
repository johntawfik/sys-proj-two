#include <stdio.h>
#include <stdlib.h>

int main() {
    // Open the dictionary file
    FILE *file = fopen("/usr/share/dict/words", "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    char word[256]; // Assuming no word exceeds 255 characters
    int count = 0;

    // Read and print the first 10 words
    while (fgets(word, sizeof(word), file) != NULL && count < 100) {
        // Print each word. Use %s to print the string and remove the newline character fgets adds
        printf("%s", word);
        count++;
    }

    // Close the file
    fclose(file);
    return EXIT_SUCCESS;
}
