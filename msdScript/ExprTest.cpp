//
// Created by Samantha Pope on 1/22/24.
//


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


TEST_CASE("hasVariable tests", "All Expressions"){
    SECTION("Num hasVariable") {
        Num num1(5);
        REQUIRE_FALSE(num1.hasVariable());
        Num num2(-3);
        REQUIRE_FALSE(num2.hasVariable());
        Num num3(0);
        REQUIRE_FALSE(num3.hasVariable());
        Num num4(100);
        REQUIRE_FALSE(num4.hasVariable());
        Num num5(-50);
        REQUIRE_FALSE(num5.hasVariable());
    }

    SECTION("Add hasVariable") {
        Num num1(5);
        Num num2(10);
        Var var1("x");
        Add add1(&num1, &num2);
        REQUIRE_FALSE(add1.hasVariable());
        Add add2(&num1, &var1);
        REQUIRE(add2.hasVariable());
        Add add3(&var1, &var1);
        REQUIRE(add3.hasVariable());
        Num num3(0);
        Add add4(&num2, &num3);
        REQUIRE_FALSE(add4.hasVariable());
        Var var2("y");
        Add add5(&var1, &var2);
        REQUIRE(add5.hasVariable());
    }

    SECTION("Mult hasVariable") {
        Num num1(5);
        Num num2(3);
        Var var1("x");
        Mult mult1(&num1, &num2);
        REQUIRE_FALSE(mult1.hasVariable());
        Mult mult2(&num1, &var1);
        REQUIRE(mult2.hasVariable());
        Mult mult3(&var1, &var1);
        REQUIRE(mult3.hasVariable());
        Num num3(0);
        Mult mult4(&num2, &num3);
        REQUIRE_FALSE(mult4.hasVariable());
        Var var2("y");
        Mult mult5(&var1, &var2);
        REQUIRE(mult5.hasVariable());
    }

    SECTION("Var hasVariable") {
        Var varExpr1("x");
        REQUIRE(varExpr1.hasVariable());
        Var varExpr2("y");
        REQUIRE(varExpr2.hasVariable());
        Var varExpr3("var");
        REQUIRE(varExpr3.hasVariable());
        Var varExpr4("123");
        REQUIRE(varExpr4.hasVariable());
        Var varExpr5("testVar");
        REQUIRE(varExpr5.hasVariable());
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
    SECTION("hasVariable()"){
        CHECK((_Let("x", new Num(5), new Var("x")).hasVariable() == true));
        CHECK((_Let("x", new Num(5), new Num(10)).hasVariable() == false));
        CHECK((_Let("x", new Add(new Var("y"), new Num(1)), new Num(10)).hasVariable() == true));
        CHECK((_Let("x", new Num(5), new Mult(new Var("x"), new Num(2))).hasVariable() == true));
        CHECK((_Let("y", new Num(5), new Var("x")).hasVariable() == true));
    }
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
        CHECK_THROWS_WITH(parse_str("(-5"), "consume mismatch");
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



TEST_CASE("Nabil's given tests") {
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
        CHECK((new Add(new Var("x"), new Num(1)))->hasVariable() == true);
        CHECK((new Mult(new Num(2), new Num(1)))->hasVariable() == false);
        CHECK((new Add(new Var("x"), new Num(7)))
                      ->subst("x", new Var("y"))
                      ->equals(new Add(new Var("y"), new Num(7))));
        CHECK((new Var("x"))
                      ->subst("x", new Add(new Var("y"), new Num(7)))
                      ->equals(new Add(new Var("y"), new Num(7))));
    }

    SECTION("Given Tests for Assignment 4") {
        CHECK((new Num(10))->to_string() == "10");

        // Create a stringstream to capture the output of pretty_print
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
    }
    SECTION("Nabil Given Test Assignment 5") {
        std::string expected = "(2 * _let x = 5\n"
                               "      _in  x + 1) * 3";
        Expr* expr = new Mult(
                new Mult(
                        new Num(2),
                        new _Let("x", new Num(5), new Add(new Var("x"), new Num(1)))
                ),
                new Num(3)
        );
        CHECK(expr->to_pp_string() == expected);
        delete expr;
    }
    SECTION("parse tests given by Nabil"){
        CHECK_THROWS_WITH( parse_str("()"), "invalid input");

        CHECK( parse_str("(((1)))")->equals(new Num(1)) );

        CHECK_THROWS_WITH( parse_str("(1"), "consume mismatch");

        CHECK( parse_str("  \n 5  ")->equals(new Num(5)) );
        CHECK_THROWS_WITH( parse_str("-"), "invalid input" );

        CHECK_THROWS_WITH( parse_str(" -   5  "), "invalid input" );

        CHECK( parse_str("xyz")->equals(new Var("xyz")) );
        CHECK( parse_str("xYz")->equals(new Var("xYz")) );
        CHECK_THROWS_WITH( parse_str("x_z"), "invalid input" );

        CHECK( parse_str("x + y")->equals(new Add(new Var("x"), new Var("y"))) );

        CHECK( parse_str("x * y")->equals(new Mult(new Var("x"), new Var("y"))) );

        CHECK( parse_str("z * x + y")
                       ->equals(new Add(new Mult(new Var("z"), new Var("x")),
                                        new Var("y"))) );
        CHECK( parse_str("z * (x + y)")
                       ->equals(new Mult(new Var("z"),
                                         new Add(new Var("x"), new Var("y"))) ));
    }

}