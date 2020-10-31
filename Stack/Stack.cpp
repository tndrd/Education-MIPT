#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#define case_of_switch(en) case en: return #en;
#define POISON NAN

#define ELEMENT_FORMAT "%lf"

#define STRUCT_CANNARY_VALUE 0xFFFFFFFFFFFFFFFF
#define STRUCT_CANNARY_FORMAT "%08X"
#define DATA_CANNARY_VALUE 2.0
#define DATA_CANNARY_FORMAT "%lf"
#define MAXSIZE 1000

typedef double StackElement;
//const int AVAILABLE_MEMORY = 4096;

enum ERROR{

    OK,//numbers
    CREATION_FAILED_NOT_ENOUGH_MEMORY,
    CREATION_FAILED_ALREADY_EXISTS,
    STACK_ALREADY_DEAD,
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
    STRUCT_CORRUPTED_CANNARY_IS_DEAD,
    DATA_CORRUPTED_WRONG_HASH,
    STRUCT_CORRUPTED_WRONG_HASH,
    PTR_UNAVAILABLE
};

struct Stack{

    long int CANNARY1 = 0;
    int minimal_capacity = 0;
    int capacity = 0;
    int stack_size = 0;
    StackElement* data = nullptr;
    int struct_hash = 0;
    int data_hash = 0;
    long int CANNARY2 = 0;
    int EXISTS = 0;
};


void ASSERTED(Stack* thou);
void StackDump(Stack* thou, ERROR status, FILE* stream);
int CheckPointer(void* ptr);
int StackPop(Stack* thou, StackElement* popped);
int StackPush(Stack* thou, StackElement value);
int ResizeDown(Stack* thou);
int ResizeUp(Stack* thou);
void LAST_WORDS(Stack* thou, ERROR error);
void INSTANT_DEATH(Stack* thou, ERROR error);
ERROR StackOK(Stack* thou);
ERROR StackCtor(Stack* thou, int capacity);
int isPOISON(StackElement value);
int RESIZE_DOWN_BIAS(Stack* thou);
int Stack_CannaryAlive(Stack* thou);
int Data_CannaryAlive(Stack* thou);
StackElement* CannaryRealloc(Stack* thou, int newsize);
StackElement* CannaryCalloc(Stack* thou, int capacity);
ERROR newStack(int capacity, Stack** new_stack_pointer_adress);
void StackPrint(Stack* thou, ERROR status, FILE* stream);
int ComputeHash(char* bufferr, size_t bytes);
void RecomputeHashes(Stack* thou);
int ComputeDataHash(Stack* thou);
int ComputeStructHash(Stack* thou);


//----------------------------------------------------
static const char* getERRORName(ERROR error){

    switch(error){
        case_of_switch(OK)
        case_of_switch(CREATION_FAILED_NOT_ENOUGH_MEMORY)
        case_of_switch(CREATION_FAILED_ALREADY_EXISTS)
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
        case_of_switch(STRUCT_CORRUPTED_CANNARY_IS_DEAD)
        case_of_switch(DATA_CORRUPTED_WRONG_HASH)
        case_of_switch(STRUCT_CORRUPTED_WRONG_HASH)
    }}
//-----------------------------------------------------------


ERROR newStack(int capacity, Stack** new_stack_pointer_adress){

    if (!CheckPointer(new_stack_pointer_adress)) return PTR_UNAVAILABLE;

    Stack* thou = (Stack*)calloc(1, sizeof(Stack));

    if (!CheckPointer(thou)) return CREATION_FAILED_NOT_ENOUGH_MEMORY;

    ERROR status = StackCtor(thou, capacity);
    if (status == OK) *new_stack_pointer_adress = thou;
    return status;
}

ERROR StackCtor(Stack* thou, int capacity){
    if (capacity == 0) capacity = 1;
    if (!CheckPointer(thou)) return STACK_PTR_UNAVAILABLE;
    if (capacity < 1) return CAPACITY_LESS_ONE;
    if (thou -> EXISTS) return CREATION_FAILED_ALREADY_EXISTS;

    thou -> EXISTS  = 1;
    thou -> CANNARY1 = STRUCT_CANNARY_VALUE;
    thou -> minimal_capacity = capacity;
    thou -> capacity = capacity;
    thou -> stack_size = 0;
    if(!CheckPointer(CannaryCalloc(thou, capacity))) return CREATION_FAILED_NOT_ENOUGH_MEMORY;
    thou -> CANNARY2 = STRUCT_CANNARY_VALUE;
    for (int i = 0; i < capacity;  (thou -> data)[i++]  = POISON);
    RecomputeHashes(thou);
    return StackOK(thou);
}

StackElement* CannaryCalloc(Stack* thou, int capacity){

    thou -> data = (StackElement*)calloc(1, (capacity + 2) * sizeof(StackElement));
    if (!CheckPointer(thou -> data)) return nullptr;
    *(thou -> data) = DATA_CANNARY_VALUE;
    (thou -> data)[capacity + 1] = DATA_CANNARY_VALUE;
    thou -> data = (StackElement*)((void*)(thou -> data) + sizeof(StackElement));
    return thou -> data;
}


StackElement* CannaryRealloc(Stack* thou, int newsize){

    StackElement* resized = (StackElement*)realloc((void*)(thou -> data) - sizeof(StackElement), newsize + 2 * sizeof(StackElement));

    if (CheckPointer(resized)) return (StackElement*)((void*)resized + sizeof(StackElement));

    return nullptr;
}


int ResizeUp(Stack* thou){

    ASSERTED(thou);

    StackElement* resized = CannaryRealloc(thou, int((thou -> capacity) * sizeof(StackElement) * 2));

    if (CheckPointer(resized)){
        thou -> capacity = thou -> capacity * 2;
        thou -> data = resized;
        (thou -> data)[thou -> capacity] = DATA_CANNARY_VALUE;

        for (int i = (thou -> stack_size); i < (thou -> capacity); (thou -> data)[i++] = POISON);

        return 1;
    }

    ASSERTED(thou);
    return 0;
}


int ResizeDown(Stack* thou){

    ASSERTED(thou);

    StackElement* resized = CannaryRealloc(thou, (int(sizeof(StackElement) * (thou -> capacity) / 2)));

    if (CheckPointer(resized)){
        thou -> capacity = int(thou -> capacity / 2);
        thou -> data = resized;
        (thou -> data)[thou -> capacity] = DATA_CANNARY_VALUE;
        return 1;
    }

    RecomputeHashes(thou);
    ASSERTED(thou);
    return 0;
}


int RESIZE_DOWN_BIAS(Stack* thou){
    return thou -> minimal_capacity - 1;

}


int StackPush(Stack* thou, StackElement value){

    ASSERTED(thou);

    if (value == POISON) return 0;

    if (thou -> stack_size == thou -> capacity){
        if(!ResizeUp(thou)) return 0;
    }

    if (!isPOISON((thou -> data)[thou -> stack_size])) LAST_WORDS(thou, DATA_CORUPTED_UNEXPECTED_NON_POISON_VALUE_ABOVE_TOP);
    thou -> data [thou -> stack_size] = value;
    thou -> stack_size++;

    RecomputeHashes(thou);
    ASSERTED(thou);
    return 1;
}


StackElement StackPop(Stack* thou, int* status_ptr){

    ASSERTED(thou);
    if (thou -> stack_size == 0){
        *status_ptr = 0;
        return 0;
    }
    if (thou -> stack_size + (RESIZE_DOWN_BIAS(thou)) < int((thou -> capacity / 2) + 1) && thou -> capacity != thou -> minimal_capacity)
        ResizeDown(thou);

    StackElement last = thou -> data [thou -> stack_size - 1];
    //if (isPOISON(last)) LAST_WORDS(thou, DATA_CORRUPTED_CONTAINS_POISON);
    thou -> data [thou -> stack_size - 1] = POISON;
    (thou -> stack_size)--;

    RecomputeHashes(thou);
    ASSERTED(thou);
    return last;
}


void StackDump(Stack* thou, ERROR status){

    FILE* fp = fopen("log.txt", "w");
    StackPrint(thou, status, fp);
    fclose(fp);
}


void StackPrint(Stack* thou, ERROR status, FILE* stream){;

    fprintf(stream, "Stack(%s)[%p]:\n    CANNARY1: |" STRUCT_CANNARY_FORMAT "|,\n    capacity: |%d|,\n    stack_size: |%d|,\n"
    "    stack_hash: |%X|,\n    data_hash: |%X|,\n    CANNARY2: |" STRUCT_CANNARY_FORMAT "|,\n    data[%p]: {\n",
    getERRORName(status), thou, thou -> CANNARY1, thou -> capacity, thou -> stack_size,
    thou -> struct_hash, thou -> data_hash, thou -> CANNARY2, thou -> data);
    if (CheckPointer(thou -> data)){
        fprintf(stream, "      CANNARY1: " DATA_CANNARY_FORMAT ",\n", (thou -> data)[-1]);
        for(int i = 0; i < (thou -> capacity); fprintf(stream, "      " ELEMENT_FORMAT ",\n", (thou -> data)[i++]));
        fprintf(stream, "      CANNARY2: " DATA_CANNARY_FORMAT ",\n", (thou -> data)[thou -> capacity]);
    }
    fprintf(stream, "    }\n");
}


void ASSERTED(Stack* thou){

    ERROR status = StackOK(thou);
    switch(status){

    case OK: return;
    case STACK_PTR_UNAVAILABLE: INSTANT_DEATH(thou, status);
    default: LAST_WORDS(thou, status);
    }
}

int CheckPointer(void* ptr){
    if (!ptr) return 0;
    return 1;
}

int Stack_CannaryAlive(Stack* thou){
    if (thou -> CANNARY1 == STRUCT_CANNARY_VALUE && thou -> CANNARY2 == STRUCT_CANNARY_VALUE) return 1;
    return 0;
}


int Data_CannaryAlive(Stack* thou){
    if ((thou -> data)[-1] == DATA_CANNARY_VALUE && (thou -> data)[thou -> capacity] == DATA_CANNARY_VALUE) return 1;
    return 0;
}


ERROR StackOK(Stack* thou){

    if (!CheckPointer(thou))                             return STACK_PTR_UNAVAILABLE;
    if (!CheckPointer(thou -> data))                     return DATA_PTR_UNAVAILABLE;
    if (((void*)thou) == ((void*)thou -> data))          return STACK_PTR_EQUALS_DATA_PTR;
    if (thou -> capacity < 1)                            return CAPACITY_LESS_ONE;
    if (thou -> capacity < thou -> minimal_capacity)     return CAPACITY_LESS_MINIMAL;
    if (thou -> stack_size < 0)                          return SIZE_LESS_ZERO;
    if (thou -> stack_size > thou -> capacity)           return SIZE_GREATER_THAN_CAPACITY;
    if (!Data_CannaryAlive(thou))                        return DATA_CORRUPTED_CANNARY_IS_DEAD;
    if (!Stack_CannaryAlive(thou))                       return STRUCT_CORRUPTED_CANNARY_IS_DEAD;

    int current_data_hash = thou -> data_hash;
    int current_struct_hash = thou -> struct_hash;
    RecomputeHashes(thou);
    if (current_data_hash != thou -> data_hash)      return DATA_CORRUPTED_WRONG_HASH;
    if (current_struct_hash != thou-> struct_hash)  return STRUCT_CORRUPTED_WRONG_HASH;
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


int isPOISON(StackElement value){
    return isnan(value);
}


ERROR StackDtor(Stack* thou){

    ASSERTED(thou);

    if (!thou -> EXISTS) return STACK_ALREADY_DEAD;
    free(thou -> data);
    free(thou);
    return OK;
}


ERROR StackClear(Stack* thou){

    ASSERTED(thou);

    thou -> capacity = thou -> minimal_capacity;
    thou -> stack_size = 0;
    thou -> data = CannaryRealloc(thou, thou -> capacity);

    for (int i = 0; i < thou -> capacity;  (thou -> data)[i++]  = POISON);

    RecomputeHashes(thou);
    ASSERTED(thou);
    return OK;

}

int ComputeHash(char* buffer, size_t bytes){
    int sum = 0xFADF; //SEED
    for (int i = 0; i < bytes; i++) sum = (sum * 5) + (int)buffer[i];
    return sum;
}

void RecomputeHashes(Stack* thou){

    thou -> struct_hash = 0;
    thou -> data_hash = 0;
    thou -> struct_hash = ComputeHash((char*)(thou), sizeof(Stack));
    thou -> data_hash = ComputeHash((char*)(thou -> data), sizeof(StackElement) * (thou -> stack_size + 2));
}

/*
int main(){

    Stack* firstStack = (Stack*)calloc(1,sizeof(Stack*));

    if(newStack(10, &firstStack) != OK) exit(-1);
    StackPrint(firstStack, StackOK(firstStack), stdout);
    int command = 0;
    double value = 1;
    while (true){
        scanf("%d", &command);
        if (command == 1){
            StackPush(firstStack, value);
            value++;
            }
        else StackPop(firstStack, &value);
        StackPrint(firstStack, StackOK(firstStack), stdout);
    }

    return 0;
}
*/