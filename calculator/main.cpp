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
    
    std::string expr = "sin(pow(2,3) + 1)";
    std::cout << "\nТест ExpressionParser:\n";
    std::cout << expr << " = " << parser.evaluate(expr) << "\n";

    return 0;
}
