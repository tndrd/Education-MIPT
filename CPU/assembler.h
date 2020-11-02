#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "../Onegin/foperations.cpp"
#include "config.h"

#define RAM_BIT (*(command_start_ptr) & 0x80)
#define REGISTER_BIT (*(command_start_ptr) & 0x40)
#define CONST_BIT (*(command_start_ptr) & 0x20)

struct Label{
    char* name = nullptr;
    int value = 0;
};

void WriteCode(const char* name, char* begin, char* end, Label* labels, int nlabels);
int Assemble(MyStr* lines, char* begin, char** endptr, int writeAssemblyList, int writeLabels, const char* assemblyList_name, Label* labels, int* nlabels_ptr, int number_of_lines);
void AddLabel(char* command, char* label_name, int* nlabels_ptr, char** label_names, int* label_arr, int* ADD_LABEL_ptr, int* status_ptr, char* rip, char* codeptr, int nline);
int CheckLabel(char* command);
int CompareWithLabel(char* str, char* label_name);
FILE* StartListing(const char* assemblyList_name);
void WriteListingLine(FILE* Listing_ptr, char* rip, char* command_start_ptr, char* command);
int ASSEMBLE_KEYWORD(char** arg_value_ptr, char* command_start_ptr, char** rip_ptr, int narg, int* status_ptr, int nline);