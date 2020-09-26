#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>

struct MyStr{

    char* pointer;
    int length;

};

typedef int(*Comparator)(MyStr a, MyStr b);

//------------------------------------------------------------------------------------
char* readFile(const char* name);
void addChar(char** line_ptr, int* length_ptr, char to_add);
MyStr* getLines(char* buffer, int* number_of_lines);
char* next_letter(char* a);
int lexicographicalCompare(char* a, char* b);
MyStr reversed(MyStr to_reverse);
int reversedLGComparator(MyStr a, MyStr b);
int LGComparator(MyStr a, MyStr b);
void arrayPrint(MyStr* arr, int length);
void printFile(const char* name, MyStr* lines, int length);
void* concat(char** a, char** b, char** c, size_t a_size, size_t b_size, size_t c_size);
void myQSort(void* lines_ptr, int length, int(*compare)(MyStr a, MyStr b));
int TEST_myQsort();
int runTests();
//------------------------------------------------------------------------------------



