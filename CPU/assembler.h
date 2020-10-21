#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../Onegin/foperations.cpp"

#define DO_POP printf("pop");
#define DO_PUSH printf("push");

const int VERSION_BASE = 0;
const int VERSION_SUB = 1;
const long int MINIMAL_FILESIZE = 1024;
const short int COMMAND_LENGTH = 16;

const double RAX_CODE = 1.123123123;
const double RBX_CODE = 1.234234234;
const double RCX_CODE = 1.345345345;
const double RDX_CODE = 1.456456456;