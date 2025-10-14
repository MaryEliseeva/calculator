#include "calculator.h"

Calculator::Calculator() {
    functions.push_back({ "+", [](const std::vector<double>& args) {
        if (args.size() != 2)
            throw std::runtime_error("sum ожидает 2 аргумента");
        return args[0] + args[1];
    } });

    functions.push_back({ "-", [](const std::vector<double>& args) {
        if (args.size() != 2)
            throw std::runtime_error("sub ожидает 2 аргумента");
        return args[0] - args[1];
    } });

    functions.push_back({ "*", [](const std::vector<double>& args) {
        if (args.size() != 2)
            throw std::runtime_error("mul ожидает 2 аргумента");
        return args[0] * args[1];
    } });

    functions.push_back({ "/", [](const std::vector<double>& args) {
        if (args.size() != 2)
            throw std::runtime_error("div ожидает 2 аргумента");
        if (args[1] == 0)
            throw std::runtime_error("деление на ноль");
        return args[0] / args[1];
    } });
}

void Calculator::addFunction(const std::string& name, Func func) {
    for (const auto& f : functions)
        if (f.name == name)
            throw std::runtime_error("Функция " + name + " уже зарегистрирована");
    functions.push_back({ name, func });
}

double Calculator::callFunction(const std::string& name, const std::vector<double>& args) {
    for (const auto& f : functions)
        if (f.name == name)
            return f.apply(args);
    throw std::runtime_error("Неизвестная функция: " + name);
}

void Calculator::listFunctions() {
    std::cout << "Доступные функции:\n";
    for (const auto& f : functions)
        std::cout << " - " << f.name << "\n";
}
