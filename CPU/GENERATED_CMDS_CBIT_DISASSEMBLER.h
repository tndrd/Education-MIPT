

#define CBITS_0_ADD\ 
    CBITS_0(ADD)\ 
    fprintf(fp, "\n");\ 

#define CBITS_1_ADD\ 
    printf("Error: code corrupted, wrong use of ADD\n");\ 
    exit(-1);\ 
 
#define CBITS_2_ADD CBITS_1_ADD
#define CBITS_3_ADD CBITS_1_ADD
#define CBITS_4_ADD CBITS_1_ADD
#define CBITS_5_ADD CBITS_1_ADD
#define CBITS_6_ADD CBITS_1_ADD
#define CBITS_7_ADD CBITS_1_ADD


#define CBITS_0_SUB\ 
    CBITS_0(SUB)\ 
    fprintf(fp, "\n");\ 

#define CBITS_1_SUB\ 
    printf("Error: code corrupted, wrong use of SUB\n");\ 
    exit(-1);\ 
 
#define CBITS_2_SUB CBITS_1_SUB
#define CBITS_3_SUB CBITS_1_SUB
#define CBITS_4_SUB CBITS_1_SUB
#define CBITS_5_SUB CBITS_1_SUB
#define CBITS_6_SUB CBITS_1_SUB
#define CBITS_7_SUB CBITS_1_SUB


#define CBITS_0_MUL\ 
    CBITS_0(MUL)\ 
    fprintf(fp, "\n");\ 

#define CBITS_1_MUL\ 
    printf("Error: code corrupted, wrong use of MUL\n");\ 
    exit(-1);\ 
 
#define CBITS_2_MUL CBITS_1_MUL
#define CBITS_3_MUL CBITS_1_MUL
#define CBITS_4_MUL CBITS_1_MUL
#define CBITS_5_MUL CBITS_1_MUL
#define CBITS_6_MUL CBITS_1_MUL
#define CBITS_7_MUL CBITS_1_MUL


#define CBITS_0_DIV\ 
    CBITS_0(DIV)\ 
    fprintf(fp, "\n");\ 

#define CBITS_1_DIV\ 
    printf("Error: code corrupted, wrong use of DIV\n");\ 
    exit(-1);\ 
 
#define CBITS_2_DIV CBITS_1_DIV
#define CBITS_3_DIV CBITS_1_DIV
#define CBITS_4_DIV CBITS_1_DIV
#define CBITS_5_DIV CBITS_1_DIV
#define CBITS_6_DIV CBITS_1_DIV
#define CBITS_7_DIV CBITS_1_DIV


#define CBITS_0_SIN\ 
    CBITS_0(SIN)\ 
    fprintf(fp, "\n");\ 

#define CBITS_1_SIN\ 
    printf("Error: code corrupted, wrong use of SIN\n");\ 
    exit(-1);\ 
 
#define CBITS_2_SIN CBITS_1_SIN
#define CBITS_3_SIN CBITS_1_SIN
#define CBITS_4_SIN CBITS_1_SIN
#define CBITS_5_SIN CBITS_1_SIN
#define CBITS_6_SIN CBITS_1_SIN
#define CBITS_7_SIN CBITS_1_SIN


#define CBITS_0_COS\ 
    CBITS_0(COS)\ 
    fprintf(fp, "\n");\ 

#define CBITS_1_COS\ 
    printf("Error: code corrupted, wrong use of COS\n");\ 
    exit(-1);\ 
 
#define CBITS_2_COS CBITS_1_COS
#define CBITS_3_COS CBITS_1_COS
#define CBITS_4_COS CBITS_1_COS
#define CBITS_5_COS CBITS_1_COS
#define CBITS_6_COS CBITS_1_COS
#define CBITS_7_COS CBITS_1_COS


#define CBITS_0_SQRT\ 
    CBITS_0(SQRT)\ 
    fprintf(fp, "\n");\ 

#define CBITS_1_SQRT\ 
    printf("Error: code corrupted, wrong use of SQRT\n");\ 
    exit(-1);\ 
 
#define CBITS_2_SQRT CBITS_1_SQRT
#define CBITS_3_SQRT CBITS_1_SQRT
#define CBITS_4_SQRT CBITS_1_SQRT
#define CBITS_5_SQRT CBITS_1_SQRT
#define CBITS_6_SQRT CBITS_1_SQRT
#define CBITS_7_SQRT CBITS_1_SQRT


#define CBITS_0_NEG\ 
    CBITS_0(NEG)\ 
    fprintf(fp, "\n");\ 

#define CBITS_1_NEG\ 
    printf("Error: code corrupted, wrong use of NEG\n");\ 
    exit(-1);\ 
 
#define CBITS_2_NEG CBITS_1_NEG
#define CBITS_3_NEG CBITS_1_NEG
#define CBITS_4_NEG CBITS_1_NEG
#define CBITS_5_NEG CBITS_1_NEG
#define CBITS_6_NEG CBITS_1_NEG
#define CBITS_7_NEG CBITS_1_NEG


#define CBITS_0_OUT\ 
    CBITS_0(OUT)\ 
    fprintf(fp, "\n");\ 

#define CBITS_1_OUT\ 
    printf("Error: code corrupted, wrong use of OUT\n");\ 
    exit(-1);\ 
 
#define CBITS_2_OUT CBITS_1_OUT
#define CBITS_3_OUT CBITS_1_OUT
#define CBITS_4_OUT CBITS_1_OUT
#define CBITS_5_OUT CBITS_1_OUT
#define CBITS_6_OUT CBITS_1_OUT
#define CBITS_7_OUT CBITS_1_OUT


#define CBITS_0_IN\ 
    CBITS_0(IN)\ 
    fprintf(fp, "\n");\ 

#define CBITS_1_IN\ 
    printf("Error: code corrupted, wrong use of IN\n");\ 
    exit(-1);\ 
 
#define CBITS_2_IN CBITS_1_IN
#define CBITS_3_IN CBITS_1_IN
#define CBITS_4_IN CBITS_1_IN
#define CBITS_5_IN CBITS_1_IN
#define CBITS_6_IN CBITS_1_IN
#define CBITS_7_IN CBITS_1_IN


#define CBITS_0_DUMP\ 
    CBITS_0(DUMP)\ 
    fprintf(fp, "\n");\ 

#define CBITS_1_DUMP\ 
    printf("Error: code corrupted, wrong use of DUMP\n");\ 
    exit(-1);\ 
 
#define CBITS_2_DUMP CBITS_1_DUMP
#define CBITS_3_DUMP CBITS_1_DUMP
#define CBITS_4_DUMP CBITS_1_DUMP
#define CBITS_5_DUMP CBITS_1_DUMP
#define CBITS_6_DUMP CBITS_1_DUMP
#define CBITS_7_DUMP CBITS_1_DUMP


#define CBITS_0_HLT\ 
    CBITS_0(HLT)\ 
    fprintf(fp, "\n");\ 

#define CBITS_1_HLT\ 
    printf("Error: code corrupted, wrong use of HLT\n");\ 
    exit(-1);\ 
 
#define CBITS_2_HLT CBITS_1_HLT
#define CBITS_3_HLT CBITS_1_HLT
#define CBITS_4_HLT CBITS_1_HLT
#define CBITS_5_HLT CBITS_1_HLT
#define CBITS_6_HLT CBITS_1_HLT
#define CBITS_7_HLT CBITS_1_HLT


#define CBITS_0_RETURN\ 
    CBITS_0(RETURN)\ 
    fprintf(fp, "\n");\ 

#define CBITS_1_RETURN\ 
    printf("Error: code corrupted, wrong use of RETURN\n");\ 
    exit(-1);\ 
 
#define CBITS_2_RETURN CBITS_1_RETURN
#define CBITS_3_RETURN CBITS_1_RETURN
#define CBITS_4_RETURN CBITS_1_RETURN
#define CBITS_5_RETURN CBITS_1_RETURN
#define CBITS_6_RETURN CBITS_1_RETURN
#define CBITS_7_RETURN CBITS_1_RETURN


#define CBITS_4_JMP\ 
    DISASM_JMP(JMP)\ 

#define CBITS_0_JMP\ 
    printf("Error: code corrupted, wrong use of JMP\n");\ 
    exit(-1);\ 
 
#define CBITS_1_JMP CBITS_0_JMP
#define CBITS_2_JMP CBITS_0_JMP
#define CBITS_3_JMP CBITS_0_JMP
#define CBITS_5_JMP CBITS_0_JMP
#define CBITS_6_JMP CBITS_0_JMP
#define CBITS_7_JMP CBITS_0_JMP


#define CBITS_4_JA\ 
    DISASM_JMP(JA)\ 

#define CBITS_0_JA\ 
    printf("Error: code corrupted, wrong use of JA\n");\ 
    exit(-1);\ 
 
#define CBITS_1_JA CBITS_0_JA
#define CBITS_2_JA CBITS_0_JA
#define CBITS_3_JA CBITS_0_JA
#define CBITS_5_JA CBITS_0_JA
#define CBITS_6_JA CBITS_0_JA
#define CBITS_7_JA CBITS_0_JA


#define CBITS_4_JAE\ 
    DISASM_JMP(JAE)\ 

#define CBITS_0_JAE\ 
    printf("Error: code corrupted, wrong use of JAE\n");\ 
    exit(-1);\ 
 
#define CBITS_1_JAE CBITS_0_JAE
#define CBITS_2_JAE CBITS_0_JAE
#define CBITS_3_JAE CBITS_0_JAE
#define CBITS_5_JAE CBITS_0_JAE
#define CBITS_6_JAE CBITS_0_JAE
#define CBITS_7_JAE CBITS_0_JAE


#define CBITS_4_JB\ 
    DISASM_JMP(JB)\ 

#define CBITS_0_JB\ 
    printf("Error: code corrupted, wrong use of JB\n");\ 
    exit(-1);\ 
 
#define CBITS_1_JB CBITS_0_JB
#define CBITS_2_JB CBITS_0_JB
#define CBITS_3_JB CBITS_0_JB
#define CBITS_5_JB CBITS_0_JB
#define CBITS_6_JB CBITS_0_JB
#define CBITS_7_JB CBITS_0_JB


#define CBITS_4_JBE\ 
    DISASM_JMP(JBE)\ 

#define CBITS_0_JBE\ 
    printf("Error: code corrupted, wrong use of JBE\n");\ 
    exit(-1);\ 
 
#define CBITS_1_JBE CBITS_0_JBE
#define CBITS_2_JBE CBITS_0_JBE
#define CBITS_3_JBE CBITS_0_JBE
#define CBITS_5_JBE CBITS_0_JBE
#define CBITS_6_JBE CBITS_0_JBE
#define CBITS_7_JBE CBITS_0_JBE


#define CBITS_4_JE\ 
    DISASM_JMP(JE)\ 

#define CBITS_0_JE\ 
    printf("Error: code corrupted, wrong use of JE\n");\ 
    exit(-1);\ 
 
#define CBITS_1_JE CBITS_0_JE
#define CBITS_2_JE CBITS_0_JE
#define CBITS_3_JE CBITS_0_JE
#define CBITS_5_JE CBITS_0_JE
#define CBITS_6_JE CBITS_0_JE
#define CBITS_7_JE CBITS_0_JE


#define CBITS_4_JNE\ 
    DISASM_JMP(JNE)\ 

#define CBITS_0_JNE\ 
    printf("Error: code corrupted, wrong use of JNE\n");\ 
    exit(-1);\ 
 
#define CBITS_1_JNE CBITS_0_JNE
#define CBITS_2_JNE CBITS_0_JNE
#define CBITS_3_JNE CBITS_0_JNE
#define CBITS_5_JNE CBITS_0_JNE
#define CBITS_6_JNE CBITS_0_JNE
#define CBITS_7_JNE CBITS_0_JNE


#define CBITS_4_CALL\ 
    DISASM_JMP(CALL)\ 

#define CBITS_0_CALL\ 
    printf("Error: code corrupted, wrong use of CALL\n");\ 
    exit(-1);\ 
 
#define CBITS_1_CALL CBITS_0_CALL
#define CBITS_2_CALL CBITS_0_CALL
#define CBITS_3_CALL CBITS_0_CALL
#define CBITS_5_CALL CBITS_0_CALL
#define CBITS_6_CALL CBITS_0_CALL
#define CBITS_7_CALL CBITS_0_CALL
