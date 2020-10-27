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

KEYWORD(rax, 'A', REGISTER_ASSEMBLING('A'), REGISTER_DISASSEMBLING)
KEYWORD(rbx, 'B', REGISTER_ASSEMBLING('B'), REGISTER_DISASSEMBLING)
KEYWORD(rcx, 'C', REGISTER_ASSEMBLING('C'), REGISTER_DISASSEMBLING)
KEYWORD(rdx, 'D', REGISTER_ASSEMBLING('D'), REGISTER_DISASSEMBLING)