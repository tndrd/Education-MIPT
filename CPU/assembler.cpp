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

int Assemble(char* buffer, char** beginptr, char** endptr, int writeAssemblyList, const char* assemblyList_name){
    
    assert(buffer);
    assert(beginptr);
    assert(endptr);
    assert(assemblyList_name);

    #define DEF_CMD(name, num, max_arg, min_arg, code, arg_check)                                                                             \
        else if (!strcmp(command, #name)){                                                                                                    \
            *rip = num;                                                                                                                       \
            command_start_ptr = rip;                                                                                                          \
            rip++;                                                                                                                            \
            int narg = 0;\
            for (; narg < max_arg; narg++){                                                                                       \
                sscanf(lines[nline].pointer + line_offset, " %s%n", arg_value, &relative_offset);                                             \
                line_offset += relative_offset;                                                                                                 \
                if (!strcmp(arg_value, "")){                                                                                                  \
                    if (narg < min_arg) {                                                                                                     \
                    printf("Line %d: not enough arguments for command %s (given %d, expected %d at least)\n", nline+1, #name, narg, min_arg); \
                    status = 0;                                                                                                               \
                    }                                                                                                                         \
                    break;                                                                                                                    \
                }                                                                                                                             \
                arg_check                                                                                                                     \
                if (ISREGISTER(arg_value) == 0){                                                                                              \
                    *(rip) = REGISTER_CODE(arg_value);                                                                               \
                    *(command_start_ptr) += (128 >> narg);\
                    rip++;\
                    arg_value+=3;                                                                                                             \
                }                                                                                                                             \
                else{                                                                                                                         \
                    *((double*)rip) = strtod(arg_value, &arg_value);                                                                          \
                    rip+=sizeof(double);                                                                                                      \
                }                                                                                                                             \
            }                                                                                                                                 \
            *(command_start_ptr) += (narg << 4);\
            if (status && writeAssemblyList) {                                                                                                \
                assert(fp);                                                                                                                   \
                fprintf(fp, "%p    |    ", command_start_ptr);                                                                                \
                for (int i = 0; i < rip - command_start_ptr; i++) fprintf(fp, "%X ", command_start_ptr[i] & 0xff);                            \
                fprintf(fp,"   |    %s\n", lines[nline].pointer);                                                                             \
            }                                                                                                                                 \
        }
    
    int status = 1;
    char* codeptr = (char*)calloc(1,MINIMAL_FILESIZE);
    char* rip = codeptr;
    char* command_start_ptr = rip;
    int number_of_lines = 0;
    MyStr* lines =  GetLines(buffer, &number_of_lines);
    
    if (number_of_lines < 1){
        printf("Error: empty file\n");
        return 0;
    }
    
    char* command = (char*)calloc(1, COMMAND_LENGTH);
    int line_offset = 0;
    int relative_offset = 0;
    char* arg_value = (char*)calloc(1, COMMAND_LENGTH);
    FILE* fp = nullptr;

    if (writeAssemblyList){
        fp = fopen(assemblyList_name, "w");
        assert(fp);
        fprintf(fp, "RIP    |   CODE    |   COMMAND\n");
    }

    for(int nline = 0; nline < number_of_lines; nline++){
        sscanf(lines[nline].pointer," %s%n", command, &line_offset);
        if (0) exit(228);
        #include "commands.h"    
        else printf("Unknown Command on line %d: %s\n", nline + 1, command);
        line_offset = 0;
        relative_offset = 0;
    }
    #undef DEF_CMD
    
    if (writeAssemblyList){
        assert(fp);
        fclose(fp);
    } 
    if (!status) return 0;
    *beginptr = codeptr;
    *endptr = rip;
    
    return 1;
}

void WriteCode(const char* name, char* begin, char* end){
    FILE* fp = fopen(name, "w");
    assert(fp);
    assert(begin);
    assert(end);
    
    fprintf(fp,SIGNATURE);
    fprintf(fp, "%c", VERSION_BASE + 48);
    fprintf(fp, "%c", VERSION_SUB + 48);
    for (int i = 0; i < end - begin; i++) fputc(begin[i], fp);
    fclose(fp);
}



int main(){
    char* begin = nullptr;
    char* end = nullptr;
    char* buffer = ReadFile("test.myasm");
    if(!Assemble(buffer, &begin, &end, 1, "test.asslist")){
        printf("Assembly failed\n");
        exit(1);
    }
    //for(int i = 0; i < end - begin; i++) printf("%X|",begin[i] & 0xff);
    WriteCode("test.run", begin, end);
    


}