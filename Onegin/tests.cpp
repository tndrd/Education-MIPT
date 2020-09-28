#include "onegin.h"
#include <string.h>

char TESTS_lexicographical_compare[6][2][5] = {

{ "acde",    "abcd" },
{ " ,ab",    "::de" },
{ "abc ",    "abcd" },
{ "beef",    "DEAD" },
{ "zzzz",    "-__-" },
{ "DING",    "DING" }};

int TEST_RESULTS_lexicographical_compare[6]{
  GREATER,
  LESS,
  LESS,
  GREATER,
  GREATER,
  EQUAL };


char TESTS_reversed[4][2][5] = {

{ "abcd",    "dcba" },
{ "I go",    "og I" },
{ "meow",    "woem" },
{ "o__-",    "-__o" }};


int TEST_reversed(){

    int result = 0;

    for (int i = 0; i < 4; i++){
        MyStr input;
        input.pointer = TESTS_reversed[i][0];
        input.length = 5;

        if (strcmp(reversed(input).pointer, TESTS_reversed[i][1]) != 0){
        printf("Test #%d failed. Input: %s | Result: %s | Expected: %s\n", i+1,
        TESTS_reversed[i][0],  reversed(input).pointer,  TESTS_reversed[i][1]);

        result++;
        }
    }
    return result;
}


int TEST_lexicographicalCompare(){

    int result = 0;

    for(int i = 0; i < 6; i++){
        if (lexicographicalCompare(TESTS_lexicographical_compare[i][0],TESTS_lexicographical_compare[i][1]) != TEST_RESULTS_lexicographical_compare[i]){
            printf ("Test #%d failed. Input: %s %s | Result: %d | Expected: %d\n", i+1,
            TESTS_lexicographical_compare[i][0],TESTS_lexicographical_compare[i][1],
            (lexicographicalCompare(TESTS_lexicographical_compare[i][0],TESTS_lexicographical_compare[i][1])),
            TEST_RESULTS_lexicographical_compare[i]);

            result++;
        }
    }
    return result;
}


int TEST_myQsort(){

    char* name = (char*)calloc(9, sizeof(char));
    int result = 0;
    int number_of_lines;

    Comparator* comparators = (Comparator*)calloc(2,sizeof(Comparator));
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
        MyStr* lines = getLines(buffer, &number_of_lines);
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
                result++;
                printf("First mismatch in test #%d at char #%d. See %dt.txt\n", i+1, j+1, i);
                continue;
            }
            j++;
            current_1 = buffer_1[j];
            current_2 = buffer_2[j];
        }

        name[3] = 't';
        remove(name);

    }
    return result;
}
