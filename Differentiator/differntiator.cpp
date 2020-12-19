#include "exprtree.cpp"

const double e = 2.71828;

Node* CreateNode(TYPE node_type, double value, Node* left, Node* right, Node* parent, Tree* dst_tree){

    Node* new_node = (Node*)calloc(1,sizeof(Node));
    
    assert(new_node);

    new_node -> type   = node_type;
    new_node -> tree   = dst_tree;
    new_node -> parent = parent;
    new_node -> value  = value;
    new_node -> left   = left;
    new_node -> right  = right;

    if (left)  left -> parent = new_node;
    if (right) right -> parent = new_node;

    return new_node;
}

Node* CopyNode(Node* source_node, Tree* dst_tree, Node* parent){
    
    assert(source_node);
    assert(dst_tree);

    Node* new_node = (Node*)calloc(1,sizeof(Node));
    assert(new_node);
    
    new_node -> tree   = dst_tree;
    new_node -> type   = source_node -> type;
    new_node -> value  = source_node -> value;
    new_node -> parent = parent;

    if (source_node -> left) {
        new_node -> left = CopyNode(source_node -> left, dst_tree, new_node);
    }
    
    if (source_node -> right) {
        new_node ->  right = CopyNode(source_node -> right, dst_tree, new_node);
    }
    


    return new_node;
}

#define d(arg) Diff(tree, arg, n_var)

#define L node -> left
#define R node -> right


#define cL CopyNode(L, tree, node)
#define cR CopyNode(R, tree, node)


#define dMUL(left, right) ADDITION(MULTIPLY(d(left), CopyNode(right, tree, node)), MULTIPLY(d(right), CopyNode(left, tree, node)))

#define UNR_OPER_NODE(operation, right)         CreateNode(OPER, (double)(operation), nullptr, right, node, tree)
#define BIN_OPER_NODE(operation, left, right)   CreateNode(OPER, (double)(operation), left, right, node, tree)

#define CONSTANT(val)         CreateNode(CONST, val, nullptr, nullptr, node, tree)     

#define ADDITION(left, right) BIN_OPER_NODE(ADD, left, right)
#define SUBTRACT(left, right) BIN_OPER_NODE(SUB, left, right)
#define MULTIPLY(left, right) BIN_OPER_NODE(MUL, left, right)
#define DIVISION(left, right) BIN_OPER_NODE(DIV, left, right)
#define EXPONENT(left, right) BIN_OPER_NODE(EXP, left, right)

#define LOGN(right)           UNR_OPER_NODE(LN, right) 
#define SINUS(right)          UNR_OPER_NODE(SIN, right)
#define COSINUS(right)        UNR_OPER_NODE(COS, right)

#define NEG(expr) MULTIPLY(CONSTANT(-1), expr)

Node* Diff (Tree* tree, Node* node, int n_var){

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


Tree* DifferentiateTree(Tree* src_tree){

    Tree* tree = (Tree*)calloc(1, sizeof(Tree));
    int n_var = 0;

    (tree -> variables).size     = (src_tree -> variables).size;
    (tree -> variables).capacity = (src_tree -> variables).capacity; 
    (tree -> variables).name_arr = (src_tree -> variables).name_arr;


    /*
    (tree -> variables).name_arr = (char**)calloc((tree -> variables).capacity, sizeof(char*));  

    for (int n_var = 0; n_var < )
    */
    (tree -> root) = d(src_tree -> root);
    
    return tree; 
}

int main(){

    char* filename = (char*)calloc(40, sizeof(char));
    printf("Open: ");
    scanf("%s", filename);
    FILE* fp = fopen("TeXDump.tex", "w");
    
    char* lab_title = ReadFile("title.txt"); 
    fprintf(fp, lab_title);
    free(lab_title);
    
    fprintf(fp, "\n");
    
    Tree* tree = ReadTree(filename);
    free(filename);

    GraphicalDump(tree);

    fprintf(fp, "Выражение, от которого мы будем брать производную:\\\\\n");
    TeXDumpExpressionTree(fp, tree);
    fprintf(fp, "Для начала, по возможности упростим наше выражение:\\\\\n");
    SimplifyTree(fp, tree);
    //GraphicalDump(tree);
    
    
    Tree* diff_tree = DifferentiateTree(tree);
    fprintf(fp, "Теперь продифференцируем:\\\\\n");
    TeXDumpExpressionTree(fp, diff_tree);
    //GraphicalDump(diff_tree);
    fprintf(fp, "Очевидно, что данное выражение можно причесать:\\\\\n");
    

    SimplifyTree(fp, diff_tree);
    //GraphicalDump(diff_tree);
    
    //TeXDumpExpressionTree(diff_tree);
    
    char* lab_end = ReadFile("end.txt");
    fprintf(fp, lab_end);
    free(lab_end);
    
    fclose(fp);

    DeleteTree(tree);
    DeleteTree(diff_tree);

    system("pdflatex TeXDump.tex");
    system("qpdfview TeXDump.pdf");
    //printf("{%d}\n", CompareStringWithOperator("sadfa", "sadf "));
    
}