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

// Чтение числа (только положительного)
double ExpressionParser::parseNumber(const std::string& expr, int& i) {
    int j = i;
    while (j < expr.length() && (isdigit(expr[j]) || expr[j] == '.')) j++;
    if (i == j) throw std::runtime_error("Ожидалось число");
    double num = std::stod(expr.substr(i, j - i));
    i = j;
    return num;
}

// Парсинг функции/плагина
double ExpressionParser::parseFunction(const std::string& expr, int& i) {
    int j = i;
    while (j < expr.length() && isalpha(expr[j])) j++;
    std::string funcName = expr.substr(i, j - i);
    i = j;

    if (expr[i] != '(') throw std::runtime_error("Ожидалась '(' после имени функции");
    i++; // пропускаем '('

    std::vector<double> args;
    while (true) {
        int start = i;
        int parenCount = 0;
        while (i < expr.length() &&
            (expr[i] != ',' || parenCount > 0) &&
            (expr[i] != ')' || parenCount > 0)) {
            if (expr[i] == '(') parenCount++;
            if (expr[i] == ')') parenCount--;
            i++;
        }
        args.push_back(evaluate(expr.substr(start, i - start)));

        if (expr[i] == ')') { i++; break; }
        if (expr[i] == ',') i++;
    }

    return calc.callFunction(funcName, args);
}

// Обработка оператора или скобки
void ExpressionParser::processOperatorOrParenthesis(
    std::stack<double>& values, std::stack<char>& ops, char token)
{
    if (token == '(') {
        ops.push('(');
    }
    else if (token == ')') {
        while (!ops.empty() && ops.top() != '(') processTopOperator(values, ops);
        if (ops.empty()) throw std::runtime_error("Неправильные скобки");
        ops.pop(); // убираем '('
    }
    else { // бинарный оператор
        while (!ops.empty() && ops.top() != '(' && precedence(ops.top()) >= precedence(token))
            processTopOperator(values, ops);
        ops.push(token);
    }
}

// Основная функция evaluate
double ExpressionParser::evaluate(const std::string& expr) {
    std::stack<double> values;
    std::stack<char> ops;
    int i = 0;

    while (i < expr.length()) {
        if (isspace(expr[i])) { i++; continue; }

        // Унарный минус
        if (expr[i] == '-') {
            bool isUnary = (i == 0) || (!isdigit(expr[i - 1]) && expr[i - 1] != ')');
            if (isUnary) {
                // Всегда превращаем в 0 - X
                values.push(0.0);
                processOperatorOrParenthesis(values, ops, '-');
                i++;
                continue;
            }
        }

        // Число
        if (isdigit(expr[i])) {
            values.push(parseNumber(expr, i));
            continue;
        }

        // Оператор или скобка
        if (expr[i] == '(' || expr[i] == ')' || expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') {
            processOperatorOrParenthesis(values, ops, expr[i]);
            i++;
            continue;
        }

        // Функция / плагин
        if (isalpha(expr[i])) {
            values.push(parseFunction(expr, i));
            continue;
        }

        throw std::runtime_error(std::string("Неизвестный символ: ") + expr[i]);
    }

    while (!ops.empty()) processTopOperator(values, ops);
    return values.top();
}