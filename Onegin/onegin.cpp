#include "onegin.h"


int main(int argc, char* argv[]){

    char* buffer;
    switch(argc){

        case 1:  buffer = readFile("hamlet.txt");
                 break;

        case 2:  buffer = readFile(argv[1]);
                 break;

        default: printf("Too many arguments\n");
                 exit(3);
    }

    int err_quantity = runTests();

    if(err_quantity > 0){
        printf("Failed %d test(s), stopping program\n", err_quantity);
        exit(-1);
    }
    printf("All tests OK\n");

    int number_of_lines;
    MyStr* lines = getLines(buffer, &number_of_lines);
    printf("File read successfully\n\n");

    printf("Processing of straight lexicographical sort started, wait please\n");
    myQSort((void*)&lines, number_of_lines, LGComparator);
    printf("Processing ended, writing to a file\n");
    printFile("straight.txt", lines, number_of_lines);
    printf("Writed successfully\n\n");

    printf("Processing of reversed lexicographical sort started, wait please\n");
    myQSort((void*)&lines, number_of_lines, reversedLGComparator);
    printf("Processing ended, writing to a file\n");
    printFile("reversed.txt", lines, number_of_lines);
    printf("Writed successfully\n");

    return 0;
}


const char* next_letter(const char* a, int step){

    assert(a);

    if (*a == '\0' || isalpha(*a)){
        return a;
    }
    else{
        return next_letter(a + step, step);
    }
}


int lexicographicalCompare(const char* a, const char* b, int step){

    assert(a);
    assert(b);
    a = next_letter(a, step);
    b = next_letter(b, step);

    if (*a > *b)          return GREATER;
    else if (*a < *b)     return LESS;
    else if (*a == '\0' ) return EQUAL;

    else return lexicographicalCompare(a + step,b + step, step);
}


int reversedLGComparator(MyStr a, MyStr b){
    return lexicographicalCompare(a.pointer + a.length - 2, b.pointer + b.length - 2, -1);
}


int LGComparator(MyStr a, MyStr b){
    return lexicographicalCompare(a.pointer, b.pointer, 1);
}


void* concat(MyStr* a, MyStr* b, MyStr* c, size_t a_size, size_t b_size, size_t c_size){

    assert(a);
    assert(b);
    assert(c);
    assert(a != b);
    assert(b != c);
    assert(a != c);

    MyStr* out = (MyStr*)calloc(1, (sizeof(MyStr) * (a_size+b_size+c_size)) );

    for (int m = 0; m < a_size; m++){
        out[m] = a[m];
    }
    for (int m = 0; m < b_size; m++){
        out[m + a_size] = b[m];
    }
    for (int m = 0; m < c_size; m++){
        out[m + a_size + b_size] = c[m];
    }

    return out;
}


void myQSort(void* lines_ptr, size_t length, int(*comparator)(MyStr a, MyStr b)){ //ТУТ БЫ ЧЕРЕЗ typedef

    assert(lines_ptr);

    MyStr* lines = *((MyStr**)lines_ptr);
    MyStr* ltp;
    MyStr* gtp;
    MyStr* etp;

    int ltp_length = 0;
    int gtp_length = 0;
    int etp_length = 0;

    MyStr pivot = lines[length - 1];

    for (int i = 0; i < length; i++){

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

    for(int i = 0; i < length; i++){

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

    int err_quantity = 0;
    printf("Starting tests...\n");
    printf("Testing myQSort\n");
    err_quantity += TEST_myQsort();
    printf("Testing lexicographicalCompare\n");
    err_quantity += TEST_lexicographicalCompare();

    return err_quantity;
}
