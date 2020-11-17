#include "Tree.cpp"


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




void SwitchNode(Node* current_node){

    printf("Is it %s? ([y]es or [n]o): ", current_node -> value);
    getc(stdin);
    char answer = getc(stdin);
    if (answer == 'y'){
        if (current_node -> left){
            SwitchNode(current_node -> left);
        }
        else{
            printf("Wee! I've guessed. Thank you for participation!\n");
        }
    }
    else if (answer == 'n'){
        if (current_node -> right){
            SwitchNode(current_node -> right);
        }
        else{
            printf("Oh No! I'm so stupid I don't know this word!\n");
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


int main(){
    printf("Hi! It`s Akinator. Do you want to load the base or create new?\n");
    Tree* DataBase = AkinatorChooseDatabase();

    GraphicalDump(DataBase); 
    AkinatorPlayGuess(DataBase);
    
    SaveTree(DataBase, "temp.txt");
}