#define REGISTER_ASSEMBLING(keyword_code)                            \
    if (((*command_start_ptr) & 0x1F) == 0x04){\
        *status_ptr = 0;\
        printf("Line %d: label name cannot be a register\n", nline + 1);\
    }\
    else if (((*(command_start_ptr) & 0x80) >> 7) == narg){ \
        **(rip_ptr) = keyword_code;                                  \
        *(command_start_ptr) += 0x40;             \
        *(rip_ptr) = *((char**)(rip_ptr)) + 1 ;                          \
    }                                                  \
    else {\
        *status_ptr = 0;\
        printf("Syntax error: wrong register position on line %d\n", nline + 1);\
    }\
    *(arg_value_ptr)+=3;

KEYWORD(rax, 'a', REGISTER_ASSEMBLING('a'), REGISTER_DISASSEMBLING)
KEYWORD(rbx, 'b', REGISTER_ASSEMBLING('b'), REGISTER_DISASSEMBLING)
KEYWORD(rcx, 'c', REGISTER_ASSEMBLING('c'), REGISTER_DISASSEMBLING)
KEYWORD(rdx, 'd', REGISTER_ASSEMBLING('d'), REGISTER_DISASSEMBLING)