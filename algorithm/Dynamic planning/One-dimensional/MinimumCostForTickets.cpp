#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

class Solution {
public:
    // 定义通行证持续天数：1天、7天、30天
    const int durations[3] = {1, 7, 30};

    // 暴力递归方法：从第i天开始计算最低票价
    int mincostTickets1(vector<int>& days, vector<int>& costs) {
        return f1(days, costs, 0); // 调用递归函数，从第0天开始计算
    }

    // 递归函数：计算从第i天开始的最低票价
    int f1(vector<int>& days, vector<int>& costs, int i) {
        if (i == days.size()) {
            return 0; // 如果已经没有旅行日了，返回0
        }
        int ans = INT_MAX; // 初始化最低票价为最大值
        for (int k = 0, j = i; k < 3; k++) { // 遍历三种票类型
            // 找到当前票能覆盖的最后一天（days[j]）
            while (j < days.size() && days[i] + durations[k] > days[j]) {
                j++;
            }
            // 当前票的价格 + 剩余天数的最低票价
            ans = min(ans, costs[k] + f1(days, costs, j));
        }
        return ans; // 返回从第i天开始的最低票价
    }

    // 记忆化搜索方法：通过记忆化优化暴力递归
    int mincostTickets2(vector<int>& days, vector<int>& costs) {
        vector<int> dp(days.size(), INT_MAX); // 初始化dp数组，记录每个位置的最低票价
        return f2(days, costs, 0, dp); // 调用记忆化递归函数，从第0天开始计算
    }

    // 记忆化递归函数：计算从第i天开始的最低票价
    int f2(vector<int>& days, vector<int>& costs, int i, vector<int>& dp) {
        if (i == days.size()) {
            return 0; // 如果已经没有旅行日了，返回0
        }
        if (dp[i] != INT_MAX) {
            return dp[i]; // 如果已经计算过，直接返回结果
        }
        int ans = INT_MAX; // 初始化最低票价为最大值
        for (int k = 0, j = i; k < 3; k++) { // 遍历三种票类型
            // 找到当前票能覆盖的最后一天（days[j]）
            while (j < days.size() && days[i] + durations[k] > days[j]) {
                j++;
            }
            // 当前票的价格 + 剩余天数的最低票价
            ans = min(ans, costs[k] + f2(days, costs, j, dp));
        }
        dp[i] = ans; // 记录从第i天开始的最低票价
        return ans; // 返回结果
    }

    // 动态规划方法：从底向上的动态规划实现
    int mincostTickets3(vector<int>& days, vector<int>& costs) {
        int n = days.size(); // 旅行天数
        vector<int> dp(n + 1, INT_MAX); // 初始化dp数组，dp[i]表示从第i天开始的最低票价
        dp[n] = 0; // 边界条件：如果已经没有旅行日了，最低票价为0
        // 从后往前计算dp数组
        for (int i = n - 1; i >= 0; i--) {
            for (int k = 0, j = i; k < 3; k++) { // 遍历三种票类型
                // 找到当前票能覆盖的最后一天（days[j]）
                while (j < days.size() && days[i] + durations[k] > days[j]) {
                    j++;
                }
                // 更新dp[i]：选择当前票的最低票价
                dp[i] = min(dp[i], costs[k] + dp[j]);
            }
        }
        return dp[0]; // 返回从第0天开始的最低票价
    }
};
// 最低票价
// 在一个火车旅行很受欢迎的国度，你提前一年计划了一些火车旅行
// 在接下来的一年里，你要旅行的日子将以一个名为 days 的数组给出
// 每一项是一个从 1 到 365 的整数
// 火车票有 三种不同的销售方式
// 一张 为期1天 的通行证售价为 costs[0] 美元
// 一张 为期7天 的通行证售价为 costs[1] 美元
// 一张 为期30天 的通行证售价为 costs[2] 美元
// 通行证允许数天无限制的旅行
// 例如，如果我们在第 2 天获得一张 为期 7 天 的通行证
// 那么我们可以连着旅行 7 天(第2~8天)
// 返回 你想要完成在给定的列表 days 中列出的每一天的旅行所需要的最低消费
// 测试链接 : https://leetcode.cn/problems/minimum-cost-for-tickets/
#include<iostream>
// 主函数
int main() {
    // 示例输入
    vector<int> days = {1, 4, 6, 7, 8, 20}; // 旅行日期
    vector<int> costs = {2, 7, 15};        // 三种票的价格

    Solution sol;

    // 调用暴力递归方法
    int result1 = sol.mincostTickets1(days, costs);
    cout << "1 " << result1 << endl;

    // 调用记忆化搜索方法
    int result2 = sol.mincostTickets2(days, costs);
    cout << "2: " << result2 << endl;

    // 调用动态规划方法
    int result3 = sol.mincostTickets3(days, costs);
    cout << "3: " << result3 << endl;

    return 0;
}
