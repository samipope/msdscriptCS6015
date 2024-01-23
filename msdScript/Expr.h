// Expr.h

#ifndef CS6015PROJECT_EXPR_H
#define CS6015PROJECT_EXPR_H
#include <string>

class Expr {
public:
    virtual bool equals(Expr *e) = 0;
};

class Num : public Expr {
public:
    int val;
    Num(int val);
    bool equals(Expr *e) override;
};

class Add : public Expr {
public:
    Expr *lhs;
    Expr *rhs;

    Add(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) override;
};

class Mult : public Expr {
public:
    Expr *lhs;
    Expr *rhs;

    Mult(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) override;
};

class VarExpr : public Expr {
public:
    std::string var;
    VarExpr(const std::string& varPassed);
    bool equals(Expr *e) override;
};


#endif //CS6015PROJECT_EXPR_H
