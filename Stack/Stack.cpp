#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>

#define case_of_switch(en) case en: return #en;
#define POISON NAN
#define ELEMENT_FORMAT "%f"
#define NAME(obj) #obj
#define CANNARY 0xBAAD

typedef double StackElement;
const int AVAILABLE_MEMORY = 4096;

enum ERROR{

    OK,
    CREATION_FAILED_NOT_ENOUGH_MEMORY,
    STACK_PTR_UNAVAILABLE,
    DATA_PTR_UNAVAILABLE,
    STACK_PTR_EQUALS_DATA_PTR,
    CAPACITY_LESS_ONE,
    CAPACITY_LESS_MINIMAL,
    SIZE_LESS_ZERO,
    SIZE_GREATER_THAN_CAPACITY,
    DATA_CORRUPTED_CONTAINS_POISON,
    DATA_CORUPTED_UNEXPECTED_NON_POISON_VALUE_ABOVE_TOP,
    DATA_CORRUPTED_CANNARY_IS_DEAD,
    DATA_CORRUPTED_WRONG_HASH,
    DATA_CORRUPTED_WRONG_TRANSACTION
};

const char* getERRORName(ERROR error){

    switch(error){
    case_of_switch(OK)
    case_of_switch(CREATION_FAILED_NOT_ENOUGH_MEMORY)
    case_of_switch(STACK_PTR_UNAVAILABLE)
    case_of_switch(DATA_PTR_UNAVAILABLE)
    case_of_switch(STACK_PTR_EQUALS_DATA_PTR)
    case_of_switch(CAPACITY_LESS_ONE)
    case_of_switch(CAPACITY_LESS_MINIMAL)
    case_of_switch(SIZE_LESS_ZERO)
    case_of_switch(SIZE_GREATER_THAN_CAPACITY)
    case_of_switch(DATA_CORRUPTED_CONTAINS_POISON)
    case_of_switch(DATA_CORUPTED_UNEXPECTED_NON_POISON_VALUE_ABOVE_TOP)
    case_of_switch(DATA_CORRUPTED_CANNARY_IS_DEAD)
    case_of_switch(DATA_CORRUPTED_WRONG_HASH)
    case_of_switch(DATA_CORRUPTED_WRONG_TRANSACTION)
    }


}


struct Stack{

    int CANNARY1 = 0;
    int minimal_capacity = 0;
    int capacity = 0;
    int stack_size = 0;
    StackElement* data = nullptr;
    int Hash = 0;
    int CANNARY2 = 0;

};

void ASSERTED(Stack* thou);
void StackDump(Stack* thou, ERROR status);
int CheckPointer(void* ptr);
int StackPop(Stack* thou, StackElement* popped);
int StackPush(Stack* thou, StackElement value);
int ResizeDown(Stack* thou);
int ResizeUp(Stack* thou);
void LAST_WORDS(Stack* thou, ERROR error);
void INSTANT_DEATH(Stack* thou, ERROR error);
ERROR StackOK(Stack* thou);
void StackCtor(Stack* thou, int capacity);
int isPOISON(StackElement value);


Stack* newStack(int capacity){

    Stack* thou = (Stack*)calloc(1,sizeof(Stack));
    StackCtor(thou, capacity);
    return thou;
}

void StackCtor(Stack* thou, int capacity){

    thou -> CANNARY1 = 0;
    thou -> minimal_capacity = capacity;
    thou -> capacity = capacity;
    thou -> stack_size = 0;
    thou -> data = (StackElement*)calloc(capacity, sizeof(StackElement));
    for (int i = 0; i < capacity; (thou -> data)[i++] = POISON);
    //thou -> Hash = getHash();
    thou -> Hash = 0;
    thou -> CANNARY2 = 0;
    ASSERTED(thou);
}

int isPOISON(StackElement value){
    return isnan(value);
}


ERROR StackOK(Stack* thou){

    if (!CheckPointer(thou)) return                          STACK_PTR_UNAVAILABLE;
    if (!CheckPointer(thou -> data)) return                  DATA_PTR_UNAVAILABLE;
    if (((void*)thou) == ((void*)thou -> data)) return       STACK_PTR_EQUALS_DATA_PTR;
    if (thou -> capacity < 1) return                         CAPACITY_LESS_ONE;
    if (thou -> capacity < thou -> minimal_capacity) return  CAPACITY_LESS_MINIMAL;
    if (thou -> stack_size < 0) return                       SIZE_LESS_ZERO;
    if (thou -> stack_size > thou -> capacity) return        SIZE_GREATER_THAN_CAPACITY;

    //if (!CannaryAlive()) return DATA_CORRUPTED_CANNARY_IS_DEAD;
    //if (!HashOK()) return DATA_CORRUPTED_WRONG_HASH;
    return OK;

}

void INSTANT_DEATH(Stack* thou, ERROR error){
    printf("Critical Error in stack[%p]: %s", thou, getERRORName(error));
    abort();
}

void LAST_WORDS(Stack* thou, ERROR error){
    printf("An error occured (Stack dumped)\n");
    StackDump(thou, error);
    abort();
}

int ResizeUp(Stack* thou){

    ASSERTED(thou);

    StackElement* resized = (StackElement*)realloc(thou -> data, int((thou -> capacity) * sizeof(StackElement) * 2));
    if (resized != nullptr){
        thou -> capacity = thou -> capacity * 2;
        thou -> data = resized;
        for (int i = (thou -> stack_size); i < (thou -> capacity); (thou -> data)[i++] = POISON);
        return 1;
    }
    ASSERTED(thou);
    return 0;
}

int ResizeDown(Stack* thou){

    ASSERTED(thou);

    StackElement* resized = (StackElement*)realloc(thou -> data, int(sizeof(StackElement) * (thou -> capacity) / 2));

    if (resized != nullptr){
        thou -> capacity = int(thou -> capacity / 2);
        thou -> data = resized;
        return 1;
    }
    ASSERTED(thou);
    return 0;
}

int StackPush(Stack* thou, StackElement value){

    ASSERTED(thou);
    if (thou -> stack_size == thou -> capacity){
        if(!ResizeUp(thou)) return 0;
    }
    if (!isPOISON((thou -> data)[thou -> stack_size])) LAST_WORDS(thou, DATA_CORUPTED_UNEXPECTED_NON_POISON_VALUE_ABOVE_TOP);
    thou -> data [thou -> stack_size] = value;
    thou -> stack_size++;
    ASSERTED(thou);
    return 1;
}

int StackPop(Stack* thou, StackElement* popped){

    ASSERTED(thou);
    if (thou -> stack_size == 0) return 0;
    if (thou -> stack_size < int((thou -> capacity / 2) + 2) && thou -> capacity != thou -> minimal_capacity) ResizeDown(thou);
    StackElement last = thou -> data [thou -> stack_size - 1];
    if (isPOISON(last)) LAST_WORDS(thou, DATA_CORRUPTED_CONTAINS_POISON);
    thou -> data [thou -> stack_size - 1] = POISON;
    (thou -> stack_size)--;
    *popped = last;
    ASSERTED(thou);
    return 1;
}

int CheckPointer(void* ptr){
    if (ptr == nullptr) return 0;
    return 1;
}

void StackDump(Stack* thou, ERROR status){

    printf("Stack(%s)[%p]:\n    CANNARY1: |%d|,\n    capacity: |%d|,\n     stack_size: |%d|,\n    Hash: |%d|,\n    CANNARY2: |%d|,\n    data[%p]: {\n",
    getERRORName(status), thou, thou -> CANNARY1, thou -> capacity, thou -> stack_size, thou -> Hash, thou -> CANNARY2, thou -> data);
    if (CheckPointer(thou -> data)){
        for(int i = 0; i < (thou -> capacity); printf("      " ELEMENT_FORMAT ",\n", (thou -> data)[i++]));
    }
    printf("    }\n");
}

void ASSERTED(Stack* thou){

    ERROR status = StackOK(thou);
    switch(status){

    case OK: return;

    case STACK_PTR_UNAVAILABLE: INSTANT_DEATH(thou, status);
    case STACK_PTR_EQUALS_DATA_PTR: INSTANT_DEATH(thou, status);

    default: StackDump(thou, status);
             abort();
    }

}

int Stack_CannaryAlive(Stack* thou);

int main(){

    Stack* firstStack = newStack(3);
    StackDump(firstStack, StackOK(firstStack));
    int command;
    double value;
    while (true){
        scanf("%d %lf", &command, &value);
        if (command == 1){
        StackPush(firstStack, value);
        }
        else StackPop(firstStack, &value);
        StackDump(firstStack, StackOK(firstStack));
    }
    return 0;
}
