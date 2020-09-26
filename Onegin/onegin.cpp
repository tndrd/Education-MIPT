#include "onegin.h"

#define GREATER 1
#define LESS -1
#define EQUAL 0


int main(int argc, char* argv[]){

    if(runTests() > 0){
        printf("Stopping program\n");
        exit(-1);
    }
    printf("All tests OK\n");

    char* buffer;

    if (argc == 1){
        buffer = readFile("hamlet.txt");
    }
    else if (argc > 2){
        printf("Too many arguments");
        exit(2);
    }
    else{
        buffer = readFile(argv[1]);
    }

    int number_of_lines;
    MyStr* lines = getLines(buffer, &number_of_lines);
    printf("File read successfully\n");
    printf("Processing started, wait please\n");
    myQSort((void*)&lines, number_of_lines, LGComparator);
    printf("Processing ended, writing to a file\n");
    printFile("out.txt", lines, number_of_lines);
    printf("Writed successfully\n");

    return 0;
}


char* next_letter(char* a){

    assert(a);

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

    assert(a);
    assert(b);
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


MyStr reversed(MyStr to_reverse){

    assert(to_reverse.pointer);
    MyStr rev_MyStr;
    rev_MyStr.length = to_reverse.length;
    rev_MyStr.pointer = (char*)calloc(1, sizeof(char) * to_reverse.length);

    for (int i = to_reverse.length - 2; i >= 0; i--){
        (rev_MyStr.pointer)[ rev_MyStr.length - i - 2 ] = to_reverse.pointer[i];
    }

    (rev_MyStr.pointer)[ rev_MyStr.length - 1 ] = '\0';
    return rev_MyStr;
}


int reversedLGComparator(MyStr a, MyStr b){

    MyStr rev_a = reversed(a);
    MyStr rev_b = reversed(b);
    int result = lexicographicalCompare(rev_a.pointer, rev_b.pointer);
    free(rev_a.pointer);
    free(rev_b.pointer);

    return result;
}


int LGComparator(MyStr a, MyStr b){
    return lexicographicalCompare(a.pointer, b.pointer);
}


void* concat(MyStr* a, MyStr* b, MyStr* c, size_t a_size, size_t b_size, size_t c_size){

    assert(a);
    assert(b);
    assert(c);
    assert(a!=b);
    assert(b!=c);
    assert(a!=c);

    MyStr* out = (MyStr*)calloc(1, (sizeof(MyStr) * (a_size+b_size+c_size)) );

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


void myQSort(void* lines_ptr, int length, int(*comparator)(MyStr a, MyStr b)){ //ТУТ БЫ ЧЕРЕЗ typedef

    assert(lines_ptr);

    MyStr* lines = *((MyStr**)lines_ptr);
    MyStr* ltp;
    MyStr* gtp;
    MyStr* etp;

    int ltp_length = 0;
    int gtp_length = 0;
    int etp_length = 0;

    MyStr pivot = lines[length-1];

    for (int i = 0; i<length; i++){

        MyStr element = lines[i];

        if ((*comparator)(element, pivot) < 0){
            ltp_length++;
        }
        if ((*comparator)(element, pivot) == 0){
            etp_length++;
        }
        if ((*comparator)(element, pivot) > 0){
            gtp_length++;
        }
    }

    ltp = (MyStr*)calloc(ltp_length, sizeof(MyStr));
    etp = (MyStr*)calloc(etp_length, sizeof(MyStr));
    gtp = (MyStr*)calloc(gtp_length, sizeof(MyStr));

    int ltp_counter = 0;
    int etp_counter = 0;
    int gtp_counter = 0;

    for(int i = 0; i<length; i++){

        MyStr element = lines[i];

        if ((*comparator)(element, pivot) < 0){
            ltp[ltp_counter] = lines[i];
            ltp_counter++;
        }
        if ((*comparator)(element, pivot) == 0){
            etp[etp_counter] = lines[i];
            etp_counter++;
        }
        if ((*comparator)(element, pivot) > 0){
            gtp[gtp_counter] = lines[i];
            gtp_counter++;
        }
    }

    if (gtp_length != 0 || ltp_length != 0){
    myQSort(&ltp, ltp_length, (*comparator));
    myQSort(&gtp, gtp_length, (*comparator));
    }

    *((MyStr**)lines_ptr) = (MyStr*)concat(ltp, etp, gtp, ltp_length, etp_length, gtp_length);
}


int runTests(){

    printf("Starting tests...\n");
    int res = TEST_myQsort();
    return res;
}

