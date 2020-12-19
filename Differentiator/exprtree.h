#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "../Onegin/foperations.cpp"

const size_t INITIAL_VAR_NAME_TABLE_CAPACITY = 4;
const size_t MAX_VAR_NAME_LENGTH = 10;

const double PI = 3.1415927;

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
    CONST = 'C',
    OPER  = 'O',
    VAR   = 'V',
    EMPTY = '#'
};


enum OPERATION{

    ERROR_OPERATION = '#', 

    ADD   = '+', //
    SUB   = '-', //
    MUL   = '*', //
    DIV   = '/', //
    SIN   = 's', //
    COS   = 'c', //
    LOG   = 'l', //
    LN    = 'n', //
    EXP   = 'e', //
    TAN   = 't', //
    COT   = 'o', //
    ATAN  = 'T',
    ACOT  = 'O',
    ACOS  = 'C',
    ASIN  = 'S'
};

enum OPERATION_TYPE{
    UNARY,
    BINARY
};

enum TEX_OPERATION_TYPE{
    PREFIX,
    INFIX
};

struct Node;
struct Tree;



struct Node{

    TYPE       type   = CONST;
    double     value  = NAN;
    Tree*      tree   = nullptr;
    Node*      parent = nullptr;
    Node*      left   = nullptr;
    Node*      right  = nullptr;
};

struct VarNameTable{
    
    char** name_arr = nullptr;
    size_t size     = 0;
    size_t capacity = 0;
};

struct Tree{
    
    Node*        root      = nullptr;
    size_t       size      = 0;
    VarNameTable variables = {0};
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

void DeleteNodeRecursively(Node* current);


void TeXDumpExpressionTree(FILE* fp, Tree* tree);