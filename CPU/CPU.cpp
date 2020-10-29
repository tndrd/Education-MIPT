#include "CPU.h"

//#define CPU_TRACE CPU_DUMP(thou, stdout);\
// EXECUTE returns error code

#ifndef CPU_TRACE
#define CPU_TRACE ;
#endif

double GET_REGISTER_VALUE(CPU* thou, char code){
    #define KEYWORD(name, keyword_code, ain, din)\
        case keyword_code: return thou -> name;\
                           break;
    switch(code){ 
    #include "registers.h"
    #undef KEYWORD
    default: printf("Code corrupted on rip %d: attempt to read from unknown register with code %X\n", rip, code & 0xff);
             exit(7);
    }

}

double* GET_REGISTER_ADRESS(CPU* thou, char code){
    #define KEYWORD(name, keyword_code, ain, din)\
        case keyword_code: return &(thou -> name);\
                           break;
    switch(code){ 
    #include "registers.h"
    #undef KEYWORD
    default: printf("Code corrupted on rip %d: attempt to get unknown register's adress with code %X\n", rip, code & 0xff);
             exit(8);
    }

}

void CPU_DUMP(CPU* thou, FILE* stream){
    fprintf(stream, "CPU[%p]:\ncode[%p]\nRAM[%p]\nfilesize: %d\n", thou, thou -> code, thou -> RAM, thou -> filesize);
    fprintf(stream, "rax: %lf\nrbx: %lf\nrcx: %lf\nrdx: %lf\nstatus: %d\nRIP: %d\n", thou -> rax, thou -> rbx, thou -> rcx, thou -> rdx, thou -> _rsx, rip );
    StackPrint(thou -> stack_ptr, OK, stream);
    fprintf(stream, "\n-----------------\n");
}


int Execute(CPU* thou){
    
    long int filesize = thou -> filesize;
    char* buffer = thou -> code;
    assert(buffer);
    thou -> _rsx = 1;
    int prev_rip = 0;

    #include "binary_work_initialization.h"
    printf("----------------------------\n");

    FILE* fp = fopen("CPU.log", "w");
    assert(fp);

    #define CASE_CBITS(cbits, name)\
        case cbits: {\
                    CBITS_ ## cbits ## _ ## name\
                    break;\
                    }
    #define DEF_CMD(name, num, max_arg, min_arg, arg_check, nkw_ain)                        \
        case (num):                                                                      \
            switch((buffer[rip] & 0xE0) >> 5){                                            \
                CASE_CBITS(0, name)\
                CASE_CBITS(1, name)\
                CASE_CBITS(2, name)\
                CASE_CBITS(3, name)\
                CASE_CBITS(4, name)\
                CASE_CBITS(5, name)\
                CASE_CBITS(6, name)\
                CASE_CBITS(7, name)\
            }\
            break;

    while(rip < filesize){
        switch(buffer[rip] & 0x1f){
        #include "CPU_COMMAND_PREFERENCES.h"
        #include "commands.h"
        default: printf("\n Error: code corrupted, unknown command code: %X\n", buffer[rip] & 0xff);
                 return 0;
        }  
    }
    
    fclose(fp);
    #undef DEF_CMD
    #undef CASE
    return 0;                                                                       


}

int Load(CPU* thou, const char* name){

    char* code = ReadFile(name, &(thou->filesize));
    if (code){
        thou -> code = code;
        return 1;
    }
    else{
        printf("Failed to load code");
        return 0;
    }
}

void Init(CPU* thou){
    
    Stack* stack_ptr = (Stack*)calloc(1, sizeof(Stack));
    if(newStack(10, &stack_ptr) != OK){
        printf("Initialisation failed: cannot create stack\n");
        exit(1);
    }
    thou -> stack_ptr = stack_ptr;

    Stack* callstack = (Stack*)calloc(1, sizeof(Stack));
    if(newStack(10, &callstack) != OK){
        printf("Initialisation failed: cannot create callstack\n");
        exit(1);
    }

    thou -> callstack = callstack;

    char* RAM = (char*)calloc(RAM_SIZE,1);
    if (!RAM){
        printf("Initialisation failed: cannot create RAM\n");
        exit(1);
    }
    thou -> RAM = RAM;
}

int main(int argc, char* argv[]){
    
    switch(argc){
        case 2:  break;
        default: printf("Wrong arguments\n");
                 exit(-1);
    }
    
    CPU* CPU_ptr = (CPU*)calloc(1,sizeof(CPU));
    Init(CPU_ptr);
    Load(CPU_ptr, argv[1]);
    Execute(CPU_ptr);
    return 0;

}