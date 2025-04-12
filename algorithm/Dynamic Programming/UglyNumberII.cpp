#include <algorithm>
using namespace std;

#include <algorithm>
using namespace std;

class Solution {
public:
    // 时间复杂度O(n)，n代表第n个丑数
    int nthUglyNumber(int n) {
        // 定义一个数组dp，用于存储从1到n的丑数序列
        // dp[0]未使用，dp[1]到dp[n]依次存储丑数
        int dp[n + 1]{}; // 初始化数组为全0
        dp[1] = 1; // 第一个丑数是1（定义）

        // 定义三个指针i2、i3、i5，分别指向当前需要乘以2、3、5的位置
        int i2 = 1, i3 = 1, i5 = 1;

        // 从第2个丑数开始计算，直到第n个丑数
        for (int i = 2; i <= n; i++) {
            // 计算当前指针位置乘以对应因子的结果
            int a = dp[i2] * 2; // 当前乘以2的结果
            int b = dp[i3] * 3; // 当前乘以3的结果
            int c = dp[i5] * 5; // 当前乘以5的结果

            // 找到a、b、c中的最小值，作为当前的丑数
            int cur = min({a, b, c});
            dp[i] = cur; // 将当前丑数存入dp数组

            // 如果当前丑数是由某个指针计算得到，则移动该指针
            // 注意：这里使用if而非elseif，因为可能存在多个指针计算出相同丑数的情况
            if (cur == a) i2++; // 如果当前丑数由i2计算得到，移动i2
            if (cur == b) i3++; // 如果当前丑数由i3计算得到，移动i3
            if (cur == c) i5++; // 如果当前丑数由i5计算得到，移动i5
        }

        // 返回第n个丑数
        return dp[n];
    }
};
// 丑数 II
// 给你一个整数 n ，请你找出并返回第 n 个 丑数
// 丑数 就是只包含质因数 2、3 或 5 的正整数
// 测试链接 : https://leetcode.cn/problems/ugly-number-ii/
#include<iostream>
// 主函数，用于测试nthUglyNumber方法
int main() {
    Solution solution;

    // 测试用例：输入n，输出第n个丑数
    int n;
    cout << "n: ";
    cin >> n;

    // 调用nthUglyNumber方法并输出结果
    int result = solution.nthUglyNumber(n);
    cout << " " << n << " : " << result << endl;

    return 0;
}