//
// Created by Samantha Pope on 1/19/24.
//

#include "Expr.h"

//--------------------constructors--------------------------------------
Num::Num(int val) {
    this->val = val;
}

Add::Add(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

Mult::Mult(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

Var::Var(const std::string& varPassed) : var(std::move(varPassed)) {
}


//------------------------equals-----------------------------------
bool Num::equals(Expr *e)  {
    //check if the object type is same (num)
    Num* numPtr = dynamic_cast<Num*>(e);
    if(!numPtr){ //if not a num, return false
        return false;
    }
    return this->val == numPtr->val;
}

bool Add::equals(Expr *e) {
    //check if object type is same (add)
    Add* addPtr = dynamic_cast<Add*>(e);
    if(!addPtr){ //if different object, return false
        return false;
    }
    return (this->lhs->equals(addPtr->lhs) && this->rhs->equals(addPtr->rhs)) ||
           (this->lhs->equals(addPtr->rhs) && this->rhs->equals(addPtr->lhs));

}

bool Mult::equals(Expr *e) {
    //check if object type is same (mult)
    Mult* multPtr = dynamic_cast<Mult*>(e);
    if(!multPtr){
        return false;
    }
    return (this->lhs->equals(multPtr->lhs) && this->rhs->equals(multPtr->rhs)) ||
           (this->lhs->equals(multPtr->rhs) && this->rhs->equals(multPtr->lhs));
}

bool Var::equals(Expr *e) {
    //check if the object type is same (num)
    Var* varPtr = dynamic_cast<Var*>(e);
    if(!varPtr){ //if not a num, return false
        return false;
    }
    return this->var == varPtr->var;
}

//--------------------------interp----------------------------------
int Num::interp() {
    return(int) this->val;
}
int Add::interp() {
    return this->lhs->interp() + this->rhs->interp();
}
int Mult::interp() {
    return this->lhs->interp() * this->rhs->interp();
}
int Var::interp() {
    throw std::runtime_error("no value for variable");
}

//------------------- hasVariable----------------------------------
bool Num::hasVariable() {
    return false;
}
bool Add::hasVariable() {
    return this->lhs->hasVariable() || this->rhs->hasVariable();
}
bool Mult::hasVariable() {
    return this->lhs->hasVariable() || this->rhs->hasVariable();
}
bool Var::hasVariable() {
    return true;
}

//-------------------------subst-------------------------------------------
Expr* Num::subst(std::string stringInput, Expr* e) {
    return new Num(this->val); }

Expr* Add::subst(std::string stringInput, Expr* e) {
    Expr* newLHS = this->lhs->subst(stringInput,e);
    Expr* newRHS = this ->rhs->subst(stringInput,e);
    return new Add(newLHS,newRHS);
}

Expr* Mult::subst(std::string stringInput, Expr *e) {
    Expr* newLHS = this->lhs->subst(stringInput,e);
    Expr* newRHS = this ->rhs->subst(stringInput,e);
    return new Mult(newLHS,newRHS);
}

Expr* Var::subst(std::string stringInput, Expr *e) {
    if (this->var == stringInput) {
        //return deep copy
        return e;
    } else { //if doesn't match, return a copy of this with no changes
        return new Var(this->var);
    }
    }



