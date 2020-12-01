#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../Onegin/foperations.cpp"

enum TREE_STATUS{
    OK              = -1,
    SIZE_LESS_ONE   = -2,
    NODES_LOOPED    = -3,
    EDGE_ALREADY_EXISTS = -4,
    INVALID_POINTER = -5,
    SAME_LEFT_AND_RIGHT_CHILDS = -6,
    ONLY_ONE_CHILD = -7,
    STRAY_NODE = -8,
    INVALID_NODE_VALUE_PTR = -9,
    WRONG_TREE_PTR = -10,
    WRONG_PARENT_PTR = -11,
    TOO_MANY_NODES_FOR_CURRENT_SIZE = -12,
};

struct Node;
struct Tree;

struct Node{
    char* value  = nullptr;
    Tree* tree   = nullptr;
    Node* parent = nullptr;
    Node* left   = nullptr;
    Node* right  = nullptr;
};

struct Tree{
    Node* root = nullptr;
    int size = 0;
};


const size_t FILENAME_LENGTH = 40;
const size_t OBJECT_NAME_LENGTH = 40;
const size_t ATTRIBUTE_NAME_LENGTH = 40;
const size_t SAY_BUFFER_LENGTH = 1024;
const float  SPEECH_SPEED = 1.5;


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