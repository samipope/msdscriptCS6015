//
// Created by Samantha Pope on 2/13/24.
//
#pragma once

#ifndef MSDSCRIPT_PARSE_H
#define MSDSCRIPT_PARSE_H


#include "Expr.h"
#include <istream>
#include <string>
#include <stdexcept>

Expr *parse_str(std::string s);
Expr *parse(std::istream &in);
Expr *parse_expr(std::istream &in);
Expr* parse_comparg(std::istream &in);
Expr *parse_addend(std::istream &in);
std::string parse_term(std::istream &in);
Expr *parse_num(std::istream &in);
Expr *parse_multicand(std::istream &in);
Expr *parse_variable(std::istream &in);
void consume(std::istream &in, int expect);
void consume(std::istream &stream, const std::string &str);
void skip_whitespace(std::istream &in);
Expr *parse_let(std::istream &in);
Expr* parse_if(std::istream &in);


#endif //MSDSCRIPT_PARSE_H
