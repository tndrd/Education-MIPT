#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>


const double RAX_CODE = 1.123123123;
const double RBX_CODE = 1.234234234;
const double RCX_CODE = 1.345345345;
const double RDX_CODE = 1.456456456;

const int HEADER_LENGTH = 7;



long int GetFileSize(FILE* fp){

    int seekResult = fseek(fp, 0L, SEEK_END);
    assert(seekResult == 0);
    long int filesize = ftell(fp);
    rewind(fp);
    return filesize;
}



char* ReadFile(const char* name, long int* filesize_ptr){

    assert(name);

    FILE* fp = fopen(name, "r");
    char* buffer = nullptr;

    if (fp==nullptr){
        printf("Error: unable to open file\n");
        exit(1);
    }


    long int filesize = GetFileSize(fp);
    *filesize_ptr = filesize;
    buffer = (char*)realloc(buffer, filesize + 1);
    int read = fread(buffer + 1, sizeof(char), filesize, fp);
    buffer = buffer + 1;
    fclose(fp);
    return buffer;
}


int Disassemble(const char* filename){
    
    long int filesize = 0;
    char* buffer = ReadFile(filename, &filesize);
    printf("Signature: ");
    for (int i = 0; i < HEADER_LENGTH - 2; i++){
        printf("%c", buffer[i]);
    }
    printf("\nVersion %d.%d\n", buffer[HEADER_LENGTH-2], buffer[HEADER_LENGTH-1]);
    fflush(stdout);
    FILE* fp = fopen("disassembled.myasm", "w");

    int rip = HEADER_LENGTH;
    
    #define DEF_CMD(name, num, max_arg, min_arg, code, arg_check)                        \
        case (num):                                                                      \
            fprintf(fp, "%s", #name);                                                    \
            rip++;                                                                       \
            for (int i = 0; i < max_arg; i++){                                           \
                if ( *((double*)(buffer + rip)) == RAX_CODE )       fprintf(fp, " rax"); \
                else if ( *((double*) (buffer + rip)) == RBX_CODE ) fprintf(fp, " rbx"); \
                else if ( *((double*) (buffer + rip)) == RCX_CODE ) fprintf(fp, " rcx"); \
                else if ( *((double*) (buffer + rip)) == RDX_CODE ) fprintf(fp, " rdx"); \
                else fprintf(fp, " %lf", *((double*)(buffer + rip)));                    \
                rip += sizeof(double);                                                   \
            }                                                                            \
            fprintf(fp, "\n");                                                           \
            break;                                                                       \
                                                                                         \
        case (num+128):                                                                  \
            fprintf(fp, "%s", #name);                                                    \
            rip++;                                                                       \
            for (int i = 0; i < min_arg; i++){                                           \
                if ( *((double*)(buffer + rip)) == RAX_CODE )       fprintf(fp, " rax"); \
                else if ( *((double*) (buffer + rip)) == RBX_CODE ) fprintf(fp, " rbx"); \
                else if ( *((double*) (buffer + rip)) == RCX_CODE ) fprintf(fp, " rcx"); \
                else if ( *((double*) (buffer + rip)) == RDX_CODE ) fprintf(fp, " rdx"); \
                else fprintf(fp, " %lf", *((double*)(buffer + rip)));                    \
                rip += sizeof(double);                                                   \
            }                                                                            \
            fprintf(fp, "\n");                                                           \
            break;
        
    while(rip < filesize){
        switch(((int)buffer[rip]) & 0xff){
        #include "commands.h"
        default: printf("\n Error: code corrupted");
                 return 0;
        }  
    }
    #undef DEF_CMD
    fclose(fp);
    return 1;
}


int main(){

    Disassemble("test.run");
    
    exit(0);
}
