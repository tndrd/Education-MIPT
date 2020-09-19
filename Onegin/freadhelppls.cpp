#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#define GREATER 1
#define LESS -1
#define EQUAL 0


char* readFile(char* name, char* buffer){
    FILE* fp = fopen(name, "r");

    if (fp==nullptr){
        printf("Error: unable to open file");
        exit(1);
    }

    assert(fseek(fp, 0L, SEEK_END)==0);
    long int filesize = ftell(fp);
    buffer = (char*)realloc(buffer, filesize);
    rewind(fp);
    int read = fread(buffer, sizeof(char), filesize, fp);
    fclose(fp);
    printf("File read successfully\n");
    return buffer;
}

char** getLines(char* buffer, int* number_of_lines){

    char* line = nullptr;
    char** lines  = nullptr;
    int char_counter = 0;
    int line_counter = 0;

    int i = 0;
    char current = buffer[i];

    while (current != '\0'){
        if (current == '\n'){
            if (char_counter > 0){
            lines = (char**)realloc(lines, (line_counter+1) * sizeof(char*));
            lines[line_counter] = line;
            line_counter++;
            }
            line = nullptr;
            char_counter = 0;
        }

        else {
            line = (char*)realloc(line, char_counter+1);
            line[char_counter] = current;
            char_counter++;
        }

        i++;
        current = buffer[i];
    }
    *number_of_lines = line_counter;
    return lines;
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
        return GREATER;
    }
    else if (*a < *b) {
        return LESS;
    }
    else {
        return lexicographicalCompare(a+1,b+1);
    }

}



char* reversed(char* str){

    int length=0;
    char current = '\0';

    do{
        current = str[length];
        length++;
    }while (current != '\0');

    char rev_str[length];
    int i = 0;
    do{
    current = str[i];
        if(i>0){
            rev_str[i-1] = str[length - i -1];
        }
        i++;
    }while (current != '\0');

    rev_str[length-1] = '\0';

    //Внутри функции перевернутую строку можно распечатать, однако при возвращении функцией происходит херня
    //Попробуй распечать ее внутри этой функции и внутри main
    //printf("%p",rev_str);
    return rev_str; //WTF
}


int reversedLGCompare(char* a, char*b){
    int length=0;
    char current = '\0';
    //тут должно быть return lexicographicalCompre(reversed(a),reversed(b));
    //однако reversed стремно работает, см. reverse()

    do{
        current = a[length];
        length++;
    }while (current != '\0');

    char rev_a[length];
    int i = 0;
    do{
    current = a[i];
        if(i>0){
            rev_a[i-1] = a[length - i -1];
        }
        i++;
    }while (current != '\0');

    rev_a[length-1] = '\0';

    length=0;
    current = '\0';

    do{
        current = b[length];
        length++;
    }while (current != '\0');

    char rev_b[length];
    i = 0;
    do{
    current = b[i];
        if(i>0){
            rev_b[i-1] = b[length - i -1];
        }
        i++;
    }while (current != '\0');

    rev_b[length-1] = '\0';


    return lexicographicalCompare(rev_a, rev_b);
}

int lgComparator(const void* a, const void* b){
    return lexicographicalCompare((char*)a, (char*)b);
}

/*
void newbubbleSort (void* base, size_t num, size_t sz, int (*compare)(const void *, const void *)){

    for (int i=0; i < num-1; i++){
        for (int j=0; j < num-i-1; j++){
                if ((*compare)(base[j], base[j+1]) > 0){
                typeof(base[j]) temp = base[j];
                base[j] = base [j+1];
                base[j+1] = temp;
                }
    }
}
*/
void bubbleSort (char** base, size_t num, size_t sz, int(*compare)(char* a, char* b)){

    for (int i=0; i < num-1; i++){
        for (int j=0; j < num-i-1; j++){
                if ((*compare)(base[j], base[j+1]) > 0){
                typeof(base[j]) temp = base[j];
                base[j] = base [j+1];
                base[j+1] = temp;
                }
        }
    }
}

void arrayPrint(char** arr, int length){
    for (int i = 0; i<length; i++){
    printf("%s\n",arr[i]);
    }
}

int main(){

    char* buffer = nullptr;
    buffer = readFile("hamlet.txt", buffer);
    int number;
    char** lines = getLines(buffer, &number);
    bubbleSort(lines, number, number*sizeof(lines), reversedLGCompare);

    //arrayPrint(lines, number);

    //printf(reversed("sdfsdf"));
    // rev не печатается, хотя внутри функции (см.reverse) она это делает

    return 0;

}
