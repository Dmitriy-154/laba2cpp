#include "queue.h"

void queuePush(Queue& queue, const std::string& value) { slistPushTail(queue.l, value); }
std::string queuePop(Queue& queue) { return slistPopHead(queue.l); }
void queueClear(Queue& queue) { slistClear(queue.l); }
