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
    if (other_num == nullptr) throw runtime_error("You can't add a non-number!");
    return new NumVal( (unsigned) other_num->numVal + (unsigned) this->numVal); //assigned to unsigned help prevent undefined behavior
}

Val* NumVal::mult_with(Val *other_val) {
    //Insert implementation
    NumVal *other_num = dynamic_cast<NumVal*>(other_val);
    if(other_num == nullptr) throw runtime_error("You can't mult a non-number!");
    return new NumVal((unsigned)this->numVal * (unsigned)other_num->numVal); //assigned to unsigned help prevent undefined behavior
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
    return new BoolExpr(this->value);
}

Val *BoolVal::add_to(Val *other_val) {
    throw std::runtime_error("Cannot add boolean values");
}

Val *BoolVal::mult_with(Val *other_val) {
    throw std::runtime_error("Cannot multiply boolean values");
}

void BoolVal::print(std::ostream &ostream) {
    ostream <<:: to_string(value);
}

bool BoolVal::is_true() {
    return value;
}