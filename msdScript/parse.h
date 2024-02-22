//
// Created by Samantha Pope on 2/13/24.
//

#ifndef MSDSCRIPT_PARSE_H
#define MSDSCRIPT_PARSE_H


#include "Expr.h"

void consume(std::istream &in, int expect);
void skip_whitespace(std::istream &in);
Expr *parse_num(std::istream &inn);
Expr *parse_expr(std::istream & inn);
Expr *parse_str(const std::string &s);
Expr *parse_variable(std::istream &in);
Expr *parse_Let(std::istream & in);
Expr* parse_multicand(std::istream &in);

#endif //MSDSCRIPT_PARSE_H
