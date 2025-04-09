#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <queue>

using namespace std;

class Solution {
public:
    static const int MAXN = 401; // 最大队列大小
    vector<vector<string>> graph; // 图结构存储贴纸信息，按字母分类
    unordered_set<string> visited; // 访问过的字符串集合，避免重复计算

    // 构造函数：初始化图结构
    Solution() : graph(26, vector<string>()) {}

    /**
     * 主函数：计算拼出目标字符串所需的最小贴纸数量
     * @param stickers 贴纸数组，每个贴纸是一个字符串
     * @param target 目标字符串
     * @return 最小贴纸数量，如果无法拼出则返回-1
     */
    int minStickers(vector<string>& stickers, string target) {
        // 清空图结构和访问集合
        for (int i = 0; i < 26; i++) {
            graph[i].clear();
        }
        visited.clear();

        // 初始化图结构
        for (string& str : stickers) {
            string sortedStr = sortString(str); // 对贴纸字符串排序
            for (int i = 0; i < sortedStr.size(); i++) {
                // 避免重复添加相同的字符对应的贴纸
                if (i == 0 || sortedStr[i] != sortedStr[i - 1]) {
                    graph[sortedStr[i] - 'a'].push_back(sortedStr);
                }
            }
        }

        target = sortString(target); // 对目标字符串排序
        queue<string> q; // 使用队列进行宽度优先搜索（BFS）
        q.push(target);
        visited.insert(target);

        int level = 1; // 当前层数，表示使用的贴纸数量
        while (!q.empty()) {
            int size = q.size(); // 当前层的节点数量
            for (int i = 0; i < size; i++) {
                string cur = q.front(); // 取出当前字符串
                q.pop();
                // 遍历当前字符串第一个字符对应的贴纸列表
                for (string& s : graph[cur[0] - 'a']) {
                    string nextStr = getNext(cur, s); // 计算下一步的状态字符串
                    if (nextStr.empty()) {
                        return level; // 如果下一个状态为空，说明已经拼出目标字符串
                    } else if (visited.find(nextStr) == visited.end()) {
                        // 如果下一个状态未被访问过，则加入队列
                        visited.insert(nextStr);
                        q.push(nextStr);
                    }
                }
            }
            level++; // 层数加1，表示使用了更多的贴纸
        }
        return -1; // 如果无法拼出目标字符串，返回-1
    }

private:
    /**
     * 对字符串进行排序
     * @param str 输入字符串
     * @return 排序后的字符串
     */
    string sortString(string str) {
        sort(str.begin(), str.end()); // 使用标准库sort对字符串排序
        return str;
    }

    /**
     * 计算下一个状态字符串
     * @param t 当前目标字符串
     * @param s 当前贴纸字符串
     * @return 下一个状态字符串
     */
    string getNext(const string& t, const string& s) {
        string result; // 存储结果字符串
        int i = 0, j = 0; // 分别指向目标字符串t和贴纸字符串s的指针
        while (i < t.size()) {
            if (j == s.size()) {
                // 如果贴纸字符串已经用完，直接将剩余的目标字符加入结果
                result += t[i++];
            } else {
                if (t[i] < s[j]) {
                    // 如果目标字符小于贴纸字符，直接加入结果
                    result += t[i++];
                } else if (t[i] > s[j]) {
                    // 如果目标字符大于贴纸字符，移动贴纸指针
                    j++;
                } else {
                    // 如果目标字符等于贴纸字符，跳过该字符
                    i++;
                    j++;
                }
            }
        }
        return result; // 返回下一个状态字符串
    }
};

// 贴纸拼词
// 我们有 n 种不同的贴纸。每个贴纸上都有一个小写的英文单词。
// 您想要拼写出给定的字符串 target ，方法是从收集的贴纸中切割单个字母并重新排列它们
// 如果你愿意，你可以多次使用每个贴纸，每个贴纸的数量是无限的。
// 返回你需要拼出 target 的最小贴纸数量。如果任务不可能，则返回 -1
// 注意：在所有的测试用例中，所有的单词都是从 1000 个最常见的美国英语单词中随机选择的
// 并且 target 被选择为两个随机单词的连接。
// 测试链接 : https://leetcode.cn/problems/stickers-to-spell-word/
// 测试代码
int main() {
    Solution solution;
    vector<string> stickers = {"with", "example", "science"}; // 贴纸数组
    string target = "thehat"; // 目标字符串
    cout << "Minimum number of stickers: " << solution.minStickers(stickers, target) << endl;
    return 0;
}
