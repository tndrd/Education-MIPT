#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>

//------------------------------------------------------------------------------------
char* readFile(const char* name);
void addChar(char** line_ptr, int* length_ptr, char to_add);
char** getLines(char* buffer, int* number_of_lines);
char* next_letter(char* a);
int lexicographicalCompare(char* a, char* b);
char* reversed(char* str);
int reversedLGComparator(const void* a, const void* b);
int LGComparator(const void* a, const void* b);
void bubbleSort (char** base, size_t num, size_t sz, int(*compare)(char* a, char* b));
void arrayPrint(char** arr, int length);
void printFile(char* name, char** lines, int length);
void* concat(char** a, char** b, char** c, int a_size, int b_size, int c_size);
void myQSort(void* lines_ptr, int length, int(*compare)(const void* a, const void* b));
int TEST_myQsort();
//------------------------------------------------------------------------------------



