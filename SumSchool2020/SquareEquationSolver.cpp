#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#define INFINITY_ROOTS 8

const double Tolerance {1E-6};

struct solvation{

    double x1 = -1;

    double x2 = -1;

    int rootQuantity = -1;  /* May be 2, 1, 0 or INFINITY_ROOTS*/

};


void SolveLnrEquation(double a = 0, double b = 0, solvation *out = nullptr){

        if (a!=0) {  /* ax + b = 0 */
            out->x1 = -b / a;
            out->rootQuantity = 1;
        }

        else if (b!=0) {  /* b = 0 */
            out->rootQuantity = 0;
        }

        else {  /* 0 = 0 */
           out->rootQuantity = INFINITY_ROOTS;
        }

}


void SolveSqrEquation(double a = 0, double b = 0, double c = 0, solvation* out = nullptr){

    int D = -1;

    if (a==0) {
        SolveLnrEquation(b, c, out);
    }

    else if (b==0 & c ==0) { /* ax^2 = 0 */
        out->x1 = 0;
        out->rootQuantity = 1;
    }

    else {  /*ax^2 + bx + c = 0*/

        D = (b * b) - (4 * a * c);

        if (D == 0 || abs(D) < Tolerance){
            out->x1 = -b / (2 * a);
            out->rootQuantity = 1;
        }

        else if (D > 0) {
            out->x1 = (-b + sqrt(D)) / (2 * a);
            out->x2 = (-b - sqrt(D)) / (2 * a);
            out->rootQuantity = 2;
        }


        else if(D < 0) {
            out->rootQuantity = 0;

        }

    }

}

int Input(double* a, double* b, double* c) {

    assert(a != NULL); //
    assert(b != NULL); //Checking does pointers exists
    assert(c != NULL); //

    assert(a != b); //
    assert(b != c); //Checking does pointers has different values

    printf("\n\nEnter a, b, c coefficients, pls:\n");

    rewind(stdin);
    int check = scanf(" %lg %lg %lg", a, b, c);
    if (check != 3){
        printf("You entered wrong type\n");
        return Input(a, b, c);
    }
}

int main(){

    printf("# Square equation solver |  /)_/) \n"
           "# (c) Tndrd, 2020        | ( o.o )\n"
           "# *cat for the mentor*   |  > ^ <");

    double a = nan("1"), b = nan("1"), c = nan("1");
    Input(&a, &b, &c);

    solvation solved;
    SolveSqrEquation(a, b, c, &solved);

    switch(solved.rootQuantity) {

        case 0:  printf("\nNo roots, sry\n");
                 break;

        case 1:  printf("\nx = %lg (Incredible!)\n", solved.x1);
                 break;

        case 2:  printf("\nx1 = %lg, x2 = %lg, i love cats\n", solved.x1, solved.x2);
                 break;

        case INFINITY_ROOTS:  printf("\nToo many roots (infinite, to be honest)\n");
                              break;

        default: printf ("\nAn error occured. Contact us (phone 88005552535) for help\n");
                 return 1;
    }

    return 0;
}

