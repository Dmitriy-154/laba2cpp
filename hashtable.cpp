#include "hashtable.h"
#include <stdexcept>
#include <limits>

static std::size_t hashIndex(const HashTable& table, const std::string& key, unsigned int which) {
    unsigned int hash = 2166136261u ^ (table.seed * 2654435761u + which * 2246822519u);
    for (unsigned char c : key) { hash ^= c; hash *= 16777619u; }
    hash ^= hash >> 16; hash *= 2246822519u; hash ^= hash >> 13;
    std::size_t index = hash % table.capacity;
    if (which && index == hashIndex(table, key, 0)) index = (index + 1) % table.capacity;
    return index;
}

static std::size_t doubled(std::size_t capacity) {
    if (capacity > std::numeric_limits<std::size_t>::max() / 2) throw std::runtime_error("слишком большая таблица");
    return capacity * 2;
}

void hashClear(HashTable& table) {
    for (std::size_t i = 0; i < table.buckets.size(); ++i) slistClear(table.buckets[i]);
    table.buckets.clear(); table.cells.clear();
    table.n = table.capacity = 0;
}

void hashInit(HashTable& table, std::size_t capacity, bool cuckoo) {
    hashClear(table);
    if (capacity < 4) capacity = 4;
    table.cuckoo = cuckoo; table.capacity = capacity;
    table.seed = 1; table.probes = table.rebuilds = 0;
    if (cuckoo) table.cells.resize(capacity);
    else table.buckets.resize(capacity);
}

static HashCell unpack(const std::string& text) {
    std::size_t separator = text.find('\n');
    return HashCell{text.substr(0, separator), std::stoi(text.substr(separator + 1)), true};
}

static SNode* findChain(HashTable& table, const std::string& key) {
    SList& chain = table.buckets[hashIndex(table, key, 0)];
    for (SNode* node = chain.head; node; node = node->next) {
        ++table.probes;
        if (node->val.substr(0, node->val.find('\n')) == key) return node;
    }
    return nullptr;
}

static bool place(HashTable& table, HashCell& incoming) {
    std::size_t index = hashIndex(table, incoming.key, 0);
    for (std::size_t step = 0; step < table.capacity * 2; ++step) {
        ++table.probes;
        if (!table.cells[index].used) { table.cells[index] = incoming; return true; }
        HashCell displaced = table.cells[index];
        table.cells[index] = incoming;
        incoming = displaced;
        std::size_t first = hashIndex(table, incoming.key, 0);
        index = index == first ? hashIndex(table, incoming.key, 1) : first;
    }
    return false;
}

static void rebuild(HashTable& table, std::size_t capacity, const HashCell* missing = nullptr) {
    std::vector<HashCell> entries;
    if (table.cuckoo) {
        for (std::size_t i = 0; i < table.capacity; ++i)
            if (table.cells[i].used) entries.push_back(table.cells[i]);
    } else {
        for (std::size_t i = 0; i < table.capacity; ++i)
            for (SNode* node = table.buckets[i].head; node; node = node->next) entries.push_back(unpack(node->val));
    }
    if (missing) entries.push_back(*missing);
    HashTable fresh;
    unsigned int seed = table.seed;
    try {
        for (int attempt = 0;; ++attempt) {
            hashInit(fresh, capacity, table.cuckoo);
            fresh.seed = ++seed;
            bool success = true;
            for (std::size_t i = 0; i < entries.size(); ++i) {
                if (fresh.cuckoo) {
                    HashCell cell = entries[i];
                    if (!place(fresh, cell)) { success = false; break; }
                } else slistPushHead(fresh.buckets[hashIndex(fresh, entries[i].key, 0)],
                                     entries[i].key + "\n" + std::to_string(entries[i].value));
            }
            table.probes += fresh.probes;
            if (success) break;
            if (attempt % 8 == 7) capacity = doubled(capacity);
        }
        hashClear(table);
        table.buckets.swap(fresh.buckets); table.cells.swap(fresh.cells);
        table.capacity = fresh.capacity; table.n = entries.size(); table.seed = fresh.seed;
        ++table.rebuilds;
    } catch (...) { hashClear(fresh); throw; }
}

bool hashGet(HashTable& table, const std::string& key, int& value) {
    if (!table.capacity) throw std::runtime_error("таблица не инициализирована");
    if (!table.cuckoo) {
        SNode* node = findChain(table, key);
        if (!node) return false;
        value = unpack(node->val).value;
        return true;
    }
    for (unsigned int which = 0; which < 2; ++which) {
        ++table.probes;
        const HashCell& cell = table.cells[hashIndex(table, key, which)];
        if (cell.used && cell.key == key) { value = cell.value; return true; }
    }
    return false;
}

void hashSet(HashTable& table, const std::string& key, int value) {
    if (!table.capacity || key.empty() || key.find('\n') != std::string::npos)
        throw std::runtime_error("неверная таблица или ключ");
    if (!table.cuckoo) {
        SNode* node = findChain(table, key);
        if (node) { node->val = key + "\n" + std::to_string(value); return; }
        slistPushHead(table.buckets[hashIndex(table, key, 0)], key + "\n" + std::to_string(value));
        ++table.n;
    } else {
        for (unsigned int which = 0; which < 2; ++which) {
            ++table.probes;
            HashCell& cell = table.cells[hashIndex(table, key, which)];
            if (cell.used && cell.key == key) { cell.value = value; return; }
        }
        HashCell cell{key, value, true};
        if (place(table, cell)) ++table.n;
        else rebuild(table, table.capacity, &cell); // Цикл вытеснений требует новых хеш-функций.
    }
    if (table.n >= table.capacity - table.capacity / 10) rebuild(table, doubled(table.capacity));
}

bool hashDel(HashTable& table, const std::string& key) {
    if (!table.capacity) throw std::runtime_error("таблица не инициализирована");
    if (table.cuckoo) {
        for (unsigned int which = 0; which < 2; ++which) {
            ++table.probes;
            HashCell& cell = table.cells[hashIndex(table, key, which)];
            if (cell.used && cell.key == key) { cell.used = false; --table.n; return true; }
        }
        return false;
    }
    SList& chain = table.buckets[hashIndex(table, key, 0)];
    SNode* before = nullptr;
    for (SNode* node = chain.head; node; node = node->next) {
        ++table.probes;
        if (node->val.substr(0, node->val.find('\n')) == key) {
            if (before) before->next = node->next; else chain.head = node->next;
            if (chain.tail == node) chain.tail = before;
            delete node; --chain.n; --table.n;
            return true;
        }
        before = node;
    }
    return false;
}
