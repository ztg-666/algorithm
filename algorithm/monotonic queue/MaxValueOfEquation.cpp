#include <iostream>
#include <vector>
#include <climits>
using namespace std;

const int MAXN = 100001;
// 和至少为K的最短子数组
// 给定一个数组arr，其中的值有可能正、负、0
// 给定一个正数k
// 返回累加和>=k的所有子数组中，最短的子数组长度
class Solution {
public:
    int deque[MAXN][2]{}; // 双端队列存储[x, y]
    int h, t;           // 队列头尾指针

    int findMaxValueOfEquation(vector<vector<int>>& points, int k) {
        h = t = 0; // 重置队列指针
        int ans = INT_MIN; // 初始化答案为最小整数
        int n = points.size(); // 获取点的数量

        for (int i = 0; i < n; ++i) {
            int x = points[i][0]; // 当前点的x坐标
            int y = points[i][1]; // 当前点的y坐标

            // 移除超出距离k的头部元素（维持窗口有效性）
            // 队列头部的x坐标如果小于当前点x坐标减去k，则移除
            while (h < t && deque[h][0] < x - k) {
                h++;
            }

            // 计算当前点与队列头部的最大值
            // 如果队列不为空，计算当前点与队列头部点的指标值
            if (h < t) {
                int frontX = deque[h][0]; // 队列头部点的x坐标
                int frontY = deque[h][1]; // 队列头部点的y坐标
                // 更新答案为当前最大值
                ans = max(ans, x + y + frontY - frontX);
            }

            // 维护队列单调递减（y - x递减）
            // 移除尾部所有y-x <= 当前y-x的元素
            // 保持队列中y-x的值从大到小排列
            while (h < t && (deque[t-1][1] - deque[t-1][0]) <= (y - x)) {
                t--;
            }

            // 将当前点加入队列尾部
            deque[t][0] = x; // 当前点的x坐标加入队列
            deque[t][1] = y; // 当前点的y坐标加入队列
            t++; // 尾指针后移
        }

        return ans; // 返回最大指标值
    }
};


int main() {
    Solution sol;

    // 测试用例 1
    vector<vector<int>> points1 = {{1, 3}, {2, 0}, {5, 10}, {6, -10}};
    int k1 = 1;
    cout << "Test Case 1: " << sol.findMaxValueOfEquation(points1, k1) << endl; // 输出: 4

    // 测试用例 2
    vector<vector<int>> points2 = {{0, 0}, {3, 0}, {9, 2}};
    int k2 = 3;
    cout << "Test Case 2: " << sol.findMaxValueOfEquation(points2, k2) << endl; // 输出: 3

    // 测试用例 3
    vector<vector<int>> points3 = {{1, 2}, {2, 3}, {3, 4}, {4, 5}};
    int k3 = 2;
    cout << "Test Case 3: " << sol.findMaxValueOfEquation(points3, k3) << endl; // 输出: 8

    // 测试用例 4
    vector<vector<int>> points4 = {{1, 1}, {2, 2}, {3, 3}, {4, 4}};
    int k4 = 3;
    cout << "Test Case 4: " << sol.findMaxValueOfEquation(points4, k4) << endl; // 输出: 9

    // 测试用例 5
    vector<vector<int>> points5 = {{1, 1}, {2, 2}, {3, 3}, {4, 4}};
    int k5 = 1;
    cout << "Test Case 5: " << sol.findMaxValueOfEquation(points5, k5) << endl; // 输出: 5

    return 0;
}