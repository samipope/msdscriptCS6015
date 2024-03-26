//
// Created by Samantha Pope on 2/13/24.
//
#pragma once

#ifndef MSDSCRIPT_PARSE_H
#define MSDSCRIPT_PARSE_H

#include "Expr.h"
#include <istream>
#include <iostream>
#include <string>

//
void consume(std::istream &in, int expect);
void consume(std::istream &in, const std::string &str);
void skip_whitespace(std::istream &in);

// parsing functions for different expressions
Expr* parse(std::istream &in);
Expr* parse_expr(std::istream &in);
Expr* parse_comparg(std::istream &in);
Expr* parse_addend(std::istream &in);
Expr* parse_multicand(std::istream &in);
Expr* parse_inner(std::istream &in);
Expr* parse_num(std::istream &in);
Expr* parse_var(std::istream &in);
Expr* parse_let(std::istream &in);
Expr* parse_if(std::istream &in);
Expr* parse_fun(std::istream &in);

// additional helper functions
void consume_word(std::istream &in, std::string str);
std::string parse_term(std::istream &in);

// function to parse input from a string
Expr* parse_str(const std::string& s);

// function to handle input parsing
Expr* parseInput();


#endif //MSDSCRIPT_PARSE_H
