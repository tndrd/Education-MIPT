int ASSEMBLE_KEYWORD(char** arg_value_ptr, char* command_start_ptr, char** rip_ptr, int narg, int* status_ptr, int nline){
   
    #define KEYWORD(name, keyword_code, ASSEMBLING_INSTR, DISASSEMBLING_INSTR) else if(!strcmp(*arg_value_ptr, #name)) {\
        ASSEMBLING_INSTR\
    }
    if(0) return 228; 
    #include "keywords.h"
    #undef KEYWORD
    else return 0;
    return 1;
}

char* GET_KEYWORD_NAME(char code){
    #define KEYWORD(name, keyword_code, ain, din)\
        case keyword_code: return #name;\
                           break;
    
    switch(code){ 
    #include "registers.h"
    #undef KEYWORD
    default: return nullptr;
    }
}