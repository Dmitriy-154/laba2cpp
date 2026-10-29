#pragma once
#include "slist.h"

struct Stack { SList l; };

void stackPush(Stack& stack, const std::string& value);
std::string stackPop(Stack& stack);
const std::string& stackGet(const Stack& stack);
void stackClear(Stack& stack);
