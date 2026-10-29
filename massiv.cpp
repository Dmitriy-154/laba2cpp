#include "massiv.h"
#include <stdexcept>

static void check(const Array& array, std::size_t index) {
    if (index >= array.len) throw std::runtime_error("индекс вне диапазона");
}

void arrayInsert(Array& array, std::size_t index, const std::string& v) {
    if (index > array.len) throw std::runtime_error("индекс вне диапазона");
    std::string value = v;
    if (array.len == array.cap) {
        std::size_t capacity = array.cap ? array.cap * 2 : 4;
        std::string* data = new std::string[capacity];
        try {
            for (std::size_t i = 0; i < array.len; ++i) data[i] = array.data[i];
        } catch (...) { delete[] data; throw; }
        delete[] array.data;
        array.data = data;
        array.cap = capacity;
    }
    for (std::size_t i = array.len; i > index; --i) array.data[i] = array.data[i - 1];
    array.data[index] = value;
    ++array.len;
}

void arrayPush(Array& array, const std::string& value) { arrayInsert(array, array.len, value); }
const std::string& arrayGet(const Array& array, std::size_t index) { check(array, index); return array.data[index]; }
void arraySet(Array& array, std::size_t index, const std::string& value) { check(array, index); array.data[index] = value; }

std::string arrayErase(Array& array, std::size_t index) {
    check(array, index);
    std::string value = array.data[index];
    for (std::size_t i = index; i + 1 < array.len; ++i) array.data[i] = array.data[i + 1];
    --array.len;
    return value;
}

void arrayClear(Array& array) {
    delete[] array.data;
    array.data = nullptr;
    array.len = array.cap = 0;
}
