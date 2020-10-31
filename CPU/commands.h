
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
                if (REGISTER_BIT){\
                    STACK_PUSH(CPU_RAM[(int)(*REGISTER_ADDR + CONST)])\
                }\
                else{\
                    STACK_PUSH(CPU_RAM[(int)CONST])\
                }\
            }\
            else{\
                if (REGISTER_BIT){\
                    STACK_PUSH(*REGISTER_ADDR + CONST)\
                }\
                else{\
                    STACK_PUSH(CONST)\
                }\
            }

    #define DO_POP\
            if (RAM_BIT){\
                if (REGISTER_BIT){\
                    STACK_POP(CPU_RAM[(int)(*REGISTER_ADDR + CONST)] =)\
                }\
                else{\
                    STACK_POP(CPU_RAM[(int)CONST] = )\
                }\
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
    
    #define CDTL_JMP(sign)\
        if (POP_TWO(sign)){\
        JUMP\
        }\
        else {\
        RIP += sizeof(LABEL_TYPE);\
        }\
        STACK_EMPTY_CHECK\

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
            STACK_EMPTY_CHECK\


    #define DO_DUMP\
            CPU_DUMP(thou, fp);\

    #define DO_HLT\
            printf("Program hult on rip %d\n", RIP);\
            return 0;

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

    #define DEFAULT_ARGUMENT_DISASSEMBLE\
            if (REGISTER_BIT){\
                fprintf(fp, "%s ", GET_KEYWORD_NAME(buffer[RIP + offset]));\
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

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

DEF_CMD(PUSH, 0x01, 5, 1, PUSH_ARGUMENTS_CHECK(nline, arg_value), DEFAULT_NON_KEYWORD_PROCESSING_INSTRUCTION, DEFAULT_DISASSEMBLING_INSTRUCTION)
DEF_CMD(POP, 0x02, 5, 0, POP_ARGUMENTS_CHECK(nline, arg_value), DEFAULT_NON_KEYWORD_PROCESSING_INSTRUCTION, DEFAULT_DISASSEMBLING_INSTRUCTION)
DEF_CMD(JMP, 0x03, 1, 1, {}, JMP_NON_KEYWORD_PROCESSING_INSTRUCTION, JMP_DISASSEMBLING_INSTRUCTION)
DEF_CMD(ADD, 0x04, 0, 0, {}, {}, {})
DEF_CMD(SUB, 0x05, 0, 0, {}, {}, {})
DEF_CMD(MUL, 0x06, 0, 0, {}, {}, {})
DEF_CMD(DIV, 0x07, 0, 0, {}, {}, {})
DEF_CMD(SIN, 0x08, 0, 0, {}, {}, {})
DEF_CMD(COS, 0x09, 0, 0, {}, {}, {})
DEF_CMD(SQRT, 0x0A, 0, 0, {}, {}, {})
DEF_CMD(NEG, 0x0B, 0, 0, {}, {}, {})
DEF_CMD(OUT, 0x0C, 0, 0, {}, {}, {})
DEF_CMD(IN, 0x0D, 0, 0, {}, {}, )
DEF_CMD(DUMP, 0x0E, 0, 0, {}, {}, {})
DEF_CMD(HLT, 0x0F, 0, 0, {}, {}, {})
DEF_CMD(JA, 0x10, 1, 1, {}, JMP_NON_KEYWORD_PROCESSING_INSTRUCTION, JMP_DISASSEMBLING_INSTRUCTION)
DEF_CMD(JAE, 0x11, 1, 1, {}, JMP_NON_KEYWORD_PROCESSING_INSTRUCTION, JMP_DISASSEMBLING_INSTRUCTION)
DEF_CMD(JB, 0x12, 1, 1, {}, JMP_NON_KEYWORD_PROCESSING_INSTRUCTION, JMP_DISASSEMBLING_INSTRUCTION)
DEF_CMD(JBE, 0x13, 1, 1, {}, JMP_NON_KEYWORD_PROCESSING_INSTRUCTION, JMP_DISASSEMBLING_INSTRUCTION)
DEF_CMD(JE, 0x14, 1, 1, {}, JMP_NON_KEYWORD_PROCESSING_INSTRUCTION, JMP_DISASSEMBLING_INSTRUCTION)
DEF_CMD(JNE, 0x15, 1, 1, {}, JMP_NON_KEYWORD_PROCESSING_INSTRUCTION, JMP_DISASSEMBLING_INSTRUCTION)
DEF_CMD(CALL, 0x16, 1, 1, {}, JMP_NON_KEYWORD_PROCESSING_INSTRUCTION, JMP_DISASSEMBLING_INSTRUCTION)
DEF_CMD(RETURN, 0x17, 0, 0, {}, {}, DEFAULT_DISASSEMBLING_INSTRUCTION)
