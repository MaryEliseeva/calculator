#include "ExpressionParser.h"
#include <cctype>
#include <stdexcept>
#include <iostream>

// приоритет операторов
int ExpressionParser::precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// применение оператора через Calculator
double ExpressionParser::applyOp(double a, double b, char op) {
    std::string opStr;
    opStr += op;
    return calc.callFunction(opStr, { a, b });
}

// вспомогательная функция для применения верхнего оператора из стека
void ExpressionParser::processTopOperator(std::stack<double>& values, std::stack<char>& ops) {
    if (values.size() < 2) throw std::runtime_error("Недостаточно чисел для операции");
    double b = values.top(); values.pop();
    double a = values.top(); values.pop();
    char op = ops.top(); ops.pop();
    values.push(applyOp(a, b, op));
}

double ExpressionParser::evaluate(const std::string& expr) {
    std::stack<double> values;   
    std::stack<char> ops;      

    int i = 0;
    while (i < expr.length()) {
        // пропускаем пробелы
        if (isspace(expr[i])) { i++; continue; }

        // число
        if (isdigit(expr[i])) {
            int j = i;
            while (j < expr.length() && (isdigit(expr[j]) || expr[j] == '.')) j++;
            double num = std::stod(expr.substr(i, j - i));
            values.push(num);
            i = j;
        }
        // открывающая скобка
        else if (expr[i] == '(') {
            ops.push('(');
            i++;
        }
        // закрывающая скобка
        else if (expr[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                processTopOperator(values, ops);
            }
            if (ops.empty()) throw std::runtime_error("Неправильные скобки");
            ops.pop(); // удаляем '('
            i++;
        }
        // оператор
        else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') {
            char currentOp = expr[i];
            while (!ops.empty() && ops.top() != '(' && precedence(ops.top()) >= precedence(currentOp)) {
                processTopOperator(values, ops);
            }
            ops.push(currentOp);
            i++;
        }
        // неизвестный символ
        else {
            throw std::runtime_error(std::string("Неизвестный символ: ") + expr[i]);
        }
    }

    //оставшиеся операторы
    while (!ops.empty()) {
        processTopOperator(values, ops);
    }

    return values.top();
}
