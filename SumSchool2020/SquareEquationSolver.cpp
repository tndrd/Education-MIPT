#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#define INFINITY_ROOTS 8

struct solvation{

    double x1 = -1;

    double x2 = -1;

    int rootQuantity = -1;  /* May be 2, 1, 0 or INFINITY_ROOTS*/

};


struct solvation SolveSqrEquation(double a = 0, double b = 0, double c = 0){

    solvation solved;
    /* ÍÅ ÐÀÁÎÒÀÅÒ, ÍÅ ÇÍÀÞ ÏÎ×ÅÌÓ
    assert(std::isfinite (a));
    assert(std::isfinite (b));
    assert(std::isfinite (c));
    */
    int D = -1;

    if (a==0) {

        if (b!=0) {  /* bx+c = 0 */
            solved.x1 = -c/b;
            solved.rootQuantity = 1;
        }

        else if (c!=0) {  /* c = 0 */
            solved.rootQuantity = 0;
        }

        else {  /* 0 = 0 */
           solved.rootQuantity = INFINITY_ROOTS;
        }

    }

    else if (b==0 & c ==0) { /* ax^2 = 0 */
        solved.x1 = 0;
        solved.rootQuantity = 1;
    }

    else {  /*ax^2+bx+c=0*/

        D = (b*b)-(4*a*c);

        if (D>0) {
            solved.x1 = (-b+sqrt(D))/(2*a);
            solved.x2 = (-b-sqrt(D))/(2*a);
            solved.rootQuantity = 2;
        }


        else if(D<0) {
            solved.rootQuantity = 0;

        }


        else {
            solved.x1 = -b/(2*a);
            solved.rootQuantity = 1;
        }
    }

    return solved;

}

int main(){

    printf("# Square equation solver |  /)_/) \n"
           "# (c) Tndrd, 2020        | ( o.o )\n"
           "# *cat for the mentor*   |  > ^ <");

    printf("\n\nEnter a, b, c coefficients, pls:\n");

    double a = 0, b = 0, c = 0;
    scanf("%lg %lg %lg", &a, &b, &c);

    struct solvation solved = SolveSqrEquation(a, b, c);

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

