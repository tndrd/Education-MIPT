#include "AST.h"

//TODO add an opportunity to end tokenisation of lexem on operator (remove CompareStringsUpToChar and update MoveCaretNextLexem)

//Constant Folding


int main(){
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
    
    if (lexed -> status == LEX_ANALYSIS_OK){
        printf("Lexical analysis complete, showing diagram...\n");
        LexGraphicalDump(lexed);

        Tree* tree = (Tree*)calloc(1,sizeof(Tree));
        tree -> root = GetG(lexed, tree);
        GraphicalDump(tree);
    }
    
    return 0;
}

void LexGraphicalDump(LexemsList* lexems_list){

    FILE* fp = fopen("show", "w");   
    
    fprintf(fp, "digraph structs {\nrankdir=LR rank=same;\n");
    
    for(size_t n_lexem = 0; n_lexem < (lexems_list -> size) - 1; n_lexem++){

        switch(LEXEM_TYPE(n_lexem)){

            case OPERATOR:   fprintf(fp, "%d [shape=record, fillcolor=yellow rank = same style=filled label=\"    %s | {  OPER | %u }\" ];\n",
                                   n_lexem, GetLexemID_Name(LEXEM_ID(n_lexem)), LEXEM_ID(n_lexem)); break;
            case KEYWORD:    fprintf(fp, "%d [shape=record, fillcolor=pink rank = same style=filled label=\"    %s | {  KWRD | %u }\" ];\n",
                                   n_lexem, GetLexemID_Name(LEXEM_ID(n_lexem)), LEXEM_ID(n_lexem)); break;
            case CONSTANT:   fprintf(fp, "%d [shape=record, fillcolor=green rank = same style=filled label=\"    %lf | { CONSTANT }\" ];\n",
                                   n_lexem, LEXEM_VAL(n_lexem)); break;
            case IDENTIFIER: fprintf(fp, "%d [shape=record, fillcolor=aquamarine rank = same style=filled label=\"    %s | {IDER | %u}\" ];\n",
                                   n_lexem, IDR_NAME(n_lexem), LEXEM_ID(n_lexem)); break;
        }

    }
    for(size_t n_lexem = 0; n_lexem < (lexems_list -> size) - 1; n_lexem++) fprintf(fp, "%d -> %d\n", n_lexem, n_lexem+1);
    fprintf(fp, "}");
    fclose(fp);
    
    system("dot -Tpng show -n -o show.png");
    system("viewnior show.png");
}


    #define LEXEMS       (lexem_list -> lexems)
    #define LEXLIST_SIZE     (lexem_list -> size) 
    #define FINAL_STATUS (lexem_list -> status)

void AddLexem(LexemsList* lexem_list, Lexem* lexem){
    
    LEXEMS = (Lexem**)realloc(LEXEMS, (LEXLIST_SIZE+1)*sizeof(Lexem*));
    LEXEMS[LEXLIST_SIZE] = lexem;    
    LEXLIST_SIZE++;
}


LexemsList* LexAnalysis(char* code_buffer){
    
    char* caret               = code_buffer;
    char* prev_caret_position = caret;

    NameTable* name_table = CreateNameTable();
    Lexem*  current_lexem = nullptr; 
    
    LexemsList* lexem_list  = (LexemsList*)calloc(1, sizeof(LexemsList));

    LEXEMS       = (Lexem**)calloc(1, sizeof(Lexem*));
    LEXLIST_SIZE     = 0;  
    FINAL_STATUS = LEX_ANALYSIS_OK;

    LexAnalysis_Status LexStatus = GetLexem(name_table, &current_lexem, &caret);

    for (; LexStatus != END_OF_CODE && LEXLIST_SIZE < 50; LexStatus = GetLexem(name_table, &current_lexem, &caret)){ 
        
        printf("#%d %s {%p} at %d ",LEXLIST_SIZE, GetLexAnalysisStatus(LexStatus), current_lexem, prev_caret_position - code_buffer);
        
        if (current_lexem && LexStatus == TOKEN_OK){

            switch(TYPE_OF(current_lexem)){
                
                case OPERATOR: case KEYWORD: printf(": %c / %s\n", TYPE_OF(current_lexem), GetLexemString(ID_OF(current_lexem))); break;

                case IDENTIFIER:             printf(": %c / %d / %s\n", TYPE_OF(current_lexem), ID_OF(current_lexem), (name_table -> identifiers)[ID_OF(current_lexem)].name); break;

                case CONSTANT:               printf(": %c / %lf\n",TYPE_OF(current_lexem), VALUE_OF(current_lexem)); break;

            }

            AddLexem(lexem_list, current_lexem);
        }
        
        else{
            printf("\n");
            FINAL_STATUS = LEX_ANALYSIS_FAILED;
        }

        prev_caret_position = caret;
        

    }

    lexem_list -> name_table = name_table;
    AddLexem(lexem_list, CreateLexem(EMPTY, LAST_LEXEM, nullptr, nullptr));

    return lexem_list;
}

NameTable* CreateNameTable(){
    
    NameTable* newNameTable     = (NameTable*)calloc(1, sizeof(NameTable));
    newNameTable -> identifiers = (Identifier*)calloc(1,INITIAL_NAMETABLE_CAPACITY * sizeof(Identifier));
    newNameTable -> size        = 0;
    newNameTable -> capacity    = INITIAL_NAMETABLE_CAPACITY;
    return newNameTable;
}

LexAnalysis_Status AddIdentifier(NameTable* name_table, char* name, Lexem_ID* ID){

    if (!name_table) return BAD_NAMETABLE_PTR;
    if (!name)       return BAD_NAME_PTR;

    if ((name_table) -> size == (name_table) -> capacity){
        Identifier* resizedIdentifiers = (Identifier*)realloc(((name_table) -> identifiers), ((name_table) -> capacity) * 2 * sizeof(Identifier));
        if (!resizedIdentifiers) return OUT_OF_MEM;
        
        (name_table) -> identifiers  = resizedIdentifiers;
        ((name_table) -> capacity)*=2; 
    }
    
    Identifier* new_identifier = ((name_table) -> identifiers) + ((name_table) -> size);

    new_identifier -> ID   = ((name_table) -> size);
    *ID = new_identifier -> ID;
    new_identifier -> name = name;
    
    ((name_table) -> size)++;
    return IDENTIFIER_ADDITION_OK;
}

int IsPossibleIdentifierChar(char chr){
    return isalpha(chr) || isdigit(chr) || chr == '_';
}

LexAnalysis_Status GetIdentifier(char** caret, char** name_ptr){

    if (!caret)    return BAD_CARET_POINTER;
    if (!(*caret)) return BAD_CARET_POSITION;
    if (!name_ptr) return BAD_NAME_PTR;

    if (!isalpha(**caret) && (**caret) != '\\') return BAD_LEXEM;

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

LexAnalysis_Status ProcessIdentifier(NameTable* name_table, char** caret, Lexem_ID* ID){
    
    if (!caret)    return BAD_CARET_POINTER;
    if (!(*caret)) return BAD_CARET_POSITION;
    
    for (size_t n_identifier = 0; n_identifier < (name_table -> size); n_identifier++){
        if (CompareWithIdentifier(*caret, ((name_table -> identifiers)[n_identifier]).name) == MATCH){
            *ID = ((name_table -> identifiers)[n_identifier]).ID;
            (*caret) += strlen(((name_table -> identifiers)[n_identifier]).name);
            return TOKEN_OK;
        }
    }
    
    
    char* newName                = nullptr;
    LexAnalysis_Status GetStatus = GetIdentifier(caret, &newName); 
    if (GetStatus != TOKEN_OK) return GetStatus;
    return AddIdentifier(name_table, newName, ID);
}

CompareResult CompareWithOperator(char* caret, const char* inline_string){

    if (!caret)         return BAD_FIRST_PTR;
    if (!inline_string) return BAD_SECOND_PTR;

    int n_char = 0;

    for (; !isalpha(caret[n_char]) && !isdigit(caret[n_char]) && caret[n_char] && inline_string[n_char] ; n_char++){
        if (caret[n_char] != inline_string[n_char]) return DIFFERENT;
    }

    if (inline_string[n_char] == '\0') return MATCH;
    else                               return DIFFERENT;
}

CompareResult CompareWithKeyword(char* caret, const char* inline_string){

    if (!caret)         return BAD_FIRST_PTR;
    if (!inline_string) return BAD_SECOND_PTR;

    int n_char = 0;

    for (; (isalpha(caret[n_char]) || caret[n_char] == '\\' || caret[n_char] == '!') && inline_string[n_char] ; n_char++){
        if (caret[n_char] != inline_string[n_char]) return DIFFERENT;
    }

    if (inline_string[n_char] == '\0' && (!isalpha(caret[n_char]))) return MATCH;
    else                                                            return DIFFERENT;
}

CompareResult CompareWithIdentifier(char* caret, const char* inline_string){

    if (!caret)         return BAD_FIRST_PTR;
    if (!inline_string) return BAD_SECOND_PTR;

    int n_char = 0;

    for (; caret[n_char] && inline_string[n_char] ; n_char++){
        if (caret[n_char] != inline_string[n_char]) return DIFFERENT;
    }

    if (inline_string[n_char] == '\0' && (!IsPossibleIdentifierChar(caret[n_char]))) return MATCH;
    else                                                                             return DIFFERENT;
}

void MoveCaretNextLexem(char** caret){
    for (; isspace(**caret) && (**caret) != '\0'; (*caret)++);
}

double FoldConstant(char** caret){

    double val = 0;

    for (;isdigit(**caret); (*caret)++){
        val = val*10 + **caret - '0';
    }
    return val;
}

LexAnalysis_Status GetLexem(NameTable* name_table, Lexem** lexem_ptr, char** caret){

    if (!lexem_ptr)         return BAD_LEXEM_POINTER;
    if (!caret)             return BAD_CARET_POINTER;
    if (!(*caret))          return BAD_CARET_POSITION;
    if (**caret == '\0')    return END_OF_CODE;

    Lexem_ID Identifier_ID = 0;

    #define LEXEM(type, ID, inline_string)                                                    \
        else if ((type == OPERATOR && CompareWithOperator(*caret, inline_string) == MATCH) || \
                 (type == KEYWORD  && CompareWithKeyword (*caret, inline_string) == MATCH))   \
            {                                                                                 \
            *lexem_ptr = CreateLexem(type, ID, nullptr, nullptr);                             \
            (*caret)+=strlen(inline_string);                                                  \
            if (!(*lexem_ptr)) return OUT_OF_MEM;                                             \
            }                                                                                 \

    if (0) printf("Ouch!\n");  //  Initial condition just to pass through it, required for DSL "LEXEM" macro checks
    
    #include "LexemList.h"
    
    else if (isdigit(**caret)) {
        *lexem_ptr = CreateConstantLexem(FoldConstant(caret));
        if (!(*lexem_ptr)) return OUT_OF_MEM;
    }

    else if (ProcessIdentifier(name_table, caret, &Identifier_ID) != BAD_LEXEM){
        *lexem_ptr = CreateLexem(IDENTIFIER, Identifier_ID, nullptr, nullptr);
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

const char* GetLexAnalysisStatus(LexAnalysis_Status current_result){

    #define CASE_RESULT(result) case result: return #result;

    switch(current_result){
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

Lexem* CreateLexem(Lexem_Type type, Lexem_ID ID, Lexem* left, Lexem* right){
    
    Lexem* newLexem = (Lexem*)calloc(1,sizeof(Lexem));

    if (!newLexem) return nullptr;

    TYPE_OF(newLexem) = type;
    ID_OF(newLexem)   = ID;

    newLexem -> left  = left;
    newLexem -> right = right;

    return newLexem;
}

Lexem* CreateConstantLexem(double value){
    
    Lexem* newLexem = (Lexem*)calloc(1,sizeof(Lexem));

    if (!newLexem) return nullptr;

    TYPE_OF (newLexem) = CONSTANT;
    VALUE_OF(newLexem) = value;

    newLexem -> left  = nullptr;
    newLexem -> right = nullptr;

    return newLexem;
}

Lexem_ID GetLexemID(char* kw_string){
    
    if (!kw_string) return LEXEM_INPUT_ERROR_ID;

    #define LEXEM(type, ID, keyword)\
        else if (!strcmp(keyword, kw_string)) return ID;
    
    if(0) return UNKNOWN_LEXEM_ID; 
    #include "LexemList.h"
    else  return UNKNOWN_LEXEM_ID;

    #undef LEXEM
}

Lexem_String GetLexemString(Lexem_ID current_id){
    
    #define LEXEM(type, ID, keyword) case ID: return keyword;

    switch(current_id){
        #include "LexemList.h"
        default: return UNKNOWN_LEXEM_STRING;
    }

    #undef LEXEM
}

Lexem_String GetLexemID_Name(Lexem_ID current_id){
    
    #define LEXEM(type, ID, keyword) case ID: return #ID;

    switch(current_id){
        #include "LexemList.h"
        default: return UNKNOWN_LEXEM_STRING;
    }

    #undef LEXEM
}

#define AST_CHECK                                   \
    AST_Error check_result = ValidateTree(tree);    \
    if(check_result != AST_OK) return check_result; \

#define READ_IF_NOT_ALREADY(lexem_name, interm_func)      \
    if (!lexem_name) lexem_name = interm_func(tree); \

LexemsList* lexems_list = nullptr;
int    n_lexem     = 0;

void RequireOperator(Lexem_ID operator_id){
    if (LexemIsOperator(n_lexem, operator_id)) n_lexem++;
    else {printf("Expected %s, got %c type: ",GetLexemString(operator_id), LEXEM_TYPE(n_lexem)); SyntaxError();}
}
void RequireKeyword(Lexem_ID keyword_id){
    if (LexemIsKeyword(n_lexem, keyword_id)) n_lexem++;
    else {printf("Expected %s, got %c type: ",GetLexemString(keyword_id), LEXEM_TYPE(n_lexem)); SyntaxError();}
}

int LexemIsOperator(int lexem_number, Lexem_ID operator_id){
    return (LEXEM_TYPE(lexem_number) == OPERATOR && LEXEM_ID(lexem_number) == operator_id);
}

int LexemIsKeyword(int lexem_number, Lexem_ID keyword_id){
    return (LEXEM_TYPE(lexem_number) == KEYWORD && LEXEM_ID(lexem_number) == keyword_id);
}

int CheckEndOfCode(int lexem_number){
    return (LEXEM_TYPE(lexem_number) == EMPTY);
}

void SyntaxError(){
    
    printf("Syntax error on #%d lexem\n", n_lexem);
    exit(0);
}

Lexem* GetDec(Tree* tree){

    if(LexemIsOperator(n_lexem, LBRACKET_SQUARE)){ 
        
        n_lexem++;

        if (LEXEM_TYPE(n_lexem) == IDENTIFIER){
            n_lexem++;
            
            RequireOperator(LBRACKET_ROUND);
            
            Lexem* vars = GetDeclVarEnum(tree);
            
            RequireOperator(RBRACKET_ROUND);
            RequireOperator(RBRACKET_SQUARE);
            
            RequireKeyword(EQDEF);

            RequireOperator(LBRACKET_SQUARE);

            Lexem* code = GetInnerFunctionCode(tree);

            RequireOperator(RBRACKET_SQUARE);

            return CreateLexem(OPERATOR, DEC, vars, code);
            
        }

        else return nullptr;

    }

    else return nullptr;
}

Lexem* GetDeclVarEnum(Tree* tree){
    
    Lexem* arg      = GetVar(tree); //T1

    while (LexemIsOperator(n_lexem, COMMA)){

        n_lexem++;
        arg = CreateLexem(OPERATOR, LR, arg, GetVar(tree));
        (tree -> size)++;

    }

    return arg;
}

Lexem* GetCondition(Tree* tree){

    if (LexemIsOperator(n_lexem, LBRACKET_SQUARE) && LexemIsKeyword(n_lexem+1, FORALL)){
        
        n_lexem+=2;

        Lexem* condition = GetLogic(tree);
        
        RequireOperator(RBRACKET_SQUARE);
        RequireKeyword (CONDITION_RIGHT_ARROW);
        RequireOperator(LBRACKET_SQUARE);

        Lexem* inner_code = GetCodeline(tree);
        RequireOperator(RBRACKET_SQUARE);

        return CreateLexem(KEYWORD, IF, condition, inner_code);
    }

    else return nullptr;
}

Lexem* GetReturn(Tree* tree){
    
    if (LexemIsKeyword(n_lexem, RETURN)){

        n_lexem++;
        
        Lexem* new_var = GetVar(tree);

        RequireOperator(EQ);

        (tree -> size)++;
        return CreateLexem(OPERATOR, RETURN, new_var, GetLogic(tree));
    }

    else return nullptr;
}

Lexem* GetVarInit(Tree* tree){
    
    if (LexemIsKeyword(n_lexem, VAR_INIT)){

        n_lexem++;
        
        Lexem* new_var = GetVar(tree);

        RequireOperator(EQ);

        (tree -> size)++;
        return CreateLexem(OPERATOR, VAR_INIT, new_var, GetLogic(tree));
    }

    else return nullptr;

}

Lexem* GetVarClaim(Tree* tree){

    Lexem* dest_var = GetVar(tree);
    if (!dest_var) return nullptr;

    RequireOperator(EQ);
    
    (tree -> size)++;
    return CreateLexem(OPERATOR, VAR_CLAIM, dest_var, GetLogic(tree));

}

Lexem* GetN(Tree* tree){
    
    if (LEXEM_TYPE(n_lexem) == CONSTANT){
        n_lexem++;
        (tree -> size)++;
        return CreateConstantLexem(LEXEM_VAL(n_lexem - 1));
    }
    else return nullptr;
}

Lexem* GetLogic(Tree* tree){

    Lexem* lvalue = GetE(tree); //T1
    Lexem* rvalue = nullptr;

    int current_operator = n_lexem;

    while (LexemIsOperator(n_lexem, GR_THEN) || LexemIsOperator(n_lexem, LR_THEN) || LexemIsOperator(n_lexem, EQ)){

        current_operator = n_lexem;
        n_lexem++;
        rvalue = GetE(tree);

        if      (LEXEM_ID(current_operator) == GR_THEN) lvalue = CreateLexem(OPERATOR, GR_THEN, lvalue, rvalue);
        else if (LEXEM_ID(current_operator) == LR_THEN) lvalue = CreateLexem(OPERATOR, LR_THEN, lvalue, rvalue);
        else                                            lvalue = CreateLexem(OPERATOR, EQ_COMP, lvalue, rvalue);

        (tree -> size)++;

        //tree -> root = lvalue; // TEMP
    }

    return lvalue;
}

Lexem* GetE(Tree* tree){

    Lexem* lvalue = GetT1(tree); //T1
    Lexem* rvalue = nullptr;

    int current_operator = n_lexem;

    while (LexemIsOperator(n_lexem, ADD) || LexemIsOperator(n_lexem, SUB)){

        current_operator = n_lexem;
        n_lexem++;
        rvalue = GetT1(tree);

        if   (LEXEM_ID(current_operator) == ADD) lvalue = CreateLexem(OPERATOR, ADD, lvalue, rvalue);
        else                                     lvalue = CreateLexem(OPERATOR, SUB, lvalue, rvalue);

        (tree -> size)++;

        //tree -> root = lvalue; // TEMP
    }

    return lvalue;
}

Lexem* GetT1(Tree* tree){

    Lexem* lvalue = GetT2(tree); //T1
    Lexem* rvalue = nullptr;

    int current_operator = n_lexem;

    while (LexemIsOperator(n_lexem, MUL) || LexemIsOperator(n_lexem, DIV)){

        current_operator = n_lexem;
        n_lexem++;
        rvalue = GetT2(tree);

        if   (LEXEM_ID(current_operator) == MUL) lvalue = CreateLexem(OPERATOR, MUL, lvalue, rvalue);
        else                                     lvalue = CreateLexem(OPERATOR, DIV, lvalue, rvalue);

        (tree -> size)++;

        //tree -> root = lvalue; // TEMP
    }

    return lvalue;
}

Lexem* GetT2(Tree* tree){

    Lexem* result_lexem = nullptr;
    
    READ_IF_NOT_ALREADY(result_lexem, GetVar)
    READ_IF_NOT_ALREADY(result_lexem, GetP)
    READ_IF_NOT_ALREADY(result_lexem, GetN)
    READ_IF_NOT_ALREADY(result_lexem, GetCall)
    
    return result_lexem;
}

Lexem* GetVar(Tree* tree){
    
    if (LEXEM_TYPE(n_lexem) == IDENTIFIER && !(LexemIsOperator(n_lexem+1, LBRACKET_ROUND))){
        n_lexem++;
        (tree -> size)++;
        return CreateLexem(VAR, LEXEM_ID(n_lexem-1), nullptr, nullptr);
    }
    else return nullptr;
}

Lexem* GetP(Tree* tree){

    Lexem* subtree = nullptr;

    if (LEXEM_TYPE(n_lexem) == OPERATOR && LEXEM_ID(n_lexem) == LBRACKET_ROUND){
        
        n_lexem++;
        subtree = GetLogic(tree);
        RequireOperator(RBRACKET_ROUND);
        return subtree;
    }

    return nullptr;
}

Lexem* GetCall(Tree* tree){

    Lexem* subtree = nullptr;

    if (LEXEM_TYPE(n_lexem) == IDENTIFIER && LexemIsOperator(n_lexem+1, LBRACKET_ROUND)){
        
        Lexem_ID func_id = LEXEM_ID(n_lexem);
        
        n_lexem+=2;
        (tree -> size)++;
        subtree = GetEnum(tree);
        RequireOperator(RBRACKET_ROUND);
        return CreateLexem(FUNC, func_id, nullptr, subtree);

    }
    else return nullptr;
}

Lexem* GetEnum(Tree* tree){

    Lexem* arg      = GetLogic(tree); //T1

    while (LexemIsOperator(n_lexem, COMMA)){

        n_lexem++;
        arg = CreateLexem(OPERATOR, LR, arg, GetLogic(tree));
        (tree -> size)++;

    }

    return arg;
}

Lexem* GetCodeline(Tree* tree){
    
    Lexem* result_line = nullptr;
    
    READ_IF_NOT_ALREADY(result_line, GetVarInit)
    READ_IF_NOT_ALREADY(result_line, GetVarClaim)
    READ_IF_NOT_ALREADY(result_line, GetCall)
    READ_IF_NOT_ALREADY(result_line, GetReturn)
    READ_IF_NOT_ALREADY(result_line, GetCondition)
    
    RequireOperator(LR);

    return result_line;
}

Lexem* GetInnerFunctionCode(Tree* tree){

    Lexem* code = GetCodeline(tree);

    while(code && !LexemIsOperator(n_lexem, RBRACKET_SQUARE)){
        code = CreateLexem(OPERATOR, LR, code, GetCodeline(tree));
        (tree -> size)++;
    }

    return code;
}

/*
Lexem* GetProgram(Lexem** declarations){
    
    Lexem* declaration = GetDec(tree); 

    while (!CheckEndOfCode(n_lexem)){
        
        declaration = Create


    }
}
*/
Lexem* GetG(LexemsList* dest_lexems_list, Tree* tree){
    
    lexems_list = dest_lexems_list;
    n_lexem = 0;

    

    return GetDec(tree);
}

/*
AST_Error AttachNode(Tree* tree, LR_Flag dest_flag, Lexem* attaching_node, Lexem* destination_node){

    if (!destination_node) return INVALID_DEST_POINTER;
    if (!attaching_node)   return INVALID_SOURCE_POINTER;

    AST_CHECK

    switch(dest_flag){
        
        case LEFT:  if (destination_node -> left) return EDGE_ALREADY_EXISTS;
                    destination_node -> left  = attaching_node; break;
        
        case RIGHT: if (destination_node -> right) return EDGE_ALREADY_EXISTS;
                    destination_node -> right = attaching_node; break;
    }

    tree -> size += 1;

    return ValidateTree(tree);
}
*/

AST_Error DumpNode(FILE* fp, Lexem* node){

    if (!node) return INVALID_NODE_POINTER;
    if (!fp)   return INVALID_FILE_POINTER;

    switch(TYPE_OF(node)){

        case CONSTANT:   fprintf(fp, "%ld [shape=record, fillcolor=green rank = same style=filled label=\"    {%lf | CONSTANT} \" ];\n",
                                node, VALUE_OF(node)); break;
                    
        case OPERATOR:   fprintf(fp, "%ld [shape=record, fillcolor=lightgray rank = same style=filled label=\"    {%s | {  OPER | %u }}\" ];\n",
                                node, GetLexemID_Name(ID_OF(node)), ID_OF(node)); break;
        
        case KEYWORD:    fprintf(fp, "%ld [shape=record, fillcolor=pink rank = same style=filled label=\"    {%s | {  KWRD | %u }}\" ];\n",
                                node, GetLexemID_Name(ID_OF(node)), ID_OF(node)); break;

        case IDENTIFIER: fprintf(fp, "%ld [shape=record, fillcolor=lightbrown rank = same style=filled label=\"    {%u | IDER} \" ];\n",
                                node, ID_OF(node)); break;
        
        case VAR: fprintf(fp, "%ld [shape=record, fillcolor=aquamarine rank = same style=filled label=\"    {%u | VAR} \" ];\n",
                                node, ID_OF(node)); break;

        case FUNC: fprintf(fp, "%ld [shape=record, fillcolor=yellow rank = same style=filled label=\"    {%u | FUNC} \" ];\n",
                                node, ID_OF(node)); break;
        

        //case VAR:      fprintf(fp, "%ld [label = \"%s \" style=filled fillcolor=aquamarine];\n", node, "VARTEMP"); break; //TODO

    }
    
    if(node -> left)  fprintf(fp, "%ld -> %ld [color=black]\n", node, node -> left);  DumpNode(fp, node -> left);
    if(node -> right) fprintf(fp, "%ld -> %ld [color=black]\n", node, node -> right); DumpNode(fp, node -> right);
    
    return AST_OK;
}

AST_Error GraphicalDump(Tree* tree){

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


/*
AST_Error ValidateTree(Tree* tree){
    
    if (!tree)            return INVALID_TREE_POINTER;
    if (tree -> size < 0) return AST_SIZE_LESS_ZERO;

    int nodes_counter = 0;

    return ValidateNodeRecursively(tree, tree -> root, &nodes_counter);
}

AST_Error ValidateNodeRecursively(Tree* tree, Lexem* node, int* nodes_counter){
    
    if (!node) return AST_OK;

    *nodes_counter += 1;

    if (node -> left && node -> left == node -> right) return SAME_LEFT_AND_RIGHT_CHILDS;
    if (node -> left == node || node -> right == node)    return NODES_LOOPED;
    if (*nodes_counter > tree -> size)                    return TOO_MANY_NODES_FOR_CURRENT_SIZE;

    AST_Error child_check_result = AST_OK;

    child_check_result = ValidateNodeRecursively(tree, node -> left, nodes_counter);
    
    if (child_check_result != AST_OK) return child_check_result;
    
    return ValidateNodeRecursively(tree, node -> right, nodes_counter);
}
*/