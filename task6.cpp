#include "hashtable.h"
#include <iostream>
#include <chrono>
#include <stdexcept>

static int integer(const std::string& token) {
    std::size_t end;
    int value = std::stoi(token, &end);
    if (end != token.size()) throw std::runtime_error("ожидалось целое число");
    return value;
}

static int readInt() {
    std::string token;
    if (!(std::cin >> token)) throw std::runtime_error("не хватает данных");
    return integer(token);
}

static void compare() {
    std::cout << "Цепочки: коллизии хранятся в списках; просто, но поиск зависит от длины цепочки.\n"
              << "Кукушка: ключ имеет два места; поиск проверяет до двух ячеек, вставка может требовать перестроения.\n"
              << "Начальный размер 1000, одинаковые ключи i*7919. Время в микросекундах.\n"
              << "Проба: просмотр узла цепочки или ячейки кукушки, включая перестроения.\n"
              << "target method size capacity actual_load rebuilds insert_us find_us erase_us probes\n";
    for (int load = 25; load <= 90; load += (load == 75 ? 15 : 25)) {
        for (int method = 0; method < 2; ++method) {
            HashTable table;
            try {
                hashInit(table, 1000, method == 1);
                int count = 1000 * load / 100;
                auto start = std::chrono::steady_clock::now();
                for (int i = 0; i < count; ++i) hashSet(table, std::to_string(i * 7919), i);
                auto inserted = std::chrono::steady_clock::now();
                std::size_t capacity = table.capacity;
                for (int i = 0; i < count; ++i) {
                    int value;
                    if (!hashGet(table, std::to_string(i * 7919), value) || value != i)
                        throw std::runtime_error("ошибка поиска при сравнении");
                }
                auto found = std::chrono::steady_clock::now();
                for (int i = 0; i < count; ++i)
                    if (!hashDel(table, std::to_string(i * 7919))) throw std::runtime_error("ошибка удаления");
                auto erased = std::chrono::steady_clock::now();
                std::cout << load << "% " << (method ? "CUCKOO " : "CHAIN ") << count << ' ' << capacity << ' '
                          << 100.0 * count / capacity << "% " << table.rebuilds << ' '
                          << std::chrono::duration<double, std::micro>(inserted - start).count() << ' '
                          << std::chrono::duration<double, std::micro>(found - inserted).count() << ' '
                          << std::chrono::duration<double, std::micro>(erased - found).count() << ' '
                          << table.probes << '\n';
                hashClear(table);
            } catch (...) { hashClear(table); throw; }
        }
    }
}

int main(int argc, char* argv[]) {
    HashTable table;
    int status = 0;
    try {
        if (argc == 2 && std::string(argv[1]) == "--bench") { compare(); return 0; }
        if (argc != 1) throw std::runtime_error("./task6 или ./task6 --bench");
        std::cout << "Метод (1 — цепочки, 2 — кукушка), начальный размер, число запросов:\n";
        int method = readInt(), capacity = readInt(), count = readInt();
        if ((method != 1 && method != 2) || capacity < 4 || count < 0)
            throw std::runtime_error("неверный метод, размер или число запросов");
        hashInit(table, capacity, method == 2);
        std::cout << "Запросы: SET ключ значение, GET ключ, DEL ключ, STATS\n";
        for (int i = 0; i < count; ++i) {
            std::string command, key;
            if (!(std::cin >> command)) throw std::runtime_error("не хватает запросов");
            if (command == "STATS") {
                std::cout << "size=" << table.n << " capacity=" << table.capacity
                          << " rebuilds=" << table.rebuilds << " probes=" << table.probes << '\n';
                continue;
            }
            if (!(std::cin >> key)) throw std::runtime_error("нет ключа");
            if (command == "SET") { int value = readInt(); hashSet(table, key, value); std::cout << "OK\n"; }
            else if (command == "GET") { int value; std::cout << (hashGet(table, key, value) ? value : -1) << '\n'; }
            else if (command == "DEL") std::cout << (hashDel(table, key) ? "TRUE\n" : "FALSE\n");
            else throw std::runtime_error("неизвестная команда");
        }
    } catch (const std::exception& error) { std::cerr << "Ошибка: " << error.what() << '\n'; status = 1; }
    hashClear(table);
    return status;
}
