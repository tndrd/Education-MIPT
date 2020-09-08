#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>


void readFile(char* name, char** buffer_ptr){

    FILE* fp = fopen(name, "r");

    if (fp==nullptr){
        printf("Error: unable to open file");
        exit(1);
    }

    assert(fseek(fp, 0L, SEEK_END)==0);
    long int filesize = ftell(fp);
    //printf("%ld", filesize); filesize почему-то на 1 больше реального (мб \0 включен в него?)

    *buffer_ptr = (char*)realloc(*buffer_ptr, filesize);
    assert(fread(*buffer_ptr, 1, filesize, fp) == filesize); //fread возвращает 0
    fclose(fp);


}

int main(){

    char* buffer = (char*)malloc(0);
    readFile("input3.txt", &buffer);
    return 0;

}
