#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>


#define GREATER 1
#define LESS -1
#define EQUAL 0

int* concat(int* a, int* b, int* c, int a_size, int b_size, int c_size){

    int* out = (int*)calloc(1, sizeof(int)*(a_size+b_size+c_size));


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

void myQSort(int** lines_ptr, int(*compare)(int* a, int* b), int length){

    int* lines = *lines_ptr;
    int* ltp;
    int* gtp;
    int* etp;

    int ltp_length = 0;
    int gtp_length = 0;
    int etp_length = 0;

    int pivot = lines[length-1];
    for (int i = 0; i<length; i++){
        int element = lines[i];
        if ((*compare)(&element, &pivot) < 0){
            ltp_length++;
        }
        if ((*compare)(&element, &pivot) == 0){
            etp_length++;
        }
        if ((*compare)(&element, &pivot) > 0){
            gtp_length++;
        }
    }
    ltp = (int*)calloc(ltp_length, sizeof(int));
    etp = (int*)calloc(etp_length, sizeof(int));
    gtp = (int*)calloc(gtp_length, sizeof(int));

    int ltp_counter = 0;
    int etp_counter = 0;
    int gtp_counter = 0;

    for(int i = 0; i<length; i++){
    int element = lines[i];
        if ((*compare)(&element, &pivot) < 0){
            ltp[ltp_counter] = lines[i];
            ltp_counter++;
        }
        if ((*compare)(&element, &pivot) == 0){
            etp[etp_counter] = lines[i];
            etp_counter++;
        }
        if ((*compare)(&element, &pivot) > 0){
            gtp[gtp_counter] = lines[i];
            gtp_counter++;
        }
    }

    if (gtp_length != 0 || ltp_length != 0){
    myQSort(&ltp, (*compare), ltp_length);
    myQSort(&gtp, (*compare), gtp_length);
    }

    *lines_ptr = concat(ltp, etp, gtp, ltp_length, etp_length, gtp_length);

}

int comparator(int* a, int* b){
    return *a - *b;
}


int main(){
    size_t sz = 9;
    int* arr = (int*)calloc(sz, sizeof(int));

    arr[0] = 8;
    arr[1] = 5;
    arr[2] = 13;
    arr[3] = 6;
    arr[4] = 7;
    arr[5] = 8;
    arr[6] = 6;
    arr[7] = 2;
    arr[8] = 1;

    myQSort(&arr, comparator, sz);

    for (int i = 0; i < sz; i++){
        printf("%d ", arr[i]);
    }

    return 0;
}
