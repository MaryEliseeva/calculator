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

// ������ ����� (������ ��������������)
double ExpressionParser::parseNumber(const std::string& expr, int& i) {
    int j = i;
    while (j < expr.length() && (isdigit(expr[j]) || expr[j] == '.')) j++;
    if (i == j) throw std::runtime_error("��������� �����");
    double num = std::stod(expr.substr(i, j - i));
    i = j;
    return num;
}

// ������� �������/�������
double ExpressionParser::parseFunction(const std::string& expr, int& i) {
    int j = i;
    while (j < expr.length() && isalpha(expr[j])) j++;
    std::string funcName = expr.substr(i, j - i);
    i = j;

    if (expr[i] != '(') throw std::runtime_error("��������� '(' ����� ����� �������");
    i++; // ���������� '('

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

// ��������� ��������� ��� ������
void ExpressionParser::processOperatorOrParenthesis(
    std::stack<double>& values, std::stack<char>& ops, char token)
{
    if (token == '(') {
        ops.push('(');
    }
    else if (token == ')') {
        while (!ops.empty() && ops.top() != '(') processTopOperator(values, ops);
        if (ops.empty()) throw std::runtime_error("������������ ������");
        ops.pop(); // ������� '('
    }
    else { // �������� ��������
        while (!ops.empty() && ops.top() != '(' && precedence(ops.top()) >= precedence(token))
            processTopOperator(values, ops);
        ops.push(token);
    }
}

// �������� ������� evaluate
double ExpressionParser::evaluate(const std::string& expr) {
    std::stack<double> values;
    std::stack<char> ops;
    int i = 0;

    while (i < expr.length()) {
        if (isspace(expr[i])) { i++; continue; }

        // ������� �����
        if (expr[i] == '-') {
            bool isUnary = (i == 0) || (!isdigit(expr[i - 1]) && expr[i - 1] != ')');
            if (isUnary) {
                // ������ ���������� � 0 - X
                values.push(0.0);
                processOperatorOrParenthesis(values, ops, '-');
                i++;
                continue;
            }
        }

        // �����
        if (isdigit(expr[i])) {
            values.push(parseNumber(expr, i));
            continue;
        }

        // �������� ��� ������
        if (expr[i] == '(' || expr[i] == ')' || expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') {
            processOperatorOrParenthesis(values, ops, expr[i]);
            i++;
            continue;
        }

        // ������� / ������
        if (isalpha(expr[i])) {
            values.push(parseFunction(expr, i));
            continue;
        }

        throw std::runtime_error(std::string("����������� ������: ") + expr[i]);
    }

    while (!ops.empty()) processTopOperator(values, ops);
    return values.top();
}