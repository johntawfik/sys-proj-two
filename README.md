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

A dictionary file, sorted lexicographically with case sensitivity, serves as the reference for correct spelling. The program uses an efficient data structure (e.g., binary search trees, hash tables, or tries) for storing the dictionary and facilitating rapid lookups.

Dictionary Structure: The dictionary is loaded into a suitable data structure that supports fast searches, considering ASCII case sensitivity. This setup is crucial for handling large dictionaries efficiently.

#### Text File Processing

The program reads text files, identifying words while managing punctuation, capitalization, and hyphenation. It keeps track of line and column numbers for accurate error reporting.

Text Parsing: Special rules for trailing punctuation, quotation marks, brackets, and hyphenation are implemented to accurately identify words. This parsing is essential for precise spelling checks.


#### Directory Traversal

spchk can recursively traverse directories, checking all .txt files while ignoring files and directories starting with .. This feature requires careful management of file paths and directory structures.

Recursive Search: The program employs a recursive strategy to navigate through directories, applying the spelling checker to each applicable text file. This comprehensive approach ensures no file is overlooked.


### Executables 
    spchk.c - 
    - make 
    - ./spchk 


### Error Reporting
    
    When spelling errors are detected, spchk reports the erroneous word along with its location (file, line, and column). This precise reporting aids in quickly locating and correcting spelling errors.


### Testing plan

 ### Correctness Testing

    Word Detection Accuracy: Tests verify the program's ability to accurately identify words within text files, considering punctuation, capitalization, and hyphenation. By comparing output against known errors, these tests ensure that `spchk` reliably detects spelling mistakes.

    Directory Traversal: This involves testing the program's ability to recursively traverse directories and correctly identify `.txt` files for spelling checks. Tests will include various directory structures to ensure comprehensive file coverage.

    Error Reporting: Tests assess the accuracy and format of error reports, ensuring that each spelling mistake is correctly identified with precise location details. This includes verifying the handling of multiple errors within a single file or across multiple files.



  #### Rationale Behind the Tests

    Comprehensive Coverage: The testing suite is designed to cover all functional aspects of `spchk`, from basic spelling checks to more complex scenarios involving directory traversal and error reporting.

    Efficiency Evaluation: By including performance tests, the suite assesses the program's usability in real-world scenarios, ensuring that it operates efficiently even with large dictionaries and numerous files.

    Error Handling: Ensuring accurate and user-friendly error reporting is crucial for a tool designed to aid in text correction. Tests focus on the clarity and precision of error messages.
