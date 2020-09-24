#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
//#include <onegin>

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


int TEST_myQsort(const char* path){




}

int main(){

return 0;
}
