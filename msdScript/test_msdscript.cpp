#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "exec.h"
#include "Expr.h"
#include "parse.h"
#include <vector>
#include <sstream>

std::string random_expr_string(int depth = 0);
std::string random_variable();


std::string random_expr_string(int depth) {
    int r = rand() % 10; // Adjust ratios as needed
    if (depth > 2 || r < 3) { // Base case: favor numbers at deeper recursion levels
        return std::to_string(rand() % 100); // Numbers from 0 to 99
    } else if (r < 6) { // Recursive case: addition
        return random_expr_string(depth + 1) + " + " + random_expr_string(depth + 1);
    } else if (r < 8) { // Variable
        return random_variable();
    } else { // Let expression
        std::string var = random_variable();
        std::string valExpr = random_expr_string(depth + 1);
        std::string bodyExpr = random_expr_string(depth + 1);
        return "let " + var + " = " + valExpr + " in " + bodyExpr;
    }
}

std::string random_variable() {
    // Generates a single lowercase letter as a variable
    return std::string(1, 'a' + (rand() % 26));
}

ExecResult run_msdscript(const std::string& msdscriptPath, const std::string& mode, const std::string& expr) {
    std::vector<const char*> args = {msdscriptPath.c_str(), mode.c_str(), NULL};
    return exec_program(args.size() - 1, args.data(), expr);
}


void validate_interp(const std::string& msdscriptPath, const std::string& expr) {
    // Run msdscript --interp
    ExecResult result = run_msdscript(msdscriptPath, "--interp", expr);

    // Generate expected outcome using your Expr class
    std::istringstream expr_stream(expr);
    Expr* expr_parsed = parse_expr(expr_stream);
    std::string expectedOutcome = std::to_string(expr_parsed->interp());

    // Compare expected outcome with actual outcome
    if (result.out != expectedOutcome + "\n") { // Assuming msdscript adds a newline
        std::cerr << "Mismatch in --interp mode for expression: " << expr << std::endl;
        std::cerr << "Expected: " << expectedOutcome << ", Got: " << result.out;
    }

    delete expr_parsed;
}

void validate_pretty_print(const std::string& msdscriptPath, const std::string& expr) {
    // Run msdscript --pretty-print
    ExecResult result = run_msdscript(msdscriptPath, "--pretty-print", expr);

    // Generate expected pretty-printed outcome
    std::istringstream expr_stream(expr);
    Expr* expr_parsed = parse_expr(expr_stream);
    std::string expectedPrettyPrint = expr_parsed->to_pp_string();

    // Compare expected outcome with actual outcome
    if (result.out != expectedPrettyPrint) {
        std::cerr << "Mismatch in --pretty-print mode for expression: " << expr << std::endl;
        std::cerr << "Expected: " << expectedPrettyPrint << ", Got: " << result.out;
    }

    delete expr_parsed;
}

void validate_output(const std::string& msdscriptPath, const std::string& mode, const std::string& expr) {
    if (mode == "--interp") {
        validate_interp(msdscriptPath, expr);
    } else if (mode == "--print") {
        ExecResult result = run_msdscript(msdscriptPath, mode, expr);
        if (result.out != expr + "\n") { // Assuming the output includes a newline
            std::cerr << "Validation failed for mode " << mode << " with expression: " << expr << std::endl;
            std::cerr << "Expected output: " << expr << "\n";
            std::cerr << "Actual output: " << result.out;
        } else {
            std::cout << "Validation succeeded for mode " << mode << " with expression: " << expr << std::endl;
        }
    } else if (mode == "--pretty-print") {
        validate_pretty_print(msdscriptPath, expr);
    }

    // No need to check for errors here as it's done within validate_interp and validate_pretty_print
}




int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <msdscript_path>" << std::endl;
        return 1;
    }

    std::string msdscriptPath = argv[1];
    std::string expr = random_expr_string();

    // Example usage for validating outputs
    validate_output(msdscriptPath, "--print", expr);
    validate_output(msdscriptPath, "--pretty-print", expr);
    validate_output(msdscriptPath, "--interp", expr);

    // Further implementation...

    return 0;
}
