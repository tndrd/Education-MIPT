#include "assembler.h"

// Add toomucharguments check
// Add comments

int CheckLabel(char* command){
    char* label_end = strchr(command, ':');
    if (label_end == 0) return 0;
    *label_end = '\0';
    return 1;
}


int Assemble(MyStr* lines, char** beginptr, char** endptr, int writeAssemblyList, const char* assemblyList_name, char** label_names, int* label_arr, int* nlabels_ptr, int number_of_lines){
    
    assert(lines);
    assert(beginptr);
    assert(endptr);
    assert(assemblyList_name);

    #define RAM_BIT (*(command_start_ptr) & 0x80)
    #define REGISTER_BIT (*(command_start_ptr) & 0x40)
    #define CONST_BIT (*(command_start_ptr) & 0x20)


    #define NECESSARY_PLUS\
        if ((REGISTER_BIT * CONST_BIT) && (narg == (1 + ((*(command_start_ptr) & 0x80) >> 7))) && strcmp(arg_value, "+")){\
            status = 0;\
            printf("Wrong syntax on line %d, expected \"+\"\n", nline + 1);\
        }

    #define DEF_CMD(name, num, max_arg, min_arg, arg_check, NON_KEYWORD_PROCESSING_INSTRUCTION)                                                                             \
        else if (!strcmp(command, #name)){                                                                                                    \
            *rip = num & 0xff;                                                                                                                       \
            command_start_ptr = rip;                                                                                                          \
            rip++;                                                                                                                            \
            int narg = 0;                                                                                                                     \
            int toRam = 0;\
            for (; narg < max_arg; narg++){                                                                                                   \
                sscanf(lines[nline].pointer + line_offset, " %s%n", arg_value, &relative_offset);                                             \
                line_offset += relative_offset;                                                                                               \
                if (!strcmp(arg_value, "")){                                                                                                  \
                    if (narg < min_arg) {                                                                                                     \
                    printf("Line %d: not enough arguments for command \"%s\" (given %d, expected %d at least)\n", nline+1, #name, narg, min_arg); \
                    status = 0;                                                                                                               \
                    }                                                                                                                         \
                    break;                                                                                                                    \
                }                                                                                                                             \
                NECESSARY_PLUS                                                                                                                \
                if (!ASSEMBLE_KEYWORD(&arg_value, command_start_ptr, &rip, narg, &status, nline)){                                                   \
                    NON_KEYWORD_PROCESSING_INSTRUCTION\
                }                                                                                                                             \
            }                                                                                                                                 \
            arg_check\
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
    int argtype = 0;
    
    if (number_of_lines < 1){
        printf("Error: empty file\n");
        return 0;
    }
    
    char* command = (char*)calloc(1, COMMAND_LENGTH);
    int line_offset = 0;
    int relative_offset = 0;
    char* arg_value = (char*)calloc(1, COMMAND_LENGTH);
    FILE* fp = nullptr;
    char* label_name = (char*)calloc(MAX_LABEL_NAME_LENGTH, sizeof(char));
    int label_length = 0;
    int ADD_LABEL = 1;

    if (writeAssemblyList){
        fp = fopen(assemblyList_name, "w");
        assert(fp);
        fprintf(fp, "RIP    |   CODE    |   COMMAND\n");
    }

    for(int nline = 0; nline < number_of_lines; nline++){
        sscanf(lines[nline].pointer," %s%n", command, &line_offset);
        
        if(!line_offset || !strcmp(command, "#")) continue;
        #include "ASSEMBLER_COMMAND_PREFERENCES.h"
        #include "commands.h"    
        else if (CheckLabel(command)){
            sscanf(command, "%s:", label_name);
            for (int nlabel = 0; nlabel < *nlabels_ptr; nlabel++){
                if (!strcmp(label_names[nlabel], label_name)){
                    if (label_arr[nlabel] != rip - codeptr){
                        printf("Error: redefinition of label %s on line %d\n", label_name, nline+1);
                        status = 0;
                    }
                    else ADD_LABEL = 0;
                }
            }
            if (ADD_LABEL){
                label_names[*nlabels_ptr] = (char*)calloc(MAX_LABEL_NAME_LENGTH,1);
                strcpy(label_names[*nlabels_ptr], label_name);
                label_arr[(*(nlabels_ptr))++] = rip - codeptr;
            }
            ADD_LABEL = 1;
        }
        else{
            status = 0;
            printf("Unknown Command on line %d: %s\n", nline + 1, command);
        }
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

void WriteCode(const char* name, char* begin, char* end, int* labels, int nlabels){
    FILE* fp = fopen(name, "w");
    assert(fp);
    assert(begin);
    assert(end);
    
    fprintf(fp,SIGNATURE);
    fprintf(fp, "%c", VERSION_BASE + 48);
    fprintf(fp, "%c", VERSION_SUB + 48);
    fprintf(fp, "%c", nlabels);
    for (int i = 0; i < nlabels * sizeof(int) / sizeof(char); i++) fprintf(fp, "%c", *((char*)labels + i));
    for (int i = 0; i < end - begin; i++) fputc(begin[i], fp);
    fclose(fp);
}


int main(int argc, char* argv[]){
    
    char* buffer = nullptr;
    int write_asm_list = 0;
    char* out_name = nullptr;
    char* list_name = nullptr;
    switch(argc){

        case 5:   if (!strcmp(argv[4], "--list")) write_asm_list = 1;
                  buffer = ReadFile(argv[1]);
                  out_name = argv[2];
                  list_name = argv[3];
                  break;
    
        case 3:   buffer = ReadFile(argv[1]);
                  out_name = argv[2];
                  break;

        default:  printf("Wrong arguments\n");
                  exit(-1);
    }

    int number_of_lines = 0;
    MyStr* lines =  GetLines(buffer, &number_of_lines);   
    char* begin = nullptr;
    char* end = nullptr;
    int* label_arr = (int*)calloc(MAX_LABELS,sizeof(int));
    char** label_names = (char**)calloc(MAX_LABELS, sizeof(char*));
    int nlabels = 0;
    for (int i = 0; i < MAX_LABELS; i++) label_arr[i] = -1;
    
    if(!Assemble(lines, &begin, &end, 0, "empty", label_names, label_arr, &nlabels, number_of_lines)){
        printf("Assembly failed\n");
        exit(1);
    }

    Assemble(lines, &begin, &end, write_asm_list, list_name, label_names, label_arr, &nlabels, number_of_lines);
    for(int i = 0; i < nlabels; i++) printf("label #%d: %s on rip %d\n",i ,label_names[i], label_arr[i]);
    WriteCode(out_name, begin, end, label_arr, nlabels);
}