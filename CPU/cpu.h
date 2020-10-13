#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "Stack.cpp"
#include "foperations.cpp"

#define CPU_IS_OK if(status == CPU_OK)
#define CPU_ISN_OK if(status != CPU_OK)

#define ASSERT CPU_ISN_OK DIE(status);


enum CPU_ERROR{

    CPU_OK,
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

    Stack* stack_ptr = nullptr;

    char* code = nullptr;
    size_t length = 0;

    StackElement rax = NAN;
    StackElement rcx = NAN;
    StackElement rbx = NAN;
    StackElement rdx = NAN;
};

CPU_ERROR status = CPU_OK;
CPU_WARNING warning = NO_WARNINGS;

void InitCPU(CPU* thou);
const char* getERRORName(CPU_ERROR error);
void DIE(CPU_ERROR error);
void ReadCommand(char* rip, int* command_ptr, StackElement* value);
void Execute(CPU* thou, char** lines, int number_of_lines);
void RunCommand(CPU* thou, int command, StackElement value);

void Push(CPU* thou, StackElement value);
StackElement Pop(CPU* thou);
void Add(CPU* thou);
void Sub(CPU* thou);
void Mul(CPU* thou);
void Div(CPU* thou);
void Sin(CPU* thou);
void Cos(CPU* thou);
void Sqrt(CPU* thou);
void Neg(CPU* thou);
void Out(CPU* thou);
void In(CPU* thou);
void Dump(CPU* thou);
void Hlt(CPU* thou);





