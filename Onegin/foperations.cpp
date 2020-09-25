#include "onegin.h"


void printFile(const char* name, char** lines, int length){

    assert(name);
    assert(lines);

    FILE* fp;
    fp = fopen(name, "w");

    if (fp==nullptr){
        printf("Error: unable to open new file");
        exit(1);
    }

    for (int i = 0; i < length; i++){
        fprintf(fp, "%s\n", lines[i]);
    }

    fclose(fp);
}


void arrayPrint(char** arr, int length){

    assert(arr);

    for (int i = 0; i<length; i++){
        printf("%s\n", arr[i]);
    }
}


char** getLines(char* buffer,  int* number_of_lines){

    assert(buffer);
    assert(number_of_lines);

    char* line = nullptr;
    char** lines  = nullptr;
    int char_counter = 0;
    int line_counter = 0;

    int i = 0;
    char current = buffer[i];

    while (current != '\0'){
        if (current == '\n'){
            if (char_counter > 0){

                addChar(&line, &char_counter, '\0');
                lines = (char**)realloc(lines, (line_counter+1) * sizeof(char*));
                lines[line_counter] = line;
                line_counter++;
            }
            line = nullptr;
            char_counter = 0;
        }

        else {
            addChar(&line, &char_counter, current);
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
        printf("Error: unable to open file");
        exit(1);
    }

    assert(fseek(fp, 0L, SEEK_END)==0);
    long int filesize = ftell(fp);
    buffer = (char*)realloc(buffer, filesize);
    rewind(fp);
    int read = fread(buffer, sizeof(char), filesize, fp);
    fclose(fp);
    return buffer;
}


void addChar(char** line_ptr, int* length_ptr, const char to_add){ //Не работает(

    assert(line_ptr);
    assert(length_ptr);

    *line_ptr = (char*)realloc(*line_ptr, (*length_ptr)+1);
    (*line_ptr)[*length_ptr] = to_add;
    (*length_ptr)++;
}