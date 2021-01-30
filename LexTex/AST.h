#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "../Onegin/onegin.h"

typedef unsigned char Lexem_ID;
typedef const char*   Lexem_String; 

const Lexem_ID UNKNOWN_LEXEM_ID     = '?';
const Lexem_ID LEXEM_INPUT_ERROR_ID = 0;

Lexem_String UNKNOWN_LEXEM_STRING   = "UNKNOWN";

const size_t INITIAL_NAMETABLE_CAPACITY = 10;

enum Lexem_Type{
    CONSTANT = 'C',
    KEYWORD = 'K',
    OPERATOR = 'O',
    IDENTIFIER = 'I',
    EMPTY = '?',
};

enum LexAnalysis_Status{
    TOKEN_OK,
    END_OF_CODE,
    BAD_CARET_POINTER,
    BAD_CARET_POSITION,
    BAD_LEXEM_POINTER,
    OUT_OF_MEM,
    BAD_LEXEM,
    IDENTIFIER_ADDITION_OK,
    BAD_NAMETABLE_PTR,
    BAD_NAME_PTR,
    LEX_ANALYSIS_OK,
    LEX_ANALYSIS_FAILED,
    LEX_ANALYSIS_UNDEFINED
};

enum INLINE_IDENTIFICATORS{
    
    #define LEXEM(type, ID, string) ID,
    #include "LexemList.h"
    #undef  LEXEM
};

enum LR_Flag{
    LEFT,
    RIGHT
};

enum CompareResult{
    MATCH,
    DIFFERENT,
    BAD_FIRST_PTR  = 1,
    BAD_SECOND_PTR = 2
};

struct Lexem{
    Lexem_Type type = EMPTY;

    union Lexem_Container{
        Lexem_ID ID = '~';
        double   value;
    }container;  
};

struct Identifier{
    char* name  = nullptr;
    Lexem_ID ID = UNKNOWN_LEXEM_ID;
};

struct NameTable{
    size_t      capacity    = INITIAL_NAMETABLE_CAPACITY;
    size_t      size        = 0;
    Identifier* identifiers = nullptr;
};

#define ID_OF(lexem)    ((lexem -> container).ID)
#define VALUE_OF(lexem) ((lexem -> container).value)
#define TYPE_OF(lexem)  (lexem -> type)


enum AST_Error{
    AST_OK                          = -1,
    AST_SIZE_LESS_ZERO              = -2,
    NODES_LOOPED                    = -3,
    EDGE_ALREADY_EXISTS             = -4,
    SAME_LEFT_AND_RIGHT_CHILDS      = -5,
    STRAY_NODE                      = -6,
    INVALID_NODE_VALUE_PTR          = -7,
    TOO_MANY_NODES_FOR_CURRENT_SIZE = -8,
    INVALID_SOURCE_POINTER          = -9,
    INVALID_DEST_POINTER            = -11,
    INVALID_FILE_POINTER            = -12,
    INVALID_NODE_POINTER            = -13,
    INVALID_TREE_POINTER            = -14,
    FILE_OPEN_ERROR                 = -15
};

struct Node{
    
    Lexem_Type type = EMPTY;
    
    Node* left  = nullptr;
    Node* right = nullptr; 
    
    double value = 0;
};

struct LexemsList{

    Lexem**            lexems     = nullptr;
    size_t             n_lexems   = 0;
    LexAnalysis_Status status     = LEX_ANALYSIS_UNDEFINED;
    NameTable*         name_table = nullptr;
};

struct Tree{
    Node* root  = nullptr;
    size_t size = 0;
};

Lexem_ID GetLexemID(char* kw_string);
Lexem_String GetLexemString(Lexem_ID kw_id);
Node* CreateNode(Lexem_Type type, double value, Node* left, Node* right);
AST_Error AttachNode(Tree* tree, LR_Flag dest_flag, Node* attaching_node, Node* destination_node);
AST_Error DumpNode(FILE* fp, Node* node);
AST_Error GraphicalDump(Tree* tree);
AST_Error ValidateTree(Tree* tree);
AST_Error ValidateNodeRecursively(Tree* tree, Node* node, int* nodes_counter);
const char* GetLexAnalysisStatus(LexAnalysis_Status current_result);

LexemsList* LexAnalysis(char* code_buffer);
void LexGraphicalDump(LexemsList* lexems_list);
Lexem* CreateLexem(Lexem_Type type, Lexem_ID ID);
LexAnalysis_Status GetLexem(NameTable* name_table, Lexem** lexem_ptr, char** caret);
CompareResult CompareWithOperator(char* caret, const char* inline_string);
CompareResult CompareWithKeyword(char* caret, const char* inline_string);
CompareResult CompareWithIdentifier(char* caret, const char* inline_string);
void MoveCaretNextLexem(char** caret);
double FoldConstant(char** caret);
Lexem* CreateConstantLexem(double value);

NameTable* CreateNameTable();
LexAnalysis_Status AddIdentifier(NameTable* name_table, char* name, Lexem_ID* ID);
int IsPossibleIdentifierChar(char chr);
LexAnalysis_Status GetIdentifier(char** caret, char** name_ptr);
LexAnalysis_Status ProcessIdentifier(NameTable* name_table, char** caret, size_t* ID);

Node* GetN(Tree* tree, Node* parent);
Node* GetP(Tree* tree, Node* parent);
Node* GetT1();
Node* GetT2();
Node* GetT3();
Node* GetE(Tree* tree, Node* parent);
Node* GetG(char* str, Tree* tree);
