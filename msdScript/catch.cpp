//
// Created by Samantha Pope on 1/22/24.
//


#include "catch.h"
#include "Expr.h"
#include "cmdline.h"
#include <stdexcept>

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
        Num num1(5);
        REQUIRE(num1.interp() == 5);
        Num num2(-3);
        REQUIRE(num2.interp() == -3);
        Num num3(0);
        REQUIRE(num3.interp() == 0);
        Num num4(100);
        REQUIRE(num4.interp() == 100);
        Num num5(-50);
        REQUIRE(num5.interp() == -50);

    }

    SECTION("Add interp") {
        Num num1(5);
        Num num2(10);
        Add add(&num1, &num2);
        REQUIRE(add.interp() == 15);
        Num num3(-5);
        Add addNegative(&num1, &num3);  // 5 + (-5)
        REQUIRE(addNegative.interp() == 0);
        Num num4(0);
        Add add3(&num2, &num4);  // 10 + 0
        REQUIRE(add3.interp() == 10);
        Num num5(-20);
        Add add4(&num5, &num5);  // -20 + (-20)
        REQUIRE(add4.interp() == -40);
        Num num6(100);
        Add add5(&num6, &num3);  // 100 + (-5)
        REQUIRE(add5.interp() == 95);
    }

    SECTION("Mult interp") {
        Num num1(5);
        Num num2(3);
        Mult mult(&num1, &num2);
        REQUIRE(mult.interp() == 15);
        Num num3(-2);
        Mult multNegative(&num1, &num3);  // 5 * (-2)
        REQUIRE(multNegative.interp() == -10);
        Num num4(0);
        Mult mult3(&num2, &num4);  // 3 * 0
        REQUIRE(mult3.interp() == 0);
        Num num5(1);
        Mult mult4(&num5, &num2);  // 1 * 3
        REQUIRE(mult4.interp() == 3);
        Num num6(-1);
        Mult mult5(&num6, &num3);  // -1 * (-2)
        REQUIRE(mult5.interp() == 2);
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




TEST_CASE("Nabil's given tests"){
    SECTION("Given Tests Assignment 2"){
        CHECK((new Var("x"))->equals(new Var("x")) == true );
        CHECK((new Var("x"))->equals(new Var("y")) == false );
        CHECK((new Num(1))->equals(new Var("x")) == false );
        CHECK( (new Add(new Num(2),new Num(3)))->equals(new Add(new Num(2),new Num(3)))==true );
        CHECK( (new Add(new Num(2),new Num(3)))->equals(new Add(new Num(3),new Num(2)))==true );
    }

    SECTION("Given Tests Assignment 3") {
        CHECK((new Mult(new Num(3), new Num(2)))
                      ->interp() == 6);
        CHECK((new Add(new Add(new Num(10), new Num(15)), new Add(new Num(20), new Num(20))))
                      ->interp() == 65);
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


}