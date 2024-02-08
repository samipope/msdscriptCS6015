#include <sstream>
#include "Expr.h"

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
 * Evaluates the numeric expression.
 * @return The integer value of the Num object.
 */
int Num::interp() {
    return this->val;
}

/**
 * Evaluates the addition expression.
 * @return The sum of the left and right expressions.
 */
int Add::interp() {
    return this->lhs->interp() + this->rhs->interp();
}

/**
 * Evaluates the multiplication expression.
 * @return The product of the left and right expressions.
 */
int Mult::interp() {
    return this->lhs->interp() * this->rhs->interp();
}

/**
 * Evaluates the variable expression. Throws an error because variables cannot be directly evaluated.
 * @throw std::runtime_error when trying to evaluate a variable.
 */
int Var::interp() {
    throw std::runtime_error("no value for variable");
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
 * Pretty prints the Num expression with appropriate precedence.
 * @param ot The output stream to print to.
 * @param prec The precedence context in which this expression is being printed.
 */
void Num::pretty_print(std::ostream &ot, precedence_t prec) {
    ot << val;
}

/**
 * Pretty prints the Add expression with appropriate precedence.
 * @param ot The output stream to print to.
 * @param prec The precedence context in which this expression is being printed.
 */
void Add::pretty_print(std::ostream &ot, precedence_t prec) {
    bool needParens = prec > prec_add;
    if (needParens) ot << "(";
    lhs->pretty_print(ot, static_cast<precedence_t>(prec_add + 1));
    ot << " + ";
    rhs->pretty_print(ot, prec_add);
    if (needParens) ot << ")";
}

/**
 * Pretty prints the Mult expression with appropriate precedence.
 * @param ot The output stream to print to.
 * @param prec The precedence context in which this expression is being printed.
 */
void Mult::pretty_print(std::ostream &ot, precedence_t prec) {
    bool needParens = prec > prec_mult;
    if (needParens) ot << "(";
    lhs->pretty_print(ot, static_cast<precedence_t>(prec_mult + 1));
    ot << " * ";
    rhs->pretty_print(ot, prec_mult);
    if (needParens) ot << ")";
}

/**
 * Pretty prints the Var expression with appropriate precedence.
 * @param ot The output stream to print to.
 * @param prec The precedence context in which this expression is being printed.
 */
void Var::pretty_print(std::ostream &ot, precedence_t prec) {
    ot << var;
}
