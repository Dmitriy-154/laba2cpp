#pragma once
#include "slist.h"

struct Queue { SList l; };

void queuePush(Queue& queue, const std::string& value);
std::string queuePop(Queue& queue);
void queueClear(Queue& queue);
