#include "AST.h"

//TODO add an opportunity to end tokenisation of lexem on operator (remove CompareStringsUpToChar and update MoveCaretNextLexem)

//Constant Folding


int main() {
    /*
    Tree* tree = (Tree*)calloc(1, sizeof(Tree));
    
    tree -> root = CreateNode(CONSTANT, 23, nullptr, nullptr); 
    tree -> size++;

    printf("%d\n", AttachNode(tree, LEFT, CreateNode(KEYWORD, (KW_ID)('-'), nullptr, nullptr), tree -> root));

    GraphicalDump(tree);
    */


    /*
    char* expression = (char*)calloc(1024, sizeof(char));
    fgets(expression, 1024, stdin);
    

    Tree* tree = (Tree*)calloc(1,sizeof(Tree));
    tree -> root = GetG(expression, tree);
    GraphicalDump(tree);
    GraphicalDump(tree);
    system("viewnior show.png");
    */
    


    char* expression = ReadFile("code.lextex");

    LexemsList* lexed = LexAnalysis(expression);
    
    if (lexed -> status == LEX_ANALYSIS_OK) {
        printf("Lexical analysis complete, showing diagram...\n");
        LexGraphicalDump(lexed);
        
        Tree* tree = (Tree*)calloc(1,sizeof(Tree));
        tree -> root = GetG(lexed);
        GraphicalDump(tree);
        GenerateASM(tree, "test.myasm");
        
    }
    
    return 0;
}

void LexGraphicalDump(LexemsList* lexems_list) {

    FILE* fp = fopen("show", "w");   
    
    fprintf(fp, "digraph structs {\nrankdir=LR rank=same;\n");
    
    for(size_t n_lexem = 0; n_lexem < (lexems_list -> size) - 1; n_lexem++) {

        switch(LEXEM_TYPE(n_lexem)) {

            case OPERATOR:   fprintf(fp, "%d [shape=record, fillcolor=lightgray rank = same style=filled label=\"    %s | {%d}\" ];\n",
                                   n_lexem, GetOperatorName(LEXEM_ID(n_lexem)), n_lexem); break;
            case CONSTANT:   fprintf(fp, "%d [shape=record, fillcolor=green rank = same style=filled label=\"    %lf| {%d}\" ];\n",
                                   n_lexem, LEXEM_VAL(n_lexem), n_lexem); break;
            case IDENTIFIER: fprintf(fp, "%d [shape=record, fillcolor=aquamarine rank = same style=filled label=\"    %s| {%d}\" ];\n",
                                   n_lexem, LEXEM_IDR_NAME(n_lexem), n_lexem); break;
        
        }

    }
    for(size_t n_lexem = 0; n_lexem < (lexems_list -> size) - 2; n_lexem++) fprintf(fp, "%d -> %d\n", n_lexem, n_lexem+1);
    fprintf(fp, "}");
    fclose(fp);
    
    system("dot -Tpng show -n -o show.png");
    system("viewnior show.png");
}


    #define LEXEMS       (lexem_list -> lexems)
    #define LEXLIST_SIZE     (lexem_list -> size) 
    #define FINAL_STATUS (lexem_list -> status)

void AddLexem(LexemsList* lexem_list, Lexem* lexem) {
    
    LEXEMS = (Lexem**)realloc(LEXEMS, (LEXLIST_SIZE+1)*sizeof(Lexem*));
    LEXEMS[LEXLIST_SIZE] = lexem;    
    LEXLIST_SIZE++;
}

void PrintLexem(Lexem* lexem, size_t n_lexem, LexAnalysis_Status status, size_t n_char) {
    
    printf("#%d %s {%p} at %d ", n_lexem, GetLexAnalysisStatus(status), lexem, n_char);
        
    if (lexem && status == TOKEN_OK) {

        switch(TYPE_OF(lexem)) {
                    
            case OPERATOR:   printf(": %c / %s\n", TYPE_OF(lexem), GetOperatorString(ID_OF(lexem))); break;

            case IDENTIFIER: printf(": %c / %s\n", TYPE_OF(lexem), IDR_NAME_OF(lexem)); break;

            case CONSTANT:   printf(": %c / %lf\n",TYPE_OF(lexem), VALUE_OF(lexem)); break;

        }   

    }
}

LexemsList* LexAnalysis(char* code_buffer) {
    
    char* caret               = code_buffer;
    char* prev_caret_position = caret;

    Lexem*  current_lexem = nullptr; 
    
    LexemsList* lexem_list  = (LexemsList*)calloc(1, sizeof(LexemsList));

    LEXEMS       = (Lexem**)calloc(1, sizeof(Lexem*));
    LEXLIST_SIZE     = 0;  
    FINAL_STATUS = LEX_ANALYSIS_OK;

    LexAnalysis_Status LexStatus = GetLexem(&current_lexem, &caret);

    for (; LexStatus != END_OF_CODE && LEXLIST_SIZE < 1000; LexStatus = GetLexem(&current_lexem, &caret)) { 
        
        PrintLexem(current_lexem, LEXLIST_SIZE, LexStatus, prev_caret_position - code_buffer);

        if (current_lexem && LexStatus == TOKEN_OK) {
            AddLexem(lexem_list, current_lexem);
        }

        else{
            printf("\n");
            FINAL_STATUS = LEX_ANALYSIS_FAILED;
        }

        prev_caret_position = caret;
        
    }

    AddLexem(lexem_list, CreateEmptyLexem());

    return lexem_list;
}

int IsPossibleIdentifierChar(char chr) {
    return isalpha(chr) || isdigit(chr) || chr == '_';
}

LexAnalysis_Status GetIdentifier(char** caret, char** name_ptr) {

    if (!caret)    return BAD_CARET_POINTER;
    if (!(*caret)) return BAD_CARET_POSITION;
    if (!name_ptr) return BAD_NAME_PTR;

    if (!isalpha(**caret) && (**caret) != '\\' && (**caret) != '_') return BAD_LEXEM;

    size_t identifier_size = 1;

    for(; IsPossibleIdentifierChar((*caret)[identifier_size]); identifier_size++);

    char* newName = (char*)calloc(identifier_size + 1, sizeof(char));

    if(!newName) return OUT_OF_MEM;

    strncpy(newName, *caret, identifier_size);
    newName[identifier_size] = '\0';

    *name_ptr = newName;
    *caret   += identifier_size;

    return TOKEN_OK;
}

CompareResult CompareWithIdentifier(char* caret, const char* inline_string) {

    if (!caret)         return BAD_FIRST_PTR;
    if (!inline_string) return BAD_SECOND_PTR;

    int n_char = 0;

    for (; caret[n_char] && inline_string[n_char] ; n_char++) {
        if (caret[n_char] != inline_string[n_char]) return DIFFERENT;
    }

    if (inline_string[n_char] == '\0' && (!IsPossibleIdentifierChar(caret[n_char]))) return MATCH;
    else                                                                             return DIFFERENT;
}

CompareResult CompareWithOperator(char* caret, const char* inline_string) {

    if (!caret)         return BAD_FIRST_PTR;
    if (!inline_string) return BAD_SECOND_PTR;

    int n_char = 0;

    for (; caret[n_char] == inline_string[n_char] && inline_string[n_char] && (caret[n_char]); n_char++);

    if (inline_string[n_char] == '\0') return MATCH;
    else                               return DIFFERENT;
}

void MoveCaretNextLexem(char** caret) {
    for (; isspace(**caret) && (**caret) != '\0'; (*caret)++);
}

double FoldConstant(char** caret) {

    double val = 0;

    for (;isdigit(**caret); (*caret)++) {
        val = val*10 + **caret - '0';
    }
    
    if (**caret == '.') {

        int n_digits = 1;

        (*caret)++;
        for (;isdigit(**caret); (*caret)++) {
            val += (**caret - '0') * pow(10, -(n_digits));
            n_digits++;
        }        

    }
    return val;
}

LexAnalysis_Status GetLexem(Lexem** lexem_ptr, char** caret) {

    if (!lexem_ptr)         return BAD_LEXEM_POINTER;
    if (!caret)             return BAD_CARET_POINTER;
    if (!(*caret))          return BAD_CARET_POSITION;
    if (**caret == '\0')    return END_OF_CODE;

    char* Identifier_name = nullptr;

    #define LEXEM(ID, inline_string)                                                    \
        else if (CompareWithOperator(*caret, inline_string) == MATCH) {                 \
            *lexem_ptr = CreateOperatorLexem(ID);                                       \
            (*caret)+=strlen(inline_string);                                            \
            if (!(*lexem_ptr)) return OUT_OF_MEM;                                       \
        }                                                                               \

    if (isdigit(**caret)) {
        *lexem_ptr = CreateConstantLexem(FoldConstant(caret));
        if (!(*lexem_ptr)) return OUT_OF_MEM;
    }
    
    #include "LexemList.h"

    else if (GetIdentifier(caret, &Identifier_name) != BAD_LEXEM) {
        *lexem_ptr = CreateIdentifierLexem(Identifier_name);
        if (!(*lexem_ptr)) return OUT_OF_MEM;
    }

    else{
        for(;!isspace(**caret) && **caret != '\0'; (*caret)++);
        MoveCaretNextLexem(caret);    
        return BAD_LEXEM;
    }

    #undef LEXEM

    MoveCaretNextLexem(caret);
    return TOKEN_OK;
}

const char* GetLexAnalysisStatus(LexAnalysis_Status current_result) {

    #define CASE_RESULT(result) case result: return #result;

    switch(current_result) {
    CASE_RESULT(TOKEN_OK)
    CASE_RESULT(END_OF_CODE)
    CASE_RESULT(BAD_CARET_POINTER)
    CASE_RESULT(BAD_CARET_POSITION)
    CASE_RESULT(BAD_LEXEM_POINTER)
    CASE_RESULT(OUT_OF_MEM)
    CASE_RESULT(BAD_LEXEM)
    default: return "Unknown Error";
    }

    #undef CASE_RESULT
}

Lexem* CreateEmptyLexem() {

    Lexem* newLexem = (Lexem*)calloc(1,sizeof(Lexem));
    if (!newLexem) return nullptr;

    TYPE_OF (newLexem) = EMPTY;

    return newLexem;

}

Lexem* CreateOperatorLexem(Operator_ID ID) {
    
    Lexem* newLexem = (Lexem*)calloc(1,sizeof(Lexem));

    if (!newLexem) return nullptr;

    TYPE_OF (newLexem) = OPERATOR;
    ID_OF(newLexem)   = ID;

    return newLexem;
}

Lexem* CreateConstantLexem(double value) {
    
    Lexem* newLexem = (Lexem*)calloc(1,sizeof(Lexem));

    if (!newLexem) return nullptr;

    TYPE_OF (newLexem) = CONSTANT;
    VALUE_OF(newLexem) = value;

    return newLexem;
}

Lexem* CreateIdentifierLexem(char* idr_name) {
    
    Lexem* newLexem = (Lexem*)calloc(1,sizeof(Lexem));

    if (!newLexem || !idr_name) return nullptr;

    TYPE_OF    (newLexem) = IDENTIFIER;
    IDR_NAME_OF(newLexem) = idr_name;

    return newLexem;
}

Lexem_String GetOperatorString(Operator_ID current_id) {
    
    #define LEXEM(ID, keyword) case ID: return keyword;

    switch(current_id) {
        #include "LexemList.h"
        default: return UNKNOWN_LEXEM_STRING;
    }

    #undef LEXEM
}

Lexem_String GetOperatorName(Operator_ID current_id) {
    
    #define LEXEM(ID, keyword) case ID: return #ID;

    switch(current_id) {
        #include "LexemList.h"
        default: return UNKNOWN_LEXEM_STRING;
    }

    #undef LEXEM
}



/*
#define AST_CHECK                                   \
    AST_Error check_result = ValidateTree(tree);    \
    if(check_result != AST_OK) return check_result; \
*/

#define READ_IF_NOT_ALREADY(node_name, interm_func)      \
    if (!node_name) node_name = interm_func(); \



LexemsList* lexems_list = nullptr;
int    n_lexem     = 0;


AST_Node* CreateOperatorNode(AST_Operator_ID ID, AST_Node* left, AST_Node* right) {
    
    AST_Node* newNode = (AST_Node*)calloc(1,sizeof(AST_Node));

    if (!newNode) return nullptr;

    TYPE_OF(newNode) = OPERATOR;
    ID_OF(newNode)   = ID;

    newNode -> left  = left;
    newNode -> right = right;

    return newNode;
}

AST_Node* CreateConstantNode(double value, AST_Node* left, AST_Node* right) {
    
    AST_Node* newNode = (AST_Node*)calloc(1,sizeof(AST_Node));

    if (!newNode) return nullptr;

    TYPE_OF (newNode) = CONSTANT;
    VALUE_OF(newNode) = value;

    newNode -> left  = left;
    newNode -> right = right;

    return newNode;
}

AST_Node* CreateIdentifierNode(char* idr_name, AST_Node* left, AST_Node* right) {
    
    AST_Node* newNode = (AST_Node*)calloc(1,sizeof(AST_Node));

    if (!newNode || !idr_name) return nullptr;

    TYPE_OF    (newNode) = IDENTIFIER;
    IDR_NAME_OF(newNode) = idr_name;

    newNode -> left  = left;
    newNode -> right = right;

    return newNode;
}

void RequireOperator(Operator_ID operator_id) {
    if (LexemIsOperator(n_lexem, operator_id)) n_lexem++;
    else {printf("Expected %s, got %c type: ",GetOperatorString(operator_id), LEXEM_TYPE(n_lexem)); SyntaxError();}
}

int LexemIsOperator(int lexem_number, Operator_ID operator_id) {
    return (LEXEM_TYPE(lexem_number) == OPERATOR && LEXEM_ID(lexem_number) == operator_id);
}

int CheckEndOfCode(int lexem_number) {
    return (LEXEM_TYPE(lexem_number) == EMPTY);
}

void SyntaxError() {
    
    printf("Syntax error on #%d lexem\n", n_lexem);
    exit(0);
}

AST_Node* GetDec() {

    if(LexemIsOperator(n_lexem, LBRACKET_SQUARE)) { 
        
        n_lexem++;

        if (LEXEM_TYPE(n_lexem) == IDENTIFIER) {
            
            char* func_name = LEXEM_IDR_NAME(n_lexem);
            
            n_lexem++;
            
            RequireOperator(LBRACKET_ROUND);
            
            AST_Node* vars = GetDeclVarEnum();
            
            RequireOperator(RBRACKET_ROUND);
            RequireOperator(RBRACKET_SQUARE);
            
            RequireOperator(EQDEF);

            RequireOperator(LBRACKET_SQUARE);

            AST_Node* code = GetInnerFunctionCode();

            RequireOperator(RBRACKET_SQUARE);

            AST_Node* func_implementation = CreateIdentifierNode(func_name, vars, code);

            return CreateOperatorNode(AST_FUNC_DEC, func_implementation, GetDec());
            
        }

        else return nullptr;

    }

    else return nullptr;
}

AST_Node* GetDeclVarEnum() {
    
    AST_Node* arg      = GetVar();

    if (LexemIsOperator(n_lexem, COMMA)) {

        n_lexem++;
        
        arg = CreateOperatorNode(AST_FUNC_IMP_PARAM, GetDeclVarEnum(), arg);

    }

    return arg;
}

AST_Node* GetCondition() {

    if (LexemIsOperator(n_lexem, LBRACKET_SQUARE) && !LexemIsOperator(n_lexem+1, FORALL)) {
        
        n_lexem++;

        AST_Node* condition = GetLogic();
        
        RequireOperator(RBRACKET_SQUARE);
        RequireOperator (CONDITION_RIGHT_ARROW);
        RequireOperator(LBRACKET_SQUARE);

        AST_Node* inner_code = GetInnerFunctionCode();
        RequireOperator(RBRACKET_SQUARE);

        return CreateOperatorNode(AST_CONDITION, condition, inner_code);
    }

    else return nullptr;
}

AST_Node* GetLoop() {

    if (LexemIsOperator(n_lexem, LBRACKET_SQUARE) && LexemIsOperator(n_lexem+1, FORALL)) {
        
        n_lexem+=2;

        AST_Node* condition = GetLogic();
        
        RequireOperator(RBRACKET_SQUARE);
        RequireOperator (CONDITION_RIGHT_ARROW);
        RequireOperator(LBRACKET_SQUARE);

        AST_Node* inner_code = GetInnerFunctionCode();
        RequireOperator(RBRACKET_SQUARE);

        return CreateOperatorNode(AST_LOOP, condition, inner_code);
    }

    else return nullptr;
}

AST_Node* GetKeyword() {
    
    #define KEYWORD(id)                                             \
    if (LexemIsOperator(n_lexem, id)) {                             \
        n_lexem++;                                                  \
        return CreateOperatorNode(AST_ ## id, GetLogic(), nullptr); \
    }                                                               \

    KEYWORD(RETURN)
    KEYWORD(PRINT)
    KEYWORD(DOT)
    
    if (LexemIsOperator(n_lexem, SHOW)) {                             
        n_lexem++;                                                    
        return CreateOperatorNode(AST_SHOW, nullptr, nullptr);        
    }

    else return nullptr;
}

AST_Node* GetVarInit() {
    
    if (LexemIsOperator(n_lexem, VAR_INIT)) {

        n_lexem++;
        
        AST_Node* new_var = GetVar();

        RequireOperator(EQ);

        
        return CreateOperatorNode(AST_VAR_ASGN, new_var, GetLogic());
    }

    else return nullptr;

}

AST_Node* GetVarClaim() {

    AST_Node* dest_var = GetVar();
    if (!dest_var) return nullptr;

    RequireOperator(EQ);
    
    return CreateOperatorNode(AST_VAR_ASGN, dest_var, GetLogic());

}

AST_Node* GetN() {
    
    if (LEXEM_TYPE(n_lexem) == CONSTANT) {
        n_lexem++;
        
        return CreateConstantNode(LEXEM_VAL(n_lexem - 1), nullptr, nullptr);
    }
    else return nullptr;
}

AST_Node* GetLogic() {

    AST_Node* lvalue = GetE(); //T1
    AST_Node* rvalue = nullptr;

    int current_operator = n_lexem;

    while (LexemIsOperator(n_lexem, GR_THEN) || LexemIsOperator(n_lexem, LR_THEN) || LexemIsOperator(n_lexem, EQ)) {

        current_operator = n_lexem;
        n_lexem++;
        rvalue = GetE();

        if      (LEXEM_ID(current_operator) == GR_THEN) lvalue = CreateOperatorNode(AST_GR_THEN, lvalue, rvalue);
        else if (LEXEM_ID(current_operator) == LR_THEN) lvalue = CreateOperatorNode(AST_LR_THEN, lvalue, rvalue);
        else                                            lvalue = CreateOperatorNode(AST_EQ, lvalue, rvalue);

        

        // -> root = lvalue; // TEMP
    }

    return lvalue;
}

AST_Node* GetE() {

    AST_Node* lvalue = GetT1(); //T1
    AST_Node* rvalue = nullptr;

    int current_operator = n_lexem;

    while (LexemIsOperator(n_lexem, ADD) || LexemIsOperator(n_lexem, SUB)) {

        current_operator = n_lexem;
        n_lexem++;
        rvalue = GetT1();

        if   (LEXEM_ID(current_operator) == ADD) lvalue = CreateOperatorNode(AST_ADD, lvalue, rvalue);
        else                                     lvalue = CreateOperatorNode(AST_SUB, lvalue, rvalue);

        

        // -> root = lvalue; // TEMP
    }

    return lvalue;
}

AST_Node* GetT1() {

    AST_Node* lvalue = GetMod(); //T1
    AST_Node* rvalue = nullptr;

    int current_operator = n_lexem;

    while (LexemIsOperator(n_lexem, MUL) || LexemIsOperator(n_lexem, DIV)) {

        current_operator = n_lexem;
        n_lexem++;
        rvalue = GetMod();

        if   (LEXEM_ID(current_operator) == MUL) lvalue = CreateOperatorNode(AST_MUL, lvalue, rvalue);
        else                                     lvalue = CreateOperatorNode(AST_DIV, lvalue, rvalue);

        

        // -> root = lvalue; // TEMP
    }

    return lvalue;
}

AST_Node* GetMod() {

    AST_Node* lvalue = GetT2(); //T1
    AST_Node* rvalue = nullptr;

    while (LexemIsOperator(n_lexem, MOD)) {

        n_lexem++;
        rvalue = GetT2();

        lvalue = CreateOperatorNode(AST_MOD, rvalue, lvalue);
    }

    return lvalue;
}

AST_Node* GetT2() {

    AST_Node* result_node = nullptr;
    
    READ_IF_NOT_ALREADY(result_node, GetVar)
    READ_IF_NOT_ALREADY(result_node, GetP)
    READ_IF_NOT_ALREADY(result_node, GetN)
    READ_IF_NOT_ALREADY(result_node, GetCall)
    READ_IF_NOT_ALREADY(result_node, GetInput)
    
    return result_node;
}

AST_Node* GetVar() {
    
    if (LEXEM_TYPE(n_lexem) == IDENTIFIER && !(LexemIsOperator(n_lexem+1, LBRACKET_ROUND))) {
        
        char* var_name = LEXEM_IDR_NAME(n_lexem);
        
        n_lexem++;
        
        return CreateIdentifierNode(var_name, nullptr, nullptr);
    }
    else return nullptr;
}

AST_Node* GetInput(){

    if (LexemIsOperator(n_lexem, INPUT)){
        n_lexem++;
        return CreateOperatorNode(AST_INPUT, nullptr, nullptr);
    }
    else return nullptr;
}

AST_Node* GetP() {

    AST_Node* subtree = nullptr;

    if (LEXEM_TYPE(n_lexem) == OPERATOR && LEXEM_ID(n_lexem) == LBRACKET_ROUND) {
        
        n_lexem++;
        subtree = GetLogic();
        RequireOperator(RBRACKET_ROUND);
        return subtree;
    }

    return nullptr;
}

AST_Node* GetCall() {

    if (LEXEM_TYPE(n_lexem) == IDENTIFIER && LexemIsOperator(n_lexem+1, LBRACKET_ROUND)) {
        
        char* func_name = LEXEM_IDR_NAME(n_lexem);
        
        n_lexem+=2;
        
        AST_Node* subtree = GetEnum();

        RequireOperator(RBRACKET_ROUND);
        
        subtree = CreateIdentifierNode(func_name, subtree, nullptr); 

        return CreateOperatorNode(AST_FUNC_CALL, subtree, nullptr); 

    }

    else return nullptr;
}

AST_Node* GetEnum() {

    AST_Node* arg = GetLogic(); //T1

    if (LexemIsOperator(n_lexem, COMMA)) {

        n_lexem++;
        arg = CreateOperatorNode(AST_FUNC_CALL_PARAM,  GetEnum(), arg);
    
    }

    return arg;
}

AST_Node* GetCodeline() {
    
    AST_Node* result_line = nullptr;
    
    READ_IF_NOT_ALREADY(result_line, GetVarInit)
    READ_IF_NOT_ALREADY(result_line, GetVarClaim)
    READ_IF_NOT_ALREADY(result_line, GetCall)
    READ_IF_NOT_ALREADY(result_line, GetKeyword)
    READ_IF_NOT_ALREADY(result_line, GetCondition)
    READ_IF_NOT_ALREADY(result_line, GetLoop)
    
    RequireOperator(LR);

    return result_line;
}

AST_Node* GetInnerFunctionCode() {

    AST_Node* code = GetCodeline();

    if (code && !LexemIsOperator(n_lexem, RBRACKET_SQUARE)) {
        code = CreateOperatorNode(AST_CONCAT, code, GetInnerFunctionCode());
    }

    return code;
}

AST_Node* GetG(LexemsList* dest_lexems_list) {
    
    lexems_list = dest_lexems_list;
    n_lexem = 0;

    

    return GetDec();
}

Lexem_String AST_GetOperatorString(AST_Operator_ID current_id) {
    
    #define AST_OPERATOR(ID, keyword) case AST_ ## ID: return keyword;

    switch(current_id) {
        #include "ASTOperatorsList.h"
        default: return "UNKNOWN_NODE";
    }

    #undef AST_OPERATOR
}

Lexem_String AST_GetOperatorName(AST_Operator_ID current_id) {
    
    #define AST_OPERATOR(ID, keyword) case AST_ ## ID: return #ID;

    switch(current_id) {
        #include "ASTOperatorsList.h"
        default: return "UNKNOWN_NODE";
    }

    #undef AST_OPERATOR
}

AST_Error DumpNode(FILE* fp, AST_Node* node) {

    if (!node) return INVALID_NODE_POINTER;
    if (!fp)   return INVALID_FILE_POINTER;

    switch(TYPE_OF(node)) {

        case CONSTANT:   fprintf(fp, "%ld [shape=record, fillcolor=green rank = same style=filled label=\"    {%lf} \" ];\n",
                                node, VALUE_OF(node)); break;
                    
        case OPERATOR:   fprintf(fp, "%ld [shape=record, fillcolor=lightgray rank = same style=filled label=\"    {%s}}\" ];\n",
                                node, AST_GetOperatorName(ID_OF(node))); break;

        case IDENTIFIER: fprintf(fp, "%ld [shape=record, fillcolor=aquamarine rank = same style=filled label=\"    {%s} \" ];\n",
                                node, IDR_NAME_OF(node)); break;
        

    }
    
    if(node -> left)  fprintf(fp, "%ld -> %ld [color=black]\n", node, node -> left);  DumpNode(fp, node -> left);
    if(node -> right) fprintf(fp, "%ld -> %ld [color=black]\n", node, node -> right); DumpNode(fp, node -> right);
    
    return AST_OK;
}

AST_Error GraphicalDump(Tree* tree) {

    if (!tree) return INVALID_TREE_POINTER;

    FILE* fp = fopen("show", "w");

    if (!fp) return FILE_OPEN_ERROR;

    fprintf(fp, "digraph G {\nrankdir=TD;\n");
    
    DumpNode(fp, tree -> root);
    
    fprintf(fp, "}");
    fclose(fp);
    system("dot -Tpng show -n -o show.png");
    system("viewnior show.png");

    return AST_OK;
}

AST_Error ValidateTree(Tree* tree) {
    
    if (!tree)            return INVALID_TREE_POINTER;
    if (tree -> size < 0) return AST_SIZE_LESS_ZERO;

    int nodes_counter = 0;

    return ValidateNodeRecursively(tree, tree -> root, &nodes_counter);
}

AST_Error ValidateNodeRecursively(Tree* tree, AST_Node* node, int* nodes_counter) {
    
    if (!node) return AST_OK;

    *nodes_counter += 1;

    if (node -> left && node -> left == node -> right) return SAME_LEFT_AND_RIGHT_CHILDS;
    if (node -> left == node || node -> right == node)    return NODES_LOOPED;
    //if (*nodes_counter > tree -> size)                    return TOO_MANY_NODES_FOR_CURRENT_SIZE;

    AST_Error child_check_result = AST_OK;

    child_check_result = ValidateNodeRecursively(tree, node -> left, nodes_counter);
    
    if (child_check_result != AST_OK) return child_check_result;
    
    return ValidateNodeRecursively(tree, node -> right, nodes_counter);
}

#define POINTER_ERROR   printf("%s pointer error\n", __FUNCTION__)
#define NODE_TYPE_ERROR printf("%s node_type error\n", __FUNCTION__)

#define CHECK_NODE_OPER(oper) if (!NodeIsOperator(node, oper)) {NODE_TYPE_ERROR; return;}
#define CHECK_POINTERS        if (!fp || !name_table || !node) {printf("fp {%p} | NT {%p | node {%p}: ", fp, name_table, node); POINTER_ERROR; return;}

int NodeIsOperator(AST_Node* node, AST_Operator_ID operator_id) {
    return TYPE_OF(node) == OPERATOR && ID_OF(node) == operator_id;
}

void GenerateASM(Tree* tree, char* filename) {

    FILE* fp = fopen(filename, "w");
    if (!fp) {printf("Failed to write the file"); return;}
    
    fprintf(fp, "push 0\ncall func_main\nhlt\n\n");

    GenerateDec(fp, tree -> root);
    fclose(fp);

}

int NameTableAddVar(VarNameTable* name_table, char* var_name) {

    if (!name_table || !var_name) {POINTER_ERROR; return -1;}
    
    printf("Adding var %s:\n", var_name);

    if (name_table -> size == name_table -> capacity) {
        
        Variable* resized = (Variable*)realloc(name_table -> vars, 2 * (name_table -> capacity) * sizeof(Variable));
        if (!resized) return -1;

        name_table -> vars        = resized;
        (name_table -> capacity) *= 2; 
    }

    #define NEW_VAR ((name_table -> vars)[name_table -> size]) 

    int new_offset = (name_table -> size); 

    NEW_VAR.offset = new_offset;
    NEW_VAR.name   = var_name;

    (name_table -> size)++;

    NameTableDump(name_table);

    return new_offset;

    #undef NEW_VAR
}

int NameTableCheckVar(VarNameTable* name_table, char* var_name) {

    if (!name_table || !var_name) {POINTER_ERROR; return -1;}

    //printf("Finding var %s:\n", var_name);

    for (size_t n_var = 0; n_var < name_table -> size; n_var++) {
        if (!strcmp(var_name, (name_table -> vars)[n_var].name)) return (name_table -> vars)[n_var].offset; 
    }

    //NameTableDump(name_table);

    return -1;
}

int NameTableProcessVar(VarNameTable* name_table, char* var_name) {

    int var_offset = NameTableCheckVar(name_table, var_name);
    
    if (var_offset == -1) {
        var_offset = NameTableAddVar(name_table, var_name);
    }

    return var_offset;
}

void NameTableDump(VarNameTable* name_table){

    printf("NameTable (%d): {\n", name_table -> capacity);

    Variable current_var = {0};

    for (size_t n_var = 0; n_var < name_table -> size; n_var++){
        
        current_var = (name_table -> vars)[n_var];
        printf("  %s : %d,\n", current_var.name, current_var.offset);
    }

    printf("}\n");
}

void GenerateDec(FILE* fp, AST_Node* node) {
    
    if (!fp || !node) POINTER_ERROR;

    CHECK_NODE_OPER(AST_FUNC_DEC);
    
    VarNameTable* name_table = (VarNameTable*)calloc(1, sizeof(VarNameTable));

    name_table -> capacity = INITIAL_NAMETABLE_CAPACITY;
    name_table -> size = 0;
    name_table -> vars = (Variable*)calloc(INITIAL_NAMETABLE_CAPACITY, sizeof(Variable));

    AST_Node* func = node -> left;

    fprintf(fp, "func_%s:\n\n", IDR_NAME_OF(func));

    //fprintf(fp, "pop rbx\n");

    if (func -> left) GenerateVarImp(fp, name_table, func -> left);

    //fprintf(fp, "push [rbx]\n");

    GenerateStatement(fp, name_table, func -> right);

    fprintf(fp, "pop rbx\n");
    fprintf(fp, "return\n");

    

    if (node -> right) GenerateDec(fp, node -> right);
    
}

void GenerateStatement(FILE* fp, VarNameTable* name_table, AST_Node* node) {
    
    CHECK_POINTERS;

    if (TYPE_OF(node) != OPERATOR) {NODE_TYPE_ERROR; return;}
    
    switch (ID_OF(node)) {

        case AST_CONCAT:    GenerateStatement(fp, name_table, node -> left);
                            GenerateStatement(fp, name_table, node -> right);
                            break;
        
        case AST_CONDITION: GenerateCondition(fp, name_table, node);
                            break;

        case AST_LOOP:      GenerateLoop(fp, name_table, node);
                            break;
        
        default:            GenerateLineOperator(fp, name_table, node); break; 

    }
    fprintf(fp, "\n");
}

void GenerateLineOperator(FILE* fp, VarNameTable* name_table, AST_Node* node) {
    
    CHECK_POINTERS;

    if (TYPE_OF(node) != OPERATOR) {NODE_TYPE_ERROR; return;}

    switch (ID_OF(node)) {

        case AST_VAR_ASGN:  GenerateAssign(fp, name_table, node); break;
        case AST_FUNC_CALL: GenerateCall(fp, name_table, node);   break;
        case AST_RETURN:    GenerateReturn(fp, name_table, node); break;
        case AST_PRINT:     GeneratePrint(fp, name_table, node);  break;
        case AST_DOT:       GenerateDot(fp, name_table, node);    break;
        case AST_SHOW:      fprintf(fp, "show\n");                break;
        default:            NODE_TYPE_ERROR;                      break;
    }

    fprintf(fp, "\n");
}

void GenerateAssign(FILE* fp, VarNameTable* name_table, AST_Node* node) {

    CHECK_POINTERS;
    CHECK_NODE_OPER(AST_VAR_ASGN);
    GenerateExpression(fp, name_table, node -> right);

    int var_offset = NameTableProcessVar(name_table, IDR_NAME_OF(node -> left));
    if (var_offset == -1) {printf("Failed to process var %s\n", IDR_NAME_OF(node -> left)); return;}

    fprintf(fp, "# %s =\n", IDR_NAME_OF(node -> left));
    fprintf(fp, "pop [ rbx + %d ]\n", var_offset);

}

void GenerateExpression(FILE* fp, VarNameTable* name_table, AST_Node* node) {

    CHECK_POINTERS;

    switch(TYPE_OF(node)) {

        case OPERATOR:   if (ID_OF(node) == AST_FUNC_CALL) {
                            GenerateCall(fp, name_table, node);
                         }
                         else if (ID_OF(node) == AST_INPUT) {
                             fprintf(fp, "in\n");
                         }
                         else {
                            GenerateExpression(fp, name_table, node -> right);
                            GenerateExpression(fp, name_table, node -> left);
                            GenerateMathOperator(fp, name_table, node);
                         }
                         break;
        
        case IDENTIFIER: GenerateVarValue(fp, name_table, node);     break;
        case CONSTANT:   fprintf(fp, "push %lf\n", VALUE_OF(node));  break;
        default:         NODE_TYPE_ERROR;                            break;
    }
}

void GenerateVarValue(FILE* fp, VarNameTable* name_table, AST_Node* node) {

    CHECK_POINTERS
    
    int var_offset = NameTableCheckVar(name_table, IDR_NAME_OF(node));

    if(var_offset == -1) {printf("Variable %s is not defined\n", IDR_NAME_OF(node)); return;}

    fprintf(fp, "# %s\npush [ rbx + %d ]\n", IDR_NAME_OF(node), var_offset);

}

void GenerateMathOperator(FILE* fp, VarNameTable* name_table, AST_Node* node) {

    CHECK_POINTERS

    #define COMMAND(name) fprintf(fp, "%s\n", name);

    switch(ID_OF(node)) {

        case AST_ADD:     COMMAND("add"); break;
        case AST_SUB:     COMMAND("sub"); break;
        case AST_MUL:     COMMAND("mul"); break;
        case AST_MOD:     COMMAND("mod"); break;
        case AST_DIV:     COMMAND("div"); break;
        case AST_EQ:      COMMAND("ce") ; break;
        case AST_GR_THEN: COMMAND("ca") ; break;
        case AST_LR_THEN: COMMAND("cb") ; break;

        default: printf("UNKNOWN EXPRESSION OPERATOR\n");
    }
}

void GenerateVarImp(FILE* fp, VarNameTable* name_table, AST_Node* node) {

    CHECK_POINTERS;

    if (TYPE_OF(node) == IDENTIFIER) {
        int new_var_offset = NameTableAddVar(name_table, IDR_NAME_OF(node));
        if(new_var_offset != -1) fprintf(fp, "# %s\npop [ rbx + %d ]\n", IDR_NAME_OF(node), new_var_offset);
        else printf("Failed adding param %s\n", IDR_NAME_OF(node));
    }
    
    else {
        CHECK_NODE_OPER(AST_FUNC_IMP_PARAM);
        GenerateVarImp(fp, name_table, node -> left);
        GenerateVarImp(fp, name_table, node -> right);
    }
} 

void GenerateReturn(FILE* fp, VarNameTable* name_table, AST_Node* node) {

    CHECK_POINTERS;
    CHECK_NODE_OPER(AST_RETURN);

    if (node -> left){
        GenerateExpression(fp, name_table, node -> left);
        fprintf(fp, "pop rax\n");
        fprintf(fp, "pop rbx\n");
        fprintf(fp, "push rax\n");
    }
    else{
        fprintf(fp, "pop rbx\n");
    }
    fprintf(fp, "return\n");
}

void GeneratePrint(FILE* fp, VarNameTable* name_table, AST_Node* node){
    
    CHECK_POINTERS;
    CHECK_NODE_OPER(AST_PRINT);

    GenerateExpression(fp, name_table, node -> left);
    fprintf(fp, "out\n");
}

void GenerateDot(FILE* fp, VarNameTable* name_table, AST_Node* node) {
    
    CHECK_POINTERS;
    CHECK_NODE_OPER(AST_DOT);

    GenerateExpression(fp, name_table, node -> left);
    fprintf(fp, "draw\n");
}

void GenerateCall(FILE* fp, VarNameTable* name_table, AST_Node* node) {
    
    CHECK_POINTERS;

    node = node -> left;
    

    if (!node) {printf("EMPTY CALL\n"); return;}
    if (TYPE_OF(node) != IDENTIFIER) NODE_TYPE_ERROR;

    /*
    FILE* dump_fp = fopen("call_param.test", "w");
    DumpNode(dump_fp, node); 
    system("viewnior show.png");
    fclose(dump_fp);
    */

    fprintf(fp, "\n# prev bp\npush rbx\n");

    if (node -> left) GenerateFuncCallParams(fp, name_table, node -> left);

    fprintf(fp, "push rbx\n");
    fprintf(fp, "push %d\n", name_table -> size);
    fprintf(fp, "add\n");
    fprintf(fp, "pop rbx\n");
    fprintf(fp, "call func_%s\n", IDR_NAME_OF(node));
}

void GenerateFuncCallParams(FILE* fp, VarNameTable* name_table, AST_Node* node){
    
    CHECK_POINTERS;

    if (NodeIsOperator(node, AST_FUNC_CALL_PARAM)) {
        GenerateFuncCallParams(fp, name_table, node -> right);
        GenerateFuncCallParams(fp, name_table, node -> left);
    }

    else {
        GenerateExpression(fp, name_table, node);
    }
}


void GenerateCondition(FILE* fp, VarNameTable* name_table, AST_Node* node) {

    CHECK_POINTERS;
    
    GenerateExpression(fp, name_table, node -> left);
    fprintf(fp, "push 0\n");
    fprintf(fp, "je endif_%p\n", node);
    
    size_t out_statement_n_vars = name_table -> size;
    GenerateStatement(fp, name_table, node -> right);    
    
    name_table -> size = out_statement_n_vars;
    fprintf(fp, "endif_%p:\n", node);

}

void GenerateLoop(FILE* fp, VarNameTable* name_table, AST_Node* node) {

    CHECK_POINTERS;
    
    fprintf(fp, "loop_%p:\n", node);
    GenerateExpression(fp, name_table, node -> left);
    fprintf(fp, "push 0\n");
    fprintf(fp, "je endif_%p\n", node);
    
    size_t out_statement_n_vars = name_table -> size;
    GenerateStatement(fp, name_table, node -> right);    
    
    name_table -> size = out_statement_n_vars;
    fprintf(fp, "jmp loop_%p\n", node);
    fprintf(fp, "endif_%p:\n", node);

}