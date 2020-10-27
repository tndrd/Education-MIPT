#define RAM_BIT (*(command_start_ptr) & 0x80)
#define REGISTER_BIT (*(command_start_ptr) & 0x40)
#define CONST_BIT (*(command_start_ptr) & 0x20)

#define IF_UNEXPECTED(criteria, value)\
    if (criteria){\
        *status_ptr = 0;\
        printf("Syntax error: unexpected \"" #value "\" on line %d\n", nline + 1);\
    }


#define ELSE_IF_IS_LABELNAME(value)\
    else if (((*command_start_ptr) & 0x1F) == 0x04){\
        *status_ptr = 0;\
        printf("Line %d: \""#value"\" cannot be a label name\n", nline + 1);\
    }\



# define LEFT_BRACKET_ASSEMBLING\
    IF_UNEXPECTED(RAM_BIT != 0, [)\
    ELSE_IF_IS_LABELNAME([)\
    else *(command_start_ptr) += 128 ;\
    *(arg_value_ptr)+=1;


#define PLUS_ASSEMBLING\
    IF_UNEXPECTED(RAM_BIT == 0 && narg != 1 || RAM_BIT != 0 && narg != 2, +)\
    ELSE_IF_IS_LABELNAME(+)\
    else if (REGISTER_BIT == 0){\
        *status_ptr = 0;\
        printf("Line %d: expression's first member should be register\n", nline+1);\
    }\
    *arg_value_ptr += 1;

#define RIGHT_BRACKET_ASSEMBLING\
    IF_UNEXPECTED(narg != (REGISTER_BIT >> 6) + (CONST_BIT >> 5) + (REGISTER_BIT >> 6) * (CONST_BIT >> 5) + 1, ]) \
    ELSE_IF_IS_LABELNAME(])\
    else if (RAM_BIT == 0){\
        *status_ptr = 0;\
        printf("Line %d: \"]\" without \"[\"", nline + 1);\
    }\
    *arg_value_ptr += 1;

KEYWORD([, 0, LEFT_BRACKET_ASSEMBLING, LEFT_BRACKET_DISASSEMBLING)
KEYWORD(+, 0, PLUS_ASSEMBLING, PLUS_DISASSEMBLING)
KEYWORD(], 0, RIGHT_BRACKET_ASSEMBLING, RIGHT_BRACKET_DISASSEMBLING)