#include <vector>
#include <string>
#include <cstring>
#include <iostream>
using namespace std;

// 火星词典
// 现有一种使用英语字母的火星语言
// 这门语言的字母顺序对你来说是未知的。
// 给你一个来自这种外星语言字典的字符串列表 words
// words 中的字符串已经 按这门新语言的字母顺序进行了排序 。
// 如果这种说法是错误的，并且给出的 words 不能对应任何字母的顺序，则返回 ""
// 否则，返回一个按新语言规则的 字典递增顺序 排序的独特字符串
// 如果有多个解决方案，则返回其中任意一个
// words中的单词一定都是小写英文字母组成的
// 测试链接(不需要会员) : https://leetcode.cn/problems/Jf1JuT/
string alienOrder(vector<string>& words) {
    // 入度表：记录每个字符的入度，26个字母对应索引0~25
    int indegree[26]{};
    memset(indegree, -1, sizeof(indegree)); // 初始化为-1，表示该字符未出现

    // 标记存在的字符，将其入度初始化为0
    for (string& w : words) { // 遍历所有单词
        for (char c : w) {    // 遍历单词中的每个字符
            indegree[c - 'a'] = 0; // 存在的字符入度设为0
        }
    }

    // 构建邻接表（图结构）
    vector<vector<int>> graph(26); // 图中每个节点对应一个字母
    for (int i = 0; i < words.size() - 1; ++i) {
        string& cur = words[i];     // 当前单词
        string& next = words[i + 1]; // 下一个单词
        int min_len = min(cur.size(), next.size()); // 取两个单词的最小长度
        int j = 0;
        // 比较两个单词的字符，找到第一个不同的字符
        for (; j < min_len; ++j) {
            if (cur[j] != next[j]) {
                // 如果发现不同字符，建立一条边：cur[j] -> next[j]
                graph[cur[j] - 'a'].push_back(next[j] - 'a');
                indegree[next[j] - 'a']++; // 增加目标字符的入度
                break; // 找到第一个不同字符后停止比较
            }
        }
        // 特殊情况处理：如果前缀相同但当前单词更长，则输入非法
        if (j == min_len && cur.size() > next.size()) {
            return ""; // 返回空字符串表示无解
        }
    }

    // 准备拓扑排序
    int queue[26], l = 0, r = 0; // 数组模拟队列，l为队头，r为队尾
    int kinds = 0;               // 记录存在的字符种类数
    for (int i = 0; i < 26; ++i) {
        if (indegree[i] != -1) kinds++; // 统计存在的字符种类数
        if (indegree[i] == 0) queue[r++] = i; // 将入度为0的字符加入队列
    }

    // 开始拓扑排序
    string ans; // 存储结果字符串
    while (l < r) { // 当队列不为空时
        int cur = queue[l++]; // 取出队首字符
        ans += (char)(cur + 'a'); // 将字符添加到结果字符串
        // 遍历当前字符的所有邻居节点
        for (int next : graph[cur]) {
            if (--indegree[next] == 0) { // 如果邻居节点的入度变为0
                queue[r++] = next;       // 将其加入队列
            }
        }
    }

    // 判断是否成功完成拓扑排序
    // 如果结果字符串的长度等于存在的字符种类数，则返回结果
    return ans.size() == kinds ? ans : "";
}


int main() {
    // 测试用例1
    vector<string> words1 = {"wrt", "wrf", "er", "ett", "rftt"};
    string result1 = alienOrder(words1);
    cout << "Test Case 1: " << (result1.empty() ? "No valid order" : result1) << endl;

    // 测试用例2
    vector<string> words2 = {"z", "x"};
    string result2 = alienOrder(words2);
    cout << "Test Case 2: " << (result2.empty() ? "No valid order" : result2) << endl;

    // 测试用例3
    vector<string> words3 = {"z", "x", "z"};
    string result3 = alienOrder(words3);
    cout << "Test Case 3: " << (result3.empty() ? "No valid order" : result3) << endl;

    // 测试用例4
    vector<string> words4 = {"abc", "ab"};
    string result4 = alienOrder(words4);
    cout << "Test Case 4: " << (result4.empty() ? "No valid order" : result4) << endl;

    // 测试用例5
    vector<string> words5 = {"a", "b", "ca", "cc"};
    string result5 = alienOrder(words5);
    cout << "Test Case 5: " << (result5.empty() ? "No valid order" : result5) << endl;

    return 0;
}