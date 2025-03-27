#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>


using namespace std;

// 数组实现版本 (适用于小写字母场景)
class TrieArray {
private:
    struct TrieNode {
        int pass; // 表示通过该节点的单词数量
        int end;  // 表示以该节点结尾的单词数量
        TrieNode* next[26]{}; // 子节点数组，支持26个小写字母

        // 构造函数初始化
        TrieNode() : pass(0), end(0) {
        }

        // 析构函数释放内存
        ~TrieNode() {
            for (auto& node : next) {
                delete node; // 递归释放子节点
            }
        }
    };

    TrieNode* root; // 根节点

public:
    TrieArray() : root(new TrieNode()) {} // 构造函数初始化根节点

    ~TrieArray() {
        delete root; // 析构函数释放根节点
    }

    // 插入一个单词到字典树中
    void insert(const string& word) {
        TrieNode* node = root;
        node->pass++; // 根节点的pass计数加1
        for (char c : word) {
            int path = c - 'a'; // 计算字符对应的索引
            if (!node->next[path]) {
                node->next[path] = new TrieNode(); // 如果子节点不存在，则创建
            }
            node = node->next[path];
            node->pass++; // 更新当前节点的pass计数
        }
        node->end++; // 单词插入完成后，更新end计数
    }

    // 删除一个单词
    void erase(string word) {
        if (countWordsEqualTo(word) == 0) return; // 如果单词不存在，则直接返回

        TrieNode* node = root;
        node->pass--; // 根节点的pass计数减1
        for (char c : word) {
            int path = c - 'a';
            if (--node->next[path]->pass == 0) { // 如果子节点的pass计数变为0，则删除该节点
                delete node->next[path];
                node->next[path] = nullptr;
                return; // 提前结束删除操作
            }
            node = node->next[path];
        }
        node->end--; // 更新end计数
    }

    // 统计完全匹配某个单词的数量
    int countWordsEqualTo(string word) {
        TrieNode* node = root;
        for (char c : word) {
            int path = c - 'a';
            if (!node->next[path]) return 0; // 如果路径不存在，则返回0
            node = node->next[path];
        }
        return node->end; // 返回end计数
    }

    // 统计具有某个前缀的单词数量
    int countWordsStartingWith(string prefix) {
        TrieNode* node = root;
        for (char c : prefix) {
            int path = c - 'a';
            if (!node->next[path]) return 0; // 如果路径不存在，则返回0
            node = node->next[path];
        }
        return node->pass; // 返回pass计数
    }
};

// 哈希表实现版本 (支持更广的字符范围)
class TrieHash {
private:
    struct TrieNode {
        int pass; // 表示通过该节点的单词数量
        int end;  // 表示以该节点结尾的单词数量
        unordered_map<char, TrieNode*> next; // 使用哈希表存储子节点，支持任意字符范围

        // 构造函数初始化
        TrieNode() : pass(0), end(0) {}

        // 析构函数释放内存
        ~TrieNode() {
            for (auto& pair : next) {
                delete pair.second; // 递归释放子节点
            }
        }
    };

    TrieNode* root; // 根节点

public:
    TrieHash() : root(new TrieNode()) {} // 构造函数初始化根节点

    ~TrieHash() {
        delete root; // 析构函数释放根节点
    }

    // 插入一个单词到字典树中
    void insert(string word) {
        TrieNode* node = root;
        node->pass++; // 根节点的pass计数加1
        for (char c : word) {
            if (!node->next.count(c)) {
                node->next[c] = new TrieNode(); // 如果子节点不存在，则创建
            }
            node = node->next[c];
            node->pass++; // 更新当前节点的pass计数
        }
        node->end++; // 单词插入完成后，更新end计数
    }

    // 删除一个单词
    void erase(string word) {
        if (countWordsEqualTo(word) == 0) return; // 如果单词不存在，则直接返回

        TrieNode* node = root;
        node->pass--; // 根节点的pass计数减1
        for (char c : word) {
            TrieNode* next = node->next[c];
            if (--next->pass == 0) { // 如果子节点的pass计数变为0，则删除该节点
                delete next;
                node->next.erase(c);
                return; // 提前结束删除操作
            }
            node = next;
        }
        node->end--; // 更新end计数
    }

    // 统计完全匹配某个单词的数量
    int countWordsEqualTo(string word) {
        TrieNode* node = root;
        for (char c : word) {
            if (!node->next.count(c)) return 0; // 如果路径不存在，则返回0
            node = node->next[c];
        }
        return node->end; // 返回end计数
    }

    // 统计具有某个前缀的单词数量
    int countWordsStartingWith(string prefix) {
        TrieNode* node = root;
        for (char c : prefix) {
            if (!node->next.count(c)) return 0; // 如果路径不存在，则返回0
            node = node->next[c];
        }
        return node->pass; // 返回pass计数
    }
};




int main() {
    // 测试 TrieArray
    TrieArray trieArray;

    // 插入单词
    trieArray.insert("hello");
    trieArray.insert("hell");
    trieArray.insert("heaven");
    trieArray.insert("heavy");

    // 查询单词数量
    std::cout << "Number of words equal to 'hello': " << trieArray.countWordsEqualTo("hello") << std::endl;
    std::cout << "Number of words starting with 'he': " << trieArray.countWordsStartingWith("he") << std::endl;

    // 删除单词
    trieArray.erase("hello");
    std::cout << "Number of words equal to 'hello' after deletion: " << trieArray.countWordsEqualTo("hello") << std::endl;
    std::cout << "Number of words starting with 'he' after deletion: " << trieArray.countWordsStartingWith("he") << std::endl;

    // 测试 TrieHash
    TrieHash trieHash;

    // 插入单词
    trieHash.insert("world");
    trieHash.insert("word");
    trieHash.insert("wonderful");
    trieHash.insert("wood");

    // 查询单词数量
    std::cout << "Number of words equal to 'world': " << trieHash.countWordsEqualTo("world") << std::endl;
    std::cout << "Number of words starting with 'wo': " << trieHash.countWordsStartingWith("wo") << std::endl;

    // 删除单词
    trieHash.erase("world");
    std::cout << "Number of words equal to 'world' after deletion: " << trieHash.countWordsEqualTo("world") << std::endl;
    std::cout << "Number of words starting with 'wo' after deletion: " << trieHash.countWordsStartingWith("wo") << std::endl;

    return 0;
}