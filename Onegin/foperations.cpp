#include "onegin.h"

void PrintFile(const char* name, const MyStr* lines, size_t length){

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


void ArrayPrint(const MyStr* arr, size_t length){

    assert(arr);

    for (int i = 0; i < length; i++){
        printf("%s\n", (arr[i]).pointer);
    }
}


int GetNumberOfLines(char* buffer){

    int counter = 0;
    for (int i = 0; buffer[i] != '\0'; i++){
        if (buffer[i] == '\n')
            counter++;
    }
    return counter;
}


MyStr* GetLines(char* buffer,  int* number_of_lines){

    assert(buffer);
    assert(number_of_lines);
    *number_of_lines = GetNumberOfLines(buffer);
    MyStr* lines  = (MyStr*)calloc(*number_of_lines, sizeof(MyStr));

    size_t line_counter = 0;
    size_t char_counter = 0;
    int i = 0;
    char current = buffer[i];

    for (; current != '\0'; current = buffer[++i]){
        if (current == '\n'){
            if (char_counter > 0){
                (lines[line_counter]).pointer = buffer + (i-char_counter);
                buffer[i] = '\0';
                buffer[i - char_counter - 1] = '\0';
                (lines[line_counter]).length = char_counter + 1;
                line_counter++;
            }
            char_counter = 0;
        }

        else {
            char_counter++;
        }
    }

    *number_of_lines = line_counter;
    return lines;
}


char* ReadFile(const char* name){

    assert(name);

    FILE* fp = fopen(name, "r");
    char* buffer = nullptr;

    if (fp==nullptr){
        return nullptr;
    }


    long int filesize = GetFileSize(fp);
    buffer = (char*)realloc(buffer, filesize + 1);
    int read = fread(buffer, sizeof(char), filesize, fp);
    //buffer = buffer + 1;
    fclose(fp);
    return buffer;
}


long int GetFileSize(FILE* fp){

    int seekResult = fseek(fp, 0L, SEEK_END);
    assert(seekResult == 0);
    long int filesize = ftell(fp);
    rewind(fp);
    return filesize;
}
