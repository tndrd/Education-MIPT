#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#define NEXT_OF(i) elements[elements[i].next]
#define PREV_OF(i) elements[elements[i].prev]

enum ESTATUS{
    BUSY,
    FREE,
    ERROR_BROKEN_NUMERATION
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
    int free_element = 1;
};

void ValidateList(List* thou);

List* NewList(int capacity){
    
    List* new_list = (List*)calloc(1, sizeof(List));
    new_list -> capacity = capacity;
    new_list -> size = 0;
    new_list -> head = 1;
    new_list -> tail = 1;
    new_list -> elements = (Element*)calloc(capacity, sizeof(Element));
    
    #define elements new_list -> elements

    elements[0].value = NAN;
    elements[0].next = 0;
    elements[0].prev = 0;
    
    for (int i = 1; i < capacity; i++) elements[i].value = NAN;
    for (int i = 1; i < capacity; i++) elements[i].status = FREE;
    for (int i = 1; i < capacity; i++) elements[i].prev = -1;
    for (int i = 1; i < capacity; i++) elements[i].next = -1;
    
    elements[capacity - 1].next = 0;
    elements[1].prev = -1;

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
    for (int i = 1; i < capacity; i++){
        if (elements[i].prev == -1) return i;
    }
    return 0;
}

int InsertAfter(List* thou, int pos, double value){
    int new_pos = FindFree(thou);

    elements[new_pos].next = elements[pos].next;
    elements[new_pos].prev = pos;
    NEXT_OF(pos).prev = new_pos;
    elements[pos].next = new_pos;
    elements[new_pos].value = value;
    elements[new_pos].status = BUSY;
    size++;
    ValidateList(thou);
    return new_pos; 
}

void Delete(List* thou, int pos){
    PREV_OF(pos).next = elements[pos].next;
    NEXT_OF(pos).prev = elements[pos].prev;
    elements[pos].value = NAN;
    elements[pos].prev = -1;
    elements[pos].status = FREE;
    size--;
    ValidateList(thou);
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
    fprintf(fp, "digraph structs {\nrankdir=TD;\n");
    for (int nelement = 1; nelement < capacity; nelement++){
        
        if(elements[nelement].status == FREE) color = "green";
        if(elements[nelement].status == BUSY) color = "blue";
        if(elements[nelement].status == ERROR_BROKEN_NUMERATION) color = "red";
        
        fprintf(fp, "%d [shape=record, fillcolor=%s style=filled label=\"   { %lf | {  %d | %d | %d }}\" ];\n", nelement, color, elements[nelement].value, elements[nelement].prev,
        nelement, elements[nelement].next);
        //fprintf(fp, "%d:<n%d> -> %d:<p%d>", nelement, elements[nelement].next, elements[nelement].next, elements[nelement].prev);
        if (elements[nelement].prev != -1 && elements[nelement].next != 0) fprintf(fp, "%d -> %d\n", nelement, elements[nelement].next);
    }
    fprintf(fp, "}");
    fclose(fp);
    system("dot -Tpng show -o show.png");
    system("viewnior show.png");
}

void ValidateList(List* thou){
    int sum = 0;
    for (int nelement = 1; nelement < capacity; nelement++){
        if (elements[nelement].status == FREE) sum++;
        if (PREV_OF(nelement).next != NEXT_OF(nelement).prev && elements[nelement].status == BUSY){
            NEXT_OF(nelement).status = ERROR_BROKEN_NUMERATION;
            PREV_OF(nelement).status = ERROR_BROKEN_NUMERATION;
            elements[nelement].status = ERROR_BROKEN_NUMERATION;
        }
    }
    if (sum != capacity - size - 1) printf("Error: not right quantity of free elements!\n");


}


int main(){
    List* thou = NewList(10);
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
