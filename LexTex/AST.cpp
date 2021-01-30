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
    
    char* expression = (char*)calloc(1024, sizeof(char));
    fgets(expression, 1024, stdin);

    LexemsList* lexed = LexAnalysis(expression);
    
    if (lexed -> status == LEX_ANALYSIS_OK){
        printf("Lexical analysis complete, showing diagram...\n");
        LexGraphicalDump(lexed);
    }
    
    return 0;
}

void LexGraphicalDump(LexemsList* lexems_list){

    FILE* fp = fopen("show", "w");   
    
    fprintf(fp, "digraph structs {\nrankdir=LR rank=same;\n");
    
    for(size_t n_lexem = 0; n_lexem < lexems_list -> n_lexems; n_lexem++){

        switch(((lexems_list -> lexems)[n_lexem]) -> type){

            case OPERATOR:   fprintf(fp, "%d [shape=record, fillcolor=yellow rank = same style=filled label=\"    %s | {  OPER | %u }\" ];\n",
                                   n_lexem, GetLexemString((((lexems_list -> lexems)[n_lexem]) -> container).ID), (((lexems_list -> lexems)[n_lexem]) -> container).ID); break;
            case KEYWORD:    fprintf(fp, "%d [shape=record, fillcolor=pink rank = same style=filled label=\"    %s | {  KWRD | %u }\" ];\n",
                                   n_lexem, GetLexemString((((lexems_list -> lexems)[n_lexem]) -> container).ID), (((lexems_list -> lexems)[n_lexem]) -> container).ID); break;
            case CONSTANT:   fprintf(fp, "%d [shape=record, fillcolor=green rank = same style=filled label=\"    %lf | { CONSTANT }\" ];\n",
                                   n_lexem, (((lexems_list -> lexems)[n_lexem]) -> container).value); break;
            case IDENTIFIER: fprintf(fp, "%d [shape=record, fillcolor=aquamarine rank = same style=filled label=\"    %s | {IDER | %u}\" ];\n",
                                   n_lexem, ((lexems_list -> name_table) -> identifiers)[(((lexems_list -> lexems)[n_lexem]) -> container).ID].name, (((lexems_list -> lexems)[n_lexem]) -> container).ID); break;
        }

    }
    for(size_t n_lexem = 0; n_lexem < (lexems_list -> n_lexems) - 1; n_lexem++) fprintf(fp, "%d -> %d\n", n_lexem, n_lexem+1);
    fprintf(fp, "}");
    fclose(fp);
    
    system("dot -Tpng show -n -o show.png");
    system("viewnior show.png");
}

LexemsList* LexAnalysis(char* code_buffer){
    
    char* caret               = code_buffer;
    char* prev_caret_position = caret;

    NameTable* name_table = CreateNameTable();
    Lexem*  current_lexem = nullptr; 
    
    LexemsList* lexem_list  = (LexemsList*)calloc(1, sizeof(LexemsList));
    
    #define LEXEMS       (lexem_list -> lexems)
    #define N_LEXEMS     (lexem_list -> n_lexems) 
    #define FINAL_STATUS (lexem_list -> status)

    LEXEMS       = (Lexem**)calloc(1, sizeof(Lexem*));
    N_LEXEMS     = 0;  
    FINAL_STATUS = LEX_ANALYSIS_OK;

    LexAnalysis_Status LexStatus = GetLexem(name_table, &current_lexem, &caret);

    for (; LexStatus != END_OF_CODE; LexStatus = GetLexem(name_table, &current_lexem, &caret)){ 
        
        printf("# %s {%p} at %d ", GetLexAnalysisStatus(LexStatus), current_lexem, prev_caret_position - code_buffer);
        
        if (current_lexem && LexStatus == TOKEN_OK){

            switch(TYPE_OF(current_lexem)){
                
                case OPERATOR: case KEYWORD: printf(": %c / %s\n", TYPE_OF(current_lexem), GetLexemString(ID_OF(current_lexem))); break;

                case IDENTIFIER:             printf(": %c / %d / %s\n", TYPE_OF(current_lexem), ID_OF(current_lexem), (name_table -> identifiers)[ID_OF(current_lexem)].name); break;

                case CONSTANT:               printf(": %c / %lf\n",TYPE_OF(current_lexem), VALUE_OF(current_lexem)); break;

            }

            LEXEMS = (Lexem**)realloc(LEXEMS, (N_LEXEMS+1)*sizeof(Lexem*));
            LEXEMS[N_LEXEMS] = current_lexem;    
        
        }
        
        else{
            printf("\n");
            FINAL_STATUS = LEX_ANALYSIS_FAILED;
        }

        prev_caret_position = caret;
        N_LEXEMS++;

    }

    lexem_list -> name_table = name_table;

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
            return TOKEN_OK;
        }
    }
    
    
    char* newName                = nullptr;
    LexAnalysis_Status GetStatus = GetIdentifier(caret, &newName); 
    if (GetStatus != TOKEN_OK) return GetStatus;
    return AddIdentifier(name_table, newName, ID);
}

#define AST_CHECK                                   \
    AST_Error check_result = ValidateTree(tree);    \
    if(check_result != AST_OK) return check_result; \

//------------------------------------------------------------------------------------------------------------------
/*
char* s = nullptr;
int p = 0;


#define Require(chr)\
    if (s[p] == chr) p++;\
    else SyntaxError(); 


void SyntaxError(){
    printf("Syntax error: %s\n", s);
    for (int i = 0; i < p + 14; i++) printf(" ");
    printf("^\n");
    for (int i = 0; i < p + 14; i++) printf(" ");
    printf("|\n");
}

#define SKIP_SPACES\
    for(; isspace(s[p]); p++);


Node* GetN(Tree* tree, Node* parent){

    double val = 0;
    int prev_p = p;
    
    while ('0' <= s[p] && s[p] <= '9'){
        
        val = val*10 + s[p] - '0';
        p++;
    }

    if (p == prev_p) SyntaxError;
    return CreateNode(CONSTANT, val, nullptr, nullptr);
}


Node* GetE(Tree* tree, Node* parent){

    SKIP_SPACES

    Node* lvalue = GetN(tree, parent); //T1

    Node* rvalue = nullptr;
    
    //double  rvalue = 0;
    
    SKIP_SPACES

    int  current_operator = p;

    while(s[p] == '+' || s[p] == '-'){
        
        SKIP_SPACES

        current_operator = p;
        p++;

        SKIP_SPACES
        
        rvalue = GetN(tree, parent); //T1

        SKIP_SPACES
        
        if (s[current_operator] == '+') lvalue = CreateNode(KEYWORD, (double)('+'), lvalue, rvalue);
        else                            lvalue = CreateNode(KEYWORD, (double)('-'), lvalue, rvalue);

        tree -> size ++;
    
    }
    
    tree -> root = lvalue;

    return lvalue;
}


Node* GetG(char* str, Tree* tree){
    
    s = str;
    p = 0;

    SKIP_SPACES

    Node* root = GetE(tree, nullptr);

    SKIP_SPACES

    Require('$')
    return root;
}
*/
//------------------------------------------------------------------------------------------------------------------

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

    for (; (isalpha(caret[n_char]) || caret[n_char] == '\\') && inline_string[n_char] ; n_char++){
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
            *lexem_ptr = CreateLexem(type, ID);                                               \
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
        *lexem_ptr = CreateLexem(IDENTIFIER, Identifier_ID);
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

Lexem* CreateLexem(Lexem_Type type, Lexem_ID ID){
    
    Lexem* newLexem = (Lexem*)calloc(1,sizeof(Lexem));

    if (!newLexem) return nullptr;

    TYPE_OF(newLexem) = type;
    ID_OF(newLexem)   = ID;

    return newLexem;
}

Lexem* CreateConstantLexem(double value){
    
    Lexem* newLexem = (Lexem*)calloc(1,sizeof(Lexem));

    if (!newLexem) return nullptr;

    TYPE_OF (newLexem) = CONSTANT;
    VALUE_OF(newLexem) = value;

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

Node* CreateNode(Lexem_Type type, double value, Node* left, Node* right){

    Node* NewNode = (Node*)calloc(1, sizeof(Node));
    
    if (!NewNode) return nullptr;

    NewNode -> type   = type;
    NewNode -> value  = value;
    NewNode -> left   = left;
    NewNode -> right  = right;

    return NewNode;
}

AST_Error AttachNode(Tree* tree, LR_Flag dest_flag, Node* attaching_node, Node* destination_node){

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

AST_Error DumpNode(FILE* fp, Node* node){

    if (!node) return INVALID_NODE_POINTER;
    if (!fp)   return INVALID_FILE_POINTER;

    switch(node -> type){

        case CONSTANT: fprintf(fp, "%ld [label = \"%lg \" style=filled fillcolor=green];\n", node, node -> value); break;
                    
        case IDENTIFIER:  fprintf(fp, "%ld [label = \"%s \" style=filled fillcolor=yellow];\n", node, GetLexemString((Lexem_ID)(node -> value))); break;
        
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

    fprintf(fp, "digraph G {\n");
    
    DumpNode(fp, tree -> root);
    
    fprintf(fp, "}");
    fclose(fp);
    system("dot -Tpng show -n -o show.png");
    system("viewnior show.png");

    return AST_OK;
}

AST_Error ValidateTree(Tree* tree){
    
    if (!tree)            return INVALID_TREE_POINTER;
    if (tree -> size < 0) return AST_SIZE_LESS_ZERO;

    int nodes_counter = 0;

    return ValidateNodeRecursively(tree, tree -> root, &nodes_counter);
}

AST_Error ValidateNodeRecursively(Tree* tree, Node* node, int* nodes_counter){
    
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