#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>

struct my_str{

    char* pointer;
    int length;

};

//------------------------------------------------------------------------------------

typedef int(*comparator)(my_str a, my_str b);

char* readFile(const char* name);
void addChar(char** line_ptr, int* length_ptr, char to_add);
my_str* getLines(char* buffer, int* number_of_lines);
char* next_letter(char* a);
int lexicographicalCompare(char* a, char* b);
my_str reversed(my_str to_reverse);
int reversedLGComparator(my_str a, my_str b);
int LGComparator(my_str a, my_str b);
void arrayPrint(my_str* arr, int length);
void printFile(const char* name, my_str* lines, int length);
void* concat(char** a, char** b, char** c, size_t a_size, size_t b_size, size_t c_size);
void myQSort(void* lines_ptr, int length, int(*compare)(my_str a, my_str b));
int TEST_myQsort();
void runTests();
//------------------------------------------------------------------------------------



