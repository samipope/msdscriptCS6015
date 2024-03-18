
//
// Created by Samantha Pope on 3/15/24.
//

#ifndef NEWMSDSCRIPT_VAL_H
#define NEWMSDSCRIPT_VAL_H


#include <cstdio>
#include <string>
#include <sstream>

using namespace std;
class Expr;

class Val {
public:
    virtual ~Val() {}
    virtual bool equals (Val *v)= 0;
    virtual Expr* to_expr()= 0;
    virtual Val* add_to(Val* other_val) = 0;
    virtual Val* mult_with(Val* other_val) = 0;
    virtual void print(ostream &ostream) = 0;
    virtual bool is_true()=0;

    std::string to_string(){
         std::stringstream st("");
        this->print(st);
        return st.str();
    }
};

class NumVal : public Val{
public:
    int numVal;
    NumVal(int i);
    bool is_true() override;
    Expr* to_expr() override;
    bool equals (Val *v) override;
    Val* add_to(Val* other_val) override;
    Val* mult_with(Val* other_val) override;
    void print (ostream &ostream) override;
};

class BoolVal : public Val{
public:
    bool value;
   bool is_true() override;
    BoolVal(bool passedBool);
    Expr* to_expr() override;
    bool equals (Val *v) override;
    Val* add_to(Val* other_val) override;
    Val* mult_with(Val* other_val) override;
    void print (ostream &ostream) override;
};



#endif //NEWMSDSCRIPT_VAL_H