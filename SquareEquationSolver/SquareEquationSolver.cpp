/**
* @file
*/

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>

#define NaN NAN

const int INFINITY_ROOTS = 8;

/** @brief Discriminant less than this value will be considered as zero */
const double Threshold {1E-6};

/** @brief Test conditions */
const double Tests[11][2][3] = {

    { {6, 0, 0},   {0, NaN, 1} },
    { {6, 0, 2},   {NaN, NaN, 0} },
    { {1, 0, -1},  {1, -1, 2} },
    { {1, 3, 0},   {-3, 0, 2} },
    { {1, -7, 12}, {4, 3, 2} },
    { {1, 2, 3},   {NaN, NaN, 0} },
    { {1, 2, 1},   {-1, NaN, 1} },
    { {0, 0, 0}, {NaN, NaN, INFINITY_ROOTS} },
    { {0, 0, 2}, {NaN, NaN, 0} },
    { {0, 5, 0}, {0, NaN, 1} },
    { {0, 5, 2}, {-0.4, NaN, 1} }

};

/** @brief Structure to store roots and their quantity. */
struct solvation{ //!!

    /*@{*/
    /** @brief First root. Is NaN if root doesn't exist. */
    double x1 = NaN;

    /** @brief Second root. Is NaN if root doesn't exist. */
    double x2 = NaN;

    /** @brief Root Quantity. May be 0, 1, 2 or INFINITY_ROOTS */
    int rootQuantity = NaN;
    /*@}*/
};

void SolveLnrEquation(double a = 0, double b = 0, solvation* out = nullptr);
void SolveSqrEquation(double a = 0, double b = 0, double c = 0, solvation* out = nullptr);
int Input(double* a, double* b, double* c);
bool Test_SolveSqrEquation();

void SolveLnrEquation(const double a, const double b, solvation* out){

    /*! Solves linear equation ax + b = 0.
    @param a a - coefficient
    @param b b - coefficient
    @param out Pointer to the output structure
    @note Changes containment of __out__ structure
    */

    if (a != 0) {  /* ax + b = 0 */
            out->x1 = -b / a;
            out->rootQuantity = 1;
        }

    else if (b != 0) {  /* b = 0 */
            out->rootQuantity = 0;
        }

    else {  /* 0 = 0 */
            out->rootQuantity = INFINITY_ROOTS;
        }
}


void SolveSqrEquation(const double a, const double b, const double c, solvation* out){

    /*! Solves square equation ax^2 + bx + c = 0.
    @param a a - coefficient
    @param b b - coefficient
    @param c c - coefficient
    @param out Pointer to the output structure (see solvation)
    @note Changes containment of __out__ structure.
    */

    if (a == 0) { /* equation is linear*/
        SolveLnrEquation(b, c, out);
    }

    else if (c == 0){

        if (b == 0) { /* ax^2 = 0 */
            out->x1 = 0;
            out->rootQuantity = 1;
        }

        else{ /*ax^2 + bx = 0*/
            out -> x1 = (-b) / a;
            out -> x2 = 0;
            out -> rootQuantity = 2;
        }

    }

    else {  /*ax^2 + bx + c = 0*/

        double D = (b * b) - (4 * a * c);

        if (abs(D) < Threshold){ // D = 0
            out->x1 = -b / (2 * a);
            out->rootQuantity = 1;
        }

        else if (D > 0) { // D > 0
            out->x1 = (-b + sqrt(D)) / (2 * a);
            out->x2 = (-b - sqrt(D)) / (2 * a);
            out->rootQuantity = 2;
        }

        else if (D < 0) { // D < 0
            out->rootQuantity = 0;
        }
    }
}


bool Test_SolveSqrEquation(){

    /*! Runs program tests (see \link Tests \endlink).
    @return Test result: __true__ if positive or __false__ if negative.
    */

    solvation test;
    bool result = true;
    printf("\n\nStarting SolveSqrEquation test:\n");

    for (int i = 0; i < 11; i++){

        SolveSqrEquation(Tests[i][0][0], Tests[i][0][1], Tests[i][0][2], &test);

        if ( (test.x1 == Tests[i][1][0] || isnan(test.x1) && isnan(Tests[i][1][0]) )  &&
             (test.x2 == Tests[i][1][1] || isnan(test.x2) && isnan(Tests[i][1][1]) )  &&
              test.rootQuantity == Tests[i][1][2])
        {
            printf("Test #%d OK\n", i+1);
        }

        else{
            printf("\nTest #%d Failed.\n"
                   "Input:  %lg %lg %lg\n"
                   "Output: %lg %lg %d\n"
                   "Expected: %lg %lg %lg\n",
                   i+1, Tests[i][0][0], Tests[i][0][1], Tests[i][0][2],
                   test.x1, test.x2, test.rootQuantity,
                   Tests[i][1][0], Tests[i][1][1], Tests[i][1][2]);
            result = false;
        }
        test.x1 = NaN;
        test.x2 = NaN;
        test.rootQuantity = NaN;
    }

    return result;
}


int Input(double* a, double* b, double* c) {

    /*! Reads from console. Verificates the correctness of input data.
    @param a Pointer to variable containing first coefficient
    @param b Pointer to variable containing second coefficient
    @param c Pointer to variable containing third coefficient
    @note Will ask for new data if previous is incorrect.
    */

    assert(a != NULL); //
    assert(b != NULL); //Checking if pointers exist
    assert(c != NULL); //

    assert(a != b); //
    assert(b != c); //Checking if pointer have different values

    printf("\n\nEnter a, b, c coefficients, pls:\n");

    int check = scanf("%lg %lg %lg", a, b, c);
    rewind(stdin);

    if (check != 3){
        printf("You entered wrong type\n");
        return Input(a, b, c);
    }
}


int main(){

    printf("# Square equation solver |  /\\_/\\ \n"
           "# (c) Tndrd, 2020        | ( o.o )\n"
           "# *cat for the mentor*   |  > ^ <");

    if(Test_SolveSqrEquation()){

        double a = NaN, b = NaN, c = NaN;
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

            default: printf ("\nAn error occured. Contact us for help\n");
                     return 1;
        }

        return 0;
    }

    else{
        printf("\nTest(s) failed. Please, send us bug report\n");
        return 1;
    }
}

