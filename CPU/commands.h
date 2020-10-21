
#define ISREGISTER(name) strcmp(name, "rax") * strcmp(name, "rbx") * strcmp(name, "rcx") * strcmp(name, "rdx") 

#define POP_ARGUMENTS_CHECK(nline, arg_value)                             \
        if (ISREGISTER(arg_value)){                                       \
            printf("Line %d: pop argument is not a register\n", nline+1); \
            status = 0;                                                   \
            break;                                                        \
        }                                                       

#define PUSH_ARGUMENTS_CHECK(nline, arg_value)                            \
    if (narg == 1 && ISREGISTER(arg_value)){                              \
        printf("Line %d: second argument is not a register\n", nline+1);  \
        status = 0;                                                       \
    }


DEF_CMD(PUSH, 0x01, 2, 1, {DO_PUSH(ARG(1), ARG(2))}, PUSH_ARGUMENTS_CHECK(nline, arg_value))
DEF_CMD(POP, 0x02, 1, 0, {DO_POP(ARG(1))}, POP_ARGUMENTS_CHECK(nline, arg_value))
