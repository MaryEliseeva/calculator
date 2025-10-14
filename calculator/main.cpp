#include "calculator.h"
#include <iostream>

void loadPlugin(Calculator& calc, const char* dllPath);

int main() {
    setlocale(LC_CTYPE, "Russian");
    Calculator calc;

    loadPlugin(calc, "funcsin.dll");
    loadPlugin(calc, "funcpow.dll");
    loadPlugin(calc, "funcsum.dll");


    std::cout << "\nСписок функций:\n";
    calc.listFunctions();

    std::cout << "sin(90) = " << calc.callFunction("sin", { 90 }) << "\n";
    std::cout << "pow(2,4) = " << calc.callFunction("pow", { 2,4 }) << "\n";
    std::cout << "sum(1,4) = " << calc.callFunction("sum", { 1,4 }) << "\n";

    return 0;
}
