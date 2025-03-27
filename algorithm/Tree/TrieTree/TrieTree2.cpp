#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
using namespace std;

// 定义最大节点数
const int MAXN = 150001;

// 定义字典树（Trie）类
class Trie {
private:
    // tree[i][j] 表示节点i的第j个子节点
    int tree[MAXN][26] = {0};
    // end[i] 表示以节点i结尾的单词数量
    int end[MAXN] = {0};
    // pass[i] 表示经过节点i的单词数量
    int pass[MAXN] = {0};
    // 当前使用的节点数
    int cnt = 0;

public:
    // 初始化字典树
    void build() {
        cnt = 1; // 根节点编号为1
        memset(tree, 0, sizeof(tree)); // 清空树结构
        memset(end, 0, sizeof(end)); // 清空结束标记
        memset(pass, 0, sizeof(pass)); // 清空经过次数
    }

    // 插入一个单词到字典树
    void insert(const string& word) {
        int cur = 1; // 从根节点开始
        pass[cur]++; // 经过根节点的单词数加1
        for (char ch : word) {
            int path = ch - 'a'; // 计算字符对应的路径
            if (tree[cur][path] == 0) {
                tree[cur][path] = ++cnt; // 如果路径不存在，创建新节点
            }
            cur = tree[cur][path]; // 移动到下一个节点
            pass[cur]++; // 经过当前节点的单词数加1
        }
        end[cur]++; // 单词结束，结束标记加1
    }

    // 搜索一个单词是否存在于字典树
    int search(const string& word) {
        int cur = 1; // 从根节点开始
        for (char ch : word) {
            int path = ch - 'a'; // 计算字符对应的路径
            if (tree[cur][path] == 0) {
                return 0; // 路径不存在，单词不存在
            }
            cur = tree[cur][path]; // 移动到下一个节点
        }
        return end[cur]; // 返回以当前节点结尾的单词数量
    }

    // 计算以某个前缀开头的单词数量
    int prefixNumber(const string& pre)  {
        int cur = 1; // 从根节点开始
        for (char ch : pre) {
            int path = ch - 'a'; // 计算字符对应的路径
            if (tree[cur][path] == 0) {
                return 0; // 路径不存在，前缀不存在
            }
            cur = tree[cur][path]; // 移动到下一个节点
        }
        return pass[cur]; // 返回经过当前节点的单词数量
    }

    // 从字典树中删除一个单词
    void deleteWord(const string& word) {
        if (search(word) > 0) { // 确保单词存在
            int cur = 1; // 从根节点开始
            pass[cur]--; // 经过根节点的单词数减1
            for (char ch : word) {
                int path = ch - 'a'; // 计算字符对应的路径
                int next = tree[cur][path];
                if (--pass[next] == 0) {
                    tree[cur][path] = 0; // 如果经过该节点的单词数为0，删除该节点
                    return;
                }
                cur = next; // 移动到下一个节点
            }
            end[cur]--; // 单词结束，结束标记减1
        }
    }

    // 清空字典树
    void clear() {
        for (int i = 1; i <= cnt; i++) {
            memset(tree[i], 0, sizeof(tree[i])); // 清空每个节点的子节点
            end[i] = 0; // 清空结束标记
            pass[i] = 0; // 清空经过次数
        }
        cnt = 0; // 重置节点计数
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    Trie trie; // 创建字典树对象
    string line;
    while (getline(cin, line)) {
        trie.build(); // 初始化字典树
        int m = stoi(line); // 读取操作数量
        while (m--) {
            getline(cin, line);
            istringstream iss(line);
            int op;
            string s;
            iss >> op >> s; // 读取操作类型和单词
            if (op == 1) {
                trie.insert(s); // 插入单词
            } else if (op == 2) {
                trie.deleteWord(s); // 删除单词
            } else if (op == 3) {
                cout << (trie.search(s) > 0 ? "YES" : "NO") << endl; // 搜索单词
            } else if (op == 4) {
                cout << trie.prefixNumber(s) << endl; // 计算前缀数量
            }
        }
        trie.clear(); // 清空字典树
    }
    return 0;
}
