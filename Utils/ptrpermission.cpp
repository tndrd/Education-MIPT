#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

struct MemReg{

        void* LeftBorder = 0;
        void* RightBorder = 0;

        int read = 5;
        int write = 0;
};

MemReg* getMemoryRegions(int* number_of_regions){
    FILE* fp = fopen("/proc/self/maps", "r");

    char* buffer = (char*)calloc(1,2000);
    int filesize = 0;
    for(; !feof(fp) ; filesize++ ) buffer[filesize] = fgetc(fp);

    buffer = (char*)realloc(buffer, filesize+10);

    int number_of_lines = 1;

    int i = 0;
    for (; buffer[i] != '\0'; i++){
        if (buffer[i] == '\n') number_of_lines++;
    }

    MemReg* regions = (MemReg*)calloc(number_of_lines, sizeof(MemReg));

    void* leftborder = 0;
    void* rightborder = 0;
    char read = 0;
    char write = 0;
    int regCounter = 0;

    for (i = 0; buffer[i] != '\0'; i++){
        if (buffer[i] == '\n'){

            if(sscanf(buffer + i, "%p-%p %c%c", &leftborder, &rightborder, &read, &write) < 4) continue;

            regions[regCounter].LeftBorder = leftborder;
            regions[regCounter].RightBorder = rightborder;
            (read == '-') ? regions[regCounter].read = 0 : regions[regCounter].read = 1;
            (write == '-') ? regions[regCounter].write = 0 : regions[regCounter].write = 1;
            //printf("%p-%p %d%d\n", regions[regCounter].LeftBorder, regions[regCounter].RightBorder, regions[regCounter].read, regions[regCounter].write);
            regCounter++;
        }
    }
    free(buffer);
    *number_of_regions = regCounter;
    return regions;
}

int IsGood_R_Ptr(void* pointer, MemReg* regions, int number_of_regions){

    for (int i = 0; i < number_of_regions; i++){
        if (regions[i].read == 1){
            if (pointer >= regions[i].LeftBorder && pointer <= regions[i].RightBorder) return 1;
        }

    }
    return 0;
}

int IsGood_W_Ptr(void* pointer, MemReg* regions, int number_of_regions){

    for (int i = 0; i < number_of_regions; i++){
        if (regions[i].write == 1){
            if (pointer >= regions[i].LeftBorder && pointer <= regions[i].RightBorder) return 1;
        }

    }
    return 0;
}

int IsGood_RW_Ptr(void* pointer, MemReg* regions, int number_of_regions){

    for (int i = 0; i < number_of_regions; i++){
        if (regions[i].read == 1 && regions[i].write == 1){
            if (pointer >= regions[i].LeftBorder && pointer <= regions[i].RightBorder) return 1;
        }

    }
    return 0;
}
