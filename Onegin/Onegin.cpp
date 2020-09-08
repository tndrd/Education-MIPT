#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>


#define GREATER 1
#define LESS -1
#define EQUAL 0

void add_line(/*char* line, */char*** lines_ptr, int counter){
    //int new_size;
    int new_size = sizeof(char*)*(counter+1);
    //int new_size = sizeof(char*)*(counter+1);
    //printf("%d",new_size);
    *lines_ptr = (char**)realloc(*lines_ptr, new_size);
     //int new_index = new_size/sizeof(char*);
    //*lines_ptr[counter] = line;
}

void add_char(char toAdd, char** line_ptr,int counter){
    //int new_size;
    int new_size = counter;
    line_ptr = (char**)realloc(line_ptr, new_size);
     //int new_index = new_size/sizeof(char);
     //printf("%d", new_index);
    //line_ptr[0] = toAdd;

}

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
        //printf("%c > %c\n", *a, *b);
        return GREATER;
    }
    else if (*a < *b) {
        //printf("%c < %c\n", *a, *b);
        return LESS;
    }
    else {
        return lexicographicalCompare(a+1,b+1);
    }

}


void readFile(char* name, char** buffer_ptr){
    FILE* fp = fopen(name, "r");

    if (fp==nullptr){
    printf("Error: unable to open file");
    exit(1);
    }

    assert(fseek(fp, 0L, SEEK_END)==0);

    long int filesize = ftell(*fp);

    assert(fread(*buffer_ptr, filesize, 1, fp) == filesize);

    fclose(fp);


}

int main(){

    printf("----------\n");
    int test = lexicographicalCompare("abc", "bk dsf glj");
    printf("\ntest: %d\n", test);
    printf("----------\n");

    FILE* fp = fopen("test2.txt", "r");
    char current = getc(fp);
    char* line;
    char** lines;
    lines = (char**)malloc(0);
    line = (char*)malloc(0);
    int char_counter = 0;
    int line_counter = 0;
    while (current != EOF){
        printf("%c",current);
        if (current == '\n'){
            lines = (char**)realloc(lines, (line_counter+1) * sizeof(char*));
            //printf("AAA %s\n",line);
            lines[line_counter] = line;
            line_counter++;


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
    //printf("\n%d", line_counter);
    int indexes[line_counter+1];
    for (int i = 0; i < line_counter; i++){
            indexes[i] = i;
    }

    for (int i = 0; i < line_counter; i++){
        for(int j = 0; j < line_counter-1; j++){
            int comparison = lexicographicalCompare(lines[indexes[j]], lines[indexes[j+1]]);
            //printf("%d\n", comparison);
            if (comparison == GREATER){
                //printf("%s < %s: %d, expected %d\n",lines[j], lines[j+1], comparison, lines[j][0]<lines[j+1][0]);


                int t = indexes[j];
                indexes[j] = indexes[j+1];
                indexes[j+1] = t;

            }
            else if (comparison == GREATER){
                //printf("%s > %s\n", lines[j], lines[j+1]);
            }
        }
        //printf("----------\n");
        for (int i = 0; i < line_counter; i++){
            //printf("%d\n",indexes[i]);
            //printf("%s\n",lines[indexes[i]]);
    }

    }
    printf("----------\n");
    for (int i = 0; i < line_counter; i++){
            //printf("%d\n",indexes[i]);
            //printf("%s\n",lines[indexes[i]]);
    }
    for (int i = 0; i < line_counter; i++){
            //printf("%d\n",indexes[i]);
            printf("%s\n",lines[indexes[i]]);
    }

    return 0;
    //char **lines;
    //readFile("test.txt", &lines);
    //printf(lines[0]);
}
