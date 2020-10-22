#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "config.h"

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
    assert(buffer);

    const int SIGNATURE_LENGTH = strlen(SIGNATURE);
    const int HEADER_LENGTH = SIGNATURE_LENGTH + 2;

    if (filesize - 1 < HEADER_LENGTH){
        printf("Error: file too small to be assembled code");
        return 0;
    }

    for (int i = 0; i < SIGNATURE_LENGTH; i++){
        if (buffer[i] != SIGNATURE[i]){
            printf("Error: unappropiate signature\n");
            return 0;
        }
    } 

    int current_version_base = buffer[HEADER_LENGTH - 2] - 48;
    int current_version_sub = buffer[HEADER_LENGTH - 1] - 48;

    if (current_version_base != VERSION_BASE || current_version_sub != VERSION_SUB){
        printf("Error: code version is %d.%d, should be %d.%d\n", current_version_base, current_version_sub, VERSION_BASE, VERSION_SUB);
        return 0;
    }

    printf("Signature: ");
    for (int i = 0; i < HEADER_LENGTH - 2; i++) printf("%c", buffer[i]);
    printf("\nVersion %d.%d\n", buffer[HEADER_LENGTH-2] - 48, buffer[HEADER_LENGTH-1] - 48);
    fflush(stdout);
    FILE* fp = fopen("disassembled.myasm", "w");
    assert(fp);
    int rip = HEADER_LENGTH;
    
    #define CASE(name, num, max_arg, min_arg, code, arg_check, num_offset, arg_count)    \
        case (num + num_offset):                                                         \
            fprintf(fp, "%s", #name);                                                    \
            rip++;                                                                       \
            if (filesize - rip < arg_count * sizeof(double)){                        \
                printf("Code corrupted");                                                \
                return 0;                                                                \
            }                                                                            \
            for (int i = 0; i < arg_count; i++){                                         \
                if ( *((double*)(buffer + rip)) == RAX_CODE )       fprintf(fp, " rax"); \
                else if ( *((double*) (buffer + rip)) == RBX_CODE ) fprintf(fp, " rbx"); \
                else if ( *((double*) (buffer + rip)) == RCX_CODE ) fprintf(fp, " rcx"); \
                else if ( *((double*) (buffer + rip)) == RDX_CODE ) fprintf(fp, " rdx"); \
                else fprintf(fp, " %lf", *((double*)(buffer + rip)));                    \
                rip += sizeof(double);                                                   \
            }                                                                            \
            fprintf(fp, "\n");                                                           \
            break;                                                                       \
        
    #define DEF_CMD(name, num, max_arg, min_arg, code, arg_check)\
        CASE(name, num, max_arg, min_arg, code, arg_check, 0, max_arg)\
        CASE(name, num, max_arg, min_arg, code, arg_check, 128, min_arg)\

    while(rip < filesize){
        switch(((int)buffer[rip]) & 0xff){
        #include "commands.h"
        default: printf("\n Error: code corrupted, unknown command code: %X\n", buffer[rip] & 0xff);
                 return 0;
        }  
    }
    #undef DEF_CMD
    #undef CASE
    assert(fp);
    fclose(fp);
    return 1;
}


int main(){

    Disassemble("test.run");
    exit(0);
}
