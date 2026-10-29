#include "massiv.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

static int integer(const std::string& token) {
    std::size_t end;
    int value = std::stoi(token, &end);
    if (end != token.size()) throw std::runtime_error("ожидалось целое число");
    return value;
}

static void findSubarrays(const Array& array, long long target) {
    bool found = false;
    for (std::size_t left = 0; left < array.len; ++left) {
        long long sum = 0;
        for (std::size_t right = left; right < array.len; ++right) {
            sum += std::stoi(arrayGet(array, right));
            if (sum != target) continue;
            found = true; std::cout << '[';
            for (std::size_t i = left; i <= right; ++i)
                std::cout << (i == left ? "" : ", ") << arrayGet(array, i);
            std::cout << "]\n";
        }
    }
    if (!found) std::cout << "Подмассивов с такой суммой нет\n";
}

int main() {
    Array array;
    int status = 0;
    try {
        std::cout << "Введите массив одной строкой:\n";
        std::string line, token;
        if (!std::getline(std::cin, line)) throw std::runtime_error("нет массива");
        std::istringstream input(line);
        while (input >> token) arrayPush(array, std::to_string(integer(token)));
        std::cout << "Введите искомую сумму:\n";
        if (!std::getline(std::cin, line)) throw std::runtime_error("нет суммы");
        std::istringstream targetInput(line);
        std::string extra;
        if (!(targetInput >> token) || (targetInput >> extra)) throw std::runtime_error("нужна одна сумма");
        findSubarrays(array, integer(token));
    } catch (const std::exception& error) { std::cerr << "Ошибка: " << error.what() << '\n'; status = 1; }
    arrayClear(array);
    return status;
}
