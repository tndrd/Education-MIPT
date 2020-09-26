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

    comparator* comparators = (comparator*)calloc(2,sizeof(comparator));
    comparators[0] = reversedLGComparator;
    comparators[1] = LGComparator;

    name[0] = 't';
    name[1] = '/';
    name[4] = '.';
    name[5] = 't';
    name[6] = 'x';
    name[7] = 't';
    name[8] = '\0';
    for(int i = 0; i < 2; i++){

        name[2] = (char)(i+48);
        name[3] = 'i';

        char* buffer = readFile(name);
        my_str* lines = getLines(buffer, &number_of_lines);
        myQSort((void*)&lines, number_of_lines, comparators[i]);
        name[3] = 't';
        printFile(name, lines, number_of_lines);

        char* buffer_1 = readFile(name);
        name[3] = 'r';
        char* buffer_2 = readFile(name);

        int j = 0;
        char current_1 = buffer_1[j];
        char current_2 = buffer_2[j];

        while (current_1 != '\0' && current_2 != '\0') {

            if (current_1 != current_2){
                return j+1;
            }
            j++;
            current_1 = buffer_1[j];
            current_2 = buffer_2[j];


        }

        name[3] = 't';
        remove(name);

    }
    return -1;

}
