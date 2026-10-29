#include "set.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cerrno>

static int integer(const std::string& token) {
    std::size_t end;
    int value = std::stoi(token, &end);
    if (end != token.size()) throw std::runtime_error("ожидалось целое число");
    return value;
}

static bool execute(Set& set, const std::string& query, bool& changed) {
    std::istringstream input(query);
    std::string command, token, extra;
    if (!(input >> command >> token) || (input >> extra)) throw std::runtime_error("команда и одно число");
    int value = integer(token);
    bool result;
    changed = false;
    if (command == "SETADD") result = changed = setAdd(set, value);
    else if (command == "SETDEL") result = changed = setDel(set, value);
    else if (command == "SET_AT") result = setAt(set, value);
    else throw std::runtime_error("команды: SETADD, SETDEL, SET_AT");
    return result;
}

int main(int argc, char* argv[]) {
    Set set;
    try {
        std::string filename, query;
        for (int i = 1; i < argc; ++i) {
            std::string option = argv[i];
            if (option == "--file" && i + 1 < argc && filename.empty()) filename = argv[++i];
            else if (option == "--query" && i + 1 < argc && query.empty()) query = argv[++i];
            else throw std::runtime_error("./task2 --file путь --query 'команда число'");
        }
        if (filename.empty() || query.empty()) throw std::runtime_error("нужны --file и --query");
        errno = 0;
        std::ifstream file(filename);
        if (!file && errno != ENOENT) throw std::runtime_error("не удалось открыть файл");
        std::string token;
        while (file >> token) setAdd(set, integer(token));
        if (file.bad()) throw std::runtime_error("ошибка чтения файла");
        file.close();
        bool changed;
        bool result = execute(set, query, changed);
        if (changed) {
            std::ofstream output(filename);
            if (!output) throw std::runtime_error("не удалось записать файл");
            for (std::size_t i = 0; i < set.buckets.size(); ++i)
                for (SNode* node = set.buckets[i].head; node; node = node->next) output << node->val << ' ';
            output << '\n'; output.close();
            if (!output) throw std::runtime_error("ошибка записи файла");
        }
        std::cout << (result ? "TRUE\n" : "FALSE\n");
        setClear(set);
    } catch (const std::exception& error) { setClear(set); std::cerr << "Ошибка: " << error.what() << '\n'; return 1; }
}
