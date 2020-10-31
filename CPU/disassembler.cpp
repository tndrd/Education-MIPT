#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "config.h"
#include "keywords.cpp"
#include "foperations.h"


int Disassemble(char* filename, char* out){
    
    long int FILESIZE = 0;
    char* buffer = ReadFile(filename, &FILESIZE);
    int RIP = 0;
    assert(buffer);
    double CONST = 0;
    int offset = 1;

    #include "binary_work_initialization.h"
    
    FILE* fp = fopen(out, "w");
    assert(fp);
    
    #define RAM_BIT (buffer[RIP] & 0x80)
    #define REGISTER_BIT (buffer[RIP] & 0x40)
    #define CONST_BIT (buffer[RIP] & 0x20)

    #define DEF_CMD(name, num, max_arg, min_arg, arg_check, nkw_pi, DISASSEMBLE_INSTRUCTION)                \
            case (num):\
                fprintf(fp, "%s ", #name);\
                DISASSEMBLE_INSTRUCTION\
                fprintf(fp, "\n");\
                RIP += offset;\
                break;\

    while(RIP < FILESIZE){
        for (int i = 0; i < nlabels; i++){
            if ((RIP - nlabels*sizeof(int) - HEADER_LENGTH - 1) == *((int*)(buffer + HEADER_LENGTH + 1 + i*sizeof(int)))){
                fprintf(fp, "\nLABEL_%d:\n", i);
            }
        }

        switch(buffer[RIP] & 0x1f){
        #include "commands.h"
        default: printf("\n Error: code corrupted, unknown command code at RIP %d: %X\n", RIP,  buffer[RIP] & 0xff);
                return 1;
        }
        offset = 1;  
    }
    #undef DEF_CMD
    #undef CASE
    assert(fp);
    fclose(fp);
    return 0;
}


int main(int argc, char* argv[]){
    
    switch(argc){

        case 3:  Disassemble(argv[1], argv[2]);
                 break;

        default: printf("Wrong arguments\n");
                 exit(-1);

    }
    return 0;
}
