#include "ExpressionParser.h"
#include <cctype>
#include <stdexcept>
#include <iostream>

// ��������� ����������
int ExpressionParser::precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// ���������� ��������� ����� Calculator
double ExpressionParser::applyOp(double a, double b, char op) {
    std::string opStr;
    opStr += op;
    return calc.callFunction(opStr, { a, b });
}

// ��������������� ������� ��� ���������� �������� ��������� �� �����
void ExpressionParser::processTopOperator(std::stack<double>& values, std::stack<char>& ops) {
    if (values.size() < 2) throw std::runtime_error("������������ ����� ��� ��������");
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
        // ���������� �������
        if (isspace(expr[i])) { i++; continue; }

        // �����
        if (isdigit(expr[i])) {
            int j = i;
            while (j < expr.length() && (isdigit(expr[j]) || expr[j] == '.')) j++;
            double num = std::stod(expr.substr(i, j - i));
            values.push(num);
            i = j;
        }
        // ����������� ������
        else if (expr[i] == '(') {
            ops.push('(');
            i++;
        }
        // ����������� ������
        else if (expr[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                processTopOperator(values, ops);
            }
            if (ops.empty()) throw std::runtime_error("������������ ������");
            ops.pop(); // ������� '('
            i++;
        }
        // ��������
        else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') {
            char currentOp = expr[i];
            while (!ops.empty() && ops.top() != '(' && precedence(ops.top()) >= precedence(currentOp)) {
                processTopOperator(values, ops);
            }
            ops.push(currentOp);
            i++;
        }
        // ����������� ������
        else {
            throw std::runtime_error(std::string("����������� ������: ") + expr[i]);
        }
    }

    //���������� ���������
    while (!ops.empty()) {
        processTopOperator(values, ops);
    }

    return values.top();
}
