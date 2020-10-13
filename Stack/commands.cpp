#include "cpu.h"


void Push(CPU* thou, StackElement value){
    if(!StackPush(thou -> stack_ptr, value))
        status = STACK_OVERFLOW;
}


StackElement Pop(CPU* thou){
    StackElement popped = POISON;
    if (!StackPop(thou -> stack_ptr, &popped))
        status = STACK_EMPTY_NOTHING_TO_POP;
    return popped;
}


void Add(CPU* thou){
    StackElement a = Pop(thou);
    StackElement b = Pop(thou);
    CPU_OK Push(thou, a + b);
}


void Sub(CPU* thou){
    StackElement a = Pop(thou);
    StackElement b = Pop(thou);
    CPU_OK Push(thou, a - b);
}


void Mul(CPU* thou){
    StackElement a = Pop(thou);
    StackElement b = Pop(thou);
    CPU_OK Push(thou, a * b);
}


void Div(CPU* thou){
    StackElement a = Pop(thou);
    StackElement b = Pop(thou);
    CPU_OK Push(thou, a / b);
}


void Sin(CPU* thou){
    StackElement a = Pop(thou);
    CPU_OK Push(thou, sin(a));
}


void Cos(CPU* thou){
    StackElement a = Pop(thou);
    CPU_OK Push(thou, cos(a));
}


void Sqrt(CPU* thou){
    StackElement a = Pop(thou);
    CPU_OK Push(thou, sqrt(a));
}


void Neg(CPU* thou){
    StackElement a = Pop(thou);
    CPU_OK Push(thou, -a);
}


void Out(CPU* thou){
    StackElement a = Pop(thou);
    CPU_OK printf(ELEMENT_FORMAT "\n", a);
}


void In(CPU* thou){
    StackElement value = POISON;

    switch(scanf(ELEMENT_FORMAT, &value)){

        case 0:  status = EMPTY_INPUT;
                 break;

        case 1:  break;

        default: warning = UNEXPECTED_SCANNED_VALUES;
                 break;

    }
    CPU_OK Push(thou, value);
}


void Dump(CPU* thou){ //TODO add stream choose
    StackPrint(thou -> stack_ptr, StackOK(thou -> stack_ptr), stdout);
}


void Hlt(CPU* thou){
    StackDtor(thou -> stack_ptr);
    abort();
}
