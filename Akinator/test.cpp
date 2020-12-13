#include <stdio.h>
#include <stdlib.h>

int main(){

    char buffer[40] = "jfjfjfj";

    fgets(buffer, 40, stdin); 
    printf(buffer);
    return 0;
}