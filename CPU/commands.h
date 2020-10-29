DEF_CMD(PUSH, 0x01, 5, 1, PUSH_ARGUMENTS_CHECK(nline, arg_value), DEFAULT_NON_KEYWORD_PROCESSING_INSTRUCTION)
DEF_CMD(POP, 0x02, 5, 0, POP_ARGUMENTS_CHECK(nline, arg_value), DEFAULT_NON_KEYWORD_PROCESSING_INSTRUCTION)
DEF_CMD(JMP, 0x03, 1, 1, {}, JMP_NON_KEYWORD_PROCESSING_INSTRUCTION)
DEF_CMD(ADD, 0x04, 0, 0, {}, {})
DEF_CMD(SUB, 0x05, 0, 0, {}, {})
DEF_CMD(MUL, 0x06, 0, 0, {}, {})
DEF_CMD(DIV, 0x07, 0, 0, {}, {})
DEF_CMD(SIN, 0x08, 0, 0, {}, {})
DEF_CMD(COS, 0x09, 0, 0, {}, {})
DEF_CMD(SQRT, 0x0A, 0, 0, {}, {})
DEF_CMD(NEG, 0x0B, 0, 0, {}, {})
DEF_CMD(OUT, 0x0C, 0, 0, {}, {})
DEF_CMD(IN, 0x0D, 0, 0, {}, {})
DEF_CMD(DUMP, 0x0E, 0, 0, {}, {})
DEF_CMD(HLT, 0x0F, 0, 0, {}, {})
DEF_CMD(JA, 0x10, 1, 1, {}, JMP_NON_KEYWORD_PROCESSING_INSTRUCTION)
DEF_CMD(JAE, 0x11, 1, 1, {}, JMP_NON_KEYWORD_PROCESSING_INSTRUCTION)
DEF_CMD(JB, 0x12, 1, 1, {}, JMP_NON_KEYWORD_PROCESSING_INSTRUCTION)
DEF_CMD(JBE, 0x13, 1, 1, {}, JMP_NON_KEYWORD_PROCESSING_INSTRUCTION)
DEF_CMD(JE, 0x14, 1, 1, {}, JMP_NON_KEYWORD_PROCESSING_INSTRUCTION)
DEF_CMD(JNE, 0x15, 1, 1, {}, JMP_NON_KEYWORD_PROCESSING_INSTRUCTION)
DEF_CMD(CALL, 0x16, 1, 1, {}, JMP_NON_KEYWORD_PROCESSING_INSTRUCTION)
DEF_CMD(RETURN, 0x17, 0, 0, {}, {})