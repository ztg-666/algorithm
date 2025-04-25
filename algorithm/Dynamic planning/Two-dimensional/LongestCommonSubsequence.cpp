#include <vector>
#include <string>
#include <algorithm> // For std::max
#include <iostream>  // 可选：用于测试/调试

// 使用标准命名空间以简化代码（可选）
using namespace std;
// 最长公共子序列
// 给定两个字符串text1和text2
// 返回这两个字符串的最长 公共子序列 的长度
// 如果不存在公共子序列，返回0
// 两个字符串的 公共子序列 是这两个字符串所共同拥有的子序列
// 测试链接 : https://leetcode.cn/problems/longest-common-subsequence/
class Solution {
public:
    // --- 方法 1: 原始递归 (下标定义) ---
    // 存在冗余计算，效率较低，且与标准LCS递归定义略有不同
    // s1[0....i1]与s2[0....i2]最长公共子序列长度
    int f1(const string& s1, const string& s2, int i1, int i2) {
        if (i1 < 0 || i2 < 0) {
            return 0;
        }
        // 注意：这部分的逻辑与标准LCS递归不同，计算了不需要的p1, p2, p3
        // 标准递归在f2中实现
        int p1 = f1(s1, s2, i1 - 1, i2 - 1); // 左上
        int p2 = f1(s1, s2, i1 - 1, i2);     // 上
        int p3 = f1(s1, s2, i1, i2 - 1);     // 左
        int p4 = (s1[i1] == s2[i2]) ? (p1 + 1) : 0; // 如果当前字符匹配，基于左上结果+1

        return max({p1, p2, p3, p4}); // 返回四个可能性的最大值
        // 标准递归应该是:
        // if (s1[i1] == s2[i2]) return f1(s1, s2, i1-1, i2-1) + 1;
        // else return max(f1(s1, s2, i1-1, i2), f1(s1, s2, i1, i2-1));
    }

    int longestCommonSubsequence1(const string& str1, const string& str2) {
        int n = str1.length();
        int m = str2.length();
        // 直接传递字符串引用和结束索引
        return f1(str1, str2, n - 1, m - 1);
    }

    // --- 方法 2: 标准递归 (长度定义) ---
    // 为了避免很多边界讨论
    // 很多时候往往不用下标来定义尝试，而是用长度来定义尝试
    // 因为长度最短是0，而下标越界的话讨论起来就比较麻烦
    // s1[前缀长度为len1] 对应 s2[前缀长度为len2] 的最长公共子序列长度
    int f2(const string& s1, const string& s2, int len1, int len2) {
        if (len1 == 0 || len2 == 0) {
            return 0; // 基本情况：任一字符串长度为0，LCS为0
        }
        int ans;
        // 比较两个字符串的最后一个字符（注意长度len对应索引len-1）
        if (s1[len1 - 1] == s2[len2 - 1]) {
            // 如果最后一个字符相同，则LCS长度等于去掉最后一个字符后的LCS长度+1
            ans = f2(s1, s2, len1 - 1, len2 - 1) + 1;
        } else {
            // 如果最后一个字符不同，则LCS长度等于以下两者中的较大值：
            // 1. s1去掉最后一个字符，s2不变
            // 2. s1不变，s2去掉最后一个字符
            ans = max(f2(s1, s2, len1 - 1, len2), f2(s1, s2, len1, len2 - 1));
        }
        return ans;
    }

    int longestCommonSubsequence2(const string& str1, const string& str2) {
        int n = str1.length();
        int m = str2.length();
        return f2(str1, str2, n, m);
    }

    // --- 方法 3: 记忆化搜索 ---
    // 基于方法2的递归，增加dp表来存储已计算过的子问题结果
    // dp[len1][len2] 存储 s1前缀长度len1 和 s2前缀长度len2 的LCS
    int f3(const string& s1, const string& s2, int len1, int len2, vector<vector<int>>& dp) {
        if (len1 == 0 || len2 == 0) {
            return 0;
        }
        // 如果dp表中已有记录，直接返回，避免重复计算
        if (dp[len1][len2] != -1) {
            return dp[len1][len2];
        }

        int ans;
        if (s1[len1 - 1] == s2[len2 - 1]) {
            ans = f3(s1, s2, len1 - 1, len2 - 1, dp) + 1;
        } else {
            ans = max(f3(s1, s2, len1 - 1, len2, dp), f3(s1, s2, len1, len2 - 1, dp));
        }
        // 将计算结果存入dp表
        dp[len1][len2] = ans;
        return ans;
    }

    int longestCommonSubsequence3(const string& str1, const string& str2) {
        int n = str1.length();
        int m = str2.length();
        // 初始化dp表，大小为 (n+1) x (m+1)，所有值设为-1表示未计算
        vector<vector<int>> dp(n + 1, vector<int>(m + 1, -1));
        return f3(str1, str2, n, m, dp);
    }

    // --- 方法 4: 严格位置依赖的动态规划 (二维DP表) ---
    // 自底向上填充dp表
    int longestCommonSubsequence4(const string& str1, const string& str2) {
        int n = str1.length();
        int m = str2.length();
        // 初始化dp表，大小为 (n+1) x (m+1)，默认值为0（对应长度为0的情况）
        vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

        // 遍历所有可能的子问题（由长度len1和len2定义）
        for (int len1 = 1; len1 <= n; ++len1) {
            for (int len2 = 1; len2 <= m; ++len2) {
                // 状态转移方程，与递归逻辑f2/f3对应
                if (str1[len1 - 1] == str2[len2 - 1]) {
                    // 当前字符匹配：来自左上方格+1
                    dp[len1][len2] = 1 + dp[len1 - 1][len2 - 1];
                } else {
                    // 当前字符不匹配：来自上方或左方格的最大值
                    dp[len1][len2] = max(dp[len1 - 1][len2], dp[len1][len2 - 1]);
                }
            }
        }
        // 最终结果存储在dp[n][m]
        return dp[n][m];
    }

    // --- 方法 5: 严格位置依赖的动态规划 + 空间压缩 (一维DP数组) ---
    // 优化方法4的空间复杂度，从 O(N*M) 到 O(min(N, M))
    int longestCommonSubsequence5(const string& str1, const string& str2) {
        // 确保s1是较长的字符串，s2是较短的，dp数组大小取决于较短字符串
        const string& s1 = (str1.length() >= str2.length()) ? str1 : str2;
        const string& s2 = (str1.length() >= str2.length()) ? str2 : str1;

        int n = s1.length(); // 较长字符串的长度
        int m = s2.length(); // 较短字符串的长度

        // dp数组大小为 m+1，dp[j] 存储处理到s1[i]时，与s2[前缀j]的LCS
        vector<int> dp(m + 1, 0);

        // 遍历较长字符串 s1
        for (int len1 = 1; len1 <= n; ++len1) {
            int leftUp = 0; // 记录左上角的值，即 dp[len1-1][len2-1]
            int backup;     // 用于临时存储 dp[len2] 的旧值（来自上一行）

            // 遍历较短字符串 s2
            for (int len2 = 1; len2 <= m; ++len2) {
                backup = dp[len2]; // 保存当前 dp[len2] 的值（即 dp[len1-1][len2]）

                if (s1[len1 - 1] == s2[len2 - 1]) {
                    // 如果字符匹配，当前值 = 左上角值 + 1
                    dp[len2] = 1 + leftUp;
                } else {
                    // 如果字符不匹配，当前值 = max(上方值, 左方值)
                    // 上方值是 backup (即旧的 dp[len2])
                    // 左方值是 dp[len2-1] (已经更新为当前行的值)
                    dp[len2] = max(backup, dp[len2 - 1]);
                }
                // 更新 leftUp 为下一轮迭代做准备
                leftUp = backup;
            }
        }
        // 最终结果是dp数组的最后一个元素
        return dp[m];
    }
};

// // 示例用法（可选）
int main() {
    Solution sol;
    string text1 = "abcde";
    string text2 = "ace";

    cout << "方法1 (递归-下标): " << sol.longestCommonSubsequence1(text1, text2) << endl; // 可能较慢或结果不标准
    cout << "方法2 (递归-长度): " << sol.longestCommonSubsequence2(text1, text2) << endl; // 慢
    cout << "方法3 (记忆化):   " << sol.longestCommonSubsequence3(text1, text2) << endl; // 快
    cout << "方法4 (DP二维):  " << sol.longestCommonSubsequence4(text1, text2) << endl; // 快
    cout << "方法5 (DP一维):  " << sol.longestCommonSubsequence5(text1, text2) << endl; // 快，省空间

    string text3 = "abc";
    string text4 = "def";
    cout << "\n文本: \"" << text3 << "\", \"" << text4 << "\"" << endl;
    cout << "方法5 (DP一维):  " << sol.longestCommonSubsequence5(text3, text4) << endl; // 0

    return 0;
}
