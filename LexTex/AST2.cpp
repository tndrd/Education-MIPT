#include "AST.h"


#define AST_CHECK                                   \
    AST_Error check_result = ValidateTree(tree);    \
    if(check_result != AST_OK) return check_result; \

//------------------------------------------------------------------------------------------------------------------

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
    return CreateNode(CONSTANT, val, nullptr, nullptr);
}

Node* GetE(Tree* tree){

    Node* lvalue = GetT1(tree, parent); //T1
    Node* rvalue = nullptr;


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