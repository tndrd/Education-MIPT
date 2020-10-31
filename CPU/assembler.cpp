#include "assembler.h"

// Add toomucharguments check

int CheckLabel(char* command){
    char* label_end = strchr(command, ':');
    if (label_end == 0) return 0;
    //printf("%s\n", command);
    return 1;
}


int CompareWithLabel(char* str, char* label_name){
    int nchar = 0;
    while (str[nchar] != '\0'){
        if (str[nchar] != label_name[nchar]) return 0;
        nchar++;
    }
    if (label_name[nchar] == ':') return 1;
    else return 0;
}

int Assemble(MyStr* lines, char* begin, char** endptr, int writeAssemblyList, int writeLabels, const char* assemblyList_name, Label* labels, int* nlabels_ptr, int number_of_lines){
    
    assert(lines);
    assert(begin);
    assert(endptr);
    assert(assemblyList_name);

    #define RAM_BIT (*(command_start_ptr) & 0x80)
    #define REGISTER_BIT (*(command_start_ptr) & 0x40)
    #define CONST_BIT (*(command_start_ptr) & 0x20)


    #define NECESSARY_PLUS\
        if ((REGISTER_BIT * CONST_BIT) && (narg == (1 + ((*(command_start_ptr) & 0x80) >> 7))) && strcmp(command, "+")){\
            status = 0;\
            printf("Wrong syntax on line %d, expected \"+\"\n", nline + 1);\
        }

    #define DEF_CMD(name, num, max_arg, min_arg, arg_check, NON_KEYWORD_PROCESSING_INSTRUCTION, dai)                                                                             \
        else if (!strcmp(command, #name)){                                                                                                    \
            *rip = num & 0xff;                                                                                                                       \
            command_start_ptr = rip;                                                                                                          \
            rip++;                                                                                                                            \
            int narg = 0;                                                                                                                     \
            int toRam = 0;\
            for (; narg < max_arg; narg++){                                                                                                   \
                sscanf(lines[nline].pointer + line_offset, " %s%n", command, &relative_offset);                                             \
                line_offset += relative_offset;                                                                                               \
                if (!strcmp(command, "")){                                                                                                  \
                    if (narg < min_arg) {                                                                                                     \
                    printf("Line %d: not enough arguments for command \"%s\" (given %d, expected %d at least)\n", nline+1, #name, narg, min_arg); \
                    status = 0;                                                                                                               \
                    }                                                                                                                         \
                    break;                                                                                                                    \
                }                                                                                                                             \
                NECESSARY_PLUS                                                                                                                \
                if (!ASSEMBLE_KEYWORD(&command, command_start_ptr, &rip, narg, &status, nline)){                                                   \
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
    char* rip = begin;
    char* command_start_ptr = rip;
    int argtype = 0;
    if (number_of_lines < 1){
        printf("Error: empty file\n");
        return 0;
    }
    printf("this can break\n");
    fflush(stdout);
    char* command = (char*)calloc(1, COMMAND_LENGTH);
    printf("not broken\n");
    fflush(stdout);
    int line_offset = 0;
    int relative_offset = 0;

    FILE* fp = nullptr;
    char* label_name = (char*)calloc(128, sizeof(char));
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
        else if (CheckLabel(lines[nline].pointer)){
            if (writeLabels){
                (labels[*nlabels_ptr]).name = lines[nline].pointer;
                (labels[(*(nlabels_ptr))++]).value = rip - begin;
            }
        }
        else{
            status = 0;
            printf("Unknown Command on line %d: %s\n", nline + 1, command);
        }
        line_offset = 0;
        relative_offset = 0;
    }
    #undef DEF_CMD
    #undef RAM_BIT
    #undef CONST_BIT
    #undef REGISTER_BIT

    if (writeAssemblyList){
        assert(fp);
        fclose(fp);
    } 
    if (!status) return 0;
    *endptr = rip;
    
    return 1;
}

void WriteCode(const char* name, char* begin, char* end, Label* labels, int nlabels){
    FILE* fp = fopen(name, "w");
    assert(fp);
    assert(begin);
    assert(end);
    
    fprintf(fp,SIGNATURE);
    fprintf(fp, "%c", VERSION_BASE + 48);
    fprintf(fp, "%c", VERSION_SUB + 48);
    fprintf(fp, "%c", nlabels);
    for (int nlabel = 0; nlabel < nlabels; nlabel++){
        for (int nbyte = 0; nbyte < sizeof(int)/sizeof(char); nbyte++) fprintf(fp, "%c", *((char*)(&(labels[nlabel].value)) + nbyte));
    }
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
    char* begin = (char*)calloc(1, MINIMAL_FILESIZE);
    char* end = nullptr;
    Label* labels = (Label*)calloc(MAX_LABELS, sizeof(Label));
    int nlabels = 0;
    
    if(!Assemble(lines, begin, &end, 0, 1, "empty", labels, &nlabels, number_of_lines)){
        printf("Assembly failed\n");
        exit(1);
    }

    Assemble(lines, begin, &end, write_asm_list, 0, list_name, labels, &nlabels, number_of_lines);
    for(int i = 0; i < nlabels; i++) printf("label #%d: %s on rip %d\n",i ,labels[i].name, labels[i].value);
    
    WriteCode(out_name, begin, end, labels, nlabels);
}