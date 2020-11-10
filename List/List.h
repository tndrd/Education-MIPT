#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>

enum LIST_STATUS{

    OK                                = 0,
    LIST_PTR_UNAVAILABLE              = -2,
    WRONG_QUANTITY_OF_FREE_ELEMENTS   = -3,
    OUT_OF_MEMORY                     = -4,
    CREATION_FAILED_NOT_ENOUGH_MEMORY = -5,
    LIST_NAME_ERROR                   = -6,
    DATA_PTR_UNAVAILABLE              = -7,
    SIZE_LESS_ZERO                    = -8,
    SIZE_GREATER_THAN_CAPACITY        = -9,
    BROKEN_NUMERATION                 = -10,
    FREE_HEAD_LESS_ZERO               = -11,
    TAIL_LESS_ZERO                    = -12,
    HEAD_LESS_ZERO                    = -13,
    CAPACITY_LESS_ZERO                = -14,
    ERROR_ON_FREE_ELEMENT_SEARCH      = -15,
    WRONG_POSITION                    = -16,
    ALREADY_ORDERED                   = -17,
    SEARCH_NO_RESULTS                 = -18
};

struct SearchResult{
    int phys_index  = 0;
    int logic_index = 0;
};

struct Element{
    double value = NAN;
    int next = -1;
    int prev = -1;
};

struct List{

    const char*   name        = nullptr;
    Element*      elements    = nullptr;
    int           capacity    = -1;
    int           size        = -1;
    int           head        = -1;
    int           tail        = -1;
    int           free_head   = 1;
    unsigned char isOrdered   = 1; 
    LIST_STATUS   list_status = OK;
};

LIST_STATUS ValidateList(List* thou);

List* NewList(int capacity);

int FindFree(List* thou);

LIST_STATUS ResizeUp(List* thou);

int InsertAfter(List* thou, int pos, double value);

int InsertBefore(List* thou, int pos, double value);

LIST_STATUS Delete(List* thou, int pos);

void GraphicalDump(List* thou);

LIST_STATUS ValidateData(List* thou);

LIST_STATUS DeleteList(List* thou);

int TextDump(List* thou);

LIST_STATUS Search(List* thou, double value, SearchResult* found);

int InsertFirst(List* thou, double value);

int InsertAfterTail(List* thou, double value);

int InsertBeforeHead(List* thou, double value);

LIST_STATUS DeleteTail(List* thou);

LIST_STATUS DeleteHead(List* thou);

LIST_STATUS LogicalOrdering(List* thou);

int PhysIndexFromLogic(List* thou, int logic_index);

int LogicIndexFromPhys(List* thou, int phys_index);

