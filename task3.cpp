#include "set.h"
#include <iostream>
#include <stdexcept>
#include <limits>
#include <cstdlib>

struct Partition {
    Array values, current, best;
    long long total = 0, difference = std::numeric_limits<long long>::max();
};

static void search(Partition& state, std::size_t index, long long sum, std::size_t count) {
    if (state.difference == state.total % 2) return;
    if (index == state.values.len) {
        if (count == state.values.len) return;
        long long difference = std::llabs(state.total - 2 * sum);
        if (difference < state.difference) {
            state.difference = difference;
            for (std::size_t i = 0; i < state.values.len; ++i)
                arraySet(state.best, i, arrayGet(state.current, i));
        }
        return;
    }
    arraySet(state.current, index, "1");
    search(state, index + 1, sum + std::stoi(arrayGet(state.values, index)), count + 1);
    arraySet(state.current, index, "0");
    search(state, index + 1, sum, count);
}

static void partition(const Set& set, Partition& state) {
    if (set.n < 2) throw std::runtime_error("нужно хотя бы два различных натуральных числа");
    setValues(set, state.values);
    for (std::size_t i = 0; i < state.values.len; ++i) {
        int value = std::stoi(arrayGet(state.values, i));
        if (value <= 0) throw std::runtime_error("нужны натуральные числа");
        state.total += value;
        arrayPush(state.current, "0"); arrayPush(state.best, "0");
    }
    // Первый элемент фиксирован: перестановка двух групп не меняет разницу.
    arraySet(state.current, 0, "1");
    search(state, 1, std::stoi(arrayGet(state.values, 0)), 1);
    for (int group = 1; group >= 0; --group) {
        std::cout << '{'; bool first = true;
        for (std::size_t i = 0; i < state.values.len; ++i)
            if (std::stoi(arrayGet(state.best, i)) == group) {
                std::cout << (first ? "" : ", ") << arrayGet(state.values, i);
                first = false;
            }
        std::cout << "}\n";
    }
    std::cout << "Разница: " << state.difference << '\n';
}

int main() {
    Set set;
    Partition state;
    int status = 0;
    try {
        std::cout << "Введите количество чисел, затем элементы:\n";
        std::string countToken; std::size_t countEnd;
        if (!(std::cin >> countToken)) throw std::runtime_error("нет количества");
        int n = std::stoi(countToken, &countEnd);
        if (countEnd != countToken.size() || n < 2) throw std::runtime_error("неверное количество");
        for (int i = 0; i < n; ++i) {
            std::string token; std::size_t end;
            if (!(std::cin >> token)) throw std::runtime_error("не хватает чисел");
            int value = std::stoi(token, &end);
            if (end != token.size() || value <= 0) throw std::runtime_error("нужны натуральные числа");
            setAdd(set, value);
        }
        partition(set, state);
    } catch (const std::exception& error) { std::cerr << "Ошибка: " << error.what() << '\n'; status = 1; }
    arrayClear(state.values); arrayClear(state.current); arrayClear(state.best); setClear(set);
    return status;
}
