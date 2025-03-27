#include <vector>
#include <string>
#include <cstring>
#include <iostream>
using namespace std;

const int MAXN = 10001; // 定义字典树的最大节点数

class Solution {
public:
    vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
        // 构建字典树
        build(words);
        vector<string> ans; // 存储结果的向量
        // 遍历棋盘的每一个位置，尝试从该位置开始搜索单词
        for (int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board[0].size(); j++) {
                dfs(board, i, j, 1, ans);
            }
        }
        // 清理字典树
        clear();
        return ans;
    }

private:
    int tree[MAXN][26]; // 字典树的存储结构
    int pass[MAXN]; // 记录每个节点经过的次数
    string end[MAXN]; // 记录每个单词的结束节点
    int cnt; // 当前字典树中的节点数

    // 构建字典树
    void build(vector<string>& words) {
        cnt = 1; // 初始化节点计数器
        memset(tree, 0, sizeof(tree)); // 初始化字典树
        memset(pass, 0, sizeof(pass)); // 初始化经过次数数组
        for (int i = 0; i < MAXN; i++) {
            end[i].clear(); // 初始化结束单词数组
        }
        // 遍历所有单词，插入字典树
        for (string& word : words) {
            int cur = 1; // 从根节点开始
            pass[cur]++; // 根节点经过次数加一
            for (char c : word) {
                int path = c - 'a'; // 计算字符对应的路径
                if (tree[cur][path] == 0) {
                    tree[cur][path] = ++cnt; // 创建新节点
                }
                cur = tree[cur][path]; // 移动到子节点
                pass[cur]++; // 子节点经过次数加一
            }
            end[cur] = word; // 在结束节点记录单词
        }
    }

    // 深度优先搜索
    int dfs(vector<vector<char>>& board, int i, int j, int t, vector<string>& ans) {
        // 检查边界条件和是否访问过
        if (i < 0 || i >= board.size() || j < 0 || j >= board[0].size() || board[i][j] == 0) {
            return 0;
        }
        char tmp = board[i][j]; // 保存当前字符
        int road = tmp - 'a'; // 计算字符对应的路径
        t = tree[t][road]; // 移动到子节点
        if (pass[t] == 0) {
            return 0; // 如果路径不存在，返回
        }
        int fix = 0; // 记录找到的单词数
        if (!end[t].empty()) {
            fix++; // 找到一个单词
            ans.push_back(end[t]); // 将单词加入结果
            end[t].clear(); // 清空结束节点的单词
        }
        board[i][j] = 0; // 标记当前位置为已访问
        // 递归搜索四个方向
        fix += dfs(board, i - 1, j, t, ans);
        fix += dfs(board, i + 1, j, t, ans);
        fix += dfs(board, i, j - 1, t, ans);
        fix += dfs(board, i, j + 1, t, ans);
        pass[t] -= fix; // 更新经过次数
        board[i][j] = tmp; // 恢复原字符
        return fix; // 返回找到的单词数
    }

    // 清理字典树
    void clear() {
        for (int i = 1; i <= cnt; i++) {
            memset(tree[i], 0, sizeof(tree[i])); // 清空字典树节点
            pass[i] = 0; // 清空经过次数
            end[i].clear(); // 清空结束单词
        }
    }
};
int main() {
    // 创建一个 Solution 对象
    Solution solution;

    // 定义棋盘
    vector<vector<char>> board = {
        {'o', 'a', 'a', 'n'},
        {'e', 't', 'a', 'e'},
        {'i', 'h', 'k', 'r'},
        {'i', 'f', 'l', 'v'}
    };

    // 定义要查找的单词列表
    vector<string> words = {"oath", "pea", "eat", "rain"};

    // 调用 findWords 方法查找单词
    vector<string> result = solution.findWords(board, words);

    // 输出结果
    cout << "Found words: ";
    for (const string& word : result) {
        cout << word << " ";
    }
    cout << endl;

    return 0;
}