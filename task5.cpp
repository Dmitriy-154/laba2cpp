#include "tree.h"
#include "queue.h"
#include "stack.h"
#include <iostream>
#include <stdexcept>

static void zigzag(const CBTree& tree) {
    std::vector<TNode*> nodes = treeNodes(tree);
    Queue queue;
    Stack reversed;
    try {
        if (!nodes.empty()) queuePush(queue, "0");
        bool reverse = false, first = true;
        while (queue.l.n) {
            std::size_t width = queue.l.n;
            for (std::size_t i = 0; i < width; ++i) {
                std::size_t index = static_cast<std::size_t>(std::stoi(queuePop(queue)));
                if (reverse) stackPush(reversed, nodes[index]->key);
                else { std::cout << (first ? "" : " ") << nodes[index]->key; first = false; }
                if (nodes[index]->left) queuePush(queue, std::to_string(index * 2 + 1));
                if (nodes[index]->right) queuePush(queue, std::to_string(index * 2 + 2));
            }
            while (reversed.l.n) { std::cout << (first ? "" : " ") << stackPop(reversed); first = false; }
            reverse = !reverse;
        }
        std::cout << '\n';
    } catch (...) { queueClear(queue); stackClear(reversed); throw; }
}

int main() {
    CBTree tree;
    int status = 0;
    try {
        std::cout << "Введите количество узлов, затем значения по уровням:\n";
        std::string countToken; std::size_t countEnd;
        if (!(std::cin >> countToken)) throw std::runtime_error("нет количества");
        int n = std::stoi(countToken, &countEnd);
        if (countEnd != countToken.size() || n < 0) throw std::runtime_error("неверное количество");
        for (int i = 0; i < n; ++i) {
            std::string token; std::size_t end;
            if (!(std::cin >> token)) throw std::runtime_error("не хватает узлов");
            int value = std::stoi(token, &end);
            if (end != token.size()) throw std::runtime_error("нужны целые числа");
            treeInsert(tree, std::to_string(value));
        }
        zigzag(tree);
    } catch (const std::exception& error) { std::cerr << "Ошибка: " << error.what() << '\n'; status = 1; }
    treeClear(tree);
    return status;
}
