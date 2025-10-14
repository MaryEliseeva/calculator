#include "calculator.h"
#include "pluginsLoader.h"
#include "ExpressionParser.h"
#include <iostream>


int main() {
    setlocale(LC_CTYPE, "Russian");

    Calculator calc;
    loadPlugin(calc, "plugins/funcsin.dll");
    loadPlugin(calc, "plugins/funcpow.dll");
    std::cout << "\nСписок функций:\n";
    calc.listFunctions();

    ExpressionParser parser(calc);
    
    std::string expr = "2 + 2 * 2";
    std::cout << "\nТест ExpressionParser:\n";
    std::cout << expr << " = " << parser.evaluate(expr) << "\n";

    return 0;
}
