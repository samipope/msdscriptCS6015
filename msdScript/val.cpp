//
// Created by Samantha Pope on 3/15/24.
//

#include "val.h"
#include "Expr.h"



NumVal::NumVal(int i) {
    numVal = i;
}

Expr* NumVal::to_expr() {
    return new Num(this->numVal);
}

bool NumVal::equals(Val *v) {
    //Insert implementation
    NumVal* numPointer = dynamic_cast<NumVal*>(v);
    if (numPointer == nullptr){
        return false;
    }
    return this->numVal == numPointer ->numVal;
}

Val* NumVal::add_to(Val *other_val) {
    //Insert implementation
    NumVal *other_num = dynamic_cast<NumVal*>(other_val);
    if (other_num == NULL) throw runtime_error("You can't add a non-number!");
    return new NumVal(other_num->numVal + this->numVal);
}

Val* NumVal::mult_with(Val *other_val) {
    //Insert implementation
    NumVal *other_num = dynamic_cast<NumVal*>(other_val);
    if(other_num == NULL) throw runtime_error("You can't mult a non-number!");
    return new NumVal(this->numVal * other_num->numVal);
}

void NumVal::print(std::ostream &ostream) {
    ostream << numVal;
}

bool NumVal::is_true() {
    throw std::runtime_error("cannot use is_true on NumVal");
}

BoolVal::BoolVal(bool passedBool){
    value = passedBool;
}

bool BoolVal::equals(Val *v) {
    BoolVal* bv = dynamic_cast<BoolVal*>(v);
    return bv != nullptr && value == bv->value;
}

Expr *BoolVal::to_expr() {
    //don't think this will be necessary for successful implementation?
    throw std::runtime_error("Not implemented");
}

Val *BoolVal::add_to(Val *other_val) {
    throw std::runtime_error("Cannot add boolean values");
}

Val *BoolVal::mult_with(Val *other_val) {
    throw std::runtime_error("Cannot multiply boolean values");
}

void BoolVal::print(std::ostream &ostream) {
    ostream << (value ? "_true" : "_false");
}

bool BoolVal::is_true() {
    return value;
}