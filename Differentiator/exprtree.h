#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "../Onegin/foperations.cpp"

enum TREE_STATUS{
    OK              = -1,
    SIZE_LESS_ONE   = -2,
    NODES_LOOPED    = -3,
    EDGE_ALREADY_EXISTS = -4,
    INVALID_POINTER = -5,
    SAME_LEFT_AND_RIGHT_CHILDS = -6,
    STRAY_NODE = -8,
    INVALID_NODE_VALUE_PTR = -9,
    WRONG_TREE_PTR = -10,
    WRONG_PARENT_PTR = -11,
    TOO_MANY_NODES_FOR_CURRENT_SIZE = -12,
};


enum TYPE{
    CONST = 0,
    OPER  = 1,
    VAR   = 2,
}


enum OPERATION{

    ADD  = '+',
    SUB  = '-',
    MUL  = '*',
    DIV  = '/',
    SIN  = 's',
    COS  = 'c',
    LOG  = 'l',
    LN   = 'n',
    EXP  = 'e',
    NEG  = 'g',
    TAN  = 't',
    COT  = 'o',
    ATAN = 'T',
    ACOT = 'O',
    ACOS = 'C',
    ASIN = 'S'
}


}

struct Node;
struct Tree;


union node_value {
    double    CONST_VAL;
    OPERATION OPER_VAL;
    int       VAR_VAL;
};

struct Node{

    TYPE       type   = CONST;
    node_value value  = {0};
    Tree*      tree   = nullptr;
    Node*      parent = nullptr;
    Node*      left   = nullptr;
    Node*      right  = nullptr;
};

struct Tree{
    Node* root = nullptr;
    int size = 0;
};

const char* GET_ERROR_NAME(TREE_STATUS status);

Tree* NewTree(char* RootValue);

TREE_STATUS AddLeftNode(Node* Parent, char* NodeValue);

TREE_STATUS AddRightNode(Node* Parent, char* NodeValue);

TREE_STATUS AddLeftNode(Node* Parent, char* NodeValue);

int DumpNode(FILE* fp, Node* node);

TREE_STATUS GraphicalDump(Tree* tree);

TREE_STATUS SaveNode(FILE* fp, Node* Parent);

TREE_STATUS SaveTree(Tree* tree, const char* filename);

Node* ReadNode(Tree* tree, char** ptr);

Tree* ReadTree(const char* filename);

TREE_STATUS ValidateNode(Node* node, int* counter_ptr);

TREE_STATUS ValidateTree(Tree* thou);

TREE_STATUS DeleteTree(Tree* tree);

void DeleteNodeRecursively(Node* current);