#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>



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





//int main(){




/*
Open(); // and check
LexicoGraphicalSort();
Write();
*/

/*
char** lines = (char**)malloc(0);
//char* new_line = (char*)malloc(0);
char new_line[] = "greetings!";
//printf("%d",sizeof(new_line));
//add_char('?', &new_line);
add_line(new_line, &lines,0);
add_line(new_line, &lines,1);
add_line(new_line, &lines,2);
printf("\n%s\n", lines[0]);
printf("\n%s\n", lines[1]);
printf("\n%s\n", lines[2]);
//lines[0] = "1";

//lines[0] = "2";

//add_char('2', &lines[0], 2);
//lines[0] = "3";
//lines[0][0] = '4';
//printf("\n%s\n", lines[0]);
//printf("\n%s\n", lines[0]);
//printf("\n%s\n", lines[1]);

//lines[0] = {"bcd"};
//printf("\n%c\n", lines[0][0]);
*/


void readFile(char* name, char*** out_pointer){

    FILE* fp = fopen(name, "r");
    char current = getc(fp);
    char* line;
    //char** lines;
    *out_pointer = (char**)malloc(0);
    line = (char*)malloc(0);
    int char_counter = 0;
    int line_counter = 0;
    while (current != EOF){

        if (current == '\n'){
            *out_pointer = (char**)realloc(*out_pointer, (line_counter+1) * sizeof(char*));
            *out_pointer[line_counter] = line;
            line = (char*)malloc(0);
            char_counter = 0;
            line_counter++;
        }

        line = (char*)realloc(line, char_counter+1);
        line[char_counter] = current;
        char_counter++;
        current = getc(fp);
    }

}

int main(){

    FILE* fp = fopen("test.txt", "r");
    char current = getc(fp);
    char* line;
    char** lines;
    lines = (char**)malloc(0);
    line = (char*)malloc(0);
    int char_counter = 0;
    int line_counter = 0;
    while (current != EOF){

        if (current == '\n'){
            if (char_counter > 1){
                lines = (char**)realloc(lines, (line_counter+1) * sizeof(char*));
                lines[line_counter] = line;
                printf(lines[line_counter]);
                line_counter++;
            }

            line = (char*)malloc(0);
            char_counter = 0;

        }

        line = (char*)realloc(line, char_counter+1);
        line[char_counter] = current;
        char_counter++;
        current = getc(fp);
    }

    return 0;


    //char **lines;
    //readFile("test.txt", &lines);
    //printf(lines[0]);
}
