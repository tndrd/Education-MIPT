#include "Tree.cpp"

#define SAY(text) system("echo " text " | festival --tts");\

#define SAY_AND_PRINT(text)\
    printf(text "\n");\
    SAY(text)\


Tree* AkinatorChooseDatabase(){
    
    printf("Type [l]oad or [n]ew: ");
    
    char mode = getc(stdin);
    Tree* DataBase = nullptr;
    
    if (mode == 'l'){
        printf("Specify database name: ");
        
        char filename[20];
        scanf("%s", filename);
        DataBase = ReadTree(filename);
        
        if(!DataBase){
            printf("There's no file with this name!\n-------\n");
            getc(stdin);
            return AkinatorChooseDatabase();
        }
    }
    else if (mode == 'n'){
        DataBase = NewTree("Unknown");
    }
    else{
        printf("Unknown mode\n");
        return AkinatorChooseDatabase();
    }
    return DataBase;
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


void AddAttribute(Node* current_node){
    
    char* new_object    = (char*)calloc(20, sizeof(char));
    char* new_attribute = (char*)calloc(20, sizeof(char));
    
    printf("Please tell me, what you wished for: ");
    scanf("%s", new_object);
    
    printf("What's the difference between %s and %s: ", new_object, current_node -> value);
    scanf("%s", new_attribute);
    
    SplitByAtribute(new_attribute, current_node, new_object);
}


void SwitchNode(Node* current_node){

    printf("Is it %s? ([y]es or [n]o): ", current_node -> value);
    
    getc(stdin);
    char answer = getc(stdin);
    
    if (answer == 'y'){
        if (current_node -> left){
            SwitchNode(current_node -> left);
        }
        else{
            SAY_AND_PRINT("Wee! I guessed. Thank you for participation!");
        }
    }
    
    else if (answer == 'n'){
        if (current_node -> right){
            SwitchNode(current_node -> right);
        }
        else{
            printf("Oh No! I'm so stupid I don't know this word!\n");
            AddAttribute(current_node);
        }
    }
    else{
        printf("I dont know what this answer mean((\n");
        SwitchNode(current_node);
    }
}


void AkinatorPlayGuess(Tree* DataBase){    
    SwitchNode(DataBase -> root);
}


Node* SearchNodeRecursively(char* value, Node* current, int* node_counter){

    assert(current -> value);
    assert(value);
    assert(node_counter);

    if (*node_counter > (current -> tree) -> size) return nullptr;
    (*node_counter)++;
    
    if (!strcmp(current -> value, value)) return current;
    
    Node* found = nullptr;
    
    if (current -> left) found = SearchNodeRecursively(value, current -> left, node_counter);
    if (found) return found;
    
    if (current -> right) found = SearchNodeRecursively(value, current -> right, node_counter);
    
    return found;

}

Node* SearchFromRoot(char* value, Node* root){
    int counter = 0;
    return SearchNodeRecursively(value, root, &counter);
}


void DefineObject(Node* current, Node* endpoint, int last, int state){
    
    assert(current);
    assert(current -> value);
    if (current -> parent != endpoint) {
        if (current == (current -> parent) -> right) DefineObject(current -> parent, endpoint, 0, 0);
        else DefineObject(current -> parent, endpoint, 0, 1);
    }
    
    if (last) printf(" and "); 
    else if (current -> parent != endpoint) printf(", ");

    if (!state){
        printf("not ");
    }
    printf("%s", current -> value);
}


int Definition(Node* object, Node* endpoint){
        
        assert(object);
        assert((object -> parent) -> right);
        
        if (object == (object -> parent) -> right)
            DefineObject(object -> parent, endpoint, 1, 0);
        else
            DefineObject(object -> parent, endpoint, 1, 1);
        
        printf("\n");
        return 0;
}


void AkinatorPlayDefinition(Tree* tree){

    char* definition = (char*)calloc(20, sizeof(char));
    
    printf("What should I tell you about: ");
    scanf("%s", definition);
    
    
    Node* object = SearchFromRoot(definition, tree -> root);
    if (!object){
        printf("I do not know anything about it. You may try something else\n");
        AkinatorPlayDefinition(tree);
    }
    else{
        printf("%s is ", object -> value);
        Definition(object, nullptr);
    }
}


int GetPath(Node** path, Node* current_node, int* counter){
        
    if (current_node -> parent) GetPath(path, current_node -> parent, counter);
    
    if (*counter > (current_node -> tree) -> size) return 0;
    
    path[*counter] = current_node;
    (*counter)++;

    return 1; 
    
}


int AkinatorCompare(Tree* tree, Node* first, Node* second){

    Node** first_path = (Node**)calloc(tree -> size, sizeof(Node*));
    Node** second_path = (Node**)calloc(tree -> size, sizeof(Node*));
    
    int first_counter = 0;
    int second_counter = 0;
    
    if(!GetPath(first_path, first,   &first_counter)) return 1;
    if(!GetPath(second_path, second, &second_counter)) return 2;

    if (first_counter > second_counter) first_counter = second_counter; //choose minimal of counters not to leave the arrays

    Node* first_different = tree -> root;

    for (int i = 1; i < first_counter; i++){
        if (first_path[i] != second_path[i]){
            first_different = (first_path[i]) -> parent;
            break;
        }
    }

    printf("%s and %s both are ", first -> value, second -> value);
    
    Definition(first_different, nullptr);
    
    printf("\nBut %s is ", first -> value);
    Definition(first, first_different -> parent);
    printf("\nAnd %s is ", second -> value);
    Definition(second, first_different -> parent);
    printf("\n");
    return 0;
}

int AkinatorPlayCompare(Tree* tree){

    char* first_value = (char*)calloc(20, sizeof(char));
    char* second_value = (char*)calloc(20, sizeof(char));

    printf("Type first  object name: ");
    scanf("%s", first_value);
    printf("Type second object name: ");
    scanf("%s", second_value);

    Node* first = SearchFromRoot(first_value, tree -> root);
    if (!first){
        printf("Haven't found %s\n", first_value);
        return 0;
    }

    Node* second = SearchFromRoot(second_value, tree -> root);
    if (!second){
        printf("Haven't found %s\n", second_value);
        return 0;
    }

    AkinatorCompare(tree, first, second);
    return 1;
}

void AkinatorPlay(Tree* tree){
    
    printf("Do you want me to [g]uess something, or to tell [d]efenition of object, or [c]ompare objects, or [s]how the database: ");
    
    getc(stdin);
    char mode = getc(stdin);
    
    if (mode == 'g'){
        printf("----------\n");
        AkinatorPlayGuess(tree);
    }
    else if (mode == 'd'){
        printf("----------\n");
        AkinatorPlayDefinition(tree);
    }
    else if (mode == 'c'){
        printf("----------\n");
        AkinatorPlayCompare(tree);
    }
    else if (mode == 's'){
        GraphicalDump(tree);
    }
    else{
        getc(stdin);
        printf("I dont have this gamemode(\n");
        printf("----------\n");
        AkinatorPlay(tree);
    }

    printf("Do you want to play again? ([y]es or [n]o): ");
    getc(stdin);
    mode = getc(stdin);

    if (mode == 'y'){
        printf("----------\n");
        AkinatorPlay(tree);
    }
    else{
        
        printf("Do you want to save the data? ([y]es or [n]o): ");
        getc(stdin);
        mode = getc(stdin);

        if (mode == 'y'){
            
            char* filename = (char*)calloc(20, sizeof(char));
            printf("Specify the filename: ");
            scanf("%s", filename);
            SaveTree(tree, filename);
        }
    } 

}


int main(){
    
    printf("Hi! It`s Akinator. Do you want to load the database or create new?\n");
    Tree* DataBase = AkinatorChooseDatabase();    
    
    AkinatorPlay(DataBase);
}