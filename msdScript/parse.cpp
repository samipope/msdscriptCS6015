////
//// Created by Samantha Pope on 2/13/24.
////

#include "Expr.h"
#include "val.h"
#include <iostream>
#include "parse.h"

/**
 * parses expression from a string
 * @param s string that we parse
 * @return the expression
 */
Expr *parse_str(string s){
    istringstream in(s);
    return parse (in);
}

/**
 * parse expression from an input stream
 * @param in input stream
 * @return expression present
 */
Expr *parse(std::istream &in) {
    Expr *e;
    e = parse_expr(in);
    skip_whitespace(in);
    if (!in.eof()) {
        throw std::runtime_error("invalid input");
    }
    return e;
}


/**
 * parses expression including equality checks from input stream
 * reads input stream --> makes expression objects
 * handles equality by looking for "=="
 * @param in input stream
 * @return pointer to parsed expression
 */
Expr *parse_expr(std::istream &in) {
    Expr* e = parse_comparg(in);
    skip_whitespace(in);
    if(in.peek() == '='){
        consume(in, '=');
        if(in.peek() != '='){
            throw std::runtime_error("need '==' to indicate equal check") ;
        }
        consume(in, '=');
        Expr* rhs = parse_expr(in);
        return new EqExpr(e, rhs);
    }
    return e;
}

/**
 * parse an expression that may include addition
 * if there is a plus, it parses both sides of the addition and returns an Add object
 */
Expr* parse_comparg(istream &in) {

    Expr *e = parse_addend(in);

    skip_whitespace(in);

    if (in.peek() == '+') {
        consume(in, '+');
        Expr *rhs = parse_comparg(in);
        return new Add(e, rhs);
    }
    return e;
}

/**
 * parses multiplication expression from stream
 * @param in input stream
 * @return the Mult object
 */
Expr *parse_addend(std::istream &in) {
    Expr *e;
    e = parse_multicand(in);
    skip_whitespace(in);

    int c = in.peek();
    if (c == '*') {
        consume(in, '*');
        skip_whitespace(in);
        Expr *rhs = parse_addend(in);
        return new Mult(e, rhs);
    } else {
        return e;
    }
}

/**
 * parses term from input stream
 * looks through until it finds a non-letter. used to parse keywords
 * @param in input stream
 * @return string of term
 */
std::string parse_term(istream &in){
    string term;
    while (true) {
        int letter = in.peek();
        if (isalpha(letter)) {
            consume(in, letter);
            char c = letter;
            term += c;
        }
        else
            break;
    }
    return term;
}


/**
 *
 * @param in
 * @return
 */
Expr *parse_num(std::istream &in) {
    int n = 0;
    bool negative = false;
    bool digitSeen = false;

    if (in.peek() == '-') {
        negative = true;
        consume(in, '-');
    }

    while (1) {
        int c = in.peek();
        if (isdigit(c)) {
            consume(in, c);
            n = n * 10 + (c - '0');
            digitSeen = true;
        } else
            break;
    }
    if (negative && !digitSeen){
        throw std::runtime_error("invalid input");
    }
    if (negative) {
        n = -n;
    }
    return new Num(n);
}

/**
 *
 * @param in
 * @return
 */
Expr *parse_multicand(std::istream &in) {
    skip_whitespace(in);
    int c = in.peek();
    if ((c == '-' || isdigit(c))) {
        return parse_num(in);
    } else if (c == '('){
        consume(in, '(');
        Expr *e = parse_expr(in);
        skip_whitespace(in);
        c = in.get();
        if (c != ')') {
            throw std::runtime_error("missing closing parentheses");
        }
        return e;}
    else if (isalpha(c)) {
        return parse_variable(in);
    }

    else if (c=='_'){
        consume(in, '_');

        string term = parse_term(in);

        if(term == "let"){
            return parse_let(in);
        }
        else if(term == "if"){
            return parse_if(in);
        }
        else if(term == "true"){
            return new BoolExpr(true);
        }
        else if(term == "false"){
            return new BoolExpr(false);
        }
        else{
            throw runtime_error("invalid input");
        }
    }
    else {
        consume(in, c);
        throw runtime_error("invalid input");
    }

}
/**
 *
 * @param in
 * @return
 */
Expr *parse_variable(std::istream &in) {
    std::string var;
    //forever loop
    while (true) {
        int c = in.peek();
        //check that the char is valid
        if (isalpha(c)) {
            consume(in, c);
            //add the char it's on to the var string
            var += static_cast<char>(c);
        }
        else {
            break;
        }
    }
    //return the var
    return new Var(var);
}





void consume(std::istream &in, int expect) {
    int c = in.get();
    if (c != expect) {
        throw std::runtime_error("consume mismatch");
    }
}

void consume(std::istream & stream, const std::string & str)
{
    for ( char c : str )
    {
        const int real = stream.get();
        if ( real != c ){
            throw std::runtime_error( "consume STRING mismatch" );
        }
    }
}

void skip_whitespace(std::istream &in) {
    while (1) {
        int c = in.peek();
        if (!isspace(c))
            break;
        consume(in, c);
    }
}

/**
 *
 * @param in
 * @return
 */
Expr *parse_let(std::istream &in) {
    skip_whitespace(in);
    Expr *e = parse_variable(in);
    string lhs = e->to_string();
    skip_whitespace(in);
    consume(in, '=');
    skip_whitespace(in);
    Expr *rhs = parse_comparg(in);
    skip_whitespace(in);
    consume(in, "_in");
    skip_whitespace(in);
    Expr *body = parse_comparg(in);
    return new _Let(lhs, rhs, body);
}

/**
 *
 * @param in
 * @return
 */
Expr* parse_if(istream &in){
    skip_whitespace(in);
    Expr* ifStatement = parse_expr(in);
    skip_whitespace(in);
    consume(in, "_then");
    skip_whitespace(in);
    Expr* thenStatement = parse_expr(in);
    skip_whitespace(in);
    consume(in, "_else");
    skip_whitespace(in);
    Expr* elseStatment = parse_expr(in);
    return new IfExpr(ifStatement, thenStatement, elseStatment);
}
