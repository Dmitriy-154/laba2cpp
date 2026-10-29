#pragma once
#include <string>
#include <cstddef>

struct Array {
    std::string* data = nullptr;
    std::size_t len = 0, cap = 0;
};

void arrayPush(Array& array, const std::string& value);
void arrayInsert(Array& array, std::size_t index, const std::string& value);
const std::string& arrayGet(const Array& array, std::size_t index);
void arraySet(Array& array, std::size_t index, const std::string& value);
std::string arrayErase(Array& array, std::size_t index);
void arrayClear(Array& array);
