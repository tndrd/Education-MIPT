#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#define NEXT_OF(i) elements[elements[i].next]
#define PREV_OF(i) elements[elements[i].prev]

enum ESTATUS{
    VACANT,
    BUSY,
    FREE,
    ERROR_BROKEN_NUMERATION
};

struct Element{
    ESTATUS status = VACANT;
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
    int free_element = 1;
};

void GraphicalDump(List* thou);


List* NewList(int capacity){
    
    List* new_list = (List*)calloc(1, sizeof(List));
    new_list -> capacity = capacity;
    new_list -> size = 0;
    new_list -> head = 1;
    new_list -> tail = 1;
    new_list -> elements = (Element*)calloc(capacity, sizeof(Element));
    new_list -> free_element = 1;
    
    #define elements new_list -> elements

    elements[0].value = NAN;
    elements[0].next = 0;
    elements[0].prev = 0;
    
    for (int i = 1; i < capacity; i++) elements[i].value = NAN;
    for (int i = 1; i < capacity; i++) elements[i].status = FREE;
    for (int i = 1; i < capacity; i++) elements[i].next = i + 1;
    for (int i = 2; i < capacity; i++) elements[i].prev = i - 1;
    elements[capacity - 1].next = 0;
    elements[1].prev = 0;

    #undef elements
    
    return new_list;
}

//TEMPORARY FOR DUMP TESTING

#define elements thou -> elements
#define capacity thou -> capacity
#define size thou -> size
#define head thou -> head
#define tail thou -> capacity
#define free_element thou -> free_element


int FindFree(List* thou){
    int old_free_element = free_element;
    free_element = elements[free_element].next;
    int abcd = PREV_OF(old_free_element).next;
    PREV_OF(old_free_element).next = NEXT_OF(old_free_element).prev;
    NEXT_OF(old_free_element).prev = abcd;
    return old_free_element;
}

int InsertAfter(List* thou, int pos, double value){
    int new_pos = FindFree(thou);
    printf("%d\n", new_pos);
    elements[new_pos].next = elements[pos].next;
    elements[new_pos].prev = pos;
    GraphicalDump(thou);
    NEXT_OF(pos).prev = new_pos;
    elements[pos].next = new_pos;
    elements[new_pos].value = value;
    elements[new_pos].status = BUSY;
    size++;
    return new_pos; 
}

void Delete(List* thou, int pos){
    PREV_OF(pos).next = elements[pos].next;
    NEXT_OF(pos).prev = elements[pos].prev;
    elements[pos].value = NAN;
    elements[pos].prev = free_element;
    elements[pos].next = elements[free_element].next;
    elements[free_element].next = pos;
    elements[pos].status = FREE;
    size--;
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
    char* color = "white";
    fprintf(fp, "digraph structs {\nrankdir=TD;\n");
    for (int nelement = 0; nelement < capacity; nelement++){
        if (elements[nelement].status == BUSY) color = "blue";
        if (elements[nelement].status == FREE) color = "green";
        if (elements[nelement].status == ERROR_BROKEN_NUMERATION) color = "red";

        fprintf(fp, "%d [shape=record, fillcolor=%s style=filled label=\"   { %lf | {  %d | %d | %d }}\" ];\n", nelement, color, elements[nelement].value, elements[nelement].prev,
        nelement, elements[nelement].next);
        //fprintf(fp, "%d:<n%d> -> %d:<p%d>", nelement, elements[nelement].next, elements[nelement].next, elements[nelement].prev);
        if (elements[nelement].status == BUSY) fprintf(fp, "%d -> %d\n", nelement, elements[nelement].next);
    }
    fprintf(fp, "}");
    fclose(fp);
    system("dot -Tpng show -o show.png");
    system("viewnior show.png");
}

void ValidateList(List* thou){
    for (int nelement = 2; nelement < capacity - 1; nelement++){
        if (PREV_OF(nelement).next != elements[nelement].prev){
            PREV_OF(nelement).status = ERROR_BROKEN_NUMERATION;
            elements[nelement].status = ERROR_BROKEN_NUMERATION;
        }
        if (NEXT_OF(nelement).prev != elements[nelement].next){
            NEXT_OF(nelement).status = ERROR_BROKEN_NUMERATION;
            elements[nelement].status = ERROR_BROKEN_NUMERATION;
        }
    }

}


int main(){
    List* thou = NewList(10);
    GraphicalDump(thou);
    for (;size < capacity - 3;){
        InsertAfter(thou, size, size + 1);
        GraphicalDump(thou);
    }
    InsertAfter(thou, 3, 15);
    GraphicalDump(thou);
    InsertAfter(thou, 7, 23);
    GraphicalDump(thou);
    Delete(thou, 3);
    GraphicalDump(thou);
    return 0;
}
