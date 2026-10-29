#pragma once
#include <string>
#include <vector>

struct TNode { std::string key; TNode *left, *right; };
struct CBTree { TNode* root = nullptr; };

std::vector<TNode*> treeNodes(const CBTree& tree);
void treeInsert(CBTree& tree, const std::string& key);
bool treeContains(const CBTree& tree, const std::string& key);
void treeClear(CBTree& tree);
