#include "exprtree.h"

TREE_STATUS check_status = OK;

#define PARSER_TRACE ;

char* DEBUG_BUFFER = nullptr;

#ifndef PARSER_TRACE

#define INIT_TRACE DEBUG_BUFFER = buffer;

#define PARSER_TRACE ParserGraphicalDump(ptr);

#else

#define INIT_TRACE ;

#endif


void ParserGraphicalDump(char** ptr);

#define TREE_CHECK(tree_ptr)\
    check_status = ValidateTree(tree_ptr);\
    if (check_status != OK){\
        if (tree_ptr) GraphicalDump(tree_ptr);\
        return check_status;\
    }

#define CASE_ERROR(error_code) case error_code: return #error_code;


const char* GET_ERROR_NAME(TREE_STATUS status){
    
    switch(status){

        CASE_ERROR(OK)
        CASE_ERROR(SIZE_LESS_ONE)
        CASE_ERROR(NODES_LOOPED)
        CASE_ERROR(INVALID_POINTER)
        CASE_ERROR(TOO_MANY_NODES_FOR_CURRENT_SIZE)
        CASE_ERROR(EDGE_ALREADY_EXISTS)
        CASE_ERROR(SAME_LEFT_AND_RIGHT_CHILDS)
        CASE_ERROR(STRAY_NODE)
        CASE_ERROR(INVALID_NODE_VALUE_PTR)
        CASE_ERROR(WRONG_TREE_PTR)
        CASE_ERROR(WRONG_PARENT_PTR)
        default: return "Unknown Error";
    }
}

/*
Tree* NewTree(node_value RootValue){

    Tree*    new_tree = (Tree*)calloc(1, sizeof(Tree));
    new_tree  -> root = (Node*)calloc(1, sizeof(Node));
    new_tree  -> size = 1;
    
    (new_tree -> root) -> value = RootValue;
    (new_tree -> root) -> tree = new_tree;
    
    return new_tree;
}
*/

TREE_STATUS AttachLeftNode(Node* to_attach, Node* Parent){

    if (!Parent || !to_attach) return INVALID_POINTER;
    if (Parent -> left)        return EDGE_ALREADY_EXISTS;

    //TREE_CHECK(Parent -> tree)

    Parent    -> left   = to_attach;
    to_attach -> parent = Parent;
    to_attach -> tree   = Parent -> tree;
    
    ((Parent -> tree) -> size)++;

    //TREE_CHECK(Parent -> tree)
    return OK;
}

TREE_STATUS AttachRightNode(Node* to_attach, Node* Parent){

    if (!Parent || !to_attach) return INVALID_POINTER;
    if (Parent -> right)       return EDGE_ALREADY_EXISTS;

    //TREE_CHECK(Parent -> tree)

    Parent    -> right  = to_attach;
    to_attach -> parent = Parent;
    to_attach -> tree   = Parent -> tree;
    
    ((Parent -> tree) -> size)++;
    
    //TREE_CHECK(Parent -> tree)
    return OK;
}

#define CASE_OPERATION(OPERATION, OPERATOR) case OPERATION: return OPERATOR;


const char* GET_OPERATOR(OPERATION operation){

    switch(operation){
        CASE_OPERATION(ADD, "+")
        CASE_OPERATION(SUB, "-")
        CASE_OPERATION(MUL, "*")
        CASE_OPERATION(DIV, "/")
        CASE_OPERATION(SIN, "sin")
        CASE_OPERATION(COS, "cos")
        CASE_OPERATION(LOG, "log")
        CASE_OPERATION(LN,  "ln")
        CASE_OPERATION(EXP, "^")
        CASE_OPERATION(TAN, "tg")
        CASE_OPERATION(COT, "ctg")
        CASE_OPERATION(ATAN, "arctg")
        CASE_OPERATION(ACOT, "arcctg")
        CASE_OPERATION(ACOS, "arccos")
        CASE_OPERATION(ASIN, "arcsin")
        default: return "ERROR";
    }
}

int DumpNode(FILE* fp, Node* node){
    
    if (!node) return 1;
    
    //assert(node -> value);

    switch(node -> type){

        case CONST: fprintf(fp, "%ld [label = \"%lf\" style=filled fillcolor=green];\n", node, node -> value);
                    break;

        case OPER:  fprintf(fp, "%ld [label = \"%s\" style=filled fillcolor=yellow];\n", node, GET_OPERATOR((OPERATION)(node -> value)));
                    if(node -> left)  fprintf(fp, "%ld -> %ld [color=black]\n", node, node -> left);
                    if(node -> right) fprintf(fp, "%ld -> %ld [color=black]\n", node, node -> right);
                    break;
        
        case VAR:   fprintf(fp, "%ld [label = \"%s\" style=filled fillcolor=aquamarine];\n", node, ((node -> tree) -> variables).name_arr[(int)(node -> value)]);
                    break;

    }
    
    DumpNode(fp, node -> left);
    DumpNode(fp, node -> right);
    
    return 0;
}


TREE_STATUS GraphicalDump(Tree* tree){

    if (!tree) return INVALID_POINTER;

    FILE* fp = fopen("show", "w");
    fprintf(fp, "digraph G {\n");
    
    DumpNode(fp, tree -> root);
    
    fprintf(fp, "}");
    fclose(fp);
    system("dot -Tpng show -n -o show.png");
    system("viewnior show.png");
    return OK;
}


TREE_STATUS SaveNode(FILE* fp, Node* Parent){

    if (!Parent) return INVALID_POINTER;
    
    fprintf(fp, "(");

    switch(Parent -> type){

        case CONST: fprintf(fp, "%lf", Parent -> value);
                    break;

        case VAR:   fprintf(fp, "%s", ((Parent -> tree) -> variables).name_arr[(int)(Parent -> value)]);
                    break;
        
        case OPER:
                    SaveNode(fp, Parent -> left);
                    fprintf(fp, " %s ", GET_OPERATOR((OPERATION)(Parent -> value)));
                    SaveNode(fp, Parent -> right);
                    break;

    }
    
    fprintf(fp, ")");
    
    return OK;
}


TREE_STATUS SaveTree(Tree* tree, const char* filename){
    
    //TREE_CHECK(tree)

    if (!tree) return INVALID_POINTER;

    FILE* fp = fopen(filename, "w");
    
    SaveNode(fp, tree -> root);
    
    fclose(fp);
    
    return OK;
}


TYPE ParseNodeType(char* ptr){
    
    char* node_begin = ptr;

    int open_bracket_counter = 0;
    int close_bracket_counter = 0;
    
    char IsConst = 1;
    char IsVar = 1;

    char IsEmpty = 1;

    for(; open_bracket_counter != close_bracket_counter || open_bracket_counter == 0; ptr++ ){
        if (*ptr == '('){
            open_bracket_counter++;
            if (open_bracket_counter > 1) return OPER;
        }
        else if (*ptr == ')'){
            close_bracket_counter++;
        }
        else{
            if (!isspace(*ptr)){
                if (!isdigit(*ptr) && *ptr != '.') IsConst = 0;
                if (!isalpha(*ptr))                IsVar = 0;
                IsEmpty = 0;
            }
        }
        //printf("|%c| [\"(\": %d | \")\": %d] {v: %d | c: %d}\n", *ptr, open_bracket_counter, close_bracket_counter, IsVar, IsConst);
        fflush(stdout);
    }

    if((IsConst == IsVar) && !IsEmpty) printf("Error here: |%s||%s|\n", node_begin, ptr);

    assert((IsConst != IsVar) || IsEmpty);

    if      (IsEmpty) return EMPTY;
    else if (IsVar)   return VAR;
    else if (IsConst) return CONST;
    else              return OPER;
}


#define PROPER_NOTATION_CHAR(chr) (isgraph(chr) && chr != '(' && chr != ')')


int CompareNotationStrings(char* str1, char* str2){  

    int n_char = 0;

    for(; PROPER_NOTATION_CHAR(str1[n_char]) && PROPER_NOTATION_CHAR(str2[n_char]) ; n_char++)
        if (str1[n_char] != str2[n_char]) return 0;

    if (PROPER_NOTATION_CHAR(str1[n_char]) || PROPER_NOTATION_CHAR(str2[n_char]))
        return 0;
    else
        return 1;
} 

int ReadVar(Tree* tree, char** ptr){

    #define VAR_NAME_ARR          (tree -> variables).name_arr
    #define VAR_NAME_ARR_SIZE     (tree -> variables).size
    #define VAR_NAME_ARR_CAPACITY (tree -> variables).capacity

    if (VAR_NAME_ARR_SIZE == VAR_NAME_ARR_CAPACITY){
        VAR_NAME_ARR_CAPACITY *= 2;
        VAR_NAME_ARR = (char**)realloc(VAR_NAME_ARR, VAR_NAME_ARR_CAPACITY * sizeof(char*));
    }

    for(; isspace(**ptr); (*ptr)++);
    
    for (int n_var = 0; n_var < VAR_NAME_ARR_SIZE; n_var++){
        printf("Compare |%s| an |%s|\n", VAR_NAME_ARR[n_var], *ptr);
        if (CompareNotationStrings(*ptr, VAR_NAME_ARR[n_var])){
            return n_var;
        }
    }
    
    VAR_NAME_ARR[VAR_NAME_ARR_SIZE] = (char*)calloc(MAX_VAR_NAME_LENGTH, sizeof(char));

    int n_char = 0;


    for (; PROPER_NOTATION_CHAR((*ptr)[n_char]); n_char++) {
        VAR_NAME_ARR[VAR_NAME_ARR_SIZE][n_char] = (*ptr)[n_char];   
    }
    
    VAR_NAME_ARR[VAR_NAME_ARR_SIZE][n_char] = '\0';

    VAR_NAME_ARR_SIZE++;

    return VAR_NAME_ARR_SIZE - 1;
}


void PrintFirstWord(char* str){
    for(; PROPER_NOTATION_CHAR(*str); str++) printf("%c", *str);
}


#define IF_OPERATOR(OPERATION, OPERATOR) else if (CompareNotationStrings(*ptr, OPERATOR))  return OPERATION;

OPERATION GET_OPERATION_NUM(char** ptr){

    if(0) return ERROR_OPERATION;
    IF_OPERATOR(ADD, "+")
    IF_OPERATOR(SUB, "-")
    IF_OPERATOR(MUL, "*")
    IF_OPERATOR(DIV, "/")
    IF_OPERATOR(SIN, "sin")
    IF_OPERATOR(COS, "cos")
    IF_OPERATOR(LOG, "log")
    IF_OPERATOR(LN,  "ln")
    IF_OPERATOR(EXP, "^")
    IF_OPERATOR(TAN, "tg")
    IF_OPERATOR(COT, "ctg")
    IF_OPERATOR(ATAN, "arctg")
    IF_OPERATOR(ACOT, "arcctg")
    IF_OPERATOR(ACOS, "arccos")
    IF_OPERATOR(ASIN, "arcsin")
    else return ERROR_OPERATION;
}


OPERATION ReadOperator(char** ptr){

    for(; isspace(**ptr); (*ptr)++);
    PARSER_TRACE
    
    OPERATION found = GET_OPERATION_NUM(ptr);
    
    if (found == ERROR_OPERATION){
        printf("UNKNOWN OPERATOR: |");
        PrintFirstWord(*ptr);
        printf("|\n");
        exit(0);
    }

    return found;
}


Node* ReadNodeRecursively(Tree* tree, char** ptr){
    
    Node* new_node = nullptr;

    *ptr = strchr(*ptr, '(');
    PARSER_TRACE

    TYPE node_type = ParseNodeType(*ptr);
    
    if (node_type == EMPTY){
        printf("YEP\n");
        goto end_reading;
    }

    new_node = (Node*)calloc(1,sizeof(Node));
    
    new_node -> type  = node_type; 
    
    new_node -> tree = tree;

    new_node -> right = nullptr;
    new_node -> left  = nullptr;

    (*ptr)++;
    PARSER_TRACE

    (tree -> size)++;

    switch(new_node -> type){
        
        case VAR:   (new_node -> value) = (double)ReadVar(tree, ptr);
                    PARSER_TRACE
                    break;

        case CONST: (new_node -> value) = strtod(*ptr, ptr);
                    PARSER_TRACE
                    break;

        case OPER:  (new_node -> left)           = ReadNodeRecursively(tree, ptr);
                    PARSER_TRACE
                    (new_node -> value) = (double)ReadOperator(ptr);
                    PARSER_TRACE
                    (new_node -> right)          = ReadNodeRecursively(tree, ptr);
                    PARSER_TRACE
                    break;

    }
    
    end_reading:

    *ptr = strchr(*ptr, ')') + 1;
    PARSER_TRACE


    return new_node;
}


void ParserGraphicalDump(char** ptr){

    if (DEBUG_BUFFER){
    FILE* fp = fopen("show", "w");
    fprintf(fp, "digraph G {\n");

    for (char* current = DEBUG_BUFFER; *current != '\0'; current++){
        fprintf(fp, "%ld [label = \"%c\"];\n", current, *current);
    }

    fprintf(fp, "current -> %ld\n", *ptr);

    fprintf(fp, "}");
    fclose(fp);
    system("dot -Tpng show -n -o show.png");
    system("viewnior show.png");
    }
}



Tree* ReadTree(char* filename){
    
    assert(filename);
    
    Tree* new_tree = (Tree*)calloc(1, sizeof(Tree));
    
    char* buffer      = ReadFile(filename);
    INIT_TRACE

    if (!buffer) return nullptr;
    
    (new_tree -> variables).size     = 0;
    (new_tree -> variables).capacity = INITIAL_VAR_NAME_TABLE_CAPACITY;
    (new_tree -> variables).name_arr = (char**)calloc(INITIAL_VAR_NAME_TABLE_CAPACITY, sizeof(char*));

    assert((new_tree -> variables).name_arr);

    char* buffer_ptr = buffer;
    new_tree -> root = ReadNodeRecursively(new_tree, &buffer_ptr);
    

    free(buffer);

    return new_tree;
}

double log_a_b(double base, double value){
    return log(value)/log(base);
}

double cotan(double value){
    return 1/tan(value);
}

double acotan(double value){
    return PI/2 - atan(value);
}

#define CASE_EVAL_UNARY(oper_name, oper) case oper_name: return oper((node -> right) -> value);

#define CASE_EVAL_BINAR_INFIX(oper_name, oper) case oper_name:  printf("%lf " #oper " %lf = %lf\n", ((node -> right) -> value), ((node -> left) -> value), ((node -> right) -> value) oper ((node -> left) -> value));\
                                                                return ((node -> right) -> value) oper ((node -> left) -> value);
                                                               
#define CASE_EVAL_BINAR_PREFIX(oper_name, oper) case oper_name: return oper(((node -> left) -> value), ((node -> right) -> value));  

double EvaluateOperation(Node* node){

    if ((node -> type) != OPER) return NAN;

    switch((OPERATION)(node -> value)){
        CASE_EVAL_BINAR_INFIX(ADD, +)
        CASE_EVAL_BINAR_INFIX(SUB, -)
        CASE_EVAL_BINAR_INFIX(MUL, *)
        CASE_EVAL_BINAR_INFIX(DIV, /)
        CASE_EVAL_UNARY(SIN, sin)
        CASE_EVAL_UNARY(COS, cos)
        CASE_EVAL_BINAR_PREFIX(LOG, log_a_b)
        CASE_EVAL_UNARY(LN,  log)
        CASE_EVAL_BINAR_PREFIX(EXP, pow)
        CASE_EVAL_UNARY(TAN, tan)
        CASE_EVAL_UNARY(COT, cotan)
        CASE_EVAL_UNARY(ATAN, atan)
        CASE_EVAL_UNARY(ACOT, acotan)
        CASE_EVAL_UNARY(ACOS, acos)
        CASE_EVAL_UNARY(ASIN, asin)
         
        default:
            printf("Failed to fold a constant, unknown operator code");
            return NAN;
    }
}

void RecursiveNodeConstantFolding(Node* node){

    if ((node -> type) == OPER){
        if ((node -> left)  -> type == OPER)  RecursiveNodeConstantFolding(node -> left);
        if ((node -> right) -> type == OPER)  RecursiveNodeConstantFolding(node -> right);

        if ((node -> left) -> type == CONST && (node -> right) -> type == CONST){
            node -> value = EvaluateOperation(node);
            node -> type = CONST;
            free(node -> left);
            free(node -> right);
            node -> left = nullptr;
            node -> right = nullptr;
        }


    }
    GraphicalDump(node -> tree);
}

void FoldConstants(Tree* tree){
    RecursiveNodeConstantFolding(tree -> root);
}






/*
int main(){

    char* filename = (char*)calloc(40, sizeof(char));
    printf("Open: ");
    scanf("%s", filename);
    
    Tree* tree = ReadTree(filename);
    GraphicalDump(tree);

    printf("Save: ");
    scanf("%s", filename);
    SaveTree(tree, filename);
    //printf("{%d}\n", CompareStringWithOperator("sadfa", "sadf "));
    
}
*/