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
        char filename[15];
        printf("Specify database name: ");
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

Node* SearchAtNode(char* value, Node* current, int* node_counter){

    assert(current -> value);
    assert(value);
    assert(node_counter);

    if (*node_counter > (current -> tree) -> size) return nullptr;
    (*node_counter)++;
    
    if (!strcmp(current -> value, value)) return current;
    
    Node* found = nullptr;
    
    if (current -> left) found = SearchAtNode(value, current -> left, node_counter);
    if (found) return found;
    
    if (current -> right) found = SearchAtNode(value, current -> right, node_counter);
    
    return found;

}

void DefineObject(Node* current, int last, int state){
    
    assert(current);
    assert(current -> value);
    
    if (current -> parent) {
        if (current == (current -> parent) -> right) DefineObject(current -> parent, 0, 0);
        else DefineObject(current -> parent, 0, 1);
    }

    if (last) printf(" and "); 
    else if (current -> parent) printf(", ");
    
    if (!state){
        printf("not ");
    }

    printf("%s", current -> value);
}


void AkinatorPlayDefinition(Tree* tree){

    char* definition = (char*)calloc(20, sizeof(char));
    
    printf("What should I tell you about: ");
    scanf("%s", definition);
    
    int node_counter = 0;
    
    Node* object = SearchAtNode(definition, tree -> root, &node_counter);
    if (!object){
        printf("I do not know anything about it. You may try something else\n");
        AkinatorPlayDefinition(tree);
    }
    else{
    
        printf("%s is ", object -> value);
        
        if (object == (object -> parent) -> right)
            DefineObject(object -> parent, 1, 0);
        else
            DefineObject(object -> parent, 1, 1);
        
        printf("\n");
    }
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
        printf("Will be available soon\n");
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