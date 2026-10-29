#pragma once
#include "slist.h"
#include "massiv.h"
#include <vector>

struct Set {
    std::vector<SList> buckets = std::vector<SList>(17);
    std::size_t n = 0;
};

bool setAdd(Set& set, int value);
bool setDel(Set& set, int value);
bool setAt(const Set& set, int value);
void setValues(const Set& set, Array& values);
void setClear(Set& set);
