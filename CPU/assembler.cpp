#include "assembler.h"

// BUG. Try assembling 1-line file. BUG IN GetLines

enum COMMANDS{
    #define DEF_CMD(name, num, max_arg, min_arg, code, arg_check) CMD_ ## name = num,
    #include "commands.h"
    #undef DEF_CMD
};

double REGISTER_CODE(char* rname){
    if (!strcmp(rname, "rax")) return RAX_CODE; 
    if (!strcmp(rname, "rbx")) return RBX_CODE;
    if (!strcmp(rname, "rdx")) return RDX_CODE;
    if (!strcmp(rname, "rcx")) return RCX_CODE;
    else return 0;
}

int Assemble(char* buffer, char** beginptr, char** endptr){
    
    #define DEF_CMD(name, num, max_arg, min_arg, code, arg_check)                                 \
        else if (!strcmp(command, #name)){                                                        \
            printf("\n%s", #name);\
            *rip = num;\
            rip++;                                                                                \
            for (int narg = 0; narg < max_arg; narg++){                                           \
                sscanf(lines[nline].pointer + line_offset, " %s%n", arg_value, &relative_offset); \
                line_offset+=relative_offset;                                                     \
                if (!strcmp(arg_value, "")){                                                      \
                    if (narg < min_arg) {                                                         \
                    printf("Line %d: not enough arguments for command %s (given %d, expected %d at least)\n", nline+1, #name, narg, min_arg);\
                    status = 0;                                                                   \
                    }                                                                             \
                    else{                                                                         \
                    *(rip - narg*sizeof(double) - 1) = num + 128;\
                    break;\
                    }\
                }                                                                                 \
                arg_check                                                                         \
                if (ISREGISTER(arg_value) == 0){                                                  \
                    *((double*)rip) = REGISTER_CODE(arg_value);                                   \
                    arg_value+=3;                                                                 \
                }                                                                                 \
                else{                                                                             \
                    *((double*)rip) = strtod(arg_value, &arg_value);                              \
                }                                                                                 \
                printf(" %lf", *((double*)rip));\
                rip+=sizeof(double);                                                              \
            }                                                                                     \
        }
    
    int status = 1;
    char* codeptr = (char*)calloc(1,MINIMAL_FILESIZE);
    char* rip = codeptr;
    int number_of_lines = 0;
    MyStr* lines =  GetLines(buffer, &number_of_lines);
    
    char* command = (char*)calloc(1, COMMAND_LENGTH);
    int line_offset = 0;
    int relative_offset = 0;
    char* arg_value = (char*)calloc(1, COMMAND_LENGTH);

    for(int nline = 0; nline < number_of_lines; nline++){
        sscanf(lines[nline].pointer," %s%n", command, &line_offset);
        if (0) exit(228);
        #include "commands.h"    
        else printf("Unknown Command on line %d: %s\n", nline + 1, command);
        line_offset = 0;
        relative_offset = 0;
    }
    #undef DEF_CMD
    if (!status) return 0;
    *beginptr = codeptr;
    *endptr = rip;
    //free(lines);
    //free(command);
    //free(arg_value);
    return 1;
}

void Write(const char* name, char* begin, char* end){
    FILE* fp = fopen(name, "w");
    fprintf(fp, "%c", 'M');
    fprintf(fp, "%c", 'Y');
    fprintf(fp, "%c", 'A');
    fprintf(fp, "%c", 'S');
    fprintf(fp, "%c", 'S');
    fprintf(fp, "%c", VERSION_BASE);
    fprintf(fp, "%c", VERSION_SUB);
    for (int i = 0; i < end - begin; i++) fputc(begin[i], fp);
    fclose(fp);


}

int main(){
    char* begin = nullptr;
    char* end = nullptr;
    char* buffer = ReadFile("test.myasm");
    Assemble(buffer, &begin, &end);
    //for(int i = 0; i < end - begin; i++) printf("%X|",begin[i] & 0xff);
    Write("test.run", begin, end);
    


}