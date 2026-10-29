#pragma once
#include "slist.h"
#include <vector>

struct HashCell {
    std::string key;
    int value = 0;
    bool used = false;
};

struct HashTable {
    std::vector<SList> buckets;
    std::vector<HashCell> cells;
    std::size_t capacity = 0, n = 0;
    bool cuckoo = false;
    unsigned int seed = 1;
    unsigned long long probes = 0, rebuilds = 0;
};

void hashInit(HashTable& table, std::size_t capacity, bool cuckoo);
void hashClear(HashTable& table);
void hashSet(HashTable& table, const std::string& key, int value);
bool hashGet(HashTable& table, const std::string& key, int& value);
bool hashDel(HashTable& table, const std::string& key);
