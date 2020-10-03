#include "onegin.h"

void printFile(const char* name, MyStr* lines, int length){

    assert(name);
    assert(lines);

    FILE* fp;
    fp = fopen(name, "w");

    if (fp == nullptr){
        printf("Error: unable to open new file");
        exit(1);
    }

    for (int i = 0; i < length; i++){
        fprintf(fp, "%s\n", (lines[i]).pointer);
    }

    fclose(fp);
}


void arrayPrint(MyStr* arr, int length){

    assert(arr);

    for (int i = 0; i < length; i++){
        printf("%s\n", (arr[i]).pointer);
    }
}

int GetNumberOfLines(char* buffer){

    int counter = 0;
    for (int i = 0; buffer[i] != '\0'; i++){
        if (buffer[i] == '\n') counter++;
    }
    return counter;
}

MyStr* getLines(char* buffer,  int* number_of_lines){

    assert(buffer);
    assert(number_of_lines);
    *number_of_lines = GetNumberOfLines(buffer);
    char* line = nullptr;
    MyStr* lines  = (MyStr*)calloc(*number_of_lines, sizeof(MyStr));
    size_t char_counter = 0;
    size_t line_counter = 0;
    int i = 0;
    char current = buffer[i];

    while (current != '\0'){
        if (current == '\n'){
            if (char_counter > 0){
                (lines[line_counter]).pointer = buffer + (i-char_counter);
                buffer[i] = '\0';
                buffer[i-char_counter-1] = '\0';
                (lines[line_counter]).length = char_counter+1;
                line_counter++;
            }
            char_counter = 0;
        }

        else {
            char_counter++;
        }

        i++;
        current = buffer[i];
    }

    *number_of_lines = line_counter;
    return lines;
}

char* readFile(const char* name){

    assert(name);

    FILE* fp = fopen(name, "r");
    char* buffer = nullptr;

    if (fp==nullptr){
        printf("Error: unable to open file\n");
        exit(1);
    }

    assert(fseek(fp, 0L, SEEK_END)==0);
    long int filesize = ftell(fp);
    buffer = (char*)realloc(buffer, filesize + 1);
    rewind(fp);
    int read = fread(buffer + 1, sizeof(char), filesize, fp);
    buffer = buffer + 1;
    fclose(fp);
    return buffer;
}
