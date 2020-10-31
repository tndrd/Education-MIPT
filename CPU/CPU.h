#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>
#include "../Stack/Stack.cpp"
#include "config.h"
#include "foperations.h"

struct CPU{

    Stack* stack_ptr = nullptr;
    Stack* callstack = nullptr;
    char* code = nullptr;
    double* RAM = nullptr;
    long int filesize = 0;
    int rip = 0;
    double rax = NAN;
    double rbx = NAN;
    double rcx = NAN;
    double rdx = NAN;
    int _rsx = 1;
};

#define RIP (thou -> rip)

#define CPU_RAM (thou -> RAM)

#define RAX (thou -> rax)

#define RBX (thou -> rbx)

#define RCX (thou -> rcx)

#define RDX (thou -> rdx)

#define CPU_Stack_ptr (thou -> stack_ptr)

#define CPU_Status (thou -> _rsx)

#define FILESIZE thou->filesize
    
#define buffer thou->code

#define CALLSTACK thou -> callstack