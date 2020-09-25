#include "onegin.h"

#define GREATER 1
#define LESS -1
#define EQUAL 0

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

    char* rev_str = (char*)calloc(length,1);
    int i = 0;

    do{
    current = str[i];

        if(i>0){
            rev_str[i-1] = str[length - i -1];
        }

        i++;

    }while (current != '\0');

    rev_str[length-1] = '\0';
    return rev_str;
}


int reversedLGComparator(const void* a, const void* b){
    return lexicographicalCompare(reversed((char*)a), reversed((char*)b));
}


int LGComparator(const void* a, const void* b){
    return lexicographicalCompare((char*)a, (char*)b);
}


void* concat(char** a, char** b, char** c, int a_size, int b_size, int c_size){

    char** out = (char**)calloc(1, (sizeof(char*) * (a_size+b_size+c_size)) );

    for (int m = 0; m < a_size; m++){
        out[m] = a[m];
    }
    for (int m = 0; m < b_size; m++){
        out[m+a_size] = b[m];
    }
    for (int m = 0; m < c_size; m++){
        out[m+a_size+b_size] = c[m];
    }

    return out;
}


void myQSort(void* lines_ptr, int length, int(*compare)(const void* a, const void* b)){

    char** lines = *((char***)lines_ptr);
    char** ltp;
    char** gtp;
    char** etp;

    int ltp_length = 0;
    int gtp_length = 0;
    int etp_length = 0;

    char* pivot = lines[length-1];

    for (int i = 0; i<length; i++){

        char* element = lines[i];

        if ((*compare)((const void*)element, (const void*)pivot) < 0){
            ltp_length++;
        }
        if ((*compare)((const void*)element, (const void*)pivot) == 0){
            etp_length++;
        }
        if ((*compare)((const void*)element, (const void*)pivot) > 0){
            gtp_length++;
        }
    }

    ltp = (char**)calloc(ltp_length, sizeof(char*));
    etp = (char**)calloc(etp_length, sizeof(char*));
    gtp = (char**)calloc(gtp_length, sizeof(char*));

    int ltp_counter = 0;
    int etp_counter = 0;
    int gtp_counter = 0;

    for(int i = 0; i<length; i++){

        char* element = lines[i];

        if ((*compare)((const void*)element, (const void*)pivot) < 0){
            ltp[ltp_counter] = lines[i];
            ltp_counter++;
        }
        if ((*compare)((const void*)element, (const void*)pivot) == 0){
            etp[etp_counter] = lines[i];
            etp_counter++;
        }
        if ((*compare)((const void*)element, (const void*)pivot) > 0){
            gtp[gtp_counter] = lines[i];
            gtp_counter++;
        }
    }

    if (gtp_length != 0 || ltp_length != 0){
    myQSort(&ltp, ltp_length, (*compare));
    myQSort(&gtp, gtp_length, (*compare));
    }

    *((char***)lines_ptr) = (char**)concat(ltp, etp, gtp, ltp_length, etp_length, gtp_length);
}


void runTests(){

    printf("Starting tests...\n");
    int res = TEST_myQsort();

    if (res < 0){
        printf("TEST #1 OK\n");
    }
    else{
        printf("Something went wrong, check t-named log on char %d\n", res);
    }
}


int main(){

    runTests();
    char* buffer = readFile("hamlet.txt");
    int number_of_lines;
    char** lines = getLines(buffer, &number_of_lines);
    printf("File read successfully\n");
    printf("Processing started, wait please\n");
    myQSort((void*)&lines, number_of_lines, reversedLGComparator);
    printf("Processing ended, writing to a file\n");
    printFile("out.txt", lines, number_of_lines);
    printf("Writed successfully\n");

    return 0;

}
