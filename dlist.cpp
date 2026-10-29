#include "dlist.h"
#include <stdexcept>

using namespace std;

static DNode* dlistFindNode(const DList& list, const string& v) {
    for (DNode* c = list.head; c; c = c->next) if (c->val == v) return c;
    return nullptr;
}

static DNode* dlistMustFind(const DList& list, const string& v) {
    DNode* t = dlistFindNode(list, v);
    if (!t) throw runtime_error("value not found: " + v);
    return t;
}

static string dlistUnlink(DList& list, DNode* x) {  
    string r = x->val;
    if (x->prev) x->prev->next = x->next; else list.head = x->next;
    if (x->next) x->next->prev = x->prev; else list.tail = x->prev;
    delete x;
    --list.n;
    return r;
}

static void dlistInsertAfter(DList& list, DNode* t, const string& v) {
    DNode* x = new DNode{v, t, t->next};
    if (t->next) t->next->prev = x; else list.tail = x;
    t->next = x;
    ++list.n;
}

void dlistPushHead(DList& list, const string& v) {
    DNode* x = new DNode{v, nullptr, list.head};
    if (list.head) list.head->prev = x; else list.tail = x;
    list.head = x;
    ++list.n;
}

void dlistPushTail(DList& list, const string& v) {
    DNode* x = new DNode{v, list.tail, nullptr};
    if (list.tail) list.tail->next = x; else list.head = x;
    list.tail = x;
    ++list.n;
}

void dlistPushAfter(DList& list, const string& target, const string& v) { dlistInsertAfter(list, dlistMustFind(list, target), v); }

void dlistPushBefore(DList& list, const string& target, const string& v) {
    DNode* t = dlistMustFind(list, target);
    if (t == list.head) dlistPushHead(list, v); else dlistInsertAfter(list, t->prev, v);
}

string dlistPopHead(DList& list) { if (!list.head) throw runtime_error("list is empty"); return dlistUnlink(list, list.head); }

string dlistPopTail(DList& list) { if (!list.head) throw runtime_error("list is empty"); return dlistUnlink(list, list.tail); }

string dlistDelAfter(DList& list, const string& target) {
    DNode* t = dlistMustFind(list, target);
    if (!t->next) throw runtime_error("no element after " + target);
    return dlistUnlink(list, t->next);
}

string dlistDelBefore(DList& list, const string& target) {
    DNode* t = dlistMustFind(list, target);
    if (!t->prev) throw runtime_error("no element before " + target);
    return dlistUnlink(list, t->prev);
}

bool dlistDelValue(DList& list, const string& v) {
    DNode* x = dlistFindNode(list, v);
    if (x) dlistUnlink(list, x);
    return x != nullptr;
}

int dlistFind(const DList& list, const string& v) {
    int i = 0;
    for (DNode* c = list.head; c; c = c->next, ++i) if (c->val == v) return i;
    return -1;
}

const string& dlistAt(const DList& list, size_t i) {
    if (i >= list.n) throw runtime_error("index out of range: " + to_string(i));
    DNode* c = list.head;
    while (i--) c = c->next;
    return c->val;
}

void dlistClear(DList& list) { while (list.head) dlistPopHead(list); }
