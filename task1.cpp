#include "stack.h"
#include <iostream>
#include <stdexcept>
#include <cctype>

static int priority(char operation) {
    if (operation == '!') return 3;
    return operation == '&' ? 2 : 1;
}

static void apply(Stack& values, Stack& operations) {
    char operation = stackPop(operations)[0];
    int right = std::stoi(stackPop(values));
    if (operation == '!') { stackPush(values, std::to_string(!right)); return; }
    int left = std::stoi(stackPop(values));
    int result = operation == '&' ? (left & right) : operation == '|' ? (left | right) : (left ^ right);
    stackPush(values, std::to_string(result));
}

static int evaluate(const std::string& expression) {
    Stack values, operations;
    bool needValue = true;
    try {
        for (char c : expression) {
            if (std::isspace(static_cast<unsigned char>(c))) continue;
            if (c == '0' || c == '1') {
                if (!needValue) throw std::runtime_error("нет операции между числами");
                stackPush(values, std::string(1, c));
                needValue = false;
            } else if (c == '(' || c == '!') {
                if (!needValue) throw std::runtime_error("нет операции перед знаком или скобкой");
                stackPush(operations, std::string(1, c));
            } else if (c == ')') {
                if (needValue) throw std::runtime_error("пустая или неверная скобка");
                while (operations.l.n && stackGet(operations) != "(") apply(values, operations);
                if (!operations.l.n) throw std::runtime_error("лишняя закрывающая скобка");
                stackPop(operations);
            } else if (c == '&' || c == '|' || c == '^') {
                if (needValue) throw std::runtime_error("нет левого операнда");
                while (operations.l.n && stackGet(operations) != "(" &&
                       priority(stackGet(operations)[0]) >= priority(c)) apply(values, operations);
                stackPush(operations, std::string(1, c));
                needValue = true;
            } else throw std::runtime_error("допустимы только 0, 1, !, &, |, ^ и скобки");
        }
        if (needValue) throw std::runtime_error("выражение не закончено");
        while (operations.l.n) {
            if (stackGet(operations) == "(") throw std::runtime_error("скобка не закрыта");
            apply(values, operations);
        }
        int result = std::stoi(stackPop(values));
        stackClear(values); stackClear(operations);
        return result;
    } catch (...) { stackClear(values); stackClear(operations); throw; }
}

int main() {
    std::string expression;
    while (true) {
        std::cout << "Введите выражение:\n";
        if (!std::getline(std::cin, expression) || expression == "EXIT" || expression == "exit") break;
        try {
            std::cout << evaluate(expression) << '\n';
        } catch (const std::exception& error) {
            std::cerr << "Ошибка: " << error.what() << '\n';
        }
    }
    return 0;
}
