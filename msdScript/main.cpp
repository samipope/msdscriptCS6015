
#define CATCH_CONFIG_RUNNER

#include <iostream>
#include "cmdline.h"
#include "parse.h"
#include "Expr.h"
#include "catch.h"


int main(int argc, const char * argv[]) {
   run_mode_t mode = useArgs(argc, argv);

    switch (mode) {
        case do_test:
            if(Catch::Session().run(1,argv) !=0){
                    //if catch returns a non-zero value, then kill the program
                    exit(1);
                }
            break;

        case do_interp: {
            std::string line;
            std::cout << "Enter expression: ";
            if (std::getline(std::cin, line)) { // Reads a line of input
                try {
                    std::istringstream expr_stream(line);
                    Expr* expr = parse_expr(expr_stream);
                    std::cout << expr->interp() << std::endl;
                    delete expr;
                } catch (const std::exception& e) {
                    std::cerr << e.what() << std::endl;
                    exit(1);
                }
            } else {
                std::cerr << "No input received.\n";
                exit(1);
            }
            break;

        }
        case do_print: {
            std::string line;
            std::cout << "Enter expression: ";
            if (std::getline(std::cin, line)) { // Reads a line of input
                try {
                    std::istringstream expr_stream(line);
                    Expr* expr = parse_expr(expr_stream);
                    expr->print(std::cout);
                    std::cout << std::endl;
                    delete expr;
                } catch (const std::exception& e) {
                    std::cerr << e.what() << std::endl;
                    exit(1);
                }
            } else {
                std::cerr << "No input received.\n";
                exit(1);
            }
            break;

        }

        case do_pretty_print: {
            std::string line;
            std::cout << "Enter expression: ";
            if (std::getline(std::cin, line)) { // Reads a line of input
                try {
                    std::istringstream expr_stream(line);
                    Expr *expr = parse_expr(expr_stream);
                    std::cout << expr->to_pp_string();
                    std::cout << std::endl;
                    delete expr;
                } catch (const std::exception &e) {
                    std::cerr << e.what() << std::endl;
                    exit(1);
                }
            } else {
                std::cerr << "No input received.\n";
                exit(1);
            }
            break;
        }

        default:
            std::cerr << "error: no valid mode selected \n";
            exit(1);
    }

    return 0;
}


