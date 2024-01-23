//
// Created by Samantha Pope on 1/19/24.
//

#include "Expr.h"


Num::Num(int val) {
    this->val = val;
}

bool Num::equals(Expr *e)  {
    //check if the object type is same (num)
    Num* numPtr = dynamic_cast<Num*>(e);
    if(!numPtr){ //if not a num, return false
        return false;
    }
    return this->val == numPtr->val;
}

Add::Add(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
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

Mult::Mult(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
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

VarExpr::VarExpr(const std::string& varPassed) {
    this->var = varPassed;
    //compiler wants me to use std::move to avoid copies here?
}

bool VarExpr::equals(Expr *e) {
    //check if the object type is same (num)
    VarExpr* varPtr = dynamic_cast<VarExpr*>(e);
    if(!varPtr){ //if not a num, return false
        return false;
    }
    return this->var == varPtr->var;
}
