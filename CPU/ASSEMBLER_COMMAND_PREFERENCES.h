 #define POP_ARGUMENTS_CHECK(nline, arg_value)                             \
        if (CONST_BIT != 0 && RAM_BIT == 0 || RAM_BIT != 0 && CONST_BIT == 0 && REGISTER_BIT == 0){\
            status = 0;\
            printf("Line %d: wrong use of \"pop\"\n", nline + 1);\
        }

#define PUSH_ARGUMENTS_CHECK(nline, arg_value)                            \
    if (CONST_BIT == 0 && REGISTER_BIT == 0){\
            status = 0;\
            printf("Line %d: wrong use of \"push\"\n", nline + 1);\
    }


#define DEFAULT_NON_KEYWORD_PROCESSING_INSTRUCTION\
    *((double*)rip) = strtod(arg_value, &arg_value);                                                                          \
    rip+=sizeof(double);                                                                                                      \
    *(command_start_ptr) += 0x20;                                                                                             \

#define JMP_NON_KEYWORD_PROCESSING_INSTRUCTION\
    *(command_start_ptr) += 0x80;\
    for (int i = 0; i < *nlabels_ptr; i++){\
        if (!strcmp(arg_value, label_names[i])){\
            *((LABEL_TYPE*)rip) = i;\
        }\
    }\
    rip+=sizeof(LABEL_TYPE);\
