#include <bits/stdc++.h>
using namespace std;
// 定义字典树的节点类
class TrieNode {
public:
    int path; // 经过该节点的单词数量
    int end;  // 以该节点为结尾的单词数量
    std::vector<TrieNode*> nexts; // 子节点指针数组，大小为26，对应a-z

    // 构造函数，初始化path和end为0，nexts数组初始化为nullptr
    TrieNode() : path(0), end(0), nexts(26, nullptr) {}
};

// 定义字典树类
class Trie {
private:
    TrieNode* root; // 根节点

public:
    // 构造函数，初始化根节点
    Trie() : root(new TrieNode()) {}

    // 析构函数，释放所有节点内存
    ~Trie() {
        clear(root);
    }

    // 辅助函数，递归删除节点及其子节点
    void clear(TrieNode* node) {
        if (node == nullptr) return;
        for (TrieNode* next : node->nexts) {
            clear(next);
        }
        delete node;
    }

    // 插入一个单词到字典树
    void insert(const std::string& word) {
        if (word.empty()) {
            return;
        }
        TrieNode* node = root;
        int index = 0;
        for (char ch : word) {
            index = ch - 'a'; // 计算字符对应的索引
            if (node->nexts[index] == nullptr) {
                node->nexts[index] = new TrieNode(); // 如果子节点不存在，创建新节点
            }
            node = node->nexts[index];
            node->path++; // 更新经过该节点的单词数量
        }
        node->end++; // 更新以该节点为结尾的单词数量
    }

    // 从字典树中删除一个单词
    void deleteWord(const std::string& word) {
        // 首先检查单词是否存在，如果不存在则不需要删除
        if (search(word) != 0) {
            // 用于存储从根节点到目标单词路径上的所有节点
            std::vector<TrieNode*> nodes;
            TrieNode* node = root; // 从根节点开始
            int index = 0;
            // 遍历单词中的每个字符，找到对应的路径
            for (char ch : word) {
                index = ch - 'a'; // 计算字符对应的索引
                nodes.push_back(node); // 将当前节点加入路径列表
                node = node->nexts[index]; // 移动到下一个节点
            }
            // 更新以该节点为结尾的单词数量
            node->end--;
            // 从最后一个字符开始回溯，删除路径上不再需要的节点
            for (int i = word.size() - 1; i >= 0; i--) {
                index = word[i] - 'a'; // 计算字符对应的索引
                node = nodes[i]; // 回到上一个节点
                // 如果经过该节点的单词数量为0，删除该节点
                if (--node->nexts[index]->path == 0) {
                    delete node->nexts[index]; // 释放内存
                    node->nexts[index] = nullptr; // 将指针置为空
                    return; // 删除完成，提前退出
                }
            }
        }
    }


    // 搜索一个单词是否存在于字典树中
    int search(const std::string& word) {
        if (word.empty()) {
            return 0;
        }
        TrieNode* node = root;
        int index = 0;
        for (char ch : word) {
            index = ch - 'a';
            if (node->nexts[index] == nullptr) {
                return 0; // 如果子节点不存在，单词不存在
            }
            node = node->nexts[index];
        }
        return node->end; // 返回以该节点为结尾的单词数量
    }

    // 计算以某个前缀开头的单词数量
    int prefixNumber(const std::string& pre) {
        if (pre.empty()) {
            return 0;
        }
        TrieNode* node = root;
        int index = 0;
        for (char ch : pre) {
            index = ch - 'a';
            if (node->nexts[index] == nullptr) {
                return 0; // 如果子节点不存在，前缀不存在
            }
            node = node->nexts[index];
        }
        return node->path; // 返回经过该节点的单词数量
    }
};

