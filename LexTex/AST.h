#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "../Onegin/onegin.h"

typedef const char*   Lexem_String; 

Lexem_String UNKNOWN_LEXEM_STRING = "UNKNOWN";

const size_t INITIAL_NAMETABLE_CAPACITY = 10;

enum AST_Unit_Type{

    OPERATOR   = 'O',
    IDENTIFIER = 'I',
    EMPTY      = '~',
    CONSTANT   = 'C',
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

enum Operator_ID{
    
    #define LEXEM(ID, string) ID,
    #include "LexemList.h"
    #undef  LEXEM

    UNKNOWN_LEXEM = '~',
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

    AST_Unit_Type type = EMPTY;

    union Lexem_Container{
        Operator_ID ID = UNKNOWN_LEXEM;
        double      value;
        char*       identifier_name;
    }container;  
};

enum AST_Operator_ID{
    
    #define AST_OPERATOR(name, format) AST_ ## name,
    #include "ASTOperatorsList.h"
    #undef AST_OPERATOR

    UNKNOWN_NODE = '~'
};

struct AST_Node{

    AST_Unit_Type type = EMPTY;

    AST_Node* left  = nullptr;
    AST_Node* right = nullptr;

    union Node_Container{
        AST_Operator_ID ID = UNKNOWN_NODE;
        double   value;
        char*    identifier_name;
    }container;  
};

struct Variable{

    int offset = 0;
    char*  name   = nullptr;
};

struct VarNameTable{
    
    Variable* vars     = nullptr;
    size_t    size   = 0;
    size_t    capacity = INITIAL_NAMETABLE_CAPACITY;
};

#define ID_OF(lexem)       ((lexem -> container).ID)
#define VALUE_OF(lexem)    ((lexem -> container).value)
#define IDR_NAME_OF(lexem) ((lexem -> container).identifier_name)

#define TYPE_OF(lexem)  (lexem -> type)

#define LEXEM_ID(n_lexem)        ID_OF      ((lexems_list -> lexems)[n_lexem])
#define LEXEM_VAL(n_lexem)       VALUE_OF   ((lexems_list -> lexems)[n_lexem])
#define LEXEM_IDR_NAME(n_lexem)  IDR_NAME_OF((lexems_list -> lexems)[n_lexem])
#define LEXEM_TYPE(n_lexem)      TYPE_OF    ((lexems_list -> lexems)[n_lexem])

#define INL_NAME(n_lexem)   GetOperatorString(LEXEM_ID(n_lexem)) 
#define IDR_NAME(n_lexem)  (((lexems_list -> name_table) -> identifiers)[LEXEM_ID(n_lexem)]).name    

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


struct LexemsList{

    Lexem**            lexems     = nullptr;
    size_t             size   = 0;
    LexAnalysis_Status status     = LEX_ANALYSIS_UNDEFINED;
};

struct Tree{
    AST_Node* root  = nullptr;
    size_t size = 0;
};

CompareResult NEW_CompareWithOperator(char* caret, const char* inline_string);
void AddLexem(LexemsList* lexem_list, Lexem* lexem);
void PrintLexem(Lexem* lexem, size_t n_lexem, LexAnalysis_Status status, size_t n_char);
Operator_ID GetLexemID(char* kw_string);
Lexem_String GetOperatorString(Operator_ID kw_id);
Lexem_String GetOperatorName(Operator_ID current_id);
const char* GetLexAnalysisStatus(LexAnalysis_Status current_result);

AST_Error ValidateTree(Tree* tree);
AST_Error ValidateNodeRecursively(Tree* tree, AST_Node* node, int* nodes_counter);


LexemsList* LexAnalysis(char* code_buffer);
void LexGraphicalDump(LexemsList* lexems_list);
LexAnalysis_Status GetLexem(Lexem** lexem_ptr, char** caret);
CompareResult CompareWithOperator(char* caret, const char* inline_string);
CompareResult CompareWithKeyword(char* caret, const char* inline_string);
CompareResult CompareWithIdentifier(char* caret, const char* inline_string);
void MoveCaretNextLexem(char** caret);
double FoldConstant(char** caret);
Lexem* CreateOperatorLexem(Operator_ID ID);
Lexem* CreateConstantLexem(double value);
Lexem* CreateIdentifierLexem(char* idr_name);
Lexem* CreateEmptyLexem();
Lexem_String GetLexemID_Name(Operator_ID current_id);

int IsPossibleIdentifierChar(char chr);
LexAnalysis_Status GetIdentifier(char** caret, char** name_ptr);

AST_Node* CreateOperatorNode(AST_Operator_ID ID, AST_Node* left, AST_Node* right);
AST_Node* CreateConstantNode(double value, AST_Node* left, AST_Node* right);
AST_Node* CreateIdentifierNode(char* idr_name, AST_Node* left, AST_Node* right);

AST_Node* GetInnerFunctionCode();
AST_Node* GetDeclVarEnum();
AST_Node* GetDec();
AST_Node* GetLogic();
AST_Node* GetCodeline();
AST_Node* GetCondition();
AST_Node* GetLoop();
AST_Node* GetVarClaim();
AST_Node* GetCall();
AST_Node* GetEnum();
AST_Node* GetP();
AST_Node* GetKeyword();
AST_Node* GetInput();
AST_Node* GetVar();
AST_Node* GetMod();
AST_Node* GetT2();
AST_Node* GetT1();
AST_Node* GetN();
AST_Node* GetE();
AST_Node* GetG(LexemsList* dest_lexems_list);

int LexemIsOperator(int lexem_number, Operator_ID operator_id);
int LexemIsKeyword(int lexem_number, Operator_ID keyword_id);

AST_Error DumpNode(FILE* fp, AST_Node* node);
AST_Error GraphicalDump(Tree* tree);

int NodeIsOperator(AST_Node* node, AST_Operator_ID operator_id);
void GenerateASM(Tree* tree, char* filename);

int NameTableAddVar(VarNameTable* name_table, char* var_name);
int NameTableCheckVar(VarNameTable* name_table, char* var_name);
int NameTableProcessVar(VarNameTable* name_table, char* var_name);
void NameTableDump(VarNameTable* name_table);

void GenerateDec(FILE* fp, AST_Node* node);
void GenerateStatement(FILE* fp, VarNameTable* name_table, AST_Node* node);
void GenerateLineOperator(FILE* fp, VarNameTable* name_table, AST_Node* node);
void GenerateAssign(FILE* fp, VarNameTable* name_table, AST_Node* node);
void GenerateExpression(FILE* fp, VarNameTable* name_table, AST_Node* node);
void GenerateVarValue(FILE* fp, VarNameTable* name_table, AST_Node* node);
void GenerateMathOperator(FILE* fp, VarNameTable* name_table, AST_Node* node);
void GenerateVarImp(FILE* fp, VarNameTable* name_table, AST_Node* node);
void GenerateReturn(FILE* fp, VarNameTable* name_table, AST_Node* node);
void GenerateCall(FILE* fp, VarNameTable* name_table, AST_Node* node);
void GenerateFuncCallParams(FILE* fp, VarNameTable* name_table, AST_Node* node);
void GenerateCondition(FILE* fp, VarNameTable* name_table, AST_Node* node);
void GenerateLoop(FILE* fp, VarNameTable* name_table, AST_Node* node);
void GeneratePrint(FILE* fp, VarNameTable* name_table, AST_Node* node);
void GenerateDot(FILE* fp, VarNameTable* name_table, AST_Node* node);

void RequireOperator(Operator_ID operator_id);
void SyntaxError();
