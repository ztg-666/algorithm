#include <vector>
#include <string>
#include <algorithm> // 用于reverse
#include <iostream>
using namespace std;

vector<int> movesToStamp(string stamp, string target) {
    int m = stamp.size(); // 印章长度
    int n = target.size(); // 目标字符串长度

    // 如果目标字符串比印章短，直接返回空数组（不可能的情况）
    if (n < m) return {};

    // 入度数组：记录每个窗口未匹配字符的数量，初始值为m（完全不匹配时）
    vector<int> indegree(n - m + 1, m);

    // 邻接表：graph[i]记录依赖位置i的所有窗口索引
    vector<vector<int>> graph(n);

    // 数组模拟队列：存储入度为0的窗口索引
    vector<int> queue(n - m + 1);
    int l = 0, r = 0; // 队列头尾指针

    // 构建图结构 O(n*m)
    for (int i = 0; i <= n - m; ++i) { // 遍历所有可能的窗口起点
        for (int j = 0; j < m; ++j) { // 检查窗口内每个字符
            if (target[i + j] == stamp[j]) {
                // 如果字符匹配，减少该窗口的入度
                if (--indegree[i] == 0) {
                    queue[r++] = i; // 如果入度为0，加入队列
                }
            } else {
                // 如果字符不匹配，记录位置i+j对窗口i的依赖
                graph[i + j].push_back(i);
            }
        }
    }

    // 标记已处理的位置
    vector<bool> visited(n, false);

    // 记录路径结果
    vector<int> path;
    path.reserve(n - m + 1); // 预分配空间以提高效率

    // 拓扑排序处理
    while (l < r) { // 当队列不为空时
        int cur = queue[l++]; // 取出队首窗口索引
        path.push_back(cur); // 将当前窗口加入路径

        // 处理该窗口覆盖的所有位置
        for (int j = 0; j < m; ++j) {
            int pos = cur + j; // 当前窗口内的位置
            if (!visited[pos]) { // 如果该位置尚未被标记
                visited[pos] = true; // 标记为已处理
                // 更新所有依赖该位置的窗口
                for (int next : graph[pos]) {
                    if (--indegree[next] == 0) { // 如果依赖窗口的入度变为0
                        queue[r++] = next; // 将其加入队列
                    }
                }
            }
        }
    }

    // 检查是否所有窗口都被处理
    if (path.size() != n - m + 1) {
        return {}; // 如果未处理完所有窗口，返回空数组
    }

    // 逆序得到正确的盖章顺序
    reverse(path.begin(), path.end());
    return path;
}
// 戳印序列
// 你想最终得到"abcbc"，认为初始序列为"?????"。印章是"abc"
// 那么可以先用印章盖出"??abc"的状态，
// 然后用印章最左字符和序列的0位置对齐，就盖出了"abcbc"
// 这个过程中，"??abc"中的a字符，被印章中的c字符覆盖了
// 每次盖章的时候，印章必须完全盖在序列内
// 给定一个字符串target是最终的目标，长度为n，认为初始序列为n个'?'
// 给定一个印章字符串stamp，目标是最终盖出target
// 但是印章的使用次数必须在10*n次以内
// 返回一个数组，该数组由每个回合中被印下的最左边字母的索引组成
// 上面的例子返回[2,0]，表示印章最左字符依次和序列2位置、序列0位置对齐盖下去，就得到了target
// 如果不能在10*n次内印出序列，就返回一个空数组
// 测试链接 : https://leetcode.cn/problems/stamping-the-sequence/
// 主函数
int main() {
    // 测试用例1
    string stamp1 = "abc";
    string target1 = "ababc";
    vector<int> result1 = movesToStamp(stamp1, target1);
    cout << "Test Case 1: ";
    if (result1.empty()) {
        cout << "No valid order" << endl;
    } else {
        for (int index : result1) {
            cout << index << " ";
        }
        cout << endl;
    }

    // 测试用例2
    string stamp2 = "abca";
    string target2 = "aabcaca";
    vector<int> result2 = movesToStamp(stamp2, target2);
    cout << "Test Case 2: ";
    if (result2.empty()) {
        cout << "No valid order" << endl;
    } else {
        for (int index : result2) {
            cout << index << " ";
        }
        cout << endl;
    }

    // 测试用例3
    string stamp3 = "z";
    string target3 = "zzzz";
    vector<int> result3 = movesToStamp(stamp3, target3);
    cout << "Test Case 3: ";
    if (result3.empty()) {
        cout << "No valid order" << endl;
    } else {
        for (int index : result3) {
            cout << index << " ";
        }
        cout << endl;
    }

    // 测试用例4
    string stamp4 = "abc";
    string target4 = "abcd";
    vector<int> result4 = movesToStamp(stamp4, target4);
    cout << "Test Case 4: ";
    if (result4.empty()) {
        cout << "No valid order" << endl;
    } else {
        for (int index : result4) {
            cout << index << " ";
        }
        cout << endl;
    }

    return 0;
}

