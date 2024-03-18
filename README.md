# Project II: Spelling Checker

# John Tawfik (jt1121) | Toye Sobayo (os224)

## Overview

This project involves the development of a spelling checker program named spchk that detects and reports spelling errors in text files against a given dictionary. It incorporates Posix functions for file and directory operations and explores efficient data structures for quick word lookups.

## Table of Contents

- [Background](#background)
- [Implementation](#implementation)
- [Executables](#executables)
- [Testing](#testing)



## Background

The spchk program compares words in text files to those in a specified dictionary file, reporting any discrepancies. It supports checking multiple text files and traversing directories to find text files recursively. The program handles punctuation, capitalization variations, and hyphenated words.

### Implementation

Dictionary Management

A dictionary file, sorted lexicographically with case sensitivity, serves as the reference for correct spelling. The program uses an array for storing the dictionary and facilitating rapid lookups using binary search.

Dictionary Structure: The dictionary is loaded into an array that we sort based upon ASCII values to facilitate fast searches in O(log(n)) time, considering ASCII case sensitivity and aspects such as hyphenation. This setup is crucial for handling large dictionaries efficiently.

#### Text File Processing

The program reads text files, identifying words while managing punctuation, capitalization, and hyphenation. It keeps track of line and column numbers for accurate error reporting.

Text Parsing: Special rules for trailing punctuation, quotation marks, brackets, and hyphenation are implemented to accurately identify words. This parsing is essential for precise spelling checks.


#### Directory Traversal

spchk can recursively traverse directories, checking all .txt files while ignoring files and directories starting with .. This feature requires careful management of file paths and directory structures.

Recursive Search: The program employs a recursive strategy to navigate through directories, applying the spelling checker to each applicable text file. This comprehensive approach ensures no file is overlooked.


### Executables 
  
    spchk.c - 
    - make 
    - ./spchk <my_dict> <files/directories to be checked> 
    - make test
    - ./spchk /usr/share/dict/words test_dir my_file.txt 

    Note: after you run "make test" it will say "command exited with non-zero status 1" because it exits with status "EXIT_FAILURE" due to finding words that are spelled incorrectly 



### Error Reporting
    
    When spelling errors are detected, spchk reports the erroneous word along with its location (file, line, and column). This precise reporting aids in quickly locating and correcting spelling errors.


### Testing plan

 ### Correctness Testing

    Set up a directory named "test_dir" that we used to test our code functionality

    Inside the "test_dir" directory we included files such as "systems.systems" and "toye" to demonstrate how the program is supposed to ignore these files during the recursive directory traversal due to them not ending in ".txt". We also included files like ".john" to demonstrate how the program is supposed to ignore files that begin with ".".

    The rest of the files in test_dir are all valid text files and should be opened and checked. We have text files that are empty like "hello.txt", files with words that contain different capitalizations, hyphenated words, and trailing punctuation (check.txt) and then files that have words spelled incorrectly (text.txt)
    

  #### Rationale Behind the Tests

    Our testing aims to satisfy the requirements of the project writeup. Each file incorporates at least one aspect of each of the different requirements:

    For trailing punctuation: We use "fix_word(word)" to preprocess each word before checking its spelling. This step modifies the word to remove leading and trailing punctuation mark. The "fix_word()" function iteratively removes these characters from the start and end of the word.

    For Hyphens: The function detects hyphenated words by searching for a hyphen character within each word using "char *hyphen = strchr(word, '-')". If a hyphen is found, the word is considered hyphenated. We then split the hyphentated word into its respective parts using "strtok_r(word, "-")" and checks each part against the dictionary using the binary search function. The word is considered correctly spelled only if all parts (hyphen separated) are found in the dictionary. 

    For Capitalization: Took the different capitalization variants and put them in the array so we had all 3 variations of each word.
