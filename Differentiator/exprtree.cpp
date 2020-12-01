#include "exprtree.h"

TREE_STATUS check_status = OK;


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


TREE_STATUS AttachLeftNode(Node* to_attach, Node* Parent){

    if (!Parent || !to_attach) return INVALID_POINTER;
    if (Parent -> left)        return EDGE_ALREADY_EXISTS;

    TREE_CHECK(Parent -> tree)

    Parent    -> left   = to_attach;
    to_attach -> parent = Parent;
    to_attach -> tree   = Parent -> tree;
    
    ((Parent -> tree) -> size)++;

    TREE_CHECK(Parent -> tree)
    return OK;
}


TREE_STATUS AddLeftNode(Node* Parent, char* NodeValue){

    if (!Parent || !NodeValue) return INVALID_POINTER;
    
    TREE_CHECK(Parent -> tree)

    Node* new_node     = (Node*)calloc(1, sizeof(Node));
    new_node -> value  = NodeValue;
    
    return AttachLeftNode(new_node, Parent);
}


TREE_STATUS AttachRightNode(Node* to_attach, Node* Parent){

    if (!Parent || !to_attach) return INVALID_POINTER;
    if (Parent -> right)       return EDGE_ALREADY_EXISTS;

    TREE_CHECK(Parent -> tree)

    Parent    -> right  = to_attach;
    to_attach -> parent = Parent;
    to_attach -> tree   = Parent -> tree;
    
    ((Parent -> tree) -> size)++;
    
    TREE_CHECK(Parent -> tree)
    return OK;
}


TREE_STATUS AddRightNode(Node* Parent, char* NodeValue){
    
    if (!Parent || !NodeValue) return INVALID_POINTER;
    
    TREE_CHECK(Parent -> tree)

    Node* new_node     = (Node*)calloc(1, sizeof(Node));
    new_node -> value  = NodeValue;
    
    return AttachRightNode(new_node, Parent);
}
*/

#define CASE_OPERATION(OPER) case OPER: return #OPER;


const char* GET_OPERATOR(OPERATION operation){

    switch(operation){
        CASE_OPERATION(ADD)
        CASE_OPERATION(SUB)
        CASE_OPERATION(MUL)
        CASE_OPERATION(DIV)
        CASE_OPERATION(SIN)
        CASE_OPERATION(COS)
        CASE_OPERATION(LOG)
        CASE_OPERATION(LN)
        CASE_OPERATION(EXP)
        CASE_OPERATION(NEG)
        CASE_OPERATION(TAN)
        CASE_OPERATION(COT)
        CASE_OPERATION(ATAN)
        CASE_OPERATION(ACOT)
        CASE_OPERATION(ACOS)
        CASE_OPERATION(ASIN)
        default: return "Unknown operation";
    }
}

int DumpNode(FILE* fp, Node* node){
    
    if (!node) return 1;
    
    //assert(node -> value);

    switch(node -> type){

        case CONST: fprintf(fp, "%ld [label = \"%lf\"];\n", node, (node -> value).CONST_VAL);
                    break;

        case OPER:  fprintf(fp, "%ld [label = \"%s\" ];\n", node, GET_OPERATOR((node -> value).OPER_VAL));
                    break;
        
        case VAR:   fprintf(fp, "%ld [label = \"%s\" ];\n", node, "TEMPVAR");
                    break;

    }
    
    if(node -> left)  fprintf(fp, "%ld -> %ld [color=black]\n", node, node -> left);
    if(node -> right) fprintf(fp, "%ld -> %ld [color=black]\n", node, node -> right);
    
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

/*
TREE_STATUS SaveNode(FILE* fp, Node* Parent){

    if (!Parent) return INVALID_POINTER;
    
    TREE_CHECK   (Parent -> tree);
    assert       (Parent -> value);
    
    fprintf(fp, "[\n");

    if (!(Parent -> left) && !(Parent -> right))
        fprintf (fp, "`%s`\n", Parent -> value);
    else{
        fprintf (fp, "?%s?\n", Parent -> value);
        
        SaveNode(fp, Parent -> left);
        SaveNode(fp, Parent -> right);
    }
    
    fprintf(fp, "]\n");
    
    TREE_CHECK(Parent -> tree);
    
    return OK;
}


TREE_STATUS SaveTree(Tree* tree, const char* filename){
    
    TREE_CHECK(tree)

    if (!tree) return INVALID_POINTER;

    FILE* fp = fopen(filename, "w");
    
    SaveNode(fp, tree -> root);
    
    fclose(fp);
    
    return OK;
}
*/

Node* ReadNodeRecursively(Tree* tree, char** ptr){
    
    Node* new_node = (Node*)calloc(1,sizeof(Node));
    
    *ptr = strchr(*ptr, '(');

    char* left_node_begin = *ptr;
    
    (*ptr)++;

    int bracket_counter = 1;
    
    char LeftNodeIsConst = 1;
    char LeftNodeIsVar = 1;

    for(; bracket_counter != 0; (*ptr)++ ){
        if (**ptr == '('){
            bracket_counter++;
            LeftNodeIsConst = 0;
            LeftNodeIsVar = 0;
        }
        else if (**ptr == ')'){
            bracket_counter--;
        }
        else{
            if (!isdigit(**ptr) && **ptr != '.') LeftNodeIsConst = 0;
            if (!isalpha(**ptr)) LeftNodeIsVar = 0;
        }
        //printf("%c %d\n", **ptr, bracket_counter);
        fflush(stdout);
    }
    

    for(; isspace(**ptr); (*ptr)++);
    
    char* operand = *ptr;
    
    for(; !isspace(**ptr); (*ptr)++);
    
    **ptr = '\0';
    (*ptr)++;


    char* right_node_begin = strchr(*ptr, '(');
    (*ptr)++;
    bracket_counter = 1;
    
    char RightNodeIsConst = 1;
    char RightNodeIsVar = 1;

    for(; bracket_counter != 0; (*ptr)++ ){
        if (**ptr == '('){
            bracket_counter++;
            RightNodeIsConst = 0;
            RightNodeIsVar = 0;
        }
        else if (**ptr == ')'){
            bracket_counter--;
        }
        else{
            //printf("%c (v: %d c: %d) -> ", **ptr, RightNodeIsVar, RightNodeIsConst);
            if (!isdigit(**ptr) && **ptr != '.') RightNodeIsConst = 0;
            if (!isalpha(**ptr)) RightNodeIsVar = 0;
            //printf("%c (v: %d c: %d)\n", **ptr, RightNodeIsVar, RightNodeIsConst);
        }
        fflush(stdout);
    }

    printf("|%s| (v: %d, c: %d) |%s| |%s| (v: %d, c: %d)\n", left_node_begin, LeftNodeIsVar, LeftNodeIsConst, operand, right_node_begin, RightNodeIsVar, RightNodeIsConst);

    /*
    for (; !isdigit(**ptr) && !isalpha(**ptr) &&  
    
    if (isdigit(node_val_begin))

    new_node -> value = *ptr + 1;
    
    new_node -> tree  = tree;
    (tree -> size)++;

    *ptr  = strchr(*ptr+1, **ptr);
    **ptr = '\0';
    
    if (node_type == '`'){    
        new_node -> left  = nullptr;
        new_node -> right = nullptr;
    }
    else if(node_type == '?'){
        
        *ptr = strchr(*ptr + 1, '[');
        
        new_node  -> left  =  ReadNodeRecursively(tree, ptr);
        
        if(!new_node -> left) return nullptr;
        
        (new_node -> left) -> parent = new_node;

        *ptr = strchr(*ptr + 1, '[');
        
        new_node -> right = ReadNodeRecursively(tree, ptr);
        
        if(!new_node -> left) return nullptr;
    
        (new_node -> right) -> parent = new_node;
    }
    */
    return new_node;
}


Tree* ReadTree(char* filename){
    
    assert(filename);
    
    Tree* new_tree = (Tree*)calloc(1, sizeof(Tree));
    
    char* buffer   = ReadFile(filename);
    if (!buffer) return nullptr;
    
    char* buffer_ptr = buffer;
    new_tree -> root = ReadNodeRecursively(new_tree, &buffer_ptr);
    
    free(buffer);

    return new_tree;
}

/*
TREE_STATUS ValidateNode(Node* node, int* counter_ptr){
    
    if (!node) return OK;

    if (!(node -> tree))                                     return WRONG_TREE_PTR;
    if (!(node -> parent) && node != (node -> tree) -> root) return WRONG_PARENT_PTR;
    if (!(node -> value))                                    return INVALID_NODE_VALUE_PTR;

    if (*counter_ptr == (node -> tree) -> size && (node -> left || node -> right)) return TOO_MANY_NODES_FOR_CURRENT_SIZE;

    if (node -> parent == node ->  left && node -> left)                           return NODES_LOOPED;
    if (node -> parent == node -> right && node -> right)                          return NODES_LOOPED;
    if (node -> left   == node -> right && node -> left)                           return SAME_LEFT_AND_RIGHT_CHILDS;

    *(counter_ptr)++;

    TREE_STATUS child_status = OK;

    child_status = ValidateNode(node -> left, counter_ptr);
    if (child_status != OK) return child_status;
    
    child_status = ValidateNode(node -> right, counter_ptr);
    
    return child_status;
}

TREE_STATUS ValidateTree(Tree* thou){    
    
    int counter = 0;
    if (!thou) return INVALID_POINTER;
    
    return ValidateNode(thou -> root, &counter);
}
*/

int main(){

    char* filename = (char*)calloc(40, sizeof(char));
    scanf("%s", filename);
    ReadTree(filename);
}