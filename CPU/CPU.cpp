#include "CPU.h"

//#define CPU_TRACE CPU_DUMP(thou, stdout);\

#ifndef CPU_TRACE
#define CPU_TRACE ;
#endif

double* GET_REGISTER_ADRESS(CPU* thou, char code){
    
    #define SSYMBOL(name, ASSEMBLING_INSTRUCTION) ;
    #define DEF_CMD(name, num, max_arg, min_arg, arg_check, ASSEMBLING_INSTRUCTION, DISASSEMBLING_INSTRUCTION) ;
    
    #define REGISTER(name, code, ASSEMBLING_INSTRUCTION)\
        case code: return &(thou -> name);\
                           break;
    switch(code){ 
    #include "commands.h"
    #undef KEYWORD
    #undef DEF_CMD
    #undef SSYMBOL
    default: printf("Code corrupted on RIP %d: attempt to get unknown register's adress with code %X\n", RIP, code & 0xff);
             return nullptr;
    }

}

void CPU_DUMP(CPU* thou, FILE* stream){
    fprintf(stream, "CPU[%p]:\ncode[%p]\nfilesize: %d\n", thou, buffer, FILESIZE);
    fprintf(stream, "rax: %lf\nrbx: %lf\nrcx: %lf\nrdx: %lf\nstatus: %d\nRIP: %d\nRAM[%p]: {\n", RAX, RBX, RCX, RDX, CPU_Status, RIP, CPU_RAM);
    for (int i = 0; i < 10; i++) fprintf(stream, "    %lf\n", CPU_RAM[i]);
    fprintf(stream,"}\nCallStack:\n");
    StackPrint(CALLSTACK, OK, stream);
    StackPrint(CPU_Stack_ptr, OK, stream);
    fprintf(stream, "\n-----------------\n");
}


int Execute(CPU* thou){

    assert(buffer);
    CPU_Status = 1;
    int prev_rip = 0;
    double CONST = 0;
    double NO_REGISTER = 0;
    double* REGISTER_ADDR = &NO_REGISTER;
    int offset = 1;

    #include "binary_work_initialization.h"
    printf("----------------------------\n");

    FILE* fp = fopen("CPU.log", "w");
    assert(fp);

    #define RAM_BIT (buffer[RIP] & 0x80)
    #define REGISTER_BIT (buffer[RIP] & 0x40)
    #define CONST_BIT (buffer[RIP] & 0x20)

    #define SSYMBOL(name, ASSEMBLING_INSTRUCTION) ;
    #define REGISTER(name, code, ASSEMBLING_INSTRUCTION) ;

    #define DEF_CMD(name, num, max_arg, min_arg, arg_check, AIN, DIN)                        \
        case (num):\
            if (REGISTER_BIT){\
                REGISTER_ADDR = GET_REGISTER_ADRESS(thou, buffer[RIP + offset]);\
                assert(REGISTER_ADDR);\
                offset++;\
            }\
            if (CONST_BIT){\
                CONST = *((double*)(buffer + RIP + offset));\
                offset += sizeof(double);\
            }\
            DO_ ## name\
            break;

    while(RIP < FILESIZE){
        switch(buffer[RIP] & 0x1f){
        #include "commands.h"
        default: printf("\n Error: code corrupted, unknown command code: %X\n", buffer[RIP] & 0xff);
                 return 0;
        }
        CPU_TRACE
        CONST = 0;
        REGISTER_ADDR = &NO_REGISTER;
        RIP+=offset;
        offset = 1;  
    }
    
    fclose(fp);
    #undef DEF_CMD
    #undef CASE
    return 1;                                                                       
}

int Load(CPU* thou, const char* name){

    char* code = ReadFile(name, &(FILESIZE));
    if (code){
        buffer = code;
        return 1;
    }
    else{
        printf("Failed to load code");
        return 0;
    }
}

int Init(CPU* thou){
    
    Stack* stack_ptr = (Stack*)calloc(1, sizeof(Stack));
    if(newStack(10, &stack_ptr) != OK){
        printf("Initialisation failed: cannot create stack\n");
        return 1;
    }
    CPU_Stack_ptr = stack_ptr;

    Stack* callstack = (Stack*)calloc(1, sizeof(Stack));
    if(newStack(10, &callstack) != OK){
        printf("Initialisation failed: cannot create callstack\n");
        return 1;
    }
    
    CALLSTACK = callstack;

    double* RAM = (double*)calloc(RAM_SIZE,1);
    if (!RAM){
        printf("Initialisation failed: cannot create RAM\n");
        return 1;
    }
    CPU_RAM = RAM;
}


int main(int argc, char* argv[]){
    
    switch(argc){
        case 2:  break;
        default: printf("Wrong arguments\n");
                 exit(-1);
    }
    CPU* CPU_ptr = (CPU*)calloc(1,sizeof(CPU));
    if(!(Init(CPU_ptr) && Load(CPU_ptr, argv[1]))) exit(1);
    return Execute(CPU_ptr);
}