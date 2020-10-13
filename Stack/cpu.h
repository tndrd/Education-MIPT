#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "Stack.cpp"
#include "../Onegin/foperations.cpp"

#define CPU_OK if(status == OK)
#define CPU_NOK if(status != OK)

#define ASSERT CPU_NOK DIE(status);


enum CPU_ERROR{

    OK,
    STACK_EMPTY_NOTHING_TO_POP,
    STACK_OVERFLOW,
    EMPTY_INPUT,
    NOT_ENOUGH_ARGUMENTS
};


enum CPU_WARNING{

    NO_WARNINGS,
    UNEXPECTED_ARGUMENTS,
    UNEXPECTED_SCANNED_VALUES
};

enum COMMANDS{

    PUSH, //0
    POP,  //1
    ADD,  //2
    SUB,  //3
    MUL,  //4
    DIV,  //5
    SIN,  //6
    COS,  //7
    SQRT, //8
    NEG,  //9
    OUT,  //10
    IN,   //11
    DUMP, //12
    HLT   //13
};


struct CPU{

    Stack* stack_ptr = nullptr

    char* code = nullptr
    size_t length = 0;

    StackElement rax = NAN;
    StackElement rcx = NAN;
    StackElement rbx = NAN;
    StackElement rdx = NAN;
}

CPU_ERROR status = OK;
CPU_WARNING warning = NO_WARNINGS;
