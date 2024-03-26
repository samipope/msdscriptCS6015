/// Title: ExprTest.cpp
/// Author: Samantha Pope
/// Date: 1.22.2024
/// Scope: This document includes all of my tests for my script using the Catch2 testing format.
///


#include "catch.h"
#include "Expr.h"
#include "parse.h"
#include "cmdline.h"
#include <stdexcept>
#include <sstream>
#include <iostream>
#include "val.h"

TEST_CASE("Num equals tests", "[Num]") {
    Num num1(5);
    Num num2(5);
    Num num3(10);

    SECTION("Equal numbers") {
        REQUIRE(num1.equals(&num2));
    }

    SECTION("Not equal numbers") {
        REQUIRE_FALSE(num1.equals(&num3));
    }

    SECTION("Comparison with null") {
        REQUIRE_FALSE(num1.equals(nullptr));
    }
}

TEST_CASE("Num Edge Cases"){
    SECTION("Negative Numbers"){
        Num num1(-5);
        Num num2(-5);
        Num num3(-10);
        Num num4(5);
        REQUIRE(num1.equals(&num2));
        REQUIRE_FALSE(num1.equals(&num3));
        REQUIRE_FALSE(num1.equals(&num4));
        REQUIRE_FALSE(num1.equals(nullptr));
    }
}

TEST_CASE("Add equals tests", "Add") {
    Num n1(3), n2(4), n3(3), n4(4);
    Add add1(&n1, &n2);
    Add add2(&n3, &n4);
    Add add3(&n2, &n1);  // Different order
    Num differentNum(7);
    Add differentAdd(&n1, &differentNum);

    SECTION("Equal Adds") {
        REQUIRE(add1.equals(&add2));
    }
    SECTION("Equal Adds with different order") {
        REQUIRE(add1.equals(&add3));
    }
    SECTION("Not equal Adds") {
        REQUIRE_FALSE(add1.equals(&differentAdd));
    }
    SECTION("Comparison with null") {
        REQUIRE_FALSE(add1.equals(nullptr));
    }
}

TEST_CASE("Mult equals tests", "[Mult]") {
    Num n1(2), n2(5), n3(2), n4(5);
    Mult mult1(&n1, &n2);
    Mult mult2(&n3, &n4);
    Mult mult3(&n2, &n1);  // Different order
    Num differentNum(9);
    Mult differentMult(&n1, &differentNum);
    SECTION("Equal Mults") {
        REQUIRE(mult1.equals(&mult2));
    }
    SECTION("Equal Mults with different order") {
        REQUIRE(mult1.equals(&mult3));
    }
    SECTION("Not equal Mults") {
        REQUIRE_FALSE(mult1.equals(&differentMult));
    }
    SECTION("Comparison with null") {
        REQUIRE_FALSE(mult1.equals(nullptr));
    }
}

TEST_CASE("Var equals tests", "[Var]") {
    Var varExpr1("x");
    Var varExpr2("x");
    Var varExpr3("y");
    Num numExpr(5);
    SECTION("Equal VarExprs with same name") {
        REQUIRE(varExpr1.equals(&varExpr2));
    }
    SECTION("Not Equal VarExprs with different names") {
        REQUIRE_FALSE(varExpr1.equals(&varExpr3));
    }
    SECTION("Not Equal when compared with different type (Num)") {
        REQUIRE_FALSE(varExpr1.equals(&numExpr));
    }
    SECTION("Not Equal when compared with nullptr") {
        REQUIRE_FALSE(varExpr1.equals(nullptr));
    }
}

TEST_CASE("interp tests", "All Expressions") {

    SECTION("Num interp") {
        CHECK((new Num(3))->interp()->equals(new NumVal(3)));
        CHECK((new Num(5))->interp()->equals(new NumVal(5)));
        CHECK((new Num(-18))->interp()->equals(new NumVal(-18)));
        CHECK((new Num(-3))->interp()->equals(new NumVal(-3)));
        CHECK((new Num(0))->interp()->equals(new NumVal(0)));

    }

    SECTION("Add interp") {
        CHECK((new Add(new Num(3), new Num(2)))->interp()->equals(new NumVal(5)));
        CHECK((new Add(new Num(5), new Num(-4)))->interp()->equals(new NumVal(1)));
        CHECK((new Add(new Num(-3), new Num(3)))->interp()->equals(new NumVal(0)));
        CHECK((new Add(new Num(-3), new Num(-3)))->interp()->equals(new NumVal(-6)));
        CHECK((new Add(new Num(0), new Num(10)))->interp()->equals(new NumVal(10)));
    }


    SECTION("Mult interp") {
        CHECK((new Mult(new Num(3), new Num(2)))->interp()->equals(new NumVal(6)));
        CHECK((new Mult(new Num(5), new Num(4)))->interp()->equals(new NumVal(20)));
        CHECK((new Mult(new Num(-3), new Num(6)))->interp()->equals(new NumVal(-18)));
        CHECK((new Mult(new Num(-3), new Num(-3)))->interp()->equals(new NumVal(9)));
        CHECK((new Mult(new Num(0), new Num(10)))->interp()->equals(new NumVal(0)));
    }

    SECTION("Var interp throws exception") {
        Var varExpr("x");
        REQUIRE_THROWS_AS(varExpr.interp(), std::runtime_error);
        Var varExpr2("y");
        REQUIRE_THROWS_AS(varExpr2.interp(), std::runtime_error);
        Var varExpr3("var");
        REQUIRE_THROWS_AS(varExpr3.interp(), std::runtime_error);
        Var varExpr4("123");
        REQUIRE_THROWS_AS(varExpr4.interp(), std::runtime_error);
        Var varExpr5("testVar");
        REQUIRE_THROWS_AS(varExpr5.interp(), std::runtime_error);
    }
}






TEST_CASE("subst tests", "All Expressions") {
    Num num5(5);
    Num num10(10);
    Var varX("x");
    Var varY("y");

    SECTION("Num subst") {
        Expr* result1 = num5.subst("x", &num10);
        REQUIRE(result1->equals(&num5));
        delete result1;
        Expr* result2 = num10.subst("y", &num5);
        REQUIRE(result2->equals(&num10));
        delete result2;
    }

    SECTION("Add subst") {
        Add add1(&num5, &varX);
        Expr* result1 = add1.subst("x", &num10);
        Add expected1(&num5, &num10);
        REQUIRE(result1->equals(&expected1));
        delete result1;
        Add add2(&varX, &varY);
        Expr* result2 = add2.subst("x", &num5);
        Add expected2(&num5, &varY);
        REQUIRE(result2->equals(&expected2));
        delete result2;
    }

    SECTION("Mult subst") {
        Mult mult1(&num5, &varX);
        Expr* result1 = mult1.subst("x", &num10);
        Mult expected1(&num5, &num10);
        REQUIRE(result1->equals(&expected1));
        delete result1;
        Mult mult2(&varX, &varY);
        Expr* result2 = mult2.subst("x", &num5);
        Mult expected2(&num5, &varY);
        REQUIRE(result2->equals(&expected2));
        delete result2;
    }

    SECTION("Var subst") {
        Expr* result1 = varX.subst("x", &num5);
        REQUIRE(result1 == &num5); // Check for same pointer, not just equality
        Var varAnotherX("x");
        Expr* result2 = varAnotherX.subst("x", &varY);
        REQUIRE(result2 == &varY); // Again, checking pointers
        Expr* result11 = varX.subst("y", &num5);
        REQUIRE(result11->equals(&varX));
        delete result11;
        Expr* result22 = varY.subst("x", &num5);
        REQUIRE(result22->equals(&varY));
        delete result22;

    }
}


TEST_CASE("to_string tests", "all expressions"){
    CHECK( (new Var("x"))->to_string() == "x" );
    CHECK( (new Add(new Num(1), new Num(2)))->to_string() == "(1+2)" );
    CHECK( (new Mult(new Num(3), new Num(4)))->to_string() == "(3*4)" );
    CHECK( (new Num(-5))->to_string() == "-5" );
    CHECK( (new Num(0))->to_string() == "0" );
    CHECK( (new Add(new Num(1), new Num(-2)))->to_string() == "(1+-2)" );
    CHECK( (new Mult(new Num(0), new Num(4)))->to_string() == "(0*4)" );
    CHECK( (new Mult(new Num(3), new Num(-4)))->to_string() == "(3*-4)" );
    CHECK( (new Add(new Num(-1), new Mult(new Num(2), new Num(-3))))->to_string() == "(-1+(2*-3))" );
    CHECK( (new Add(new Var("x"), new Mult(new Add(new Num(0), new Num(-5)), new Var("y"))))->to_string() == "(x+((0+-5)*y))" );
    CHECK( (new Mult(new Add(new Num(0), new Num(-2)), new Add(new Num(-3), new Num(4))))->to_string() == "((0+-2)*(-3+4))" );
    CHECK( (new Add(new Var("z"), new Num(-10)))->to_string() == "(z+-10)" );
    CHECK( (new Mult(new Add(new Var("a"), new Mult(new Num(-1), new Var("b"))), new Add(new Num(0), new Var("c"))))->to_string() == "((a+(-1*b))*(0+c))" );
}


TEST_CASE("pretty_print_at Tests", "All expression classes"){
    std::stringstream ss3;
    (new Add(new Num(4), new Num(5)))->pretty_print_at(ss3);
    CHECK(ss3.str() == "4 + 5");
    std::stringstream ss4;
    (new Add(new Num(4), new Mult(new Num(5), new Num(6))))->pretty_print_at(ss4);
    CHECK(ss4.str() == "4 + 5 * 6");
    std::stringstream ss5;
    (new Add(new Mult(new Num(1), new Num(2)), new Add(new Num(3), new Mult(new Num(4), new Num(5)))))->pretty_print_at(ss5);
    CHECK(ss5.str() == "1 * 2 + 3 + 4 * 5");
    CHECK((new Add(new Mult(new Num(1), new Add(new Num(2), new Num(3))), new Num(4)))->to_pp_string() == "1 * (2 + 3) + 4");
    CHECK((new Mult(new Add(new Num(2), new Num(3)), new Num(4)))->to_pp_string() == "(2 + 3) * 4");
    CHECK((new Add(new Num(2), new Mult(new Add(new Num(3), new Num(4)), new Num(5))))->to_pp_string() == "2 + (3 + 4) * 5");
    CHECK((new Add(new Var("x"), new Mult(new Var("y"), new Num(2))))->to_pp_string() == "x + y * 2");
    CHECK((new Mult(new Add(new Var("a"), new Var("b")), new Add(new Num(5), new Var("c"))))->to_pp_string() == "(a + b) * (5 + c)");
    CHECK((new Mult(new Mult(new Var("z"), new Num(3)), new Add(new Num(4), new Num(5))))->to_pp_string() == "(z * 3) * (4 + 5)");

}


TEST_CASE("_Let Tests"){

    SECTION("equals"){
        CHECK((_Let("x", new Num(5), new Var("x")).equals(new _Let("x", new Num(5), new Var("x"))) == true));
        CHECK((_Let("x", new Num(5), new Var("x")).equals(new _Let("y", new Num(5), new Var("x"))) == false));
        CHECK((_Let("x", new Add(new Num(1), new Num(2)), new Var("x")).equals(new _Let("x", new Add(new Num(1), new Num(2)), new Var("x"))) == true));
        CHECK((_Let("x", new Num(5), new Var("y")).equals(new _Let("x", new Num(5), new Var("x"))) == false));
        CHECK((_Let("x", new Mult(new Num(2), new Num(3)), new Var("x")).equals(new _Let("x", new Mult(new Num(3), new Num(2)), new Var("x"))) == true)); // Assuming Mult equals is commutative
    }

    SECTION("subst"){
        CHECK(_Let("x", new Num(5), new Var("x")).subst("y", new Num(20))->equals(new _Let("x", new Num(5), new Var("x"))));
        CHECK(_Let("x", new Add(new Var("y"), new Num(1)), new Var("x")).subst("y", new Num(5))->equals(new _Let("x", new Add(new Num(5), new Num(1)), new Var("x"))));
        CHECK(_Let("y", new Num(5), new Mult(new Var("y"), new Var("x"))).subst("x", new Num(3))->equals(new _Let("y", new Num(5), new Mult(new Var("y"), new Num(3)))));
        CHECK(_Let("z", new Mult(new Var("x"), new Num(2)), new Var("z")).subst("x", new Num(4))->equals(new _Let("z", new Mult(new Num(4), new Num(2)), new Var("z"))));
    }

    SECTION("interp"){
        CHECK((new _Let("var", new Num(0), new Add(new Var("var"), new Num(1))))->interp()->equals(new NumVal(1)));
        CHECK((new _Let("z", new Num(-10), new Mult(new Var("z"), new Num(5))))->interp()->equals(new NumVal(-50)));
        CHECK((new _Let("!", new Num(0), new Mult(new Var("!"), new Num(0))))->interp()->equals(new NumVal(0)));
        CHECK((new _Let("x", new Num(5), new Add(new _Let("y", new Num(3), new Add(new Var("y"), new Num(2))), new Var("x"))))->interp()->equals(new NumVal(10)));
        CHECK((new _Let("x", new Num(10), new Add(new _Let("y", new Num(-15), new Add(new Var("y"), new Num(2))), new Var("x"))))->interp()->equals(new NumVal(-3)));
    }

    SECTION("print and pretty_print") {
        //Let nested as right argument of parenthesized multiplication expression
        CHECK ( (new Mult(new Mult(new Num (2), new _Let("x", new Num(5), new Add(new Var("x") , new Num(1)) )), new Num(3)))->to_pp_string() == "(2 * _let x = 5\n"
                                                                                                                                                 "      _in  x + 1) * 3");
        //Let nested to the left in add expression which is nested to the right within a multiplication expression
        CHECK((new Mult(new Num(5), new Add(new _Let("x", new Num(5), new Var("x")), new Num(1))))->to_pp_string() == "5 * ((_let x = 5\n"
                                                                                                                      "       _in  x) + 1)");
        //Let in lhs of add
        CHECK ( (new Add(new _Let("x", new Num(2), new Add(new Var("x"), new Num(9))), new Num(4)))->to_pp_string() == "(_let x = 2\n"
                                                                                                                       "  _in  x + 9) + 4");
        //Let in lhs of multiplication expression
        CHECK((new Mult(new _Let("x", new Num(5), new Add(new Var("x"), new Num(8))), new Num(3)))->to_pp_string() == "(_let x = 5\n"
                                                                                                                      "  _in  x + 8) * 3");
        //Let nest as right argument of un-parenthesized multiplication expression
        CHECK((new Add (new Mult(new Num(4), new _Let("x", new Num(5), new Add(new Var("x"), new Num(1)))), new Num(9)))->to_pp_string() == "4 * (_let x = 5\n"
                                                                                                                                            "      _in  x + 1) + 9");
        //Let nested to the left within let that is nested to the left within add
        CHECK ((new Add(new _Let("x", new Num(3), new _Let("y", new Num(3), new Add(new Var("y"), new Num(2))) ), new Var("x")))->to_pp_string() == "(_let x = 3\n"
                                                                                                                                                    "  _in  _let y = 3\n"
                                                                                                                                                    "       _in  y + 2) + x");
        //Let nested in lhs of Add expression nested within body of let expression
        CHECK((new _Let("x", new Num(5), new Add(new _Let("y" , new Num(3), new Add(new Var("y"), new Num(2))), new Var("x"))))
                      ->to_pp_string() == "_let x = 5\n"
                                          " _in  (_let y = 3\n"
                                          "       _in  y + 2) + x");
    }

}


TEST_CASE("parse"){
    SECTION("parsing single numbers"){
       CHECK(parse_str("1")->equals(new Num(1)));
        CHECK(parse_str("(((1)))")->equals(new Num(1)));
        CHECK(parse_str("  \n 5  ")->equals(new Num(5)));
    }
    SECTION("Handling invalid input") {
        CHECK_THROWS_WITH(parse_str("(-5"), "missing closing parentheses");
        CHECK_THROWS_WITH(parse_str("-"), "invalid input");
        // Assuming negative numbers are allowed but require a digit after '-'
        CHECK_THROWS_WITH(parse_str(" -   "), "invalid input");
        CHECK_THROWS_WITH(parse_str("x_z"), "invalid input");
    }

    SECTION("Parsing variables") {
        CHECK(parse_str("xyz")->equals(new Var("xyz")));
        CHECK(parse_str("xYz")->equals(new Var("xYz")));
    }

    SECTION("Parsing addition expressions") {
        CHECK(parse_str("x + y")->equals(new Add(new Var("x"), new Var("y"))));
    }

    SECTION("Parsing multiplication expressions") {
        CHECK(parse_str("x * y")->equals(new Mult(new Var("x"), new Var("y"))));
    }

        // Here's an added test case to handle negative numbers correctly
    SECTION("Parsing negative numbers") {
        CHECK(parse_str("-5")->equals(new Num(-5)));
        // This test assumes spaces between '-' and digits are not allowed for negative numbers
        CHECK_THROWS_WITH(parse_str(" -   5"), "invalid input");
    }

}

TEST_CASE("NumVal"){
    //NumVal constructor is taken care of in other interp tests
    SECTION("equals method") {
        NumVal a(10), b(10), c(5);
        Val* nonNumVal = reinterpret_cast<Val*>(&c); // Simulating a different type of Val
        CHECK(a.equals(&b) == true);
        CHECK(a.equals(&c) == false);
        CHECK(a.equals(nonNumVal) == false);
    }

    SECTION("add_to method") {
        NumVal a(10), b(5);
        CHECK(dynamic_cast<NumVal*>(a.add_to(&b))->numVal == 15);
    }

    SECTION("mult_with method") {
        NumVal a(10), b(2);
        CHECK(dynamic_cast<NumVal*>(a.mult_with(&b))->numVal == 20);
    }

    SECTION("print method") {
        NumVal a(10);
        stringstream ss;
        a.print(ss);
        CHECK(ss.str() == "10");
    }

    SECTION("is_true method") {
        NumVal a(10);
        CHECK_THROWS_AS(a.is_true(), runtime_error);
    }


}


TEST_CASE("BoolVal") {
    SECTION("constructor/print") {
        BoolVal trueVal(true);
        BoolVal falseVal(false);
        ostringstream outputTrue, outputFalse;

        trueVal.print(outputTrue);
        falseVal.print(outputFalse);

        CHECK(outputTrue.str() == "1");
        CHECK(outputFalse.str() == "0");
    }

    SECTION("to_expr"){
        BoolVal boolVal(true);
        Expr* expr = boolVal.to_expr();
        ostringstream output;
        expr->print(output);
        CHECK(output.str() == "_true");

        BoolVal boolVal2(false);
        Expr* expr2 = boolVal2.to_expr();
        ostringstream output2;
        expr2->print(output2);
        CHECK(output2.str() == "_false");
    }

    SECTION("equals") {
        CHECK( (new BoolVal(true))->equals(new BoolVal(true))==true );
        CHECK( (new BoolVal(true))->equals(new BoolVal(false))==false );
    }

    //TODO add tests for add_to with positive and negative numbers
    SECTION("add_to") {
        BoolVal boolVal(true);
        BoolVal anotherBoolVal(false);

        REQUIRE_THROWS_AS(boolVal.add_to(&anotherBoolVal), runtime_error);
    }

    SECTION("mult_with") {
        REQUIRE_THROWS_AS((new BoolVal(true))->mult_with(new BoolVal(false)), runtime_error);
    }
}

TEST_CASE("Testing BoolExpr") {
    SECTION("constructor, print"){
        BoolExpr trueExpr(true);
        BoolExpr falseExpr(false);
        ostringstream outputTrue, outputFalse;
        trueExpr.print(outputTrue);
        falseExpr.print(outputFalse);
        CHECK( outputTrue.str() == "_true" );
        CHECK( outputFalse.str() == "_false" );

    }

    SECTION("equals") {
        CHECK( (new BoolExpr(true))->equals(new BoolExpr(true)) );
        CHECK_FALSE( (new BoolExpr(true))->equals(new BoolExpr(false)) );
    }

    SECTION("interp") {
        BoolExpr boolExpr(true);
        Val* val = boolExpr.interp();
        BoolVal* boolVal = dynamic_cast<BoolVal*>(val);
        REQUIRE(boolVal != nullptr);
        ostringstream output;
        boolVal->print(output);
        REQUIRE(output.str() == "1");

    }

}

TEST_CASE("Testing EqExpr") {
    Var* varX = new Var("x");
    Num* num1 = new Num(1);
    EqExpr eqExpr(varX, num1);
    SECTION("constructor and print") {
        ostringstream output;
        eqExpr.print(output);
        CHECK(output.str() == "x == 1");
    }

    SECTION("equals") {
        EqExpr similarExpr(varX, num1);
        EqExpr differentExpr(varX, new Num(2));
        CHECK(eqExpr.equals(&similarExpr));
        CHECK_FALSE(eqExpr.equals(&differentExpr));
    }

    SECTION("interp") {
        CHECK_THROWS_AS(eqExpr.interp(), std::runtime_error);
    }

}

TEST_CASE("Testing IfExpr") {
    Var* varX = new Var("x");
    Num* num1 = new Num(1);
    Num* num2 = new Num(2);
    EqExpr* condition = new EqExpr(varX, num1);
    IfExpr ifExpr(condition, num1, num2);

    SECTION("constructor, print") {
        ostringstream output;
        ifExpr.print(output);
        CHECK(output.str() == "_if x == 1 _then 1 _else 2");
    }

    SECTION("equals") {
        IfExpr similarExpr(condition, num1, num2);
        IfExpr differentExpr(new EqExpr(varX, num2), num1, num2);
        CHECK(ifExpr.equals(&similarExpr));
        CHECK_FALSE(ifExpr.equals(&differentExpr));
    }

    SECTION("interp") {
        CHECK_THROWS_AS(ifExpr.interp(), std::runtime_error);
    }

    SECTION("pretty_print"){

        CHECK( (new IfExpr(new EqExpr(new Var("x"), new Num(1)), new Num(1), new Num(2)))->to_pp_string()
               ==
               "_if x == 1\n"
               "_then 1\n"
               "_else 2\n" );
    }

    CHECK( (new IfExpr(new EqExpr(new Var("x"), new Var("x")), new Num(100), new Num(200)))->to_pp_string()
           ==
           "_if x == x\n"
           "_then 100\n"
           "_else 200\n" );

}

TEST_CASE("Parse If, Bool and Functions"){

    SECTION("Parsing IfExpr") {
        CHECK(parse_str(("_if _true _then 4 _else 5"))->equals(
                new IfExpr(new BoolExpr(true), new Num(4), new Num(5))));
        CHECK(parse_str(("_if _false _then 4 _else 5"))->equals(
                new IfExpr(new BoolExpr(false), new Num(4), new Num(5))));
        CHECK(parse_str(("_true"))->equals(new BoolExpr(true)));
        CHECK(parse_str(("_false"))->equals(new BoolExpr(false)));
    }

    SECTION("Parsing EqExpr") {
        CHECK( parse_str("1 == 2")->interp()->equals(new BoolVal(false)) );
        CHECK( parse_str("2 == 2")->interp()->equals(new BoolVal(true)) );
        CHECK((((parse_str("_if 1 == 2 _then 3 _else 4"))->interp())->to_string()) == "4");
        CHECK((parse_str("1 + 2 == 3 + 0"))->interp()->equals(new BoolVal(true)));

        CHECK( parse_str("_if 1==1 _then 1 _else 2") ->interp() ->equals(new NumVal(1)));
    }

    SECTION("If Expr Interp") {
        CHECK( parse_str("_if 1==1 _then 1 _else 2") ->interp() ->equals(new NumVal(1)));
        CHECK( parse_str("_if 10==12 _then 7 _else 5") ->interp() ->equals(new NumVal(5)));
        CHECK( parse_str("_if 0==0 _then 14 _else 7") ->interp() ->equals(new NumVal(14)));
        CHECK( parse_str("_if -4==-5 _then 6 _else 8") ->interp() ->equals(new NumVal(8)));
    }


    SECTION("TEST FUNCTIONS PARSE - from Ziz :)"){
        CHECK( parse_str("_let f = _fun (x) x+ 1\n"
                         "_in f(5)")->interp()->equals(new NumVal(6)));
        CHECK( parse_str("_let f = _fun (x)\n"
                         "7\n"
                         "_in f(5) ")->interp()->equals(new NumVal(7)));

        CHECK( parse_str("_let f = _fun (x)\n"
                         "_true\n"
                         "_in f(5)  ")->interp()->equals(new BoolVal(true)));
        CHECK_THROWS( parse_str("_let f = _fun (x)\n"
                                "x + _true\n"
                                "_in f(5) ")->interp() );
        CHECK( parse_str("_let f = _fun (x)\n       "
                         "x +   _true\n"
                         "_in        5 + 1 ")->interp()->equals(new NumVal(6)) );
        CHECK_THROWS( parse_str("_let f = _fun (x)\n"
                                "7"
                                "_in  f(5 + _true)")->interp() );
        CHECK_THROWS( parse_str("_let f = _fun (x) x+ 1\n"
                                "_in f + 5")->interp() );
        CHECK( parse_str("_let f = _fun (x) x+ 1\n"
                         "_in _if _false\n"
                         "_then f(5)\n"
                         "_else f(6)")->interp()->equals(new NumVal(7)) );
        CHECK( parse_str("_let f = _fun (x) x+ 1\n"
                         "_in _let g = _fun (y) y+ 2\n"
                         "_in _if _true     \n"
                         "_then f(5)\n"
                         "_else g(5)")->interp()->equals(new NumVal(6)) );
        CHECK( parse_str("_let f = _fun (x) x+ 1\n"
                         "_in _let g = _fun (y) y+ 2\n"
                         "_in f(g(5))\n")->interp()->equals(new NumVal(8)) );

        CHECK( parse_str("_let f =       _fun (x) x+ 1\n"
                         "_in _let g = _fun (y)\n"
                         "f(y + 2)\n"
                         "_in g(5)")->interp()->equals(new NumVal(8)) );
        CHECK( parse_str("_let f = _fun (x) x+ 1\n"
                         "_in _let g = _fun (x)\n"
                         "f(2) + x\n"
                         "_in g(5) ")->interp()->equals(new NumVal(8)) );
        CHECK_THROWS( parse_str("_let f = _fun (x) x+ 1\n"
                                "_in f 5  ")->interp() );
        CHECK( parse_str("_let f = _fun (x) x+ 1\n"
                         "_in (f)(5)  ")->interp()->equals(new NumVal(6)) );
        CHECK( parse_str("_fun (x) x+ 1")->interp()->equals(new FunVal("x", new Add(new Var("x"),new Num(1)))) );
        CHECK( parse_str("_let f = _fun (x) x+ 1\n"
                         "_in f")->interp()->equals(new FunVal("x", new Add(new Var("x"),new Num(1)))) );
        CHECK( parse_str("(_fun (x)\n"
                         "x + 1)(5)")->interp()->equals(new NumVal(6)));
        CHECK( parse_str("_let f = _if _false\n"
                         "_then _fun (x)\n"
                         "x+ 1\n"
                         "_else _fun (x)\n"
                         "x+ 2\n"
                         "_in f(5)")->interp()->equals(new NumVal(7)));
        CHECK( parse_str("(_if _false\n"
                         "_then _fun (x)\n"
                         "x+ 1\n"
                         "_else _fun (x)\n"
                         "x + 2)(5)")->interp()->equals(new NumVal(7)));
        CHECK( parse_str("_let f = _fun (g)\n"
                         "              g(5)\n"
                         "_in _let g = _fun (y)\n"
                         "              y + 2\n"
                         "_in f(g) ")->interp()->equals(new NumVal(7)));
        CHECK( parse_str("_let f = _fun (g)"
                         "              g(5)\n"
                         "_in f(_fun (y)\n"
                         "y + 2) ")->interp()->equals(new NumVal(7)));
        CHECK( parse_str("_let f = _fun (x)\n"
                         "          _fun (y)\n"
                         "x+ y _in (f(5))(1) ")->interp()->equals(new NumVal(6)));
        CHECK( parse_str("_let f = _fun (x)\n"
                         "          _fun (y)\n"
                         "x+ y _in f(5)(1)  ")->interp()->equals(new NumVal(6)));
        CHECK( parse_str("_let f = _fun (x)\n"
                         "          _fun (g)\n"
                         "           g(x + 1)\n"
                         "_in _let g = _fun (y)\n"
                         "              y+ 2"
                         "_in (f(5))(g)")->interp()->equals(new NumVal(8)));
        CHECK( parse_str("_let f = _fun (x)\n"
                         "          _fun (g)\n"
                         "          g(x + 1)\n"
                         "_in _let g = _fun (y)\n"
                         "y+ 2 _in f(5)(g) ")->interp()->equals(new NumVal(8)));
        CHECK( parse_str("_let f = _fun (f)\n"
                         "          _fun (x)\n"
                         "      _if x == 0\n"
                         "      _then 0\n"
                         "      _else x + f(f)(x + -1)\n"
                         "_in f(f)(3)")->interp()->equals(new NumVal(6)));
        CHECK( parse_str("_let factrl = _fun (factrl)\n"
                         "                  _fun (x)\n"
                         "                  _if x == 1\n"
                         "                  _then 1\n"
                         "_else x * factrl(factrl)(x + -1)\n"
                         "_in  factrl(factrl)(10)")->interp()->equals(new NumVal(3628800)));

    }

    SECTION("Sami's parse Fun tests"){
        CHECK( parse_str("(_if _false\n"
                         "    _then _fun (x)\n"
                         "    x + 1\n"
                         "    _else _fun (x)\n"
                         "    x + 2)(5)")->interp()->equals(new NumVal(7)));

        CHECK( parse_str("_let f = _fun (g)\n"
                         "              g(5)\n"
                         "_in _let g = _fun (y)\n"
                         "              y + 2\n"
                         "_in f(g)")->interp()->equals(new NumVal(7)));

        CHECK( parse_str("_let f = _fun (g)\n"
                         "              g(5)\n"
                         "_in f(_fun (y)\n"
                         "    y + 2)")->interp()->equals(new NumVal(7)));

        CHECK( parse_str("_let f = _fun (x)\n"
                         "          _fun (y)\n"
                         "          x+ y _in (f(5))(1)")->interp()->equals(new NumVal(6)));

        CHECK( parse_str("_let f = _fun (x)\n"
                         "          _fun (y)\n"
                         "          x+ y _in f(5)(1)")->interp()->equals(new NumVal(6)));

        CHECK( parse_str("_let f = _fun (x)\n"
                         "          _fun (g)\n"
                         "           g(x + 1)\n"
                         "_in _let g = _fun (y)\n"
                         "              y+ 2\n"
                         "_in (f(5))(g)")->interp()->equals(new NumVal(8)));

        CHECK( parse_str("_let f = _fun (x)\n"
                         "          _fun (g)\n"
                         "          g(x + 1)\n"
                         "_in _let g = _fun (y)\n"
                         "              y+ 2 _in f(5)(g)")->interp()->equals(new NumVal(8)));

        CHECK( parse_str("_let f = _fun (f)\n"
                         "          _fun (x)\n"
                         "      _if x == 0\n"
                         "      _then 0\n"
                         "      _else x + f(f)(x + -1)\n"
                         "_in f(f)(3)")->interp()->equals(new NumVal(6)));
    }
}

TEST_CASE("Function Print tests"){
    Var* varX = new Var("x");
    Num* num1 = new Num(1);
    Add* body = new Add(varX, num1);
    Mult* body1 = new Mult(varX,num1);
    FunExpr funExpr("x", body);
    FunExpr fun1Expr("y", body1);

    SECTION("constructor, print") {
        ostringstream output;
        funExpr.print(output);
        CHECK(output.str() == "(_fun (x) (x+1))");
        fun1Expr.print(output);
        CHECK(output.str()=="(_fun (x) (x+1))(_fun (y) (x*1))");
    }

    delete varX;
    delete num1;
    delete body;
    delete body1;

}


TEST_CASE("Professor's given tests") {
    SECTION("Given Tests Assignment 2") {
        CHECK((new Var("x"))->equals(new Var("x")) == true);
        CHECK((new Var("x"))->equals(new Var("y")) == false);
        CHECK((new Num(1))->equals(new Var("x")) == false);
        CHECK((new Add(new Num(2), new Num(3)))->equals(new Add(new Num(2), new Num(3))) == true);
        CHECK((new Add(new Num(2), new Num(3)))->equals(new Add(new Num(3), new Num(2))) == true);
    }

    SECTION("Given Tests Assignment 3") {
        CHECK((new Mult(new Num(3), new Num(2)))
                      ->interp()->equals(new NumVal(6)));
        CHECK((new Add(new Add(new Num(10), new Num(15)), new Add(new Num(20), new Num(20))))
                      ->interp()->equals(new NumVal(65)));
        CHECK_THROWS_WITH((new Var("x"))->interp(), "no value for variable");

        CHECK((new Add(new Var("x"), new Num(7)))
                      ->subst("x", new Var("y"))
                      ->equals(new Add(new Var("y"), new Num(7))));
        CHECK((new Var("x"))
                      ->subst("x", new Add(new Var("y"), new Num(7)))
                      ->equals(new Add(new Var("y"), new Num(7))));
    }

    SECTION("Given Tests for Assignment 4") {
        CHECK((new Num(10))->to_string() == "10");

        std::stringstream ss1;
        (new Add(new Num(1), new Mult(new Num(2), new Num(3))))->pretty_print_at(ss1);
        CHECK(ss1.str() == "1 + 2 * 3");

        std::stringstream ss2;
        (new Mult(new Num(1), new Add(new Num(2), new Num(3))))->pretty_print_at(ss2);
        CHECK(ss2.str() == "1 * (2 + 3)");

        CHECK ((new Mult(new Num(1), new Add(new Num(2), new Num(3))))->to_pp_string() == "1 * (2 + 3)");
        CHECK ((new Mult(new Mult(new Num(8), new Num(1)), new Var("y")))->to_pp_string() == "(8 * 1) * y");
        CHECK ((new Mult(new Add(new Num(3), new Num(5)), new Mult(new Num(6), new Num(1))))->to_pp_string() ==
               "(3 + 5) * 6 * 1");
        CHECK ((new Mult(new Mult(new Num(7), new Num(7)), new Add(new Num(9), new Num(2))))->to_pp_string() ==
               "(7 * 7) * (9 + 2)");
    }SECTION("Nabil Given Test Assignment 5") {
        std::string expected = "(2 * _let x = 5\n"
                               "      _in  x + 1) * 3";
        Expr *expr = new Mult(
                new Mult(
                        new Num(2),
                        new _Let("x", new Num(5), new Add(new Var("x"), new Num(1)))
                ),
                new Num(3)
        );
        CHECK(expr->to_pp_string() == expected);
        delete expr;
    }SECTION("parse tests given by Nabil") {
        CHECK_THROWS_WITH(parse_str("()"), "invalid input");

        CHECK(parse_str("(((1)))")->equals(new Num(1)));

        CHECK_THROWS_WITH(parse_str("(1"), "missing closing parentheses");

        CHECK(parse_str("  \n 5  ")->equals(new Num(5)));
        CHECK_THROWS_WITH(parse_str("-"), "invalid input");

        CHECK_THROWS_WITH(parse_str(" -   5  "), "invalid input");

        CHECK(parse_str("xyz")->equals(new Var("xyz")));
        CHECK(parse_str("xYz")->equals(new Var("xYz")));
        CHECK_THROWS_WITH(parse_str("x_z"), "invalid input");

        CHECK(parse_str("x + y")->equals(new Add(new Var("x"), new Var("y"))));

        CHECK(parse_str("x * y")->equals(new Mult(new Var("x"), new Var("y"))));

        CHECK(parse_str("z * x + y")
                      ->equals(new Add(new Mult(new Var("z"), new Var("x")),
                                       new Var("y"))));
        CHECK(parse_str("z * (x + y)")
                      ->equals(new Mult(new Var("z"),
                                        new Add(new Var("x"), new Var("y")))));
    }

    SECTION("HW9 tests from Assignment Description"){
        std::string input = "1==2+3";
        std::istringstream in(input);
        Expr* expr = parse(in);
       CHECK(expr->interp()->to_string() == "0"); //false !
        delete expr;

        std::string inputTwo = "1+1 == 2+0";
        std::istringstream inTwo(inputTwo);
        Expr* exprTwo = parse(inTwo);
        CHECK(exprTwo->interp()->to_string() == "1"); //true !
        delete exprTwo;


        std::string inputThree = "_let x=5 _in _if x==5 _then _true _else _false";
        std::istringstream inThree(inputThree);
        Expr* exprThree = parse(inThree);
        CHECK(exprThree->interp()->to_string() == "1");
        delete exprThree;

    }

    }





