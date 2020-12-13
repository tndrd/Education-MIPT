
char*  SAY_BUFFER              = nullptr;
size_t PHRASE_BEGINNING_OFFSET = 0;

#define DO_SPEAK ;

#ifdef DO_SPEAK
//стрлен
#define SAY_PHRASE\
    sprintf(strchr(SAY_BUFFER, '\0'), "\" | text2wave -eval '(voice_msu_ru_nsh_clunits)' | play -q - tempo %f", SPEECH_SPEED);\
    system(SAY_BUFFER);\

#else

#define SAY_PHRASE ; 

#endif

#define SAY_AND_PRINT(format, ...)\
    START_PHRASE\
    ADD_WORD_TO_PHRASE(format, __VA_ARGS__)\
    SAY_AND_PRINT_ALL_PHRASE\

#define SAY(format, ...)\
    START_PHRASE\
    ADD_WORD_TO_PHRASE(format, __VA_ARGS__)\
    SAY_PHRASE\

#define SAY_AND_PRINT_ALL_PHRASE\
    printf("%s",SAY_BUFFER + PHRASE_BEGINNING_OFFSET);\
    fflush(stdout);\
    SAY_PHRASE\

#define ADD_WORD_TO_PHRASE_NOARG(string) ADD_WORD_TO_PHRASE(string, nullptr);

#define SAY_AND_PRINT_NOARG(string) SAY_AND_PRINT(string, nullptr);

#define SAY_NOARG(string) SAY(string, nullptr);

#define START_PHRASE sprintf(SAY_BUFFER, "echo \"" "%n", &PHRASE_BEGINNING_OFFSET);
    
#define ADD_WORD_TO_PHRASE(format, ...) sprintf( strchr(SAY_BUFFER, '\0') , format, __VA_ARGS__);
    