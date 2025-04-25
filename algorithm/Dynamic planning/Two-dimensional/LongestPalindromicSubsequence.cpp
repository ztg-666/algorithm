#include <vector>
#include <string>
#include <algorithm> // 包含 std::max
#include <iostream>  // 可选：用于测试/调试

// 使用标准命名空间以简化代码（可选）
using namespace std;
// 最长回文子序列
// 给你一个字符串 s ，找出其中最长的回文子序列，并返回该序列的长度
// 测试链接 : https://leetcode.cn/problems/longest-palindromic-subsequence/
class Solution {
public:
    // 最长回文子序列问题可以转化成最长公共子序列问题
    // (字符串s 与其反转字符串reverse(s) 的最长公共子序列)
    // 不过这里讲述区间动态规划的思路
    // 区间dp还会有单独的视频做详细讲述

    // --- 方法 1: 纯递归 ---
    // 计算 s[l...r] 范围内的最长回文子序列长度
    // 要求 l <= r
    int f1(const string& s, int l, int r) {
        // 基本情况 1: 区间只有一个字符
        if (l == r) {
            return 1;
        }
        // 基本情况 2: 区间只有两个字符
        if (l + 1 == r) {
            return (s[l] == s[r]) ? 2 : 1;
        }
        // 递归情况:
        // 如果两端字符相同，则长度为 2 + 中间部分的最长回文子序列长度
        if (s[l] == s[r]) {
            return 2 + f1(s, l + 1, r - 1);
        } else {
            // 如果两端字符不同，则长度为以下两种情况的最大值：
            // 1. 忽略左端字符 (l+1 到 r)
            // 2. 忽略右端字符 (l 到 r-1)
            return max(f1(s, l + 1, r), f1(s, l, r - 1));
        }
    }

    // 方法1 的入口函数
    int longestPalindromeSubseq1(const string& str) {
        int n = str.length();
        if (n == 0) return 0; // 处理空字符串
        return f1(str, 0, n - 1);
    }

    // --- 方法 2: 记忆化搜索 ---
    // 使用 dp 表存储已计算过的子问题结果，避免重复计算
    // dp[l][r] 存储 s[l...r] 的最长回文子序列长度
    int f2(const string& s, int l, int r, vector<vector<int>>& dp) {
        // 基本情况 1 和 2 (与 f1 相同)
        if (l == r) {
            return 1;
        }
        if (l + 1 == r) {
            return (s[l] == s[r]) ? 2 : 1;
        }
        // 检查是否已计算过
        if (dp[l][r] != -1) { // 使用 -1 表示未计算
            return dp[l][r];
        }

        int ans;
        if (s[l] == s[r]) {
            ans = 2 + f2(s, l + 1, r - 1, dp);
        } else {
            ans = max(f2(s, l + 1, r, dp), f2(s, l, r - 1, dp));
        }
        // 存储结果到 dp 表
        dp[l][r] = ans;
        return ans;
    }

    // 方法2 的入口函数
    int longestPalindromeSubseq2(const string& str) {
        int n = str.length();
        if (n == 0) return 0;
        // 初始化 dp 表，大小为 n x n，所有值设为 -1
        vector<vector<int>> dp(n, vector<int>(n, -1));
        return f2(str, 0, n - 1, dp);
    }

    // --- 方法 3: 严格位置依赖的动态规划 (二维 DP 表) ---
    // 自底向上填充 dp 表
    int longestPalindromeSubseq3(const string& str) {
        int n = str.length();
        if (n == 0) return 0;
        // 初始化 dp 表，大小为 n x n，值为 0
        vector<vector<int>> dp(n, vector<int>(n, 0));

        // l 从 n-1 递减到 0
        for (int l = n - 1; l >= 0; --l) {
            // 对角线：单个字符，长度为 1
            dp[l][l] = 1;

            // 相邻两个字符
            if (l + 1 < n) {
                dp[l][l + 1] = (str[l] == str[l + 1]) ? 2 : 1;
            }

            // r 从 l+2 递增到 n-1 (处理更长的子串)
            // 依赖于左下方、左边、下方的值，这些值在之前的迭代中已计算好
            for (int r = l + 2; r < n; ++r) {
                if (str[l] == str[r]) {
                    // 两端相等，依赖左下角的值
                    dp[l][r] = 2 + dp[l + 1][r - 1];
                } else {
                    // 两端不等，依赖左边或下方的值的最大值
                    dp[l][r] = max(dp[l + 1][r], dp[l][r - 1]);
                }
            }
        }
        // 最终结果是整个字符串 (0 到 n-1) 的最长回文子序列长度
        return dp[0][n - 1];
    }

    // --- 方法 4: 严格位置依赖的动态规划 + 空间压缩 (一维 DP 数组) ---
    // 优化方法3的空间复杂度，从 O(N^2) 到 O(N)
    int longestPalindromeSubseq4(const string& str) {
        int n = str.length();
        if (n == 0) return 0;
        // dp 数组大小为 n
        vector<int> dp(n, 0);

        // 遍历方向与方法3类似，但只用一维数组
        for (int l = n - 1; l >= 0; --l) {
            int leftDown = 0; // 存储 dp[l+1][r-1] 的值 (即上一轮迭代的 dp[r-1])
            int backup;       // 用于在更新 dp[r] 前备份其旧值 (即 dp[l+1][r])

            // dp[l] : 想象中的dp[l][l]
            dp[l] = 1; // 处理长度为1的子串

            // 处理长度为2的子串
            if (l + 1 < n) {
                // 在更新 dp[l+1] 之前，它代表 dp[l+1][l+1] (来自上一个 l 迭代)
                // 但我们需要的是 dp[l+1][l] 的值，这个逻辑在 1D 压缩中不易直接获取
                // 实际上，leftDown 在内部循环开始时需要持有 dp[l+1][l+1] 的值(即上一个l迭代的dp[l+1])
                // 所以，我们在更新 dp[l+1] 之前，需要保存它
                leftDown = dp[l+1]; // 保存旧的 dp[l+1] (即上一行的 dp[l+1][l+1])

                // 更新 dp[l+1] : 代表 dp[l][l+1]
                dp[l+1] = (str[l] == str[l+1]) ? 2 : 1;
            }

            // 处理长度大于等于3的子串
            for (int r = l + 2; r < n; ++r) {
                // 在更新 dp[r] 之前，我们需要保存两个值：
                // 1. leftDown: 代表 dp[l+1][r-1] (这是上一轮外层循环 l+1 时，内层循环到 r-1 时的值)
                // 2. dp[r]: 代表 dp[l+1][r] (这是上一轮外层循环 l+1 时，内层循环到 r 时的值)
                backup = dp[r]; // 备份 dp[r]，它代表上一行的 dp[l+1][r]

                if (str[l] == str[r]) {
                    // dp[l][r] = 2 + dp[l+1][r-1]
                    // 此处的 leftDown 存储着上一轮迭代结束时的 dp[r-1]，即所需要的 dp[l+1][r-1]
                    dp[r] = 2 + leftDown;
                } else {
                    // dp[l][r] = max(dp[l+1][r], dp[l][r-1])
                    // dp[l+1][r] 是 backup
                    // dp[l][r-1] 是当前的 dp[r-1] (因为内层 r 循环是从左到右的)
                    dp[r] = max(backup, dp[r - 1]);
                }
                // 更新 leftDown 供下一次内层循环 (r+1) 使用
                // 下一次需要的 leftDown 是 dp[l+1][r]，正是我们备份在 backup 里的值
                leftDown = backup;
            }
        }
        // 最终结果存储在 dp[n-1]，因为它在 l=0 的迭代结束后，代表 dp[0][n-1]
        return dp[n - 1];
    }
};

// // 示例用法（可选）
int main() {
    Solution sol;
    string s1 = "bbbab";
    string s2 = "cbbd";
    string s3 = "a";
    string s4 = "";

    cout << "字符串: \"" << s1 << "\"" << endl;
    cout << "方法1 (递归): " << sol.longestPalindromeSubseq1(s1) << endl; // 4
    cout << "方法2 (记忆化): " << sol.longestPalindromeSubseq2(s1) << endl; // 4
    cout << "方法3 (DP二维): " << sol.longestPalindromeSubseq3(s1) << endl; // 4
    cout << "方法4 (DP一维): " << sol.longestPalindromeSubseq4(s1) << endl; // 4

    cout << "\n字符串: \"" << s2 << "\"" << endl;
    cout << "方法4 (DP一维): " << sol.longestPalindromeSubseq4(s2) << endl; // 2

    cout << "\n字符串: \"" << s3 << "\"" << endl;
    cout << "方法4 (DP一维): " << sol.longestPalindromeSubseq4(s3) << endl; // 1

    cout << "\n字符串: \"" << s4 << "\"" << endl;
    cout << "方法4 (DP一维): " << sol.longestPalindromeSubseq4(s4) << endl; // 0

    return 0;
}
