#include "cpu.h"
#define HAS_ARGUMENT_ASSERT if (isnan(value)) DIE(NOT_ENOUGH_ARGUMENTS);


void InitCPU(*CPU thou){

    Stack* stack_ptr = (Stack*)calloc(1,sizeof(Stack*));
    ERROR StackERROR = newStack(10, &stk);
    if (StackERROR != OK){
    printf("Failed to create stack");
    abort();
    }
    thou -> stk = stk;
}

const char* getERRORName(CPU_ERROR error){

    switch(error){
    case_of_switch(OK)
    case_of_switch(STACK_EMPTY_NOTHING_TO_POP)
    case_of_switch(STACK_OVERFLOW)
    }
}

void DIE(CPU_ERROR error){
    printf("An error occured: "getERRORName(error) "\n");
    abort();
}

void ReadCommand(char* rip, int* command_ptr, StackElement* value){

    *value = POISON;
    int nChars = 0;
    sscanf(rip, " %d%n", command_ptr, &nChars);
    sscanf(rip + nChars, " " ELEMENT_FORMAT "\n", value);
}

void Execute(CPU* thou, char** lines, int number_of_lines){

    char* rip = lines[0];
    int command = -1;
    StackElement value = POISON;

    for (int i = 0; i < number_of_lines; rip = rip[++i]){

        value = POISON;
        ReadCommand(rip, &command, &value);
        RunCommand(thou, command, value);

    }

}


void RunCommand(CPU* thou, int command, StackElement value){

    switch(command){

        case PUSH: HAS_ARGUMENT_ASSERT
                   Push(thou, value);
                   break;

        case POP:  Pop(thou);
                   break;

        case ADD:  Add(thou);
                   break;

        case SUB:  Pop(thou);
                   break;

        case MUL:  Mul(thou);
                   break;

        case DIV:  Div(thou);
                   break;

        case SIN:  Sin(thou);
                   break;

        case COS:  Cos(thou);
                   break;

        case SQRT: Sqrt(thou);
                   break;

        case NEG:  Neg(thou);
                   break;

        case OUT:  Out(thou);
                   break;

        case IN:   In(thou);
                   break;

        default:
    }

}

