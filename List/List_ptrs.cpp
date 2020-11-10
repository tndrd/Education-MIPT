
#include "List.h"

#define NEXT_OF(element) (element -> next)
#define PREV_OF(element) (element -> prev)

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
    
    new_list -> name        = name;
    new_list -> capacity    = capacity;
    new_list -> size        = 0;
    new_list -> head        = nullptr;
    new_list -> tail        = nullptr;
    new_list -> list_status = OK;
    new_list -> isOrdered   = 1;
    
    Element* NewElements = (Element*)calloc(capacity, sizeof(Element));

    if (!(NewElements)) new_list -> list_status = CREATION_FAILED_NOT_ENOUGH_MEMORY;

    for (int i = 0; i < capacity; i++){ 
        NewElements[i].value = NAN;
        NewElements[i].prev  =  -1;
        NewElements[i].next  = NewElements + i + 1;
    }

    NewElements[capacity - 1].next = 0;
    new_list -> free_head = NewElements;

    return new_list;
}

#define capacity     thou -> capacity
#define size         thou -> size
#define head         thou -> head
#define tail         thou -> tail
#define free_head    thou -> free_head
#define list_status  thou -> list_status
#define exists       thou -> exists
#define isOrdered    thou -> isOrdered
#define list_name    thou -> name
#define new_elem_ptr thou -> new_elem_ptr

LIST_STATUS FindFree(List* thou){
    
    LIST_ASSERT
    
    if(free_head == nullptr) {
        list_status = ERROR_ON_FREE_ELEMENT_SEARCH;
        return ERROR_ON_FREE_ELEMENT_SEARCH;
    }
    
    thou -> new_elem_ptr =         free_head;
    free_head            = free_head -> next;

    LIST_ASSERT

    return OK;
}


LIST_STATUS ResizeUp(List* thou){
    
    LIST_ASSERT

    Element* NewElements = (Element*)calloc(capacity, sizeof(Element));
    
    if (!NewElements){
        list_status = OUT_OF_MEMORY;
        return list_status;
    }
    
    for (int nelement = 0; nelement < new_capacity; nelement++){
        NewElements[nelement].value = NAN;
        NewElements[nelement].prev  = nullptr;
        NewElements[nelement].next  = NewElements + nelement + 1;
    }
    
    NewElements[capacity - 1].next = free_head;
    
    free_head = NewElements;
    capacity  = capacity * 2;

    LIST_ASSERT

    return OK;
}


LIST_STATUS InsertFirst(List* thou, double value){
    
    LIST_ASSERT

    if (FindFree(thou) != OK) return list_status;
    
    new_elem_ptr -> next  = nullptr;
    new_elem_ptr -> prev  = nullptr;
    new_elem_ptr -> value = value;
    
    tail = new_elem_ptr;
    head = new_elem_ptr;
    
    size++;
    isOrdered = 1;
    
    LIST_ASSERT
    return OK;
}


LIST_STATUS InsertAfterTail(List* thou, double value){
    
    LIST_ASSERT

    if (FindFree(thou) != OK) return list_status;
    
    new_elem_ptr -> next  = nullptr;
    new_elem_ptr -> prev  = tail;
    tail         -> next  = new_pos_ptr;
    new_elem_ptr -> value = value;
    
    tail = new_elem_ptr;
    size++;
    
    LIST_ASSERT
    return OK;
}  


LIST_STATUS InsertAfter(List* thou, Element* element_ptr, double value){

    LIST_ASSERT

    if (size == 0 && element_ptr == head) return InsertFirst (thou, value);
    if (isnan(element_ptr -> value))      return WRONG_ELEMENT;

    if (free_head == nullptr){
        if (ResizeUp(thou) != OK) return list_status;
    }

    if (pos == tail) return InsertAfterTail(thou, value);
    
    if (FindFree(thou) != OK) return list_status;
    
    new_elem_ptr         -> next    = element_ptr -> next;
    new_elem_ptr         -> prev    = element_ptr;
    NEXT_OF(element_ptr) -> prev    = new_elem_ptr;
    element_ptr          -> next    = new_pos;
    new_elem_ptr         -> value   = value;
    
    isOrdered = 0;
    size++;
    
    LIST_ASSERT
    return OK; 
}


LIST_STATUS InsertBeforeHead(List* thou, double value){
    
    LIST_ASSERT
    
    if (FindFree(thou) != OK) return list_status;
    
    new_elem_ptr -> prev  = nullptr;
    new_elem_ptr -> next  = head;
    head         -> prev  = new_pos;
    new_elem_ptr -> value = value;
    
    size++;
    head = new_elem_ptr;
    isOrdered = 0;
    
    LIST_ASSERT

    return OK;
}


LIST_STATUS InsertBefore(List* thou, Element* element_ptr, double value){
    
    LIST_ASSERT

    if (size == 0 && element_ptr == head) return InsertFirst (thou, value);
    if (isnan(element_ptr -> value))      return WRONG_ELEMENT;

    if (free_head == nullptr){
        if (ResizeUp(thou) != OK) return list_status;
    }

    if (pos == head) return InsertBeforeHead(thou, value);
    
    if (FindFree(thou) != OK) return list_status;
    
    new_element_ptr      -> prev   = element_ptr -> prev;
    new_element_ptr      -> next   = element_ptr;
    PREV_OF(element_ptr) -> next   = new_elem_ptr;
    element_ptr          -> prev   = new_pos;
    new_element_ptr      -> value  = value;
    
    size++;
    isOrdered = 0;
    
    LIST_ASSERT
    return OK; 
}


LIST_STATUS DeleteTail(List* thou){
    
    LIST_ASSERT

    Element* old_tail = tail;
    tail              = tail -> prev;

    old_tail -> value = NAN;
    old_tail -> prev  = nullptr;

    old_tail -> next  = free_head;
    free_head         = old_tail;
    
    tail -> next      = nullptr;
    
    size--;
    
    if (size == 0){
        isOrdered = 1;
        head      = nullptr;
        tail      = nullptr;
    }
    
    LIST_ASSERT
    return OK;
}


LIST_STATUS DeleteHead(List* thou){

    LIST_ASSERT

    Element* old_head = head;
    head              = head -> next;

    old_head -> value  = NAN;
    old_head -> prev   = nullptr;

    old_head -> next   = free_head;
    free_head          = old_head;
    
    head -> prev       = 0;
    
    size--;
    isOrdered = 0;

    LIST_ASSERT
    return OK;

}


LIST_STATUS Delete(List* thou, Element* element_ptr){
    
    LIST_ASSERT

    if (element_ptr == tail) return DeleteTail(thou);
    if (element_ptr == head) return DeleteHead(thou);
    
    if (isnan(element_ptr -> value)) return 0;
    
    PREV_OF(element_ptr).next    = element_ptr -> next;
    NEXT_OF(element_ptr).prev    = element_ptr -> prev;
    
    element_ptr -> value  = NAN;
    element_ptr -> prev   = -1;
    
    element_ptr -> next   = free_head;
    free_head             = element_ptr;
    
    isOrdered = 0;
    size--;

    LIST_ASSERT
    return OK;
}


LIST_STATUS DeleteList(List* thou){ // TODO
    
    LIST_ASSERT
    //free(elements);
    free(thou);
    return OK;
}


LIST_STATUS LogicalOrdering(List* thou){
    
    LIST_ASSERT
    
    if (isOrdered) return ALREADY_ORDERED;
    
    Element* elements_buffer = (Element*)calloc(sizeof(Element), capacity);
    
    elements_buffer[0].value = NAN;
    elements_buffer[0].next  = nullptr;
    elements_buffer[0].prev  = nullptr;
    
    Element* current_element = head;

    for (int i = 1; i < size + 1; i++){
        elements_buffer[i].value  = current_element -> value;
        elements_buffer[i].next   = elements_buffer + i + 1;
        elements_buffer[i].prev   = elements_buffer + i - 1;
        
        current_element = current_element -> next;
    }
    for (int i = size + 1; i < capacity; i++){
        elements_buffer[i].next  = elements_buffer + i + 1;
        elements_buffer[i].prev  = nullptr;
        elements_buffer[i].value = NAN;
    }
    
    head      = elements_buffer + 1;
    tail      = elements_buffer + size;
    free_head = elements_buffer + size + 1;

    head -> prev         = nullptr;
    tail -> next         = nullptr;
    elements_buffer[capacity - 1].next = nullptr; //free_tail
    
    isOrdered = 1;

    //TODO Free old elements
    //free(elements);
    //elements = elements_buffer;
    
    LIST_ASSERT
    return OK;
}


LIST_STATUS GetElement(List* thou, int logic_index, Element** element_ptr){

    LIST_ASSERT
    
    if (logic_index > size - 1 || logic_index < 1) return WRONG_ELEMENT;
    
    *element_ptr = head;

    for(int i = 0; i < logic_index; i++) *element_ptr = *(element_ptr) -> next;
    
    return OK;
}

LIST_STATUS Search(List* thou, double value, SearchResult* found){
    
    LIST_ASSERT;
    
    int nelement = head;
    
    for (int i = 0; i < size; i++, nelement = elements[nelement].next){
        if (elements[nelement].value == value){
            found -> logic_index = i;
            found -> phys_index  = nelement;
            return 0;
        }
    }

    return SEARCH_NO_RESULTS;
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
        
        else if (isnan(elements[nelement].value)) color = "chartreuse";
        else                                      color = "deepskyblue";
        
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
    else              fprintf(fp, "\"%s\"",    list_name);
    
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

LIST_STATUS ValidateList(List* thou){
    
    if (!thou)                       return LIST_PTR_UNAVAILABLE;
    
    else if (!list_name)      list_status = LIST_NAME_ERROR;
    else if (capacity < 0)    list_status = CAPACITY_LESS_ZERO;
    else if (size < 0)        list_status = SIZE_LESS_ZERO;
    else if (size > capacity) list_status = SIZE_GREATER_THAN_CAPACITY;    
    
    return list_status;
}


int main(){
    
    CREATE_LIST(thou, 10)

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

        GraphicalDump(thou);
    }

    return 0;
}
