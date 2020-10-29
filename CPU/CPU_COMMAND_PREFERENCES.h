//NE PEREBOR

#define STACK_EMPTY_CHECK\
        if (!(thou -> _rsx)){\
            printf("Failed to pop on rip %d\n", rip);\
            exit(9);\
        }

#define DO_JUMP(destination)\
        rip++;\
        rip = destination + HEADER_LENGTH + nlabels*sizeof(int) + 1;\


#define DO_POP(destination_prefix, offset)\
        rip++;\
        destination_prefix StackPop(thou -> stack_ptr, &(thou -> _rsx));\
        STACK_EMPTY_CHECK\
        rip += (offset);


#define DO_PUSH(value, offset)\
        rip++;\
        if(!StackPush(thou -> stack_ptr, value)){\
            printf("Failed to push on rip %d\n", rip);\
            exit(9);\
        }\
        rip += (offset);

//-----------------------------------------------------------------

#define CBITS_0_PUSH\
        printf("Error: code corrupted, wrong use of \"PUSH\"\n");\
        exit(1);

#define CBITS_1_PUSH\
        DO_PUSH(*((double*)(buffer + rip)), sizeof(double))\
        CPU_TRACE\

#define CBITS_2_PUSH\
        DO_PUSH(*GET_REGISTER_ADRESS(thou, *(buffer + rip)), 1)\
        CPU_TRACE\

#define CBITS_3_PUSH\
        DO_PUSH(*(GET_REGISTER_ADRESS(thou, *(buffer + rip))) + *((double*)(buffer + rip + 1)), sizeof(double) + 1)\
        CPU_TRACE\

#define CBITS_4_PUSH\
        CBITS_0_PUSH\

#define CBITS_5_PUSH\
        DO_PUSH((thou -> RAM)[(int)(*((double*)(buffer + rip)))], sizeof(double))\
        CPU_TRACE\

#define CBITS_6_PUSH\
        DO_PUSH((thou -> RAM)[(int)(*GET_REGISTER_ADRESS(thou, *(buffer + rip)))], 1)\
        CPU_TRACE\


#define CBITS_7_PUSH\
        DO_PUSH((thou -> RAM)[(int)(*(GET_REGISTER_ADRESS(thou, *(buffer + rip))) + *((double*)(buffer + rip + 1)))], sizeof(double) + 1)\
        CPU_TRACE\

//-----------------------------------------------------------------

#define CBITS_0_POP\
        DO_POP( ; , 0);\
        CPU_TRACE\


#define CBITS_1_POP\
        printf("Error: code corrupted, wrong use of \"POP\"\n");\
        exit(1);

#define CBITS_2_POP\
        DO_POP(*(GET_REGISTER_ADRESS(thou, *(buffer+rip))) = , 1)\
        CPU_TRACE\

#define CBITS_3_POP\
        CBITS_1_POP

#define CBITS_4_POP\
        CBITS_1_POP

#define CBITS_5_POP\
        DO_POP((thou -> RAM)[(int)(*((double*)(buffer + rip)))] = , sizeof(double))\
        CPU_TRACE\

#define CBITS_6_POP\
        DO_POP((thou -> RAM)[(int)(*(GET_REGISTER_ADRESS(thou, *(buffer+rip))))] = , 1)\
        CPU_TRACE\

#define CBITS_7_POP\
        DO_POP((thou -> RAM)[(int)(*((double*)(buffer + rip + 1)) + *(GET_REGISTER_ADRESS(thou, *(buffer+rip))))] = , sizeof(double) + 1)\
        CPU_TRACE\

//-------------------------------------------------------------------
#define DO_JMP\
        DO_JUMP(buffer[HEADER_LENGTH + 1 + (*(buffer + rip)) * sizeof(int)])\
        CPU_TRACE

#define CDTL_JMP(sign)\
        if (StackPop(thou -> stack_ptr, &(thou -> _rsx)) sign StackPop(thou -> stack_ptr, &(thou -> _rsx))){\
        DO_JMP\
        }\
        else {\
        rip += sizeof(LABEL_TYPE) + 1;\
        }\
        STACK_EMPTY_CHECK\

#define DO_CALL\
        if(!StackPush(thou -> callstack, rip+1+sizeof(LABEL_TYPE))){\
            printf("Callstack overflow on rip %d\n", rip);\
            exit(11);\
        }\
        DO_JMP\
        CPU_TRACE

#define DO_RETURN\
        prev_rip = rip;\
        rip = StackPop(thou -> callstack, &(thou -> _rsx));\
        if (!(thou -> _rsx)){\
            printf("Critical Error: attempt to return while callstack is empty on rip %d\n", prev_rip);\
            exit(12);\
        }\
        CPU_TRACE

#define DO_JA CDTL_JMP(<)
#define DO_JAE CDTL_JMP(<=)
#define DO_JB CDTL_JMP(>)
#define DO_JBE CDTL_JMP(>=)
#define DO_JE CDTL_JMP(==)
#define DO_JNE CDTL_JMP(!=)

//--------------------------------------------------------------------

#define BIN_OPERATION(sign)\
        DO_PUSH(StackPop(thou -> stack_ptr, &(thou -> _rsx)) sign StackPop(thou -> stack_ptr, &(thou -> _rsx)), 0)\
        STACK_EMPTY_CHECK\
        CPU_TRACE

#define DO_ADD BIN_OPERATION(+)
#define DO_SUB BIN_OPERATION(-)
#define DO_MUL BIN_OPERATION(*)
#define DO_DIV BIN_OPERATION(/)

//---------------------------------------------------------------------

#define UN_OPERATION(func)\
        DO_PUSH(func(StackPop(thou -> stack_ptr, &(thou -> _rsx))), 0)\
        STACK_EMPTY_CHECK\
        CPU_TRACE

#define DO_SIN UN_OPERATION(sin)
#define DO_COS UN_OPERATION(cos)
#define DO_SQRT UN_OPERATION(sqrt)
#define DO_NEG UN_OPERATION(-)

//----------------------------------------------------------------------

#define DO_IN\
        double value = NAN;\
        if (!scanf("%lf", &value)){\
            printf("Failed to read value from stdin at rip %d\n", rip);\
            exit(10);\
        }\
        DO_PUSH(value, 0);\
        CPU_TRACE

#define DO_OUT\
        printf("%lf\n", StackPop(thou -> stack_ptr, &(thou -> _rsx)));\
        rip++;\
        STACK_EMPTY_CHECK\


#define DO_DUMP\
        CPU_DUMP(thou, fp);\
        rip++;\
        CPU_TRACE

#define DO_HLT\
        printf("Program hult on rip %d\n", rip);\
        exit(0);

#include "GENERATED_CMDS_CBIT_CPU.h" //Generated by python script