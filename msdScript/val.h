
//
// Created by Samantha Pope on 3/15/24.
//

#ifndef NEWMSDSCRIPT_VAL_H
#define NEWMSDSCRIPT_VAL_H


#include <stdio.h>
#include <string>

using namespace std;
class Expr;

class Val {
public:
    virtual bool equals (Val *v)= 0;
    virtual Expr* to_expr()= 0;
    virtual Val* add_to(Val* other_val) = 0;
    virtual Val* mult_with(Val* other_val) = 0;
    virtual void print(ostream &ostream) = 0;
    string to_string();
};

class NumVal : public Val{
public:
    int numVal;
    NumVal(int i);
    virtual Expr* to_expr();
    virtual bool equals (Val *v);
    virtual Val* add_to(Val* other_val);
    virtual Val* mult_with(Val* other_val);
    virtual void print (ostream &ostream);
};



#endif //NEWMSDSCRIPT_VAL_H