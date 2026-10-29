#pragma once
#include <string>
#include <cstddef>

struct SNode { std::string val; SNode* next; };

struct SList {
    SNode *head = nullptr, *tail = nullptr;
    std::size_t n = 0;
};

void slistPushHead(SList& list, const std::string& v);
void slistPushTail(SList& list, const std::string& v);
void slistPushAfter(SList& list, const std::string& target, const std::string& v);
void slistPushBefore(SList& list, const std::string& target, const std::string& v);
std::string slistPopHead(SList& list);
std::string slistPopTail(SList& list);
std::string slistDelAfter(SList& list, const std::string& target);
std::string slistDelBefore(SList& list, const std::string& target);
bool slistDelValue(SList& list, const std::string& v);
int slistFind(const SList& list, const std::string& v);
const std::string& slistAt(const SList& list, std::size_t i);
void slistClear(SList& list);
