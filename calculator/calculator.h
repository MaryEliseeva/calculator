#pragma once
#include <string>
#include <vector>
#include <functional>
#include <stdexcept>
#include <locale.h>

class Calculator {
public:
    using Func = std::function<double(const std::vector<double>&)>;

    struct FunctionBase {
        std::string name;
        Func apply;
    };
    void listFunctions();
    void addFunction(const std::string& name, Func func);
    double callFunction(const std::string& name, const std::vector<double>& args);

private:
    std::vector<FunctionBase> functions;
};
