#include "set.h"

static std::size_t bucketOf(int value, std::size_t capacity) {
    long long index = static_cast<long long>(value) % static_cast<long long>(capacity);
    return static_cast<std::size_t>(index < 0 ? index + static_cast<long long>(capacity) : index);
}

static void rehash(Set& set) {
    std::vector<SList> fresh(set.buckets.size() * 2 + 1);
    for (std::size_t i = 0; i < set.buckets.size(); ++i) {
        SNode* node = set.buckets[i].head;
        while (node) {
            SNode* next = node->next;
            SList& chain = fresh[bucketOf(std::stoi(node->val), fresh.size())];
            node->next = chain.head;
            chain.head = node;
            if (!chain.tail) chain.tail = node;
            ++chain.n;
            node = next;
        }
    }
    set.buckets.swap(fresh);
}

bool setAt(const Set& set, int value) {
    return slistFind(set.buckets[bucketOf(value, set.buckets.size())], std::to_string(value)) >= 0;
}

bool setAdd(Set& set, int value) {
    if (setAt(set, value)) return false;
    if (set.n + 1 > set.buckets.size()) rehash(set);
    slistPushHead(set.buckets[bucketOf(value, set.buckets.size())], std::to_string(value));
    ++set.n;
    return true;
}

bool setDel(Set& set, int value) {
    if (!slistDelValue(set.buckets[bucketOf(value, set.buckets.size())], std::to_string(value))) return false;
    --set.n;
    return true;
}

void setValues(const Set& set, Array& values) {
    arrayClear(values);
    for (std::size_t i = 0; i < set.buckets.size(); ++i)
        for (SNode* node = set.buckets[i].head; node; node = node->next) arrayPush(values, node->val);
}

void setClear(Set& set) {
    for (std::size_t i = 0; i < set.buckets.size(); ++i) slistClear(set.buckets[i]);
    set.n = 0;
}
