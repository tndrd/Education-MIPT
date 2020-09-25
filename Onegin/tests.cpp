#include "onegin.h"

#define GREATER "grtr"
#define LESS "less"
#define EQUAL "eqal"


char TESTS_lexicographical_compare[6][3][5] = {

{ "abcd",    "acde",    GREATER },
{ " ,ab",    "::de",       LESS },
{ "abc ",    "abcd",       LESS },
{ "DEAD",    "beef",    GREATER },
{ "zzzz",    "-__-",    GREATER },
{ "DING",    "DING",      EQUAL }};

char TESTS_reversed[4][2][5] = {

{ "abcd",    "dcba" },
{ "I go",    "og I" },
{ "meow",    "woem" },
{ "o__-",    "-__o" }};


int TEST_myQsort(){

    char* name = (char*)calloc(9, sizeof(char));

    int number_of_lines;

    name[0] = 't';
    name[1] = '/';
    name[4] = '.';
    name[5] = 't';
    name[6] = 'x';
    name[7] = 't';
    name[8] = '\0';
    for(int i = 0; i < 1; i++){

        name[2] = (char)(i+48);
        name[3] = 'i';

        char* buffer = readFile(name);
        char** lines = getLines(buffer, &number_of_lines);
        myQSort((void*)&lines, number_of_lines, reversedLGComparator);
        name[3] = 't';
        printFile(name, lines, number_of_lines);

        char* buffer_1 = readFile(name);
        name[3] = 'r';
        char* buffer_2 = readFile(name);

        i = 0;
        char current_1 = buffer_1[i];
        char current_2 = buffer_2[i];

        while (current_1 != '\0' && current_2 != '\0') {

            if (current_1 != current_2){
                return i+1;
            }
            i++;
            current_1 = buffer_1[i];
            current_2 = buffer_2[i];


        }
        name[3] = 't';
        remove(name);

    }
    return -1;

}
