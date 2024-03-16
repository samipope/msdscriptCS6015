//
// Created by Samantha Pope on 2/13/24.
//

#include "Expr.h"
#include <iostream>

Expr *parse_expr(std::istream & in); //declaring so other methods can use it

void consume(std::istream &in, int expect) {
    int c = in.get();
    if (c!=expect) {
        throw std::runtime_error("consume mismatch");
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

Expr *parse_num(std::istream &inn) {
    int n = 0;
    bool negative = false;
    bool hasDigits = false;

    if (inn.peek() == '-') {
        negative = true;
        consume(inn, '-');
    }
    while (1) {
        int c = inn.peek();
        if (isdigit(c)) {
            hasDigits = true;
            consume(inn, c);
            n = n * 10 + (c - '0');
        } else {
            break;
        }
    }
    if (negative && !hasDigits) {
        throw std::runtime_error("invalid input");
    }
    if (negative) {
        n = -n;
    }
    // Return a new Num instance with the parsed integer value
    return new Num(n);
}

Expr *parse_variable(std::istream &in) {
    std::string var_name;
    while (isalpha(in.peek())) {
        var_name += in.get();
    }
    if (var_name.empty()) throw std::runtime_error("invalid input");
    return new Var(var_name);
}


Expr* parse_factor(std::istream &in) {
    skip_whitespace(in);
    char c = in.peek();

    if (isdigit(c) || c == '-') {
        return parse_num(in);
    } else if (isalpha(c)) {
        return parse_variable(in);
    } else if (c == '(') {
        consume(in, '(');
        Expr* e = parse_expr(in);
        consume(in, ')');
        skip_whitespace(in); // Make sure to handle whitespace after closing parenthesis
        return e;
    } else {
        throw std::runtime_error("invalid input");
    }
}
Expr* parse_multicand(std::istream &in) {
    Expr* lhs = parse_factor(in); // Parse the left-hand side factor (a number, variable, or parenthesized expression)
    skip_whitespace(in);
    while (true) {
        int c = in.peek();
        if (c == '*') {
            consume(in, c);
            Expr* rhs = parse_factor(in); // Parse the right-hand side factor
            lhs = new Mult(lhs, rhs); // Combine lhs and rhs with multiplication
            skip_whitespace(in);
        } else {
            break;
        }
    }
    return lhs;
}

// This function checks if the next characters form the 'let' keyword followed by a space or an end of line
bool is_let_keyword(std::istream &in) {
    std::streampos startPosition = in.tellg(); // Remember start position

    std::string potentialLet;

    potentialLet += (char)in.get(); // Read first character

    // If the first character is 'l', check the next two characters
    if (potentialLet == "l") {
        char nextChar = in.get(); // Attempt to read second character
        if (nextChar == 'e') {
            potentialLet += nextChar;
            nextChar = in.get(); // Attempt to read third character
            if (nextChar == 't') {
                potentialLet += nextChar;
                if (isspace(in.peek()) || in.peek() == EOF) {
                    return true; // Correctly identified 'let' followed by a space or EOF
                }
            }
        }
    }

    in.seekg(startPosition); // Rewind to start if 'let' not found
    return false;
}


Expr *parse_Let(std::istream & in){
    skip_whitespace(in);
    std::string var_name = parse_variable(in)->to_string(); // Parse variable name

    skip_whitespace(in);
    if (in.get() != '=') throw std::runtime_error("Expected '=' in let expression");

    Expr* value = parse_expr(in); // Parse the value expression

    skip_whitespace(in);
    // Ensure 'in' keyword is present
    char in_keyword[3];
    in.read(in_keyword, 2);
    in_keyword[2] = '\0';
    if (std::string(in_keyword) != "in") throw std::runtime_error("Expected 'in' keyword in let expression");

    Expr* body = parse_expr(in); // Parse the body expression

    return new _Let(var_name, value, body);
}



Expr *parse_expr (std::istream & in) {
    skip_whitespace(in);

    if (is_let_keyword(in)) {
        // Handle 'let' expressions specifically
        return parse_Let(in);
    }

    // Parse the first term which could be a single number, a variable, or a parenthesized expression
    Expr* expr = parse_multicand(in);

    skip_whitespace(in);
    while (true) {
        int nextChar = in.peek();
        if (nextChar == '+') {
            consume(in, nextChar); // Consume the '+'
            Expr *rhs = parse_multicand(in); // Parse the right-hand side expression
            expr = new Add(expr, rhs); // Create an Add expression
        }
        else {
            break; // No more operators, exit loop
        }
        skip_whitespace(in);
    }


    return expr;
}



Expr *parse_str(const std::string &s) {
    std::istringstream in(s);
    Expr *expr = parse_expr(in);
    skip_whitespace(in);
    if (in.peek() != EOF) {
        throw std::runtime_error("invalid input");
    }
    return expr;
}



