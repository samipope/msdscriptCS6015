#include <sstream>
#include "Expr.h"
#include "val.h"


/**
 * Constructs a Num object with a given integer value.
 * @param val The integer value to initialize the Num object with.
 */
Num::Num(int val) {
    this->val = val;
}

/**
 * Constructs an Add object with left and right expressions.
 * @param lhs Pointer to the left-hand side expression.
 * @param rhs Pointer to the right-hand side expression.
 */
Add::Add(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

/**
 * Constructs a Mult object with left and right expressions.
 * @param lhs Pointer to the left-hand side expression.
 * @param rhs Pointer to the right-hand side expression.
 */
Mult::Mult(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

/**
 * Constructs a Var object with a given variable name.
 * @param varPassed The name of the variable.
 */
Var::Var(const std::string& varPassed) : var(std::move(varPassed)) {
}

/**
 * Constructor for the _Let class
 * @param varName the name of the expression ex: "x"
 * @param expr1 the expression being passed ex: "Num(5)" or "Add(2,3)"
 * @param expr2 the expression being passed ex: "Num(5)" or "Add(2,3)"
 */
_Let::_Let(std::string varName, Expr* expr1, Expr* expr2){
    this->varName = varName;
    this->head=expr1;
    this->body=expr2;
}

/**
 * Checks if this Num object is equal to another expression.
 * @param e Pointer to the expression to compare with.
 * @return true if the expressions are equivalent, false otherwise.
 */
bool Num::equals(Expr *e) {
    Num* numPtr = dynamic_cast<Num*>(e);
    return numPtr && this->val == numPtr->val;
}

/**
 * Checks if this Add object is equal to another expression.
 * @param e Pointer to the expression to compare with.
 * @return true if the expressions are equivalent, false otherwise.
 */
bool Add::equals(Expr *e) {
    Add* addPtr = dynamic_cast<Add*>(e);
    return addPtr && ((this->lhs->equals(addPtr->lhs) && this->rhs->equals(addPtr->rhs)) ||
                      (this->lhs->equals(addPtr->rhs) && this->rhs->equals(addPtr->lhs)));
}

/**
 * Checks if this Mult object is equal to another expression.
 * @param e Pointer to the expression to compare with.
 * @return true if the expressions are equivalent, false otherwise.
 */
bool Mult::equals(Expr *e) {
    Mult* multPtr = dynamic_cast<Mult*>(e);
    return multPtr && ((this->lhs->equals(multPtr->lhs) && this->rhs->equals(multPtr->rhs)) ||
                       (this->lhs->equals(multPtr->rhs) && this->rhs->equals(multPtr->lhs)));
}

/**
 * Checks if this Var object is equal to another expression.
 * @param e Pointer to the expression to compare with.
 * @return true if the expressions are equivalent, false otherwise.
 */
bool Var::equals(Expr *e) {
    Var* varPtr = dynamic_cast<Var*>(e);
    return varPtr && this->var == varPtr->var;
}

/**
 * returns if the object passed was the same type of object and if they are the same
 * @param e
 * @return
 */
bool _Let::equals(Expr *e) {
    auto other = dynamic_cast<_Let*>(e); //dyamic cast it to that type
    if (other == nullptr) return false; // if type is not the same, return false
    return varName == other->varName && head->equals(other->head) && body->equals(other->body);
}


/**
 * Evaluates the numeric expression.
 * @return The integer value of the Num object.
 */
Val* Num::interp() {
    return new NumVal(val);
}

/**
 * Evaluates the addition expression.
 * @return The sum of the left and right expressions.
 */
Val* Add::interp() {
    return this->lhs->interp()->add_to(this->rhs->interp());}

/**
 * Evaluates the multiplication expression.
 * @return The product of the left and right expressions.
 */
Val* Mult::interp() {
    return this->lhs->interp()->mult_with(this->rhs->interp());
}

/**
 * Evaluates the variable expression. Throws an error because variables cannot be directly evaluated.
 * @throw std::runtime_error when trying to evaluate a variable.
 */
Val* Var::interp() {
    throw std::runtime_error("no value for variable");
    return new NumVal(-1);
}

/**
 * Interprets the function by passing in the value the variable is set to and solving
 * @return int that the solution equals
 */
Val* _Let::interp() {
    Val* rhsValue = head->interp();
    return body->subst(varName, rhsValue->to_expr())->interp();
}

/**
 * Checks if the Num expression contains a variable.
 * @return false because Num does not contain a variable.
 */
bool Num::hasVariable() {
    return false;
}

/**
 * Checks if the Add expression contains a variable.
 * @return true if either the left or right expressions contain a variable, false otherwise.
 */
bool Add::hasVariable() {
    return this->lhs->hasVariable() || this->rhs->hasVariable();
}

/**
 * Checks if the Mult expression contains a variable.
 * @return true if either the left or right expressions contain a variable, false otherwise.
 */
bool Mult::hasVariable() {
    return this->lhs->hasVariable() || this->rhs->hasVariable();
}

/**
 * Checks if the Var expression contains a variable.
 * @return true because Var represents a variable.
 */
bool Var::hasVariable() {
    return true;
}

/**
 * @return true if either side has a variable (Var object) or not
 */
bool _Let::hasVariable() {
    return head->hasVariable() || body->hasVariable();
}

/**
 * Substitutes a variable with another expression in a Num object.
 * @param stringInput The name of the variable to substitute.
 * @param e The expression to substitute the variable with.
 * @return A new Num object with the same value, since no substitution is needed.
 */
Expr* Num::subst(std::string stringInput, Expr* e) {
    return new Num(this->val);
}

/**
 * Substitutes a variable with another expression in an Add object.
 * @param stringInput The name of the variable to substitute.
 * @param e The expression to substitute the variable with.
 * @return A new Add object with the substituted expressions.
 */
Expr* Add::subst(std::string stringInput, Expr* e) {
    Expr* newLHS = this->lhs->subst(stringInput,e);
    Expr* newRHS = this->rhs->subst(stringInput,e);
    return new Add(newLHS,newRHS);
}

/**
 * Substitutes a variable with another expression in a Mult object.
 * @param stringInput The name of the variable to substitute.
 * @param e The expression to substitute the variable with.
 * @return A new Mult object with the substituted expressions.
 */
Expr* Mult::subst(std::string stringInput, Expr *e) {
    Expr* newLHS = this->lhs->subst(stringInput,e);
    Expr* newRHS = this->rhs->subst(stringInput,e);
    return new Mult(newLHS,newRHS);
}

/**
 * Substitutes a variable with another expression in a Var object.
 * @param stringInput The name of the variable to substitute.
 * @param e The expression to substitute the variable with.
 * @return A new Var object with the variable substituted if the names match, otherwise returns a copy of itself.
 */
Expr* Var::subst(std::string stringInput, Expr *e) {
    if (this->var == stringInput) {
        return e;
    } else {
        return new Var(this->var);
    }
}

/**
 * pass an expression in to sub
 * @param stringInput (new head for new Let object)
 * @param e (Expr)
 * @return new _Let object
 */
Expr *_Let::subst(std::string stringInput, Expr *e) {
    Expr* newHead = head->subst(stringInput, e);
    Expr* newBody = (stringInput == varName) ? body : body->subst(stringInput, e);
    return new _Let(varName, newHead, newBody);
}

/**
 * Prints the Num expression to a given output stream.
 * @param stream The output stream to print to.
 */
void Num::print(std::ostream &stream) {
    stream << std::to_string(val);
}

/**
 * Prints the Add expression to a given output stream.
 * @param stream The output stream to print to.
 */
void Add::print(std::ostream &stream) {
    stream << "(";
    lhs->print(stream);
    stream << "+";
    rhs->print(stream);
    stream << ")";
}

/**
 * Prints the Mult expression to a given output stream.
 * @param stream The output stream to print to.
 */
void Mult::print(std::ostream &stream) {
    stream << "(";
    lhs->print(stream);
    stream << "*";
    rhs->print(stream);
    stream << ")";
}

/**
 * Prints the Var expression to a given output stream.
 * @param stream The output stream to print to.
 */
void Var::print(std::ostream &stream) {
    stream << var;
}

/**
 * Prints out object using a stream
 * @param stream
 */
void _Let::print(std::ostream &stream) {
    stream << "(_let " << varName << "=" << head->to_string() << " _in " << body->to_string() << ")";
}

/**
 * Pretty prints the Num expression with appropriate precedence.
 * @param ot The output stream to print to.
 * @param prec The precedence context in which this expression is being printed.
 */
void Num::pretty_print(std::ostream &ot, precedence_t prec, std::streampos& lastNewLinePos, bool paren) {
    ot << val;
}

/**
 * Pretty prints the Add expression with appropriate precedence.
 * @param ot The output stream to print to.
 * @param prec The precedence context in which this expression is being printed.
 */
void Add::pretty_print(std::ostream &ot, precedence_t prec, std::streampos& lastNewLinePos, bool paren) {
    bool needParens = prec > prec_add;

    //add should always be true??
    if (needParens) ot << "(";
    lhs->pretty_print(ot, static_cast<precedence_t>(prec_add + 1),lastNewLinePos, false);
    ot << " + ";
    rhs->pretty_print(ot, prec_add,lastNewLinePos, needParens);
    if (needParens) ot << ")";
}

/**
 * Pretty prints the Mult expression with appropriate precedence.
 * @param ot The output stream to print to.
 * @param prec The precedence context in which this expression is being printed.
 */
void Mult::pretty_print(std::ostream &ot, precedence_t prec, std::streampos& lastNewLinePos, bool paren) {
    bool needParens = prec > prec_mult;
    if (needParens) ot << "(";
    this->lhs->pretty_print(ot, static_cast<precedence_t>(prec_mult + 1),lastNewLinePos, needParens);
    ot << " * ";
    this->rhs->pretty_print(ot, prec_mult,lastNewLinePos, needParens);
    if (needParens) ot << ")";
}

/**
 * Pretty prints the Var expression with appropriate precedence.
 * @param ot The output stream to print to.
 * @param prec The precedence context in which this expression is being printed.
 */
void Var::pretty_print(std::ostream &ot, precedence_t prec, std::streampos& lastNewLinePos, bool paren) {
    ot << var;
}


/**
 * Prints out object in a more readable way using precedence, stream and streampos
 * @param ot
 * @param prec
 * @param lastNewLinePos
 */
void _Let::pretty_print(std::ostream &ot, precedence_t prec, std::streampos& lastNewLinePos, bool paren) {
    if (!paren && prec != prec_none) {
        ot << "(";
    }
    std::streampos letPosition = ot.tellp();
    std::streampos depth = letPosition - lastNewLinePos;
    ot << "_let " << this->varName<<" = ";
    //print bound expression with passing difference
    this->head->pretty_print(ot,prec_none, depth,paren);
    ot << "\n ";
    std::streampos nextPos = ot.tellp();
    //start print with indentation
    for ( int i = 0; i < letPosition - lastNewLinePos; i++ ) {
        ot << " ";
    }
    ot<< "_in  ";
    this->body->pretty_print(ot, prec_none, nextPos,paren);
    if (!paren && prec != prec_none) {
        ot << ")";
    }

}

void Num::pretty_print_at(std::ostream &ot) {
    std::streampos lastNewLinePos =ot.tellp(); //initiate to 0
    this-> pretty_print(ot,prec_none,lastNewLinePos, false);
}

void Var::pretty_print_at(std::ostream &ot) {
    std::streampos lastNewLinePos =ot.tellp(); //initiate to 0
    this-> pretty_print(ot,prec_none,lastNewLinePos, false);

}

//fixed
void Mult::pretty_print_at(std::ostream &ot) {
    std::streampos lastNewLinePos = ot.tellp(); //initiate to 0
    this-> pretty_print(ot,prec_mult,lastNewLinePos, false);
}

//fixed
void Add::pretty_print_at(std::ostream &ot) {
    std::streampos lastNewLinePos =ot.tellp();
    this-> pretty_print(ot,prec_add,lastNewLinePos, false);
}

//fixed
void _Let::pretty_print_at(std::ostream &ot) {
    std::streampos lastNewLinePos =ot.tellp();
    this-> pretty_print(ot,prec_none,lastNewLinePos, false);
}


 //-----------------------EqExpr--------------------------
/**
 * Constructor for EqExpr
 * @param lhs
 * @param rhs
 */
EqExpr::EqExpr(Expr *lhs, Expr *rhs) {
    this->rhs = rhs;
    this->lhs =lhs;
}

bool EqExpr::equals(Expr *e) {
    EqExpr* eq = dynamic_cast<EqExpr*>(e);
    if (eq == nullptr) return false;
    return lhs->equals(eq->lhs) && rhs->equals(eq->rhs);
}

Val *EqExpr::interp() {
    Val* leftVal = lhs->interp();
    Val* rightVal = rhs->interp();
    bool result = leftVal->equals(rightVal);
    delete leftVal;
    delete rightVal;
    return new BoolVal(result);
}

bool EqExpr::hasVariable() {
    return lhs->hasVariable() || rhs->hasVariable();
}

Expr *EqExpr::subst(std::string stringInput, Expr *e) {
    return new EqExpr(lhs->subst(stringInput,e), rhs->subst(stringInput, e));
}

void EqExpr::print(std::ostream &stream) {
    lhs->print(stream);
    stream << " == ";
    rhs->print(stream);
}

void EqExpr::pretty_print_at(std::ostream &ot) {
    std::streampos lastNewLinePos =ot.tellp(); //TODO what is precedence??
    this-> pretty_print(ot,prec_none,lastNewLinePos, false);
}

void EqExpr::pretty_print(std::ostream &ot, precedence_t prec, std::streampos &lastNewLinePos, bool paren) {
    if (paren) ot << "(";
    lhs->pretty_print(ot, prec, lastNewLinePos, false);
    ot << " == ";
    rhs->pretty_print(ot, prec, lastNewLinePos, false);
    if (paren) ot << ")";
}


//----------------------IfExpr-----------------------

IfExpr::IfExpr(Expr *condition, Expr *thenExpr, Expr *elseExpr) {
    this->condition=condition;
    this->thenExpr=thenExpr;
    this->elseExpr=elseExpr;
}

bool IfExpr::equals(Expr *e) {
    IfExpr* other = dynamic_cast<IfExpr*>(e);
    return other != nullptr
           && condition->equals(other->condition)
           && thenExpr->equals(other->thenExpr)
           && elseExpr->equals(other->elseExpr);
}

Val *IfExpr::interp() {
    Val* condVal = condition->interp();
    BoolVal* boolVal = dynamic_cast<BoolVal*>(condVal);
    if (boolVal == nullptr) {
        throw std::runtime_error("Condition expression did not evaluate to a boolean");
    }
    Val* result = boolVal->is_true() ? thenExpr->interp() : elseExpr->interp();
    delete condVal;
    return result;
}

bool IfExpr::hasVariable() {
    return condition->hasVariable() || thenExpr->hasVariable() || elseExpr->hasVariable();
}

Expr *IfExpr::subst(std::string stringInput, Expr *e) {
    return new IfExpr(condition->subst(stringInput, e),
                      thenExpr->subst(stringInput, e),
                      elseExpr->subst(stringInput, e));
}


void IfExpr::print(std::ostream &stream) {
    stream << "_if ";
    condition->print(stream);
    stream << " _then ";
    thenExpr->print(stream);
    stream << " _else ";
    elseExpr->print(stream);
}

void IfExpr::pretty_print_at(std::ostream &ot) {
    std::streampos lastNewLinePos =ot.tellp(); //TODO what is precedence??
    this-> pretty_print(ot,prec_none,lastNewLinePos, false);
}

void IfExpr::pretty_print(std::ostream &ot, precedence_t prec, std::streampos &lastNewLinePos, bool paren) {
    if (paren) ot << "(";
    ot << "_if ";
    condition->pretty_print(ot, prec, lastNewLinePos, false);
    ot << " _then ";
    thenExpr->pretty_print(ot, prec, lastNewLinePos, false);
    ot << " _else ";
    elseExpr->pretty_print(ot, prec, lastNewLinePos, false);
    if (paren) ot << ")";
}

//------------BoolExpr---------------------

BoolExpr::BoolExpr(bool value) {
    this->value =value;
}

bool BoolExpr::equals(Expr *e) {
    BoolExpr* be = dynamic_cast<BoolExpr*>(e);
    return be != nullptr && value == be->value;
}

Val *BoolExpr::interp() {
    return new BoolVal(value);
}

bool BoolExpr::hasVariable() {
    return false;
}

Expr *BoolExpr::subst(std::string stringInput, Expr *e) {
    return new BoolExpr(value);
}

void BoolExpr::print(std::ostream &stream) {
    stream << (value ? "_true" : "_false");
}

void BoolExpr::pretty_print(std::ostream &ot, precedence_t prec, std::streampos &lastNewLinePos, bool paren) {
    print(ot);
}

void BoolExpr::pretty_print_at(std::ostream &ot) {
    print(ot);
}