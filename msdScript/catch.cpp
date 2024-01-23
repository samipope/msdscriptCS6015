//
// Created by Samantha Pope on 1/22/24.
//


#include "catch.h"
#include "Expr.h"
#include "cmdline.h"

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

TEST_CASE("Add equals tests", "[Add]") {
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

TEST_CASE("VarExpr equals tests", "[VarExpr]") {
    VarExpr varExpr1("x");
    VarExpr varExpr2("x");
    VarExpr varExpr3("y");
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
