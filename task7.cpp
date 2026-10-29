#include "hashtable.h"
#include "dlist.h"
#include "massiv.h"
#include <iostream>
#include <stdexcept>

struct LFUCache {
    int capacity = 0;
    HashTable index;
    DList order;
    Array keys, values, frequency, accessed;
    long long clock = 0;
};

static void clearCache(LFUCache& cache) {
    hashClear(cache.index); dlistClear(cache.order);
    arrayClear(cache.keys); arrayClear(cache.values); arrayClear(cache.frequency); arrayClear(cache.accessed);
}

static void initCache(LFUCache& cache, int capacity) {
    if (capacity < 0) throw std::runtime_error("ёмкость не может быть отрицательной");
    cache.capacity = capacity;
    hashInit(cache.index, static_cast<std::size_t>(capacity) * 2 + 4, false);
}

static void touch(LFUCache& cache, int slot) {
    long long count = std::stoll(arrayGet(cache.frequency, slot));
    arraySet(cache.frequency, slot, std::to_string(count + 1));
    arraySet(cache.accessed, slot, std::to_string(++cache.clock));
}

static int get(LFUCache& cache, int key) {
    int slot;
    if (!hashGet(cache.index, std::to_string(key), slot)) return -1;
    touch(cache, slot);
    return std::stoi(arrayGet(cache.values, slot));
}

static void set(LFUCache& cache, int key, int value) {
    if (!cache.capacity) return;
    int slot;
    std::string name = std::to_string(key);
    if (hashGet(cache.index, name, slot)) {
        arraySet(cache.values, slot, std::to_string(value));
        touch(cache, slot);
        return;
    }
    if (cache.order.n == static_cast<std::size_t>(cache.capacity)) {
        DNode* victim = cache.order.head;
        for (DNode* node = victim->next; node; node = node->next) {
            int candidate = std::stoi(node->val), selected = std::stoi(victim->val);
            long long frequency = std::stoll(arrayGet(cache.frequency, candidate));
            long long minimum = std::stoll(arrayGet(cache.frequency, selected));
            if (frequency < minimum || (frequency == minimum &&
                std::stoll(arrayGet(cache.accessed, candidate)) < std::stoll(arrayGet(cache.accessed, selected)))) victim = node;
        }
        // При равной частоте удаляем самый давний элемент.
        slot = std::stoi(victim->val);
        hashSet(cache.index, name, slot);
        hashDel(cache.index, arrayGet(cache.keys, slot));
        arraySet(cache.keys, slot, name); arraySet(cache.values, slot, std::to_string(value));
        arraySet(cache.frequency, slot, "1");
        arraySet(cache.accessed, slot, std::to_string(++cache.clock));
    } else {
        slot = static_cast<int>(cache.order.n);
        arrayPush(cache.keys, name); arrayPush(cache.values, std::to_string(value)); arrayPush(cache.frequency, "1");
        dlistPushTail(cache.order, std::to_string(slot));
        arrayPush(cache.accessed, std::to_string(++cache.clock));
        hashSet(cache.index, name, slot);
    }
}

static int readInt() {
    std::string token; std::size_t end;
    if (!(std::cin >> token)) throw std::runtime_error("не хватает данных");
    int value = std::stoi(token, &end);
    if (end != token.size()) throw std::runtime_error("ожидалось целое число");
    return value;
}

int main() {
    LFUCache cache;
    int status = 0;
    try {
        std::cout << "Ёмкость кеша и количество запросов, затем SET x y или GET x:\n";
        int capacity = readInt(), count = readInt();
        if (count < 0) throw std::runtime_error("отрицательное количество запросов");
        initCache(cache, capacity);
        for (int i = 0; i < count; ++i) {
            std::string command;
            if (!(std::cin >> command)) throw std::runtime_error("не хватает запросов");
            int key = readInt();
            if (command == "SET") { int value = readInt(); set(cache, key, value); }
            else if (command == "GET") std::cout << get(cache, key) << '\n';
            else throw std::runtime_error("неизвестная команда");
        }
    } catch (const std::exception& error) { std::cerr << "Ошибка: " << error.what() << '\n'; status = 1; }
    clearCache(cache);
    return status;
}
