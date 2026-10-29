#pragma once
#include <string>
#include <cstddef>

struct DNode { std::string val; DNode *prev, *next; };

struct DList {
    DNode *head = nullptr, *tail = nullptr;
    std::size_t n = 0;
};

void dlistPushHead(DList& list, const std::string& v);
void dlistPushTail(DList& list, const std::string& v);
void dlistPushAfter(DList& list, const std::string& target, const std::string& v);
void dlistPushBefore(DList& list, const std::string& target, const std::string& v);
std::string dlistPopHead(DList& list);
std::string dlistPopTail(DList& list);
std::string dlistDelAfter(DList& list, const std::string& target);
std::string dlistDelBefore(DList& list, const std::string& target);
bool dlistDelValue(DList& list, const std::string& v);
int dlistFind(const DList& list, const std::string& v);
const std::string& dlistAt(const DList& list, std::size_t i);
void dlistClear(DList& list);
