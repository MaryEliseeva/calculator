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
        //плагины
        else if (isalpha(expr[i])) {
            int j = i;
            // определяем названии функции плагина
            while (j < expr.length() && isalpha(expr[j])) j++;
            std::string funcName = expr.substr(i, j - i);
            i = j;
            if (expr[i] != '(') throw std::runtime_error("Ожидалась '(' после имени функции");
            i++;

            //Парсим аргументы
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
                double arg = evaluate(expr.substr(start, i - start));
                args.push_back(arg);

                if (expr[i] == ')') { i++; break; } // конец списка аргументов
                if (expr[i] == ',') i++;// следующий аргумент
            }

            values.push(calc.callFunction(funcName, args));
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
