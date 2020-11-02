#define RAM_BIT (*(command_start_ptr) & 0x80)
#define REGISTER_BIT (*(command_start_ptr) & 0x40)
#define CONST_BIT (*(command_start_ptr) & 0x20)

#define IF_UNEXPECTED(criteria, value)\
    if (criteria){\
        *status_ptr = 0;\
        printf("Syntax error: unexpected \"" #value "\" on line %d\n", nline + 1);\
    }

# define LEFT_BRACKET_ASSEMBLING\
    IF_UNEXPECTED(RAM_BIT != 0, [)\
    else *(command_start_ptr) += 128 ;\
    *(arg_value_ptr)+=1;


#define PLUS_ASSEMBLING\
    IF_UNEXPECTED(RAM_BIT == 0 && narg != 1 || RAM_BIT != 0 && narg != 2, +)\
    else if (REGISTER_BIT == 0){\
        *status_ptr = 0;\
        printf("Line %d: expression's first member should be register\n", nline+1);\
    }\
    *arg_value_ptr += 1;

#define RIGHT_BRACKET_ASSEMBLING\
    IF_UNEXPECTED(narg != (REGISTER_BIT >> 6) + (CONST_BIT >> 5) + (REGISTER_BIT >> 6) * (CONST_BIT >> 5) + 1, ]) \
    else if (RAM_BIT == 0){\
        *status_ptr = 0;\
        printf("Line %d: \"]\" without \"[\"", nline + 1);\
    }\
    *arg_value_ptr += 1;

KEYWORD([, 0, LEFT_BRACKET_ASSEMBLING, {}})
KEYWORD(+, 0, PLUS_ASSEMBLING, {}})
KEYWORD(], 0, RIGHT_BRACKET_ASSEMBLING, {}})