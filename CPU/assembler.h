#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "../Onegin/foperations.cpp"
#include "config.h"
#include "keywords.cpp"

void WriteCode(const char* name, char* begin, char* end, int* labels, int nlabels);
int Assemble(MyStr* lines, char** beginptr, char** endptr, int writeAssemblyList, const char* assemblyList_name, char** label_names, int* label_arr, int* nlabels_ptr, int number_of_lines);
void AddLabel(char* command, char* label_name, int* nlabels_ptr, char** label_names, int* label_arr, int* ADD_LABEL_ptr, int* status_ptr, char* rip, char* codeptr, int nline);
int CheckLabel(char* command);

struct Label{
    char* name = nullptr;
    int value = 0;
};