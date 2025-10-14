#include "calculator.h"
#include <iostream>

void Calculator::addFunction(const std::string& name, Func func) {
    for (const auto& f : functions) {
        if (f.name == name) {
            throw std::runtime_error("Функция " + name + " уже зарегистрирована");
        }
    }
    functions.push_back({ name, func });
}

double Calculator::callFunction(const std::string& name, const std::vector<double>& args) {
    for (const auto& f : functions) {
        if (f.name == name) {
            return f.apply(args);
        }
    }
    throw std::runtime_error("Неизвестная функция: " + name);
}

void Calculator::listFunctions() {
    std::cout << "Доступные функции:\n";
    for (const auto& f : functions) {
        std::cout << " - " << f.name << "\n";
    }
}
