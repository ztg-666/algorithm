#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <algorithm>

using namespace std;
class Solution {
public:
    unordered_set<string> dict; // 存储字典中的单词
    unordered_set<string> curLevel; // 当前层的单词集合（BFS使用）
    unordered_set<string> nextLevel; // 下一层的单词集合（BFS使用）
    unordered_map<string, vector<string>> graph; // 构建图，存储每个单词的前驱单词
    vector<string> path; // 存储当前路径
    vector<vector<string>> ans; // 存储所有结果路径

    // 主函数：寻找从 beginWord 到 endWord 的所有最短转换序列
    vector<vector<string>> findLadders(string beginWord, string endWord, vector<string>& wordList) {
        dict = unordered_set<string>(wordList.begin(), wordList.end()); // 初始化字典
        if (!dict.count(endWord)) return {}; // 如果字典中不存在 endWord，直接返回空结果

        graph.clear(); // 清空图
        ans.clear(); // 清空结果集
        curLevel.clear(); // 清空当前层
        nextLevel.clear(); // 清空下一层

        // 使用 BFS 构建图，如果找到路径则进行 DFS 回溯
        if (bfs(beginWord, endWord)) {
            path.clear(); // 清空路径
            dfs(endWord, beginWord); // 从 endWord 开始回溯到 beginWord
        }
        return ans; // 返回所有结果路径
    }

    // 广度优先搜索（BFS）构建图
    bool bfs(string begin, string end) {
        bool find = false; // 标记是否找到目标单词
        curLevel.insert(begin); // 将起始单词加入当前层

        while (!curLevel.empty()) {
            // 从字典中移除当前层的所有单词，避免重复访问
            for (auto& word : curLevel) {
                dict.erase(word);
            }

            // 遍历当前层的每个单词，尝试生成下一个可能的单词
            for (auto& word : curLevel) {
                string w = word;
                for (int i = 0; i < w.size(); i++) { // 遍历单词的每个字符位置
                    char old = w[i]; // 保存原始字符
                    for (char c = 'a'; c <= 'z'; c++) { // 尝试替换为其他字母
                        w[i] = c;
                        if (dict.count(w) && w != word) { // 如果新单词在字典中且不是原单词
                            if (w == end) find = true; // 如果找到目标单词，标记为已找到
                            graph[w].push_back(word); // 在图中记录前驱关系
                            nextLevel.insert(w); // 将新单词加入下一层
                        }
                    }
                    w[i] = old; // 恢复原始字符
                }
            }

            if (find) return true; // 如果找到目标单词，返回 true

            swap(curLevel, nextLevel); // 将下一层作为当前层继续搜索
            nextLevel.clear(); // 清空下一层
        }
        return false; // 如果无法到达目标单词，返回 false
    }

    // 深度优先搜索（DFS）回溯路径
    void dfs(string word, string& aim) {
        path.insert(path.begin(), word); // 将当前单词插入路径的开头

        if (word == aim) { // 如果当前单词是起始单词，将路径加入结果集
            ans.push_back(path);
        } else if (graph.count(word)) { // 如果当前单词有前驱节点
            for (auto& next : graph[word]) { // 遍历所有前驱节点
                dfs(next, aim); // 递归回溯
            }
        }

        path.erase(path.begin()); // 移除当前单词，回溯到上一层
    }
};

// 单词接龙 II
// 按字典 wordList 完成从单词 beginWord 到单词 endWord 转化
// 一个表示此过程的 转换序列 是形式上像 
// beginWord -> s1 -> s2 -> ... -> sk 这样的单词序列，并满足：
// 每对相邻的单词之间仅有单个字母不同
// 转换过程中的每个单词 si（1 <= i <= k）必须是字典 wordList 中的单词
// 注意，beginWord 不必是字典 wordList 中的单词
// sk == endWord
// 给你两个单词 beginWord 和 endWord ，以及一个字典 wordList
// 请你找出并返回所有从 beginWord 到 endWord 的 最短转换序列
// 如果不存在这样的转换序列，返回一个空列表
// 每个序列都应该以单词列表 [beginWord, s1, s2, ..., sk] 的形式返回
// 测试链接 : https://leetcode.cn/problems/word-ladder-ii/
#include<iostream>
int main() {
    // 示例输入
    string beginWord = "hit";
    string endWord = "cog";
    vector<string> wordList = {"hot", "dot", "dog", "lot", "log", "cog"};

    // 创建 Solution 对象
    Solution solution;

    // 调用 findLadders 方法
    vector<vector<string>> result = solution.findLadders(beginWord, endWord, wordList);

    // 输出结果
    cout << "shortest：" << endl;
    for (const auto& path : result) {
        for (const auto& word : path) {
            cout << word << " -> ";
        }
        cout << "\b\b\b   " << endl; // 删除最后一个箭头并换行
    }

    return 0;
}