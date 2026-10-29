#include "stack.h"

void stackPush(Stack& stack, const std::string& value) { slistPushHead(stack.l, value); }
std::string stackPop(Stack& stack) { return slistPopHead(stack.l); }
const std::string& stackGet(const Stack& stack) { return slistAt(stack.l, 0); }
void stackClear(Stack& stack) { slistClear(stack.l); }
