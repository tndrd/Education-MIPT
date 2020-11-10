
#include "List.h"

#define LIST_ASSERT                                                                                              \
    if (ValidateList(thou) == LIST_PTR_UNAVAILABLE) return LIST_PTR_UNAVAILABLE;                                 \
    else if (list_status != OK){                                                                                 \
        printf("List error: %s (dumped)\n", GET_ERROR_NAME(list_status));                                        \
        if (elements) GraphicalDump(thou);                                                                       \
        TextDump(thou);                                                                                          \
        return list_status;                                                                                      \
    }                                                                                                            \


#define CASE_ERROR(error_code) case error_code: return #error_code;

const char* GET_ERROR_NAME(LIST_STATUS status){
    
    switch(status){
        CASE_ERROR(OK)
        CASE_ERROR(WRONG_QUANTITY_OF_FREE_ELEMENTS)
        CASE_ERROR(OUT_OF_MEMORY)
        CASE_ERROR(CREATION_FAILED_NOT_ENOUGH_MEMORY)
        CASE_ERROR(LIST_NAME_ERROR)
        CASE_ERROR(LIST_PTR_UNAVAILABLE)
        CASE_ERROR(DATA_PTR_UNAVAILABLE)
        CASE_ERROR(SIZE_LESS_ZERO)
        CASE_ERROR(SIZE_GREATER_THAN_CAPACITY)
        CASE_ERROR(BROKEN_NUMERATION)
        CASE_ERROR(FREE_HEAD_LESS_ZERO)
        CASE_ERROR(TAIL_LESS_ZERO)
        CASE_ERROR(HEAD_LESS_ZERO)
        CASE_ERROR(CAPACITY_LESS_ZERO)
        CASE_ERROR(ERROR_ON_FREE_ELEMENT_SEARCH)
        default: return "Unknown error";
    }

}

#define CREATE_LIST(name, capacity)\
    List* name = NewList( #name, capacity );

List* NewList(const char* name, int capacity){
    
    List* new_list = (List*)calloc(1, sizeof(List));
    if (!new_list || !name) return nullptr;
    
    new_list -> name = name;
    new_list -> capacity = capacity;
    new_list -> size = 0;
    new_list -> list_status = OK;
    new_list -> isOrdered = 1;
    
    Element* new_Elements = (Element*)calloc(capacity, sizeof(Element)); 
    
    new_list -> head = nullptr;
    new_list -> tail = nullptr;
    new_list -> free_head = new_Elements;

    if (!(new_Elements)) new_list -> list_status = CREATION_FAILED_NOT_ENOUGH_MEMORY;
    else{        
        for (int i = 0; i < capacity; i++){ 
            new_Elements[i].value = NAN;
            new_Elements[i].prev = nullptr;
            new_Elements[i].next = new_Elements + i + 1;
        }
        new_Elements[capacity - 1].next = nullptr;
        new_Elements[0].prev = 0;
    }
    
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
#define list_name   thou -> name

int ResizeUp(List* thou){
    
    LIST_ASSERT
    
    int new_capacity = capacity*2;
    Element* resized = (Element*)realloc(elements, sizeof(Element)*new_capacity);
    if (!resized){
        list_status = OUT_OF_MEMORY;
        return list_status;
    }
    elements = resized;
    for (int nelement = capacity; nelement < new_capacity; nelement++){
        elements[nelement].value = NAN;
        elements[nelement].prev = nullptr;
        elements[nelement].next = elements + nelement + 1;
    }
    elements[new_capacity - 1].next = free_head;
    free_head = elements + capacity;
    capacity = new_capacity;

    LIST_ASSERT

    return 0;
}

int InsertFirst(List* thou, double value){
    
    LIST_ASSERT

    Element* new_pos = FindFree(thou);
    if (new_pos < 0) return new_pos;
    free_head = elements[new_pos].next;
    
    elements[new_pos].next = 0;
    elements[new_pos].prev = 0;
    elements[new_pos].value = value;
    
    tail = new_pos;
    head = new_pos;
    size++;
    isOrdered = 1;
    LIST_ASSERT
    return new_pos;
}

int InsertAfterTail(List* thou, double value){
    
    LIST_ASSERT

    int new_pos = FindFree(thou);
    if (new_pos < 0) return new_pos;
    free_head = elements[new_pos].next;
    
    elements[new_pos].next = 0;
    elements[new_pos].prev = tail;
    elements[tail].next = new_pos;
    elements[new_pos].value = value;
    
    tail = new_pos;
    size++;
    LIST_ASSERT
    return new_pos;
}  

int InsertAfter(List* thou, int pos, double value){

    LIST_ASSERT

    if (pos > capacity) return 0;

    if (free_head == 0){
        int STATUS_AFTER_RESIZE = ResizeUp(thou);
        if(STATUS_AFTER_RESIZE != 0){
            return list_status;
        }
    }

    if      (size ==    0) return InsertFirst    (thou, value);
    else if (pos  == tail) return InsertAfterTail(thou, value);
    

    if (isnan(elements[pos].value)) return 0;

    int new_pos = FindFree(thou);
    if (new_pos < 0) return new_pos;
    free_head = elements[new_pos].next;
    
    elements[new_pos].next = elements[pos].next;
    elements[new_pos].prev = pos;
    NEXT_OF(pos).prev = new_pos;
    elements[pos].next = new_pos;
    elements[new_pos].value = value;
    
    isOrdered = 0;
    
    size++;
    LIST_ASSERT

    return new_pos; 
}


int InsertBeforeHead(List* thou, double value){
    
    int new_pos = FindFree(thou);
    
    if (new_pos < 0) return new_pos; 
    free_head = elements[new_pos].next;
    elements[new_pos].prev = 0;
    elements[new_pos].next = head;
    elements[head].prev = new_pos;
    elements[new_pos].value = value;
    size++;
    head = new_pos;
    isOrdered = 0;
    LIST_ASSERT

    return new_pos;
}

int InsertBefore(List* thou, int pos, double value){
    
    LIST_ASSERT

    if (pos > capacity) return 0;

    if (free_head == 0){
        int STATUS_AFTER_RESIZE = ResizeUp(thou);
        if(STATUS_AFTER_RESIZE != 0){
            return list_status;
        }
    }

    if      (size ==    0) return InsertFirst     (thou, value);
    else if (pos  == head) return InsertBeforeHead(thou, value);
    

    if (isnan(elements[pos].value)) return 0;
    
    int new_pos = FindFree(thou);
    
    if (new_pos < 0) return new_pos; 
    free_head = elements[new_pos].next;
    elements[new_pos].prev = elements[pos].prev;
    elements[new_pos].next = pos;
    PREV_OF(pos).next = new_pos;
    elements[pos].prev = new_pos;
    elements[new_pos].value = value;
    size++;
    if (pos == head) head = new_pos;
    isOrdered = 0;
    LIST_ASSERT

    return new_pos; 
}

int DeleteTail(List* thou){
    
    LIST_ASSERT
    int old_tail = tail;

    elements[old_tail].value  = NAN;
    elements[old_tail].next   = free_head;
    free_head                 = old_tail;
    tail                      = elements[tail].prev;
    elements[old_tail].prev   = -1;
    elements[tail].next = 0;
    size--;
    
    if (size == 0){
        isOrdered = 1;
        head      = 0;
        tail      = 0;
    }
    LIST_ASSERT

    return 1;


}

int DeleteHead(List* thou){

    LIST_ASSERT

    int old_head = head;

    elements[old_head].value  = NAN;
    head                      = elements[head].next;
    elements[old_head].next   = free_head;
    free_head                 = old_head;
    
    elements[old_head].prev   = -1;
    elements[head].prev = 0;
    size--;

    LIST_ASSERT
    return 1;

}

int Delete(List* thou, int pos){
    
    LIST_ASSERT
    if (pos == tail) return DeleteTail(thou);
    if (pos == head) return DeleteHead(thou);
    
    if (isnan(elements[pos].value)) return 0;
    
    PREV_OF(pos).next = elements[pos].next;
    NEXT_OF(pos).prev    = elements[pos].prev;
    elements[pos].value  = NAN;
    elements[pos].prev   = -1;
    
    elements[pos].next   = free_head;
    free_head            = pos;
    
    isOrdered = 0;
    size--;
    LIST_ASSERT

    return 1;
}

int DeleteList(List* thou){
    
    LIST_ASSERT
    if (!list_name) return 0;
    free(elements);
    free(thou);
    return 1;
}


int LogicalOrdering(List* thou){
    
    LIST_ASSERT
    
    if (isOrdered) return 0;
    
    Element* elements_buffer = (Element*)calloc(sizeof(Element), capacity);
    
    elements_buffer[0].value = NAN;
    elements_buffer[0].next = 0;
    elements_buffer[0].prev = 0;
    int nelement = head;

    for (int i = 1; i < size + 1; i++){
        elements_buffer[i].value  = elements[nelement].value;
        elements_buffer[i].next   = i+1;
        elements_buffer[i].prev   = i-1;
        nelement = elements[nelement].next;
    }
    for (int i = size + 1; i < capacity; i++){
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

int Search(List* thou, double value, SearchResult* found){
    
    LIST_ASSERT;
    
    int nelement = head;
    for (int i = 0; i < size; i++, nelement = elements[nelement].next){
        if (elements[nelement].value == value){
            found -> logic_index = i;
            found -> phys_index = nelement;
        }
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
    
    if   (isOrdered) color  =   "aquamarine";
    else             color  = "lemonchiffon";
    
    fprintf(fp, "digraph structs {\nrankdir=LR rank=same;\n");
    fprintf(fp, "%d [ style=invis ];", capacity);
    fprintf(fp, "%d [ style=invis ];", 0);
    fprintf(fp, "subgraph cluster {\nstyle=filled;\ncolor=%s;\n", color);
    fprintf(fp, "%d [shape=record, fillcolor=lightgrey rank = same style=filled label=\"    %lf | {  %d | %d | %d }\" ];\n",
    0, elements[0].value, elements[0].prev, 0, elements[0].next);
    fprintf(fp, "0 -> 1 [ style = invis rank = same]; \n");
    for (int nelement = 1; nelement < capacity; nelement++){

        if      (PREV_OF(nelement).next != NEXT_OF(nelement).prev && nelement != head && nelement != tail && !isnan(elements[nelement].value)) color = "red";
        else if                     (isnan(elements[nelement].value))                                       color = "chartreuse";
        else                                                                                                color = "deepskyblue";
        
        fprintf(fp, "%d [shape=record, fillcolor=%s rank = same style=filled label=\"    %lf | {  %d | %d | %d }\" ];\n", 
        nelement, color, elements[nelement].value, elements[nelement].prev, nelement, elements[nelement].next);
        
        fprintf(fp, "%d -> %d [ style = invis rank = same]; \n", nelement, nelement + 1);
        if (nelement != tail && !isnan(elements[nelement].value))
        fprintf(fp, "%d -> %d [constraint=false color=%s]; \n", nelement, elements[nelement].next, color);
    }
    fprintf(fp, "}\n");
    for (int nelement = 1; nelement < capacity; nelement++){
        if (isnan(elements[nelement].value) && elements[nelement].next != 0) fprintf(fp, "%d -> %d [constraint=false color=grey]; \n", nelement, elements[nelement].next);
    }
    fprintf(fp, "HEAD -> %d\n", head);
    fprintf(fp, "TAIL -> %d\n", tail);
    fprintf(fp, "FREE_HEAD -> %d\n", free_head);
    fprintf(fp, "}");
    fclose(fp);
    system("dot -Tpng show -n -o show.png");
    system("viewnior show.png");
}


int TextDump(List* thou){
    
    FILE* fp = fopen(list_name, "w");
    assert(fp); 

    fprintf(fp, "List ");

    if   (!list_name) fprintf(fp, "\"(nil)\"");
    else fprintf(fp, "\"%s\"",    list_name);
    
    fprintf(fp, "[%p](%s):\n", thou, GET_ERROR_NAME(list_status));

    fprintf(fp, "  capacity: %d,\n  size: %d,\n  head: %d,\n",   capacity,      size,      head);
    fprintf(fp, "  tail: %d,\n  free_head: %d,\n  isOrdered: %d,\n", tail, free_head, isOrdered);
    
    fprintf(fp, "  Elements[%p]: \n", elements);
    if (!elements) return 0;
    for (int i = 0; i < capacity; i++){
        fprintf(fp, "    [%d]: value = %lf; prev = %d; next = %d;\n", i, elements[i].value, elements[i].prev, elements[i].next);
    }

    fclose(fp);
    return 0;    
}

LIST_STATUS ValidateData(List* thou){
    
    unsigned char numeration_is_broken = 0;
    int sum = 0;

    for (int nelement = 1; nelement < capacity; nelement++){
        if (isnan(elements[nelement].value)) sum++;
        if (PREV_OF(nelement).next != NEXT_OF(nelement).prev && !isnan(elements[nelement].value) && nelement != head && nelement != tail){
            numeration_is_broken = 1;
        }
    }

    if (numeration_is_broken) list_status = BROKEN_NUMERATION;
    if (sum != capacity - size - 1) list_status = WRONG_QUANTITY_OF_FREE_ELEMENTS;
    return list_status;
}

LIST_STATUS ValidateList(List* thou){
    if (!thou)                       return LIST_PTR_UNAVAILABLE;
    else if (!list_name)           list_status = LIST_NAME_ERROR;
    else if (!elements)       list_status = DATA_PTR_UNAVAILABLE;
    else if (capacity < 0)    list_status = CAPACITY_LESS_ZERO;
    else if (size < 0)        list_status = SIZE_LESS_ZERO;
    else if (size > capacity) list_status = SIZE_GREATER_THAN_CAPACITY;
    else if (free_head < 0)   list_status = FREE_HEAD_LESS_ZERO;
    else if (head < 0)        list_status = HEAD_LESS_ZERO;
    else if (tail < 0)        list_status = TAIL_LESS_ZERO;
    else ValidateData(thou);
    return list_status;
}

int main(){
    List* thou = NewList("named",10);
    GraphicalDump(thou);
    for (int i = 0; i < capacity - 5; i++){
        printf("%d\n", InsertAfter(thou, i, i + 1));
        GraphicalDump(thou);
    }
    char* command = (char*)calloc(10,1);
    int pos = 0;
    double value = 0;
    while (true){
        scanf("%s %d %lf", command, &pos, &value);
        if (!strcmp(command, "ia"))   printf("%d\n", InsertAfter(thou, pos, value));
        if (!strcmp(command, "ib"))   InsertBefore(thou, pos, value);
        if (!strcmp(command, "del"))  Delete(thou, pos);
        if (!strcmp(command, "rup"))  ResizeUp(thou);
        if (!strcmp(command, "pi"))   printf("%d\n", PhysIndexFromLogic(thou, pos));
        if (!strcmp(command, "li"))   printf("%d\n", LogicIndexFromPhys(thou, pos));
        if (!strcmp(command, "sort")) LogicalOrdering(thou);
        //if (!strcmp(command, "srch")) printf("%d\n", Search(thou, value));
        //ListPrint(thou);
        GraphicalDump(thou);
    }
    return 0;
}
