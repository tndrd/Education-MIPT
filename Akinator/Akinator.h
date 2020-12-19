#include "../Stack/Stack.cpp"

#include "Tree.h"
#include "locale.h"
#include "Gvrlk.h"

const int INITIAL_COMPARE_STACK_CAPACITY = 10;

enum GAMEMODE{
    GUESS      = 'g',
    DEFINITION = 'd',
    COMPARE    = 'c',
    SHOW       = 's',
    HACK       = 'h'
};

enum AKINATOR_ACTION_STATUS{

    GO_ON     = 'g',
    TRY_AGAIN = 't',
    GAME_END  = 'e',
    AKINATOR_TREE_ERROR = '#'
};

void ReadFromConsole(char* destination);
AKINATOR_ACTION_STATUS AkinatorChooseDatabase();
TREE_STATUS SplitByAtribute(char* new_attribute, Node* Parent_Node, char* NewObject);
TREE_STATUS AddAttribute(Node* current_node);
AKINATOR_ACTION_STATUS SwitchNode(Node* current_node);
AKINATOR_ACTION_STATUS AkinatorPlayGuess(Tree* DataBase);
Node* SearchObjectRecursively(char* value, Node* current, int* node_counter);
Node* SearchFromRoot(char* value, Node* root);
void DefineObjectRecursively(Node* current, Node* endpoint, int last, int state);
void Definition(Node* object, Node* endpoint);
AKINATOR_ACTION_STATUS AkinatorPlayDefinition(Tree* tree);
TREE_STATUS GetPath(Stack* path, Node* current_node, int* counter);
AKINATOR_ACTION_STATUS AkinatorCompare(Tree* tree, Node* first, Node* second);
AKINATOR_ACTION_STATUS AkinatorPlayCompare(Tree* tree);
AKINATOR_ACTION_STATUS AkinatorPlay(Tree* tree);
AKINATOR_ACTION_STATUS AkinatorSaveTree(Tree* tree);

AKINATOR_ACTION_STATUS AkinatorEndGame(Tree* tree);
