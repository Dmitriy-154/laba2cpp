#include "slist.h"
#include <stdexcept>

using namespace std;

static SNode* slistFindNode(const SList& list, const string& v) {
    for (SNode* c = list.head; c; c = c->next) if (c->val == v) return c;
    return nullptr;
}

static SNode* slistMustFind(const SList& list, const string& v) {
    SNode* t = slistFindNode(list, v);
    if (!t) throw runtime_error("value not found: " + v);
    return t;
}

void slistPushHead(SList& list, const string& v) {
    list.head = new SNode{v, list.head};
    if (!list.tail) list.tail = list.head;
    ++list.n;
}

void slistPushTail(SList& list, const string& v) {
    SNode* x = new SNode{v, nullptr};
    if (list.tail) list.tail->next = x; else list.head = x;
    list.tail = x;
    ++list.n;
}

void slistPushAfter(SList& list, const string& target, const string& v) {
    SNode* t = slistMustFind(list, target);
    t->next = new SNode{v, t->next};
    if (t == list.tail) list.tail = t->next;
    ++list.n;
}

void slistPushBefore(SList& list, const string& target, const string& v) {
    slistMustFind(list, target);
    if (list.head->val == target) return slistPushHead(list, v);
    SNode* p = list.head;
    while (p->next->val != target) p = p->next;  
    p->next = new SNode{v, p->next};
    ++list.n;
}

string slistPopHead(SList& list) {
    if (!list.head) throw runtime_error("list is empty");
    SNode* t = list.head;
    string r = t->val;
    list.head = t->next;
    if (!list.head) list.tail = nullptr;
    delete t;
    --list.n;
    return r;
}

string slistPopTail(SList& list) {  
    if (!list.head) throw runtime_error("list is empty");
    if (list.head == list.tail) return slistPopHead(list);
    SNode* p = list.head;
    while (p->next != list.tail) p = p->next;
    string r = list.tail->val;
    delete list.tail;
    list.tail = p;
    p->next = nullptr;
    --list.n;
    return r;
}

string slistDelAfter(SList& list, const string& target) {
    SNode* t = slistMustFind(list, target);
    if (!t->next) throw runtime_error("no element after " + target);
    SNode* x = t->next;
    string r = x->val;
    t->next = x->next;
    if (x == list.tail) list.tail = t;
    delete x;
    --list.n;
    return r;
}

string slistDelBefore(SList& list, const string& target) {
    SNode* t = slistMustFind(list, target);
    if (t == list.head) throw runtime_error("no element before " + target);
    if (list.head->next == t) return slistPopHead(list);
    SNode* p = list.head;
    while (p->next->next != t) p = p->next;  
    SNode* x = p->next;
    string r = x->val;
    p->next = t;
    delete x;
    --list.n;
    return r;
}

bool slistDelValue(SList& list, const string& v) {
    if (!slistFindNode(list, v)) return false;
    if (list.head->val == v) { slistPopHead(list); return true; }
    SNode* p = list.head;
    while (p->next->val != v) p = p->next;
    SNode* x = p->next;
    p->next = x->next;
    if (x == list.tail) list.tail = p;
    delete x;
    --list.n;
    return true;
}

int slistFind(const SList& list, const string& v) {
    int i = 0;
    for (SNode* c = list.head; c; c = c->next, ++i) if (c->val == v) return i;
    return -1;
}

const string& slistAt(const SList& list, size_t i) {
    if (i >= list.n) throw runtime_error("index out of range: " + to_string(i));
    SNode* c = list.head;
    while (i--) c = c->next;
    return c->val;
}

void slistClear(SList& list) { while (list.head) slistPopHead(list); }
