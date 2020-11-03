//#########################################################################
//-----------------------------CPU-----------------------------------------

#define STATUS_CHECK(error)\
        if (!(thou -> _rsx)){\
        printf(error, RIP);\
        return 9;\
    }

#define STACK_EMPTY_CHECK STATUS_CHECK("Failed to pop on RIP %d\n")
#define STACK_OVERFLOW_CHECK STATUS_CHECK("Failed to push on RIP %d\n")
    
//-------------------------------------------------------------------------

#define STACK_POP(destination)\
        destination StackPop(CPU_Stack_ptr, &(CPU_Status));\
        STACK_EMPTY_CHECK

#define STACK_PUSH(value)\
        CPU_Status = StackPush(CPU_Stack_ptr, value);\
        STACK_OVERFLOW_CHECK 

#define POPPED StackPop(CPU_Stack_ptr, &(CPU_Status))
#define POP_TWO(sign) POPPED sign POPPED

#define DO_PUSH\
        if (RAM_BIT){\
            STACK_PUSH(CPU_RAM[(int)(*REGISTER_ADDR + CONST)])\
        }\
        else{\
            STACK_PUSH(*REGISTER_ADDR + CONST)\
        }

#define DO_POP\
        if (RAM_BIT){\
            STACK_POP(CPU_RAM[(int)(*REGISTER_ADDR + CONST)] =)\
        }\
        else{\
            if (REGISTER_BIT){\
                STACK_POP(*REGISTER_ADDR = )\
            }\
            else{\
                STACK_POP()\
            }\
        }

//-------------------------------------------------------------------------

#define JUMP\
    RIP = *((int*)(buffer + HEADER_LENGTH + 1 + (*(buffer + RIP + 1)) * sizeof(int))) + HEADER_LENGTH + nlabels * sizeof(int);\

#define DO_JMP JUMP

#define DO_CALL\
        if(!StackPush(CALLSTACK, RIP + sizeof(LABEL_TYPE))){\
            printf("Callstack overflow on rip %d\n", RIP);\
            return 11;\
        }\
        DO_JMP\

#define DO_RETURN\
        prev_rip = RIP;\
        RIP = StackPop(CALLSTACK, &(thou -> _rsx));\
        if (!(CPU_Status)){\
            printf("Critical Error: attempt to return while callstack is empty on rip %d\n", prev_rip);\
            return 12;\
        }\


#define CDTL_JMP(sign)\
    if (POP_TWO(sign)){\
    JUMP\
    }\
    else {\
    RIP += sizeof(LABEL_TYPE);\
    }\
    STACK_EMPTY_CHECK\

#define DO_JA CDTL_JMP(<)
#define DO_JAE CDTL_JMP(<=)
#define DO_JB CDTL_JMP(>)
#define DO_JBE CDTL_JMP(>=)
#define DO_JE CDTL_JMP(==)
#define DO_JNE CDTL_JMP(!=)

//-------------------------------------------------------------------------

#define BIN_OPERATION(sign)\
    STACK_PUSH(POP_TWO(sign))\
    STACK_EMPTY_CHECK\

#define DO_ADD BIN_OPERATION(+)
#define DO_SUB BIN_OPERATION(-)
#define DO_MUL BIN_OPERATION(*)
#define DO_DIV BIN_OPERATION(/)

//---------------------------------------------------------------------

#define UN_OPERATION(func)\
        STACK_PUSH(func(POPPED))\
        STACK_EMPTY_CHECK\

#define DO_SIN UN_OPERATION(sin)
#define DO_COS UN_OPERATION(cos)
#define DO_SQRT UN_OPERATION(sqrt)
#define DO_NEG UN_OPERATION(-)

//-------------------------------------------------------------------------

#define DO_IN\
    if (!scanf("%lf", &CONST)){\
        printf("Failed to read value from stdin at rip %d\n", RIP);\
        return 10;\
    }\
    STACK_PUSH(CONST)\

#define DO_OUT\
        printf("%lf\n", POPPED);\
        fflush(stdout);\
        STACK_EMPTY_CHECK\


#define DO_DUMP\
        CPU_DUMP(thou, fp);\

#define DO_HLT\
        printf("Program hult on rip %d\n", RIP);\
        return 0;

//-------------------------------------------------------------------------

#define PIXEL(x, y) CPU_RAM[y*YSIZE + x]

#define DO_SHOW                              \
        for (int y = 0; y < YSIZE; y+=2){     \
            for (int x = 0; x < XSIZE; x++){ \
                if (PIXEL(x,y) == 1) printf("\x1b[8;44m@\x1b[0m");\
                else printf("\x1b[8;40mE\x1b[0m");\
            }                                \
            printf("\n");                    \
        }


//#########################################################################
//------------------DISASSEMBLER-------------------------------------------

#define DEFAULT_ARGUMENT_DISASSEMBLE\
        if (REGISTER_BIT){\
            fprintf(fp, "r%cx ", buffer[RIP + offset]);\
            offset++;\
        }\
        if (CONST_BIT){\
            if (REGISTER_BIT) fprintf(fp, "+ ");\
            fprintf(fp, "%lf ", *((double*)(buffer + RIP + offset)));\
            offset += sizeof(double);\
        }\

#define DEFAULT_DISASSEMBLING_INSTRUCTION\
        if (RAM_BIT){\
            fprintf(fp, "[ ");\
            DEFAULT_ARGUMENT_DISASSEMBLE\
            fprintf(fp, "]");\
        }\
        else{\
            DEFAULT_ARGUMENT_DISASSEMBLE\
        }

#define JMP_DISASSEMBLING_INSTRUCTION\
        fprintf(fp, "LABEL_%d ", *((LABEL_TYPE*)(buffer + RIP + offset)));\
        offset+=sizeof(LABEL_TYPE);\

//#########################################################################
//--------------------ASSEMBLER--------------------------------------------

#define POP_ARGUMENTS_CHECK(nline, command)                             \
    if (CONST_BIT != 0 && RAM_BIT == 0 || RAM_BIT != 0 && CONST_BIT == 0 && REGISTER_BIT == 0){\
        status = 0;\
        printf("Line %d: wrong use of \"pop\"\n", nline + 1);\
    }

#define PUSH_ARGUMENTS_CHECK(nline, command)                            \
    if (CONST_BIT == 0 && REGISTER_BIT == 0){\
            status = 0;\
            printf("Line %d: wrong use of \"push\"\n", nline + 1);\
    }

#define DEFAULT_ASSEMBLING_INSTRUCTION\
    if (!ASSEMBLE_KEYWORD(&command, command_start_ptr, &rip, narg, &status, nline)){ \
        *((double*)rip) = strtod(command, &command);                                 \
        rip+=sizeof(double);                                                         \
        *(command_start_ptr) += 0x20;                                                \
    }

#define JMP_ASSEMBLING_INSTRUCTION \
    for (int i = 0; i < *nlabels_ptr; i++){\
        if (CompareWithLabel(command, (labels[i]).name)){\
            *((LABEL_TYPE*)rip) = i;\
        }\
    }\
    rip+=sizeof(LABEL_TYPE);\

//#########################################################################
//--------------------------------COMMANDS---------------------------------


DEF_CMD (PUSH,   0x01, 5, 1, PUSH_ARGUMENTS_CHECK (nline, arg_value), DEFAULT_ASSEMBLING_INSTRUCTION, DEFAULT_DISASSEMBLING_INSTRUCTION)
DEF_CMD (POP,    0x02, 5, 0, POP_ARGUMENTS_CHECK  (nline, arg_value), DEFAULT_ASSEMBLING_INSTRUCTION, DEFAULT_DISASSEMBLING_INSTRUCTION)
DEF_CMD (JMP,    0x03, 1, 1, {},                                      JMP_ASSEMBLING_INSTRUCTION,     JMP_DISASSEMBLING_INSTRUCTION)
DEF_CMD (ADD,    0x04, 0, 0, {}, {}, {})
DEF_CMD (SUB,    0x05, 0, 0, {}, {}, {})
DEF_CMD (MUL,    0x06, 0, 0, {}, {}, {})
DEF_CMD (DIV,    0x07, 0, 0, {}, {}, {})
DEF_CMD (SIN,    0x08, 0, 0, {}, {}, {})
DEF_CMD (COS,    0x09, 0, 0, {}, {}, {})
DEF_CMD (SQRT,   0x0A, 0, 0, {}, {}, {})
DEF_CMD (NEG,    0x0B, 0, 0, {}, {}, {})
DEF_CMD (OUT,    0x0C, 0, 0, {}, {}, {})
DEF_CMD (IN,     0x0D, 0, 0, {}, {}, {})
DEF_CMD (DUMP,   0x0E, 0, 0, {}, {}, {})
DEF_CMD (HLT,    0x0F, 0, 0, {}, {}, {})
DEF_CMD (JA,     0x10, 1, 1, {}, JMP_ASSEMBLING_INSTRUCTION, JMP_DISASSEMBLING_INSTRUCTION)
DEF_CMD (JAE,    0x11, 1, 1, {}, JMP_ASSEMBLING_INSTRUCTION, JMP_DISASSEMBLING_INSTRUCTION)
DEF_CMD (JB,     0x12, 1, 1, {}, JMP_ASSEMBLING_INSTRUCTION, JMP_DISASSEMBLING_INSTRUCTION)
DEF_CMD (JBE,    0x13, 1, 1, {}, JMP_ASSEMBLING_INSTRUCTION, JMP_DISASSEMBLING_INSTRUCTION)
DEF_CMD (JE,     0x14, 1, 1, {}, JMP_ASSEMBLING_INSTRUCTION, JMP_DISASSEMBLING_INSTRUCTION)
DEF_CMD (JNE,    0x15, 1, 1, {}, JMP_ASSEMBLING_INSTRUCTION, JMP_DISASSEMBLING_INSTRUCTION)
DEF_CMD (CALL,   0x16, 1, 1, {}, JMP_ASSEMBLING_INSTRUCTION, JMP_DISASSEMBLING_INSTRUCTION)
DEF_CMD (RETURN, 0x17, 0, 0, {}, {}, {})
DEF_CMD (SHOW,   0x18, 0, 0, {}, {}, {})

//#########################################################################
//-----------------------REGISTERS----------------------------------------

#define REGISTER_ASSEMBLING(keyword_code)                            \
    if (((*(command_start_ptr) & 0x80) >> 7) == narg){               \
        **(rip_ptr) = keyword_code;                                  \
        *(command_start_ptr) += 0x40;                                \
        *(rip_ptr) = *((char**)(rip_ptr)) + 1 ;                      \
    }                                                                \
    else {                                                           \
        *status_ptr = 0;                                             \
        printf("Syntax error: wrong register position on line %d\n", nline + 1);\
    }                                                                \
    *(arg_value_ptr) += 3;

REGISTER(rax, 'a', REGISTER_ASSEMBLING('a'))
REGISTER(rbx, 'b', REGISTER_ASSEMBLING('b'))
REGISTER(rcx, 'c', REGISTER_ASSEMBLING('c'))
REGISTER(rdx, 'd', REGISTER_ASSEMBLING('d'))

//#########################################################################
//-----------------------SPECIAL_SYMBOLS-----------------------------------

#define IF_UNEXPECTED(criteria, value)\
    if (criteria){\
        *status_ptr = 0;\
        printf("Syntax error: unexpected \"" #value "\" on line %d\n", nline + 1);\
    }

#define LEFT_BRACKET_ASSEMBLING\
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

SSYMBOL([, LEFT_BRACKET_ASSEMBLING)
SSYMBOL(+, PLUS_ASSEMBLING)
SSYMBOL(], RIGHT_BRACKET_ASSEMBLING)

//#########################################################################
