#pragma once
#include "calculator.h"
#include <string>
#include <stack>

class ExpressionParser {
public:
    ExpressionParser(Calculator& c) : calc(c) {}

    double evaluate(const std::string& expr);

private:
    Calculator& calc;

    int precedence(char op);// приоритет оператора
    double applyOp(double a, double b, char op);
    void processTopOperator(std::stack<double>& values, std::stack<char>& ops);
};
