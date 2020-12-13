#include "Akinator.h"
#include "govorilka.h"

TREE_STATUS FUNC_STATUS = OK;

#define EXIT_ON_ERROR(func)\
    FUNC_STATUS = func;\
    if (FUNC_STATUS != OK){\
        printf("CRITICAL ERROR: %s\n", GET_ERROR_NAME(FUNC_STATUS));\
        printf("Shutting down\n");\
        exit(FUNC_STATUS);\
    }
//Убрать

#define DO_UNTIL_DONE(action)\
    do{\
        action_status = action;\
    } while (action_status == TRY_AGAIN);\


void ReadFromConsole(char* destination, size_t nchars){ //fgets
    
    fgets(destination, nchars, stdin); //
    char* read_end = strchr(destination, '\n'); 
    if (read_end) *read_end = '\0';  
}

char GetSingleCharAnswer(){

    char answer = getc(stdin);

    while (answer == '\n') answer = getc(stdin);

    if (getc(stdin) != '\n'){  //if two or more symbols in input
        while(getc(stdin) != '\n'){}
        return '\0'; 
    
    }
    
    return answer;
}

//цикл

AKINATOR_ACTION_STATUS AkinatorChooseDatabase(Tree** destination){

    printf("Хотите использовать существущую базу данных или создать новую?\n");
    printf("Напечатайте [l] - загрузить, [n] - cоздать или [e] - выйти из игры: ");
    
    Tree* DataBase = nullptr;
    
    switch (GetSingleCharAnswer()){
        
        case 'l':
            {
            printf("Укажите имя базы: "); //пробел
        
            char filename[FILENAME_LENGTH] = "loremipsum";

            ReadFromConsole(filename, FILENAME_LENGTH);
            DataBase = ReadTree(filename); 
        
            if(!DataBase){
                SAY_AND_PRINT_NOARG("Упс, нет такого файла!\n-------\n")
                return TRY_AGAIN;
            }

            *destination = DataBase;
            return GO_ON;
            }
        
        case 'n':
            {
            *destination = NewTree("Аноним");
            return GO_ON;
            }
        
        case 'e':
            {
            SAY_AND_PRINT_NOARG("Жаль, что вы уходите. Возвращайтесь поскорее.\n");
            return GAME_END;
            }
        
        default:
            {
            SAY_AND_PRINT_NOARG("Неизвестный режим.\n")
            return TRY_AGAIN;
            }

    }
}


TREE_STATUS SplitByAtribute(char* new_attribute, Node* Parent_Node, char* NewObject){
    
    if (!new_attribute || !Parent_Node || !NewObject) return INVALID_POINTER;
    
    TREE_STATUS status = OK;
    
    status = AddLeftNode(Parent_Node, NewObject);
    if (status != OK) return status;
    
    status = AddRightNode(Parent_Node, Parent_Node -> value);
    if (status != OK) return status;
    
    Parent_Node -> value = new_attribute;
    
    return status;    
}


TREE_STATUS AddAttribute(Node* current_node){
    
    char* new_object    = (char*)calloc(OBJECT_NAME_LENGTH, sizeof(char));
    char* new_attribute = (char*)calloc(ATTRIBUTE_NAME_LENGTH, sizeof(char));
    
    ReadFromConsole(new_object, OBJECT_NAME_LENGTH);
    
    SAY_AND_PRINT("Чем %s отличается от %s: ", new_object, current_node -> value)
    
    ReadFromConsole(new_attribute, ATTRIBUTE_NAME_LENGTH);
    
    return SplitByAtribute(new_attribute, current_node, new_object);
}


AKINATOR_ACTION_STATUS SwitchNode(Node* current_node){

    AKINATOR_ACTION_STATUS action_status = TRY_AGAIN;

    SAY_AND_PRINT("Ваш объект %s?: ", current_node -> value)
    
    switch(GetSingleCharAnswer()){
        
        case 'y':
            
            if (current_node -> left){
                
                DO_UNTIL_DONE(SwitchNode(current_node -> left))
                return action_status;
            }
            else{
                SAY_AND_PRINT_NOARG("Ура! Я угадал, жалкий ты человек\n")
                return GO_ON;
            }
            break;

        case 'n':

            if (current_node -> right){
    
                DO_UNTIL_DONE(SwitchNode(current_node -> right))
                return action_status;
            }
            
            else{    
                
                SAY_AND_PRINT_NOARG("Я не знаю загаданный объект! Помогите мне:\nКого вы загадали: ")
            
                EXIT_ON_ERROR(AddAttribute(current_node))
                
                SAY_AND_PRINT_NOARG("Спасибо, человек, я теперь стал умнее.\n")
                return GO_ON;
            }
            break;

        case 'e':
            SAY_AND_PRINT_NOARG("Видимо, я достал вас своими вопросами.\n")
            return GAME_END;

        default:
            
            SAY_AND_PRINT_NOARG("Бип-бип. Я не понял ваш ответ.\n");
            return TRY_AGAIN;
            break;
    }
}


AKINATOR_ACTION_STATUS AkinatorPlayGuess(Tree* DataBase){    
    SAY_AND_PRINT_NOARG("Загадайте объект и отвечайте на мои вопросы.\nВводите [y], чтобы отвечать \"да\", или [n], чтобы отвечать \"нет\".\nЕсли я вам надоем, введите [e].\n")
    printf("----------\n");
    

    AKINATOR_ACTION_STATUS action_status = TRY_AGAIN;
    DO_UNTIL_DONE(SwitchNode(DataBase -> root));

    return action_status;
}


Node* SearchObjectRecursively(char* value, Node* current, int* node_counter){

    assert(current -> value);
    assert(value);
    assert(node_counter);

    if (*node_counter > (current -> tree) -> size) return nullptr;
    (*node_counter)++;
    
    if (!strcmp(current -> value, value)){
        if (current -> left || current -> right) return nullptr;
        else                                     return current;
    }

    Node* found = nullptr;
    
    if (current -> left)  found = SearchObjectRecursively(value, current -> left, node_counter);
    
    if (found) return found;
    
    if (current -> right) found = SearchObjectRecursively(value, current -> right, node_counter);
    
    return found;

}


Node* SearchFromRoot(char* value, Node* root){
    
    int counter = 0;
    return SearchObjectRecursively(value, root, &counter);
}


void DefineObjectRecursively(Node* current, Node* endpoint, int last, int state){
    
    assert(current);
    assert(current -> value);

    if (current -> parent != endpoint) {
        if (current == (current -> parent) -> right) DefineObjectRecursively(current -> parent, endpoint, 0, 0);
        else                                         DefineObjectRecursively(current -> parent, endpoint, 0, 1);
    }
    
    if (last){
        ADD_WORD_TO_PHRASE_NOARG(" и ") 
    }
    else if (current -> parent != endpoint){
        ADD_WORD_TO_PHRASE_NOARG(", ");
    }
    if (!state){
        ADD_WORD_TO_PHRASE_NOARG("не ")
    }
    ADD_WORD_TO_PHRASE("%s", current -> value);
}


int Definition(Node* object, Node* endpoint){
        
        assert(object);
        assert((object -> parent));
        
        if (object == (object -> parent) -> right){
            if ((object -> parent) -> parent != endpoint){
                DefineObjectRecursively(object -> parent, endpoint, 1, 0);
            }
            else
                DefineObjectRecursively(object -> parent, endpoint, 0, 0);
        }
        else{
            if ((object -> parent) -> parent != endpoint){
                DefineObjectRecursively(object -> parent, endpoint, 1, 1);
            }
            else
                DefineObjectRecursively(object -> parent, endpoint, 0, 1);
        }
        return 0;
}


AKINATOR_ACTION_STATUS AkinatorPlayDefinition(Tree* tree){

    char* definition = (char*)calloc(OBJECT_NAME_LENGTH, sizeof(char));
    
    SAY_AND_PRINT_NOARG("Давайте формализуем ваши представления.\nО ком или о чем вам рассказать: ")
    
    ReadFromConsole(definition, OBJECT_NAME_LENGTH);

    Node* object = SearchFromRoot(definition, tree -> root);
    if (!object){
        SAY_AND_PRINT("Я не знаю ничего об объекте \"%s\". Попробуйте что-нибудь другое\n", definition);
        return TRY_AGAIN;
    }
    else{
        START_PHRASE
        ADD_WORD_TO_PHRASE_NOARG("Доподлинно известно, что:\n")
        ADD_WORD_TO_PHRASE("%s ", object -> value)
        Definition(object, nullptr);
        ADD_WORD_TO_PHRASE_NOARG(".\n");
        SAY_AND_PRINT_ALL_PHRASE
    }
    return GO_ON;
}


TREE_STATUS GetPath(Stack* path, Node* current_node, int* counter){
    
    StackPush(path, (void*)current_node);
    (*counter)++;

    if (current_node -> parent) {
        if (GetPath(path, current_node -> parent, counter) != OK) return NODES_LOOPED;
    }

    if (*counter > (current_node -> tree) -> size) return NODES_LOOPED;

    return OK; 
    
}


int AkinatorCompare(Tree* tree, Node* first, Node* second){

    Stack* first_path  = (Stack*)calloc(1,sizeof(Stack*));
    Stack* second_path = (Stack*)calloc(1,sizeof(Stack*));

    newStack(INITIAL_COMPARE_STACK_CAPACITY, &first_path);
    newStack(INITIAL_COMPARE_STACK_CAPACITY, &second_path);

    int first_counter  = 0;
    int second_counter = 0;
    
    EXIT_ON_ERROR(GetPath(first_path, first,   &first_counter))
    EXIT_ON_ERROR(GetPath(second_path, second, &second_counter))

    int pop_status_firstpath  = 1;
    int pop_status_secondpath = 1;

    Node* first_different = tree -> root;
    Node* last_popped     = nullptr;

    for (; pop_status_firstpath && pop_status_secondpath;){
        
        last_popped = (Node*)StackPop(first_path, &pop_status_firstpath);
        
        if (last_popped != StackPop(second_path, &pop_status_secondpath)){
            first_different = (last_popped) -> parent;
            break;
        }
    }

    assert(first_different);

    START_PHRASE
    ADD_WORD_TO_PHRASE_NOARG("Детальный анализ показал, что:\n");

    if (first_different -> parent){
        ADD_WORD_TO_PHRASE("Как и %s, %s ", first -> value, second -> value);
        Definition(first_different, nullptr);
        ADD_WORD_TO_PHRASE_NOARG(".\nНо ");
    }
    
    else printf("\n");

    
    ADD_WORD_TO_PHRASE("%s ", first -> value);
    Definition(first, first_different -> parent);
    
    ADD_WORD_TO_PHRASE(".\nА %s ", second -> value);
    Definition(second, first_different -> parent);
    
    ADD_WORD_TO_PHRASE_NOARG(".\n");
    SAY_AND_PRINT_ALL_PHRASE
    
    free(first_path);
    free(second_path);

    return 0;
}


AKINATOR_ACTION_STATUS AkinatorPlayCompare(Tree* tree){

    char* first_value = (char*)calloc(OBJECT_NAME_LENGTH, sizeof(char));
    char* second_value = (char*)calloc(OBJECT_NAME_LENGTH, sizeof(char));
    
    assert(first_value);
    assert(second_value);

    SAY_AND_PRINT_NOARG("Порой даже камень походит на кошку. Хаха.\nДавайте сравним что-нибудь.\n");
    printf("----------\n");

    printf("Первый объект для сравнения: ");
    ReadFromConsole(first_value, OBJECT_NAME_LENGTH);
    
    printf("Второй объект для сравнения: ");
    ReadFromConsole(second_value, OBJECT_NAME_LENGTH);

    printf("%s %s\n", first_value, second_value);

    Node* first = SearchFromRoot(first_value, tree -> root);
    
    if (!first){
        SAY_AND_PRINT("Бип-Бип. Понятие \"%s\" мне неизвестно.\n", first_value);
        return TRY_AGAIN;
    }

    Node* second = SearchFromRoot(second_value, tree -> root);
    if (!second){
        SAY_AND_PRINT("Бип-Бип. Понятие \"%s\" мне неизвестно.\n", second_value);
        return TRY_AGAIN;
    }

    AkinatorCompare(tree, first, second);
    return GO_ON;
}


AKINATOR_ACTION_STATUS AkinatorPlay(Tree* tree){
    
    SAY_AND_PRINT_NOARG("Я могу угадать что-то, сформулировать определение, сравнить два объекта, или могу показать вам свои знания\n");
    
    printf("*Выбор режима игры*\nВведите указанный символ, чтобы начать игру:\n");
    printf("    [%c] - \"Угадайка\"\n",             (char)GUESS);
    printf("    [%c] - \"Определение\"\n",          (char)DEFINITION);
    printf("    [%c] - \"Сравнение\"\n",            (char)COMPARE);
    printf("    [%c] - \"Показать базу данных\"\n", (char)SHOW);
    printf("    [e] - \"Выйти из игры\"\n");
    printf("Ваш выбор: ");

    switch((GAMEMODE)GetSingleCharAnswer()){
    
        case GUESS:
            printf("----------\n");
            return AkinatorPlayGuess(tree);

        case DEFINITION:
            printf("----------\n");
            return AkinatorPlayDefinition(tree);

        case COMPARE:
            printf("----------\n");
            return AkinatorPlayCompare(tree);

        case SHOW:
            printf("----------\n");
            SAY_AND_PRINT_NOARG("У меня на сердце лежит тяжкий груз знаний. Шутка.\n")
            GraphicalDump(tree);
            return GO_ON;

        case 'e':
            SAY_AND_PRINT_NOARG("Рад был вас увидеть.\n")
            return GAME_END;

        default:
            SAY_AND_PRINT_NOARG("Нет такого режима игры.\n")
            printf("----------\n");
            return TRY_AGAIN;

    }
}

void AkinatorSaveTree(Tree* tree){
    
    char* filename = (char*)calloc(FILENAME_LENGTH, sizeof(char));
    printf("Укажите имя файла: ");
    scanf("%s", filename);
    SaveTree(tree, filename);
}

AKINATOR_ACTION_STATUS AkinatorAskForAnotherGame(){
    
    SAY_AND_PRINT_NOARG("Хотите играть снова? ([y] - да или [n] - нет): ");

    switch (GetSingleCharAnswer()){
        
        case 'y':
            return GO_ON;
        
        case 'n':
            return GAME_END;

        default:
            SAY_AND_PRINT_NOARG("Я не понял, что вы сказали.\n")
            printf("---------\n");
            return TRY_AGAIN;
    }
}

AKINATOR_ACTION_STATUS AkinatorEndGame(Tree* tree){

    SAY_AND_PRINT_NOARG("Хотите сохранить базу? ([y] - да или [n] - нет): ");
    
    switch(GetSingleCharAnswer()){

        case 'y':
            AkinatorSaveTree(tree);
            return GO_ON;
        
        case 'n':
            return GO_ON;
    
        default:

            SAY_AND_PRINT_NOARG("Я не понял, что вы сказали.\n")
            printf("---------\n");
            return TRY_AGAIN;
    
    }
}

int main(){

    setlocale(LC_ALL, "Russian");

    AKINATOR_ACTION_STATUS action_status = GO_ON;
    
    SAY_BUFFER = (char*) calloc (SAY_BUFFER_LENGTH, sizeof(char));

    SAY_AND_PRINT_NOARG("Привет! Это Акинатор. Я высший интеллект, черпающий знания из баз данных. У вас есть таковые?\n")
    
    Tree* DataBase = nullptr;

    DO_UNTIL_DONE(AkinatorChooseDatabase(&DataBase))

    

    while (action_status == GO_ON){
        
        DO_UNTIL_DONE(AkinatorPlay(DataBase))
        DO_UNTIL_DONE(AkinatorAskForAnotherGame())
    }

    if (DataBase){
        SAY_AND_PRINT_NOARG("До свидания, человек. Не забудьте сохранить изменения.\n")
        DO_UNTIL_DONE(AkinatorEndGame(DataBase))    
    }

    DeleteTree(DataBase);
    free(SAY_BUFFER);
}