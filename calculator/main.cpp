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

    std::cout << "\nПроверим базовые операции:\n";
    std::cout << "sum(3, 5) = " << calc.callFunction("sum", { 3, 5 }) << "\n";
    std::cout << "sub(10, 4) = " << calc.callFunction("sub", { 10, 4 }) << "\n";
    std::cout << "mul(6, 7) = " << calc.callFunction("mul", { 6, 7 }) << "\n";
    std::cout << "div(12, 3) = " << calc.callFunction("div", { 12, 3 }) << "\n";

    std::cout << "\nПроверим загруженные плагины:\n";
    std::cout << "sin(90) = " << calc.callFunction("sin", { 90 }) << "\n";
    std::cout << "pow(2,4) = " << calc.callFunction("pow", { 2,4 }) << "\n";

    // тест ExpressionParser с приоритетом и скобками
    ExpressionParser parser(calc);
    std::string expr = "16 + 4 * (3 - 1)";
    std::cout << "\nТест ExpressionParser:\n";
    std::cout << expr << " = " << parser.evaluate(expr) << "\n";


    return 0;
}
