// Expr.h

#ifndef CS6015PROJECT_EXPR_H
#define CS6015PROJECT_EXPR_H
#include <string>
#include <stdexcept>
#include <sstream>

class Val;

//TODO do i need an equals precedent??
typedef enum {
    prec_none,   // = 0
    prec_add,    // = 1
    prec_mult    // = 2
} precedence_t;

class Expr {
public:
    virtual ~Expr() = default; //virtual destructor - allows me to write tests using "delete" to test my deepCopy (prevents memory links)
    virtual bool equals(Expr *e) = 0;
    virtual Val* interp() =0;
    virtual Expr* subst(std::string stringInput, Expr* e)=0;

    std::string to_string() {
        std::stringstream st("");
        this->print(st);
        return st.str();
    }

    std::string to_pp_string(){
        std::stringstream st("");
        this->pretty_print_at(st);
        return st.str();
    }

    virtual void print(std::ostream& stream)=0;
    virtual void pretty_print(std::ostream& ot, precedence_t prec, std::streampos& lastNewLinePos, bool paren)=0;
    virtual void pretty_print_at(std::ostream &ot)=0;

};

class Num : public Expr {
public:
    int val;
    Num(int val);
    bool equals(Expr *e) override;
    Val* interp() override;
    Expr* subst(std::string stringInput, Expr* e) override;
    void print(std::ostream& stream) override;
    void pretty_print_at(std::ostream &ot) override;
protected:
    void pretty_print(std::ostream& ot, precedence_t prec, std::streampos& lastNewLinePos, bool paren) override;
};

class Add : public Expr {
public:
    Expr *lhs;
    Expr *rhs;

    Add(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) override;
    Val* interp() override;
    Expr* subst(std::string stringInput, Expr* e) override;
    void print(std::ostream& stream) override;
    void pretty_print_at(std::ostream &ot) override;

protected:
    void pretty_print(std::ostream& ot, precedence_t prec, std::streampos& lastNewLinePos, bool paren) override;

};

class Mult : public Expr {
public:
    Expr *lhs;
    Expr *rhs;

    Mult(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) override;
    Val* interp() override;
    Expr* subst(std::string stringInput, Expr* e) override;
    void print(std::ostream& stream) override;
    void pretty_print_at(std::ostream &ot) override;

protected:
    void pretty_print(std::ostream& ot, precedence_t prec, std::streampos& lastNewLinePos, bool paren) override;

};

class Var : public Expr {
public:
    std::string var;
    Var(const std::string& varPassed);
    bool equals(Expr *e) override;
    Val* interp() override;
    Expr* subst(std::string stringInput, Expr* e) override;
    void print(std::ostream& stream) override;
    void pretty_print_at(std::ostream &ot) override;

protected:
    void pretty_print(std::ostream& ot, precedence_t prec, std::streampos& lastNewLinePos, bool paren) override;

};


class _Let : public Expr {
public:
    std::string varName;
    Expr* head ;
    Expr* body;

    bool equals(Expr *e) override;
    Val* interp() override;
    Expr* subst(std::string stringInput, Expr* e) override;
    void print(std::ostream& stream) override;
    void pretty_print_at(std::ostream &ot) override;
    _Let(std::string varName, Expr* expr1, Expr* expr2);
    void pretty_print(std::ostream& ot, precedence_t prec, std::streampos& lastNewLinePos, bool paren) override;
};


class BoolExpr: public Expr{
    bool value;
    Expr* subst(std::string stringInput, Expr* e) override;
    void pretty_print_at(std::ostream &ot) override;
    void pretty_print(std::ostream& ot, precedence_t prec, std::streampos& lastNewLinePos, bool paren) override;

public:
    BoolExpr(bool value);
    Val* interp() override;
    void print(std::ostream& stream) override;
    bool equals(Expr *e) override;
};


class IfExpr: public Expr{
    Expr* condition;
    Expr* thenExpr;
    Expr* elseExpr;

    Expr* subst(std::string stringInput, Expr* e) override;
    void pretty_print_at(std::ostream &ot) override;
    void pretty_print(std::ostream& ot, precedence_t prec, std::streampos& lastNewLinePos, bool paren) override;
public:
    IfExpr(Expr* condition, Expr* thenExpr, Expr* elseExpr);
    bool equals(Expr *e) override;
    Val* interp() override;
    void print(std::ostream& stream) override;
};

class EqExpr: public Expr{
    Expr* lhs;
    Expr* rhs;
    Expr* subst(std::string stringInput, Expr* e) override;
    void pretty_print_at(std::ostream &ot) override;
    void pretty_print(std::ostream& ot, precedence_t prec, std::streampos& lastNewLinePos, bool paren) override;
public:
    EqExpr(Expr* lhs, Expr* rhs);
    Val* interp() override;
    bool equals(Expr *e) override;
    void print(std::ostream& stream) override;
};

class FunExpr: public Expr{
public:
    std::string formalArg;
    Expr *body;
    FunExpr(std::string passedArg, Expr *passedBody);
    bool equals(Expr *e) override;
    Val * interp() override;
    Expr * subst(std::string stringInput, Expr *e) override;
    void print(std::ostream &stream) override;
    void pretty_print_at(std::ostream &ot) override;
    void pretty_print(std::ostream &ot, precedence_t prec, std::streampos &lastNewLinePos, bool paren) override;
};

class CallExpr : public Expr{
public:
    Expr *toBeCalled;
    Expr *actualArg;
    CallExpr(Expr *toBePassed, Expr *argPassed);
    bool equals(Expr *e) override;
    Val * interp() override;
    Expr * subst(std::string stringInput, Expr *e) override;
    void print(std::ostream &stream) override;
    void pretty_print_at(std::ostream &ot) override;
    void pretty_print(std::ostream &ot, precedence_t prec, std::streampos &lastNewLinePos, bool paren) override;
};

#endif //CS6015PROJECT_EXPR_H
