#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>

#define GREATER 1
#define LESS -1
#define EQUAL 0


/** Struct to store line and its length */
struct MyStr{

    char* pointer;
    int length;

};

typedef int(*Comparator)(MyStr a, MyStr b);

//------------------------------------------------------------------------------------

/*! Reads file to a string buffer
    @param name name of file
    @return pointer to the buffer
*/
char* ReadFile(const char* name);

/*! Creates an array of \link MyStr \endlink structs from buffer
    @param buffer pointer to the buffer
    @param number_of_lines pointer to number of lines - storing variable
    @return \link MyStr \endlink array
*/
MyStr* GetLines(char* buffer, int* number_of_lines);

/*! Function to get first letter after or before pointer
    @param a string
    @param step direction parameter, should be 1 if you are searching after pointer or -1 if before
    @return pointer to the found letter
*/
const char* Next_letter(const char* a, int step);

/*! Lexicographically compares two strings
    @param a first string
    @param b second string
    @return result of comparison
    @note returns 1 if a > b, 2 if a < b or 0 if a = b
*/
int LexicographicalCompare(const char* a, const char* b, int step);

/*! Lexicographically compares two \link MyStr \endlink strings starting from their ends
    @param a first \link MyStr \endlink string
    @param b second \link MyStr \endlink string
    @return result of comparison
    @note returns 1 if a > b, 2 if a < b or 0 if a = b
*/
int ReversedLGComparator(MyStr a, MyStr b);


/*! Lexicographically compares two \link MyStr \endlink strings
    @param a first \link MyStr \endlink string
    @param b second \link MyStr \endlink string
    @return result of comparison
    @note returns 1 if a > b, 2 if a < b or 0 if a = b
*/
int LGComparator(MyStr a, MyStr b);


/*! Prints \link MyStr \endlink array
    @param arr \link MyStr \endlink array
    @param length length of array
*/
void ArrayPrint(const MyStr* arr, size_t length);


/*! Prints \link MyStr \endlink array to a file
    @param name name of the output file
    @param lines \link MyStr \endlink array
    @param length length of array
*/
void PrintFile(const char* name, const MyStr* lines, size_t length);


/*! Concatenates 3 \link MyStr \endlink arrays
    @param a first array
    @param b second array
    @param c third array
    @param a_size first array length
    @param b_size second array length
    @param c_size third array length
    @return pointer to the new array
*/
void* Concat(const MyStr* a, const MyStr* b, const MyStr* c, const size_t a_size, const size_t b_size, const size_t c_size);


/*! Does the quick-sort of \link MyStr \endlink array
    @param lines_ptr pointer to \link MyStr \endlink array
    @param length \link MyStr \endlink array length
    @param comparator pointer to the comparator function
*/
void MyQSort(void* lines_ptr, const size_t length, int(*const comparator)(MyStr a, MyStr b));


/*! Tests \link myQSort \endlink function
    @note takes tests from "t" folder. The ones ended with i are input data, r-ended are expected results. If test fails, prints number of tests and number orfirst mismatched char. Creates failed tests output files.
*/
int TEST_myQsort();

/*! Founds file length
    @param fp file pointer
    @return size of file
*/
long int GetFileSize(FILE* fp);

/*! Tests \link lexicographicalCompare \endlink function */
int TEST_lexicographicalCompare();


/*! Runs all the tests */
int RunTests();
//------------------------------------------------------------------------------------



