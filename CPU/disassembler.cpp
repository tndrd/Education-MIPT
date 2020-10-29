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
    
    long int filesize = 0;
    char* buffer = ReadFile(filename, &filesize);
    int rip = 0;
    assert(buffer);

    #include "binary_work_initialization.h"
    FILE* fp = fopen(out, "w");
    assert(fp);
    
    #define CASE_CBITS(cbits, name)\
        case cbits: CBITS_ ## cbits ## _ ## name\
                    break;

    #define DEF_CMD(name, num, max_arg, min_arg, arg_check, nkw_pi)                \
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
        for (int i = 0; i < nlabels; i++){
            if ((rip - nlabels*sizeof(int) - HEADER_LENGTH - 1) == *((int*)(buffer + HEADER_LENGTH + 1 + i*sizeof(int)))){
                fprintf(fp, "LABEL_%d:\n", i);
            }
        }
        switch(buffer[rip] & 0x1f){
        #include "DISASSEMBLER_COMMAND_PREFERENCES.h"
        #include "commands.h"
        default: printf("\n Error: code corrupted, unknown command code: %X\n", buffer[rip] & 0xff);
                return 1;
        }  
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
