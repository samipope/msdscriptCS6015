//
// Created by Samantha Pope on 3/15/24.
//

#include "val.h"
#include "Expr.h"

string Val::to_string(){
    stringstream stream("");
    this->print(stream);
    return stream.str();
}

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
    ostream <<::to_string(numVal);
}
