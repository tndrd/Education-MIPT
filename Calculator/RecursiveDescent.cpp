#include "RecursiveDescent.h"

const int expression_buffer_length = 1024;

const char* SIN_OPER = "sin";
const char* COS_OPER = "cos";
    


char* s = nullptr;
int p = 0;


#define Require(chr)\
    if (s[p] == chr) p++;\
    else SyntaxError(); 


void SyntaxError(){
    printf("Syntax error: %s\n", s);
    for (int i = 0; i < p + 14; i++) printf(" ");
    printf("^\n");
    for (int i = 0; i < p + 14; i++) printf(" ");
    printf("|\n");
}

double GetN();
double GetP();
double GetT1();
double GetT2();
double GetT3();
double GetE();
double GetG(char* str);


#define SKIP_SPACES\
    for(; isspace(s[p]); p++);


double GetN(){

    double val = 0;
    int prev_p = p;
    
    while ('0' <= s[p] && s[p] <= '9'){
        
        val = val*10 + s[p] - '0';
        p++;
    }

    if (p == prev_p) SyntaxError;
    return val;
}


double GetE(){

    SKIP_SPACES

    double  lvalue = GetT1();
    double  rvalue = 0;
    
    SKIP_SPACES

    int  current_operator = p;

    while(s[p] == '+' || s[p] == '-'){
        
        SKIP_SPACES

        current_operator = p;
        p++;

        SKIP_SPACES
        
        rvalue = GetT1();

        SKIP_SPACES

        if (s[current_operator] == '+') lvalue += rvalue;
        else                            lvalue -= rvalue;
    }
    return lvalue;
}


double GetT1(){

    SKIP_SPACES    

    double  lvalue = GetT2();
    double  rvalue = 0;

    SKIP_SPACES

    int  current_operator = p;

    while(s[p] == '*' || s[p] == '/'){
        
        SKIP_SPACES

        current_operator = p;
        p++;

        SKIP_SPACES

        rvalue = GetT2();

        SKIP_SPACES

        if (s[current_operator] == '*') lvalue *= rvalue;
        else                            lvalue /= rvalue;
    }
    return lvalue;
}

double GetT2(){

    SKIP_SPACES

    double  lvalue = GetT3();
    double  rvalue = 0;
    
    SKIP_SPACES

    int  current_operator = p;

    while(s[p] == '^'){
        
        SKIP_SPACES

        current_operator = p;
        p++;
        
        SKIP_SPACES

        rvalue = GetT3();
        
        SKIP_SPACES

        lvalue = pow(lvalue, rvalue);
    }
    return lvalue;
}

double GetT3(){

    SKIP_SPACES
    
    int current_operator_begginning = p;
    
    if (!strncmp(SIN_OPER, s + current_operator_begginning, strlen(SIN_OPER))){
        
        p+=strlen(SIN_OPER);
        return sin(GetP());
    }
    if (!strncmp(COS_OPER, s + current_operator_begginning, strlen(COS_OPER))){
        
        p+=strlen(COS_OPER);
        return cos(GetP());
    }

    return GetP();
}

double GetP(){

    SKIP_SPACES

    if(s[p] == '('){

        SKIP_SPACES

        p++;

        SKIP_SPACES

        double val = GetE();
        
        SKIP_SPACES

        Require(')')
        
        SKIP_SPACES

        return val;
    }
    else
        return GetN();
}


double GetG(char* str){
    
    s = str;
    p = 0;

    SKIP_SPACES

    double val = GetE();

    SKIP_SPACES

    Require('$')
    return val;
}


int main(){

    char* expression = (char*)calloc(1024, sizeof(char));
    fgets(expression, 1024, stdin);
    
    printf("%lf\n", GetG(expression));
}