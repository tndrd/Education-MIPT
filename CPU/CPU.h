#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>
#include "../Stack/Stack.cpp"
#include "config.h"
#include "keywords.cpp"

struct CPU{

    Stack* stack_ptr = nullptr;
    Stack* callstack = nullptr;
    char* code = nullptr;
    char* RAM = nullptr;
    long int filesize = 0;
    int rip = 0;
    double rax = NAN;
    double rbx = NAN;
    double rcx = NAN;
    double rdx = NAN;
    int _rsx = 1;
};

#define rip (thou -> rip) 