#include <vector>
using namespace std;

// 递归法实现斐波那契数列（时间复杂度O(2^n)）
// 参数：n - 第n个斐波那契数
// 返回值：第n个斐波那契数
int fib1(int n) {
    // 基础情况：F(0) = 0, F(1) = 1
    if (n == 0) return 0;
    if (n == 1) return 1;
    // 递归调用计算前两项之和
    return fib1(n - 1) + fib1(n - 2);
}

// 记忆化搜索辅助函数（递归+缓存）
// 参数：i - 当前计算的斐波那契数索引，dp - 缓存数组
// 返回值：第i个斐波那契数
int f2(int i, vector<int>& dp) {
    // 基础情况：F(0) = 0, F(1) = 1
    if (i == 0) return 0;
    if (i == 1) return 1;
    // 如果缓存中已有结果，直接返回
    if (dp[i] != -1) return dp[i];
    // 递归计算并存储到缓存中
    int ans = f2(i - 1, dp) + f2(i - 2, dp);
    dp[i] = ans; // 更新缓存
    return ans;
}

// 记忆化搜索主函数（时间复杂度O(n)）
// 参数：n - 第n个斐波那契数
// 返回值：第n个斐波那契数
int fib2(int n) {
    // 初始化缓存数组，初始值为-1表示未计算
    vector<int> dp(n + 1, -1);
    // 调用辅助函数进行计算
    return f2(n, dp);
}

// 动态规划实现斐波那契数列（时间复杂度O(n)）
// 参数：n - 第n个斐波那契数
// 返回值：第n个斐波那契数
int fib3(int n) {
    // 基础情况处理
    if (n == 0) return 0;
    if (n == 1) return 1;
    // 创建动态规划数组
    vector<int> dp(n + 1);
    dp[0] = 0; // F(0) = 0
    dp[1] = 1; // F(1) = 1
    // 自底向上填充动态规划表
    for (int i = 2; i <= n; i++) {
        dp[i] = dp[i - 1] + dp[i - 2]; // 状态转移方程
    }
    return dp[n]; // 返回最终结果
}

// 空间优化迭代法实现斐波那契数列（时间复杂度O(n)，空间复杂度O(1)）
// 参数：n - 第n个斐波那契数
// 返回值：第n个斐波那契数
int fib4(int n) {
    // 基础情况处理
    if (n == 0) return 0;
    if (n == 1) return 1;
    // 使用两个变量存储前两项
    int lastLast = 0; // F(0)
    int last = 1;     // F(1)
    // 迭代计算当前项
    for (int i = 2; i <= n; i++) {
        int cur = lastLast + last; // 当前项等于前两项之和
        lastLast = last;           // 更新前一项
        last = cur;                // 更新当前项
    }
    return last; // 最终结果存储在last中
}


// 斐波那契数
// 斐波那契数 （通常用 F(n) 表示）形成的序列称为 斐波那契数列
// 该数列由 0 和 1 开始，后面的每一项数字都是前面两项数字的和。
// 也就是：F(0) = 0，F(1) = 1
// F(n) = F(n - 1) + F(n - 2)，其中 n > 1
// 给定 n ，请计算 F(n)
// 测试链接 : https://leetcode.cn/problems/fibonacci-number/


#include<iostream>
int main() {
    int n;
    cout << "n: ";
    cin >> n;

    // 测试递归法
    cout << "fib1(" << n << ") = " << fib1(n) << endl;

    // 测试记忆化搜索法
    cout << "fib2(" << n << ") = " << fib2(n) << endl;

    // 测试动态规划法
    cout << "fib3(" << n << ") = " << fib3(n) << endl;

    // 测试空间优化迭代法
    cout << "fib4(" << n << ") = " << fib4(n) << endl;

    return 0;
}