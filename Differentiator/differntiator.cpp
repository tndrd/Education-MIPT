#include "exprtree.cpp"

const double e = 2.71828;

Node* CreateNode(TYPE node_type, double value, Node* left, Node* right){

    Node* new_node = (Node*)calloc(1,sizeof(Node));
    
    new_node -> type   = node_type;
    new_node -> parent = nullptr;
    new_node -> value = value;
    new_node -> left   = left;
    new_node -> right  = right;
    return new_node;
}

Node* CopyNode(Node* source_node){
    
    Node* new_node = (Node*)calloc(1,sizeof(Node));
    
    *new_node = *source_node;
    if (source_node -> left) new_node -> left = CopyNode(source_node -> left);
    if (source_node -> right) new_node -> right = CopyNode(source_node -> right);
    
    return new_node;
}

#define d(arg) Diff(arg, n_var)

#define L node -> left
#define R node -> right


#define cL CopyNode(L)
#define cR CopyNode(R)


#define dMUL(left, right) ADDITION(MULTIPLY(d(left), CopyNode(right)), MULTIPLY(d(right), CopyNode(left)))

#define UNR_OPER_NODE(operation, right)         CreateNode(OPER, (double)(operation), nullptr, right)
#define BIN_OPER_NODE(operation, left, right)   CreateNode(OPER, (double)(operation), left, right)

#define CONSTANT(val)         CreateNode(CONST, val, nullptr, nullptr)     

#define ADDITION(left, right) BIN_OPER_NODE(ADD, left, right)
#define SUBTRACT(left, right) BIN_OPER_NODE(SUB, left, right)
#define MULTIPLY(left, right) BIN_OPER_NODE(MUL, left, right)
#define DIVISION(left, right) BIN_OPER_NODE(DIV, left, right)
#define EXPONENT(left, right) BIN_OPER_NODE(EXP, left, right)

#define LOGN(right)           UNR_OPER_NODE(LN, right) 
#define SINUS(right)          UNR_OPER_NODE(SIN, right)
#define COSINUS(right)        UNR_OPER_NODE(COS, right)

#define NEG(expr) MULTIPLY(CONSTANT(-1), expr)

Node* Diff (Node* node, int n_var){

    Node* rdiff = nullptr;

    switch(node -> type){

        case CONST:
            return CONSTANT(0);
        
        case VAR:
            if ((int)(node -> value) == n_var) 
                return CONSTANT(1);
            else
                return CONSTANT(0);

        case OPER:

            switch ((OPERATION)(node -> value)){

                case ADD: return ADDITION(d(L), d(R));
                
                case SUB:  return SUBTRACT(d(L), d(R));

                case MUL:  return dMUL(L,R);
                
                case DIV:  return DIVISION(SUBTRACT(MULTIPLY(d(L), cR), MULTIPLY(d(R), cL)), EXPONENT(cR, CONSTANT(2)));
                
                case LN:   return DIVISION(d(R), cR);

                case EXP:  return MULTIPLY(EXPONENT(cL, cR), dMUL(LOGN(L), R));
                        
                case SIN:  return MULTIPLY(d(R), COSINUS(cR));

                case COS:  return MULTIPLY(d(R), NEG(SINUS(cR)));

                case TAN:  return DIVISION(d(R), EXPONENT(COSINUS(cR), CONSTANT(2)));

                case COT:  return NEG(DIVISION(d(R), EXPONENT(SINUS(cR), CONSTANT(2))));

                case ATAN: return DIVISION(d(R), ADDITION(CONSTANT(1), EXPONENT(cR,CONSTANT(2))));

                case ACOT: return NEG(DIVISION(d(R), ADDITION(CONSTANT(1), EXPONENT(cR,CONSTANT(2)))));

                case ASIN: return DIVISION(d(R), EXPONENT(SUBTRACT(CONSTANT(1), EXPONENT(cR, CONSTANT(2))), CONSTANT(0.5)));

                case ACOS: return NEG(DIVISION(d(R), EXPONENT(SUBTRACT(CONSTANT(1), EXPONENT(cR, CONSTANT(2))), CONSTANT(0.5))));

                default:
                    printf("Unknown operation");
                    exit(0);




            }

        default:
            exit(1);  



    }
}


Tree* DifferentiateTree(Tree* tree){

    Tree* diff_tree = (Tree*)calloc(1, sizeof(Tree));
    int n_var = 0;
    (diff_tree -> root) = d(tree -> root);
    
    return diff_tree; 
}

int main(){

    char* filename = (char*)calloc(40, sizeof(char));
    printf("Open: ");
    scanf("%s", filename);
    
    Tree* tree = ReadTree(filename);
    GraphicalDump(tree);
    SimplifyTree(tree);
    GraphicalDump(tree);
    
    
    Tree* diff_tree = DifferentiateTree(tree);
    GraphicalDump(diff_tree);
    
    SimplifyTree(diff_tree);
    GraphicalDump(diff_tree);
    TeXDumpExpressionTree(diff_tree);
    //printf("{%d}\n", CompareStringWithOperator("sadfa", "sadf "));
    
}