// Expr.h

#ifndef CS6015PROJECT_EXPR_H
#define CS6015PROJECT_EXPR_H
#include <string>
#include <stdexcept>


typedef enum {
    prec_none,   // = 0
    prec_add,    // = 1
    prec_mult    // = 2
} precedence_t;

class Expr {
public:
    virtual ~Expr() = default; //virtual destructor - allows me to write tests using "delete" to test my deepCopy (prevents memory links)
    virtual bool equals(Expr *e) = 0;
    virtual int interp() =0;
    virtual bool hasVariable()=0;
    virtual Expr* subst(std::string stringInput, Expr* e)=0;
//    virtual void print(std::ostream& stream)=0;
//    virtual void pretty_print(std::ostream& ot, precedence_t prec)=0;
//    void pretty_print(std::ostream &ot){
//        pretty_print(ot,prec_none);
 //   }
};

class Num : public Expr {
public:
    int val;
    Num(int val);
    bool equals(Expr *e) override;
    int interp() override;
    bool hasVariable() override;
    Expr* subst(std::string stringInput, Expr* e) override;
//    void print(std::ostream& stream) override;
//    std::string to_string();
//protected:
//    void pretty_print(std::ostream& ot, precedence_t prec) override;
};

class Add : public Expr {
public:
    Expr *lhs;
    Expr *rhs;

    Add(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) override;
    int interp() override;
    bool hasVariable() override;
    Expr* subst(std::string stringInput, Expr* e) override;
//    void print(std::ostream& stream) override;
//    std::string to_string();
//protected:
//    void pretty_print(std::ostream& ot, precedence_t prec) override;

};

class Mult : public Expr {
public:
    Expr *lhs;
    Expr *rhs;

    Mult(Expr *lhs, Expr *rhs);
    bool equals(Expr *e) override;
    int interp() override;
    bool hasVariable() override;
    Expr* subst(std::string stringInput, Expr* e) override;
//    void print(std::ostream& stream) override;
//    std::string to_string();
//protected:
//    void pretty_print(std::ostream& ot, precedence_t prec) override;

};

class Var : public Expr {
public:
    std::string var;
    Var(const std::string& varPassed);
    bool equals(Expr *e) override;
    int interp() override;
    bool hasVariable() override;
    Expr* subst(std::string stringInput, Expr* e) override;
//    void print(std::ostream& stream) override;
//    std::string to_string();
//protected:
//    void pretty_print(std::ostream& ot, precedence_t prec) override;

};


#endif //CS6015PROJECT_EXPR_H
