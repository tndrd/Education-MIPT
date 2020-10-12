#include "onegin.h"


int main(int argc, char* argv[]){

    char* buffer = nullptr;
    switch(argc){

        case 1:  buffer = ReadFile("hamlet.txt");
                 break;

        case 2:  buffer = ReadFile(argv[1]);
                 break;

        default: printf("Too many arguments\n");
                 exit(3);
    }

    int err_quantity = RunTests();

    if(err_quantity > 0){
        printf("Failed %d test(s), stopping program\n", err_quantity);
        exit(-1);
    }
    printf("All tests OK\n");

    int number_of_lines = 0;
    MyStr* lines = GetLines(buffer, &number_of_lines);
    printf("File read successfully\n\n");

    printf("Processing of straight lexicographical sort started, wait please\n");
    MyQSort((void*)&lines, number_of_lines, LGComparator);
    printf("Processing ended, writing to a file\n");
    PrintFile("straight.txt", lines, number_of_lines);
    printf("Writed successfully\n\n");

    printf("Processing of reversed lexicographical sort started, wait please\n");
    MyQSort((void*)&lines, number_of_lines, ReversedLGComparator);
    printf("Processing ended, writing to a file\n");
    PrintFile("reversed.txt", lines, number_of_lines);
    printf("Writed successfully\n");

    return 0;
}


const char* Next_letter(const char* a, int step){

    assert(a);

    if (*a == '\0' || isalpha(*a)){
        return a;
    }
    else{
        return Next_letter(a + step, step);
    }
}


int LexicographicalCompare(const char* a, const char* b, int step){

    assert(a);
    assert(b);
    a = Next_letter(a, step);
    b = Next_letter(b, step);

    if (*a > *b)          return GREATER;
    else if (*a < *b)     return LESS;
    else if (*a == '\0' ) return EQUAL;

    else return LexicographicalCompare(a + step,b + step, step);
}


int ReversedLGComparator(MyStr a, MyStr b){
    return LexicographicalCompare(a.pointer + a.length - 2, b.pointer + b.length - 2, -1);
}


int LGComparator(MyStr a, MyStr b){
    return LexicographicalCompare(a.pointer, b.pointer, 1);
}


void* Concat(const MyStr* a, const MyStr* b, const MyStr* c, const size_t a_size, const size_t b_size, const size_t c_size){

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


void MyQSort(void* lines_ptr, const size_t length, int(*const comparator)(MyStr a, MyStr b)){ //ТУТ БЫ ЧЕРЕЗ typedef

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
        MyQSort(&ltp, ltp_length, (*comparator));
        MyQSort(&gtp, gtp_length, (*comparator));
    }

    *((MyStr**)lines_ptr) = (MyStr*)Concat(ltp, etp, gtp, ltp_length, etp_length, gtp_length);
}


int RunTests(){

    int err_quantity = 0;
    printf("Starting tests...\n");
    printf("Testing myQSort\n");
    err_quantity += TEST_myQsort();
    printf("Testing lexicographicalCompare\n");
    err_quantity += TEST_lexicographicalCompare();

    return err_quantity;
}
