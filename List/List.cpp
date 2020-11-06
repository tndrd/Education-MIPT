#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>


#define NEXT_OF(i) elements[elements[i].next]
#define PREV_OF(i) elements[elements[i].prev]


enum ESTATUS{
    BUSY,
    FREE,
    ERROR_BROKEN_NUMERATION,
};

enum LIST_STATUS{
    OK,
    NOT_RIGHT_QUANTITY_OF_FREE_ELEMENTS,//
    OVERFLOW,//
    
    CREATION_FAILED_NOT_ENOUGH_MEMORY,//
    LIST_ALREADY_DEAD,
    LIST_PTR_UNAVAILABLE,//
    DATA_PTR_UNAVAILABLE,//
    SIZE_LESS_ZERO,//
    SIZE_GREATER_THAN_CAPACITY,//
    BROKEN_NUMERATION,//
    DATA_CORRUPTED_UNEXPECTED_NON_POISON_VALUE_AT_FREE_ELEMENT,
    FREE_HEAD_LESS_ZERO,
    TAIL_LESS_ONE,
    HEAD_LESS_ONE,
    ERROR_ON_FREE_ELEMENT_SEARCH

};

struct Element{
    ESTATUS status = FREE;
    double value = NAN;
    int next = -1;
    int prev = -1;
};

struct List{
    Element* elements = nullptr;
    int capacity = -1;
    int size = -1;
    int head = -1;
    int tail = -1;
    int free_head = 1;
    unsigned char isOrdered = 1; 
    unsigned char exists = 0;
    LIST_STATUS list_status = OK;
};

LIST_STATUS ValidateList(List* thou);
List* NewList(int capacity);
int FindFree(List* thou);
int ResizeUp(List* thou);
int InsertAfter(List* thou, int pos, double value);
int InsertBefore(List* thou, int pos, double value);
int Delete(List* thou, int pos);
void GraphicalDump(List* thou);
LIST_STATUS ValidateData(List* thou);
int DeleteList(List* thou);

LIST_STATUS ValidationResult = OK;

#define LIST_ASSERT                                              \
    ValidationResult = ValidateList(thou);                       \
    if (ValidationResult != OK){                                 \
        if (ValidationResult == LIST_PTR_UNAVAILABLE) return -3; \
        else{                                                    \
            GraphicalDump(thou);                                 \
            printf("ListError\n");                               \
            return -2;                                           \
        }                                                        \
    }                                                            \

List* NewList(int capacity){
    
    List* new_list = (List*)calloc(1, sizeof(List));
    if (!new_list) return nullptr;
    
    new_list -> capacity = capacity;
    new_list -> size = 0;
    new_list -> head = 1;
    new_list -> tail = 1;
    new_list -> elements = (Element*)calloc(capacity, sizeof(Element));
    new_list -> free_head = 1;
    new_list -> list_status = OK;
    new_list -> exists = 1;
    new_list -> isOrdered = 1;
    
    #define elements new_list -> elements
    
    if (!(elements)) new_list -> list_status = CREATION_FAILED_NOT_ENOUGH_MEMORY;
    
    else{        
        elements[0].value = NAN;
        elements[0].next = 0;
        elements[0].prev = 0;
        for (int i = 1; i < capacity; i++){ 
            elements[i].value = NAN;
            elements[i].status = FREE;
            elements[i].prev = -1;
            elements[i].next = i+1;
        }
        elements[capacity - 1].next = 0;
    }
    #undef elements
    return new_list;
}

//TEMPORARY FOR DUMP TESTING

#define elements    thou -> elements
#define capacity    thou -> capacity
#define size        thou -> size
#define head        thou -> head
#define tail        thou -> tail
#define free_head   thou -> free_head
#define list_status thou -> list_status
#define exists      thou -> exists
#define isOrdered   thou -> isOrdered

int FindFree(List* thou){
    LIST_ASSERT
    if(free_head == 0) list_status = ERROR_ON_FREE_ELEMENT_SEARCH;
    LIST_ASSERT
    return free_head;
}


int ResizeUp(List* thou){
    
    LIST_ASSERT
    
    int new_capacity = capacity*2;
    Element* resized = (Element*)realloc(elements, sizeof(Element)*new_capacity);
    if (!resized){
        list_status = OVERFLOW;
        return 0;
    }
    elements = resized;
    for (int nelement = capacity; nelement < new_capacity; nelement++){
        elements[nelement].value = NAN;
        elements[nelement].status = FREE;
        elements[nelement].prev = -1;
        elements[nelement].next = nelement+1;
    }
    elements[new_capacity - 1].next = free_head;
    free_head = capacity;
    capacity = new_capacity;

    LIST_ASSERT

    return 1;
}


int InsertAfter(List* thou, int pos, double value){

    LIST_ASSERT

    if (elements[pos].status == FREE || pos > capacity || pos == 0 && size != 0) return 0;

    if (free_head == 0){
        int STATUS_AFTER_RESIZE = ResizeUp(thou);
        if(STATUS_AFTER_RESIZE != 1){
            return STATUS_AFTER_RESIZE;
        }
    }
    
    int new_pos = FindFree(thou);
    if (new_pos < 0) return new_pos;
    free_head = elements[new_pos].next;
    
    elements[new_pos].next = elements[pos].next;
    elements[new_pos].prev = pos;
    NEXT_OF(pos).prev = new_pos;
    elements[pos].next = new_pos;
    elements[new_pos].value = value;
    elements[new_pos].status = BUSY;
    if (pos == tail) tail = new_pos;
    else if (size > 0) isOrdered = 0;
    size++;
    LIST_ASSERT

    return new_pos; 
}

int InsertBefore(List* thou, int pos, double value){
    
    LIST_ASSERT

    if (elements[pos].status == FREE || pos > capacity || pos == 0 && size != 0) return 0;

    if (free_head == 0){
        int STATUS_AFTER_RESIZE = ResizeUp(thou);
        if(STATUS_AFTER_RESIZE != 1){
            return STATUS_AFTER_RESIZE;
        }
    }
    
    int new_pos = FindFree(thou);
    
    if (new_pos < 0) return new_pos; 
    free_head = elements[new_pos].next;
    elements[new_pos].prev = elements[pos].prev;
    elements[new_pos].next = pos;
    PREV_OF(pos).next = new_pos;
    elements[pos].prev = new_pos;
    elements[new_pos].value = value;
    elements[new_pos].status = BUSY;
    size++;
    if (pos == head) head = new_pos;
    isOrdered = 0;
    LIST_ASSERT

    return new_pos; 
}

int Delete(List* thou, int pos){
    
    LIST_ASSERT

    if (elements[pos].status == FREE) return 0;
    
    if (pos != tail) isOrdered = 0;
    
    if (pos == head) head = elements[pos].next;
    else if (pos == tail) tail = elements[pos].prev;
    else PREV_OF(pos).next = elements[pos].next;
    
    NEXT_OF(pos).prev    = elements[pos].prev;
    elements[pos].value  = NAN;
    elements[pos].prev   = -1;
    elements[pos].status = FREE;
    
    elements[pos].next   = free_head;
    free_head            = pos;
    size--;
    
    if (size == 0){
        isOrdered = 1;
        head      = 1;
        tail      = 1;
    }
    LIST_ASSERT

    return 1;
}

int DeleteList(List* thou){
    
    LIST_ASSERT

    free(elements);
    free(thou);
    return 1;
}

int LogicalComparator(const void* a, const void* b){
    
    if ((((Element*)a) -> prev) == -1 && (((Element*)b) -> prev) != -1) return 1;
    if ((((Element*)a) -> prev) != -1 && (((Element*)b) -> prev) == -1) return -1;
    return (((Element*)a) -> prev) - (((Element*)b) -> prev); 
}

int LogicalOrdering(List* thou){
    
    LIST_ASSERT
    
    if (isOrdered) return 0;
    
    Element* elements_buffer = (Element*)calloc(sizeof(Element), capacity);
    
    elements_buffer[0].value = NAN;
    elements_buffer[0].next = -1;
    elements_buffer[0].prev = -1;
    int nelement = head;

    for (int i = 1; i < size + 1; i++){
        elements_buffer[i].value  = elements[nelement].value;
        elements_buffer[i].next   = i+1;
        elements_buffer[i].prev   = i-1;
        elements_buffer[i].status = BUSY;
        nelement = elements[nelement].next;
    }
    for (int i = size + 1; i < capacity; i++){
        elements_buffer[i].status = FREE;
        elements_buffer[i].next = i + 1;
        elements_buffer[i].prev = -1;
        elements_buffer[i].value = NAN;
    }
    
    head = 1;
    tail = size;
    free_head = size + 1;

    elements_buffer[head].prev = 0;
    elements_buffer[tail].next = 0;
    elements_buffer[capacity - 1].next = 0;
    isOrdered = 1;
    free(elements);
    elements = elements_buffer;
    
    LIST_ASSERT

    return 1;
}

int PhysIndexFromLogic(List* thou, int logic_index){

    LIST_ASSERT
    
    if (isOrdered) return logic_index;
    
    if (logic_index > size - 1) return -1;
    int phys_index = head;
    for(int i = 0; i < logic_index; i++) phys_index = elements[phys_index].next;
    return phys_index;
}

int LogicIndexFromPhys(List* thou, int phys_index){

    LIST_ASSERT
    
    if (isOrdered) return phys_index;
    if (phys_index > capacity - 1) return -1;
    
    int logic_index = 0;
    for(; elements[phys_index].prev != 0; phys_index = elements[phys_index].prev) logic_index++;
    return logic_index;
}

int Search(List* thou, double value){
    
    LIST_ASSERT;
    
    int nelement = head;
    for (int i = 0; i < size; i++, nelement = elements[nelement].next){
        if (elements[nelement].value == value) return i;
    }
    return -1;
}

void ListPrint(List* thou){
    for (int i = 0; i < capacity; i++) printf("%lf ", elements[i].value);
    printf("\n");
    for (int i = 0; i < capacity; i++) printf("%d ", elements[i].next);
    printf("\n");
    for (int i = 0; i < capacity; i++) printf("%d ", elements[i].prev);
    printf("\n--------------------\n");
}

void GraphicalDump(List* thou){
    FILE* fp = fopen("show", "w");
    const char* color = "white";
    if (isOrdered) color = "aquamarine";
    else color = "lemonchiffon";
    fprintf(fp, "digraph structs {\nrankdir=LR rank=same;\n");
    fprintf(fp, "%d [ style=invis ];", capacity);
    fprintf(fp, "%d [ style=invis ];", 0);
    fprintf(fp, "subgraph cluster {\nstyle=filled;\ncolor=%s;\n", color);
    for (int nelement = 1; nelement < capacity; nelement++){

        if(elements[nelement].status == FREE) color = "chartreuse";
        if(elements[nelement].status == BUSY) color = "deepskyblue";
        if(elements[nelement].status == ERROR_BROKEN_NUMERATION) color = "red";
        fprintf(fp, "%d [shape=record, fillcolor=%s rank = same style=filled label=\"    %lf | {  %d | %d | %d }\" ];\n", nelement, color, elements[nelement].value, elements[nelement].prev,
        nelement, elements[nelement].next);
        fprintf(fp, "%d -> %d [ style = invis rank = same]; \n", nelement, nelement + 1);
        if (nelement != tail && elements[nelement].status == BUSY) fprintf(fp, "%d -> %d [constraint=false color=%s]; \n", nelement, elements[nelement].next, color);
    }
    fprintf(fp, "}\n");
    for (int nelement = 1; nelement < capacity; nelement++){
        if (elements[nelement].status == FREE && elements[nelement].next != 0) fprintf(fp, "%d -> %d [constraint=false color=grey]; \n", nelement, elements[nelement].next);
    }
    fprintf(fp, "HEAD -> %d\n", head);
    fprintf(fp, "TAIL -> %d\n", tail);
    fprintf(fp, "FREE_HEAD -> %d\n", free_head);
    fprintf(fp, "}");
    fclose(fp);
    system("dot -Tpng show -n -o show.png");
    system("viewnior show.png");
}

LIST_STATUS ValidateData(List* thou){
    
    unsigned char numeration_is_broken = 0;
    int sum = 0;

    for (int nelement = 1; nelement < capacity; nelement++){
        if (elements[nelement].status == FREE) sum++;
        if (PREV_OF(nelement).next != NEXT_OF(nelement).prev && elements[nelement].status == BUSY && nelement != head && nelement != tail){
            NEXT_OF(nelement).status  = ERROR_BROKEN_NUMERATION;
            PREV_OF(nelement).status  = ERROR_BROKEN_NUMERATION;
            elements[nelement].status = ERROR_BROKEN_NUMERATION;
            numeration_is_broken = 1;
        }
    }

    if (numeration_is_broken) list_status = BROKEN_NUMERATION;
    if (sum != capacity - size - 1) list_status = NOT_RIGHT_QUANTITY_OF_FREE_ELEMENTS;
    return list_status;
}

LIST_STATUS ValidateList(List* thou){
    if (!thou)                       return LIST_PTR_UNAVAILABLE;
    else if (!exists)         list_status = LIST_ALREADY_DEAD;
    else if (!elements)       list_status = DATA_PTR_UNAVAILABLE;
    else if (size < 0)        list_status = SIZE_LESS_ZERO;
    else if (size > capacity) list_status = SIZE_GREATER_THAN_CAPACITY;
    else if (free_head < 0)   list_status = FREE_HEAD_LESS_ZERO;
    else if (head < 1)        list_status = HEAD_LESS_ONE;
    else if (tail < 1)        list_status = TAIL_LESS_ONE;
    else ValidateData(thou);
    return list_status;
}

int main(){
    List* thou = NewList(10);
    GraphicalDump(thou);
    for (int i = 0; i < capacity - 5; i++){
        InsertAfter(thou, i, i + 1);
        GraphicalDump(thou);
    }
    char* command = (char*)calloc(10,1);
    int pos = 0;
    double value = 0;
    while (true){
        scanf("%s %d %lf", command, &pos, &value);
        if (!strcmp(command, "ia"))   InsertAfter(thou, pos, value);
        if (!strcmp(command, "ib"))   InsertBefore(thou, pos, value);
        if (!strcmp(command, "del"))  Delete(thou, pos);
        if (!strcmp(command, "rup"))  ResizeUp(thou);
        if (!strcmp(command, "pi"))   printf("%d\n", PhysIndexFromLogic(thou, pos));
        if (!strcmp(command, "li"))   printf("%d\n", LogicIndexFromPhys(thou, pos));
        if (!strcmp(command, "sort")) LogicalOrdering(thou);
        if (!strcmp(command, "srch")) printf("%d\n", Search(thou, value));
        //ListPrint(thou);
        GraphicalDump(thou);
    }
    return 0;
}
