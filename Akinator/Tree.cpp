#include "Tree.h"

#define CASE_ERROR(error_code) case error_code: return #error_code;

const char* GET_ERROR_NAME(TREE_STATUS status){
    
    switch(status){
        CASE_ERROR(OK)
        CASE_ERROR(SIZE_LESS_ONE)
        CASE_ERROR(NODES_LOOPED)
        CASE_ERROR(INVALID_POINTER)
        CASE_ERROR(TOO_MANY_NODES_FOR_CURRENT_SIZE)
        default: return "Unknown Error";
    }
}

Tree* NewTree(char* RootValue){

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

    Parent    -> left   = to_attach;
    to_attach -> parent = Parent;
    to_attach -> tree   = Parent -> tree;
    
    ((Parent -> tree) -> size)++;

    return OK;
}


TREE_STATUS AddLeftNode(Node* Parent, char* NodeValue){
    
    if (!Parent || !NodeValue) return INVALID_POINTER;
    
    Node* new_node     = (Node*)calloc(1, sizeof(Node));
    new_node -> value  = NodeValue;
    
    return AttachLeftNode(new_node, Parent);
}


TREE_STATUS AttachRightNode(Node* to_attach, Node* Parent){

    if (!Parent || !to_attach) return INVALID_POINTER;
    if (Parent -> right)       return EDGE_ALREADY_EXISTS;

    Parent    -> right  = to_attach;
    to_attach -> parent = Parent;
    to_attach -> tree   = Parent -> tree;
    
    ((Parent -> tree) -> size)++;

    return OK;
}


TREE_STATUS AddRightNode(Node* Parent, char* NodeValue){
    
    if (!Parent || !NodeValue) return INVALID_POINTER;
    
    Node* new_node     = (Node*)calloc(1, sizeof(Node));
    new_node -> value  = NodeValue;
    
    return AttachRightNode(new_node, Parent);
}
/*
TREE_STATUS AddRightNode(Node* Parent, char* NodeValue){
    
    if (!Parent || !NodeValue) return INVALID_POINTER;
    if (Parent -> right)                return EDGE_ALREADY_EXISTS;
    
    Parent  -> right            = (Node*)calloc(1, sizeof(Node));
    (Parent -> right) -> value  = NodeValue;
    (Parent -> right) -> parent = Parent;
    (Parent -> right) -> tree   = Parent -> tree;

    ((Parent -> tree) -> size)++;

    return OK;
}
*/

int DumpNode(FILE* fp, Node* node){
    
    if (!node) return 1;
    assert(node -> value);
    
    fprintf(fp, "%ld [label=\"%s\"];\n", node, node -> value);
    fprintf(fp, "%ld -> %ld [color = grey]\n", node, node -> parent);
    if(node -> left) fprintf(fp, "%ld -> %ld [color=green]\n", node, node -> left);
    if(node -> right) fprintf(fp, "%ld -> %ld [color=red]\n", node, node -> right);
    
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
}

int SaveNode(FILE* fp, Node* Parent){

    if (!Parent) return 0;
    assert (Parent -> value);
    
    fprintf(fp, "[\n");

    if (!(Parent -> left) && !(Parent -> right))
        fprintf(fp, "`%s`\n", Parent -> value);
    else{
        fprintf(fp, "?%s?\n", Parent -> value);
        SaveNode(fp, Parent -> left);
        SaveNode(fp, Parent -> right);
    }
    
    fprintf(fp, "]\n");
    return 1;
}


TREE_STATUS SaveTree(Tree* tree, const char* filename){
    
    if (!tree) return INVALID_POINTER;

    FILE* fp = fopen(filename, "w");
    SaveNode(fp, tree -> root);
    fclose(fp);
    return OK;
}

Node* ReadNode(Tree* tree, char** ptr){
    Node* new_node = (Node*)calloc(1,sizeof(Node));
    
    for(;**ptr != '`' && **ptr != '?'; (*ptr)++){
        if (**ptr == ']'){
            printf("File format error, unexpected \"]\"\n");
        }
    }
    

    char node_type = **ptr;
    new_node -> value = *ptr + 1;
    new_node -> tree  = tree;
    (tree -> size)++;

    *ptr = strchr(*ptr+1, **ptr);
    **ptr = '\0';
    
    if (node_type == '`'){    
        new_node -> left  = nullptr;
        new_node -> right = nullptr;
    }
    else if(node_type == '?'){
        *ptr = strchr(*ptr + 1, '[');
        new_node  -> left  =  ReadNode(tree, ptr);
        assert(new_node -> left);
        (new_node -> left) -> parent = new_node;

        *ptr = strchr(*ptr + 1, '[');
        new_node -> right = ReadNode(tree, ptr);
        assert(new_node -> right);
        (new_node -> right) -> parent = new_node;
    }
    return new_node;
}

Tree* ReadTree(char* filename){
    
    assert(filename);
    Tree* new_tree = (Tree*)calloc(1, sizeof(Tree));
    char* buffer = ReadFile(filename);
    if (!buffer) return nullptr;
    new_tree -> root = ReadNode(new_tree, &buffer);
    return new_tree;
}

TREE_STATUS ValidateNode(Node* node, int* counter_ptr){
    
    if (!node) return OK;

    if (!(node -> tree))  return WRONG_TREE_PTR;
    if (!(node -> parent) && node != (node -> tree) -> root) return WRONG_PARENT_PTR;

    if (*counter_ptr == (node -> tree) -> size && (node -> left || node -> right)) return TOO_MANY_NODES_FOR_CURRENT_SIZE;

    if (node -> parent == node ->  left && node -> left)                           return NODES_LOOPED;
    if (node -> parent == node -> right && node -> right)                          return NODES_LOOPED;
    if (node -> left   == node -> right && node -> left)                           return SAME_LEFT_AND_RIGHT_CHILDS;
    if (node -> left && !(node -> right) || node -> right && !(node -> left))      return ONLY_ONE_CHILD;
    
    if (!(node -> value)) return INVALID_NODE_VALUE_PTR;

    *(counter_ptr)++;
    TREE_STATUS child_status = OK;
    child_status = ValidateNode(node -> left, counter_ptr);
    if (child_status != OK) return child_status;
    child_status = ValidateNode(node -> right, counter_ptr);
    return child_status;
}

TREE_STATUS ValidateTree(Tree* thou){
    
    int counter = 0;
    return ValidateNode(thou -> root, &counter);
}
/*
int main(){
    
    /*
    Tree* thou = NewTree("First tree!");
    AddLeftNode( thou -> root, "Left");
    AddRightNode(thou -> root, "Right");
    AddLeftNode((thou -> root) -> left, "aaaaa");
    AddRightNode((thou -> root) -> left, "bbbb");
    AddLeftNode((thou -> root) -> right, "cccc");
    AddRightNode((thou -> root) -> right, "dddd");
    SaveTree(thou, "mytree.txt");
    GraphicalDump(thou);
    
    Tree* thou = ReadTree("mytree.txt");
    GraphicalDump(thou);
    printf("%s\n", GET_ERROR_NAME(ValidateTree(thou)));
}
*/