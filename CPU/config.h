const int VERSION_BASE = 0;
const int VERSION_SUB = 1;
const long int MINIMAL_FILESIZE = 1024;
const short int COMMAND_LENGTH = 16;
const char* SIGNATURE = "MYASS";

typedef char LABEL_TYPE;
const int MAX_LABELS = 256;

const int COMMAND_NUMBER_BYTES = 4;

const int ARGUMENTS_NUMBER_MASK = 0x30;
const int ARGUMENTS_NUMBER_BYTES = 2;

const int ARGUMENT_TYPE_MASK = 0xC0;

const int INITIAL_STACK_CAPACITY = 10;
const int RAM_SIZE = 1024;
