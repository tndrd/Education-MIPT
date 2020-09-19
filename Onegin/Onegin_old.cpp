#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>


#define GREATER 1
#define LESS -1
#define EQUAL 0
/*
void add_line(/*char* line, char*** lines_ptr, int counter){
    //int new_size;
    int new_size = sizeof(char*)*(counter+1);
    //int new_size = sizeof(char*)*(counter+1);
    //printf("%d",new_size);
    *lines_ptr = (char**)realloc(*lines_ptr, new_size);
     //int new_index = new_size/sizeof(char*);
    //*lines_ptr[counter] = line;


void add_char(char toAdd, char** line_ptr,int counter){
    //int new_size;
    int new_size = counter;
    line_ptr = (char**)realloc(line_ptr, new_size);


}
*/



char* next_letter(char* a){

    if (*a == '\0'){
        return nullptr;
    }

    else if (isalpha(*a)){
        return a;
    }
    else{
        return next_letter(a+1);
    }
}

int lexicographicalCompare(char* a, char* b){
    a = next_letter(a);
    b = next_letter(b);
    //printf("comparing %c and %c \n", *a, *b);
    if (a == nullptr || b == nullptr){

        if (a == nullptr && b == nullptr){
            return EQUAL;
        }

        if (a == nullptr && b != nullptr){
            return LESS;
        }

        if (a != nullptr && b == nullptr){
            return GREATER;
        }

    }

    if (*a > *b) {
        return GREATER;
    }
    else if (*a < *b) {
        return LESS;
    }
    else {
        return lexicographicalCompare(a+1,b+1);
    }

}

char** getLines(char* name, int* number_of_lines){

    FILE* fp = fopen("input1.txt", "r");
    char* line = (char*)malloc(0);
    char** lines  = (char**)malloc(0);
    int char_counter = 0;
    int line_counter = 0;

    char current = getc(fp);

    while (current != EOF){
        //printf("%c",current);
        if (current == '\n'){

            lines = (char**)realloc(lines, (line_counter+1) * sizeof(char*));
            lines[line_counter] = line;
            line_counter++;
            printf(lines[0]);

            line = (char*)malloc(0);
            char_counter = 0;
        }

        else {
            line = (char*)realloc(line, char_counter+1);
            line[char_counter] = current;
            char_counter++;
        }
        current = getc(fp);
    }
    *number_of_lines = line_counter;
    return lines;
}

int main(){
    int line_counter = 0;
    char **lines = getLines("asd", &line_counter);

    int indexes[line_counter+1];
    for (int i = 0; i < line_counter; i++){
            indexes[i] = i;
    }

    for (int i = 0; i < line_counter; i++){
        for(int j = 0; j < line_counter-1; j++){
            int comparison = lexicographicalCompare(lines[indexes[j]], lines[indexes[j+1]]);
            if (comparison == GREATER){
                int t = indexes[j];
                indexes[j] = indexes[j+1];
                indexes[j+1] = t;
            }
        }

    }

    printf("----------\n");
    for (int i = 0; i < line_counter; i++){
            printf("%s\n",lines[indexes[i]]);
    }

    return 0;
}
