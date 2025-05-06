#include <string>       // 引入字符串类
#include <vector>       // 引入向量（动态数组）类
#include <functional>   // 如果使用 std::function 进行递归可能需要，但这里使用了直接递归
// 正则表达式匹配
// 给你字符串s、字符串p
// s中一定不含有'.'、'*'字符，p中可能含有'.'、'*'字符
// '.' 表示可以变成任意字符，数量1个
// '*' 表示可以让 '*' 前面那个字符数量任意(甚至可以是0个)
// p中即便有'*'，一定不会出现以'*'开头的情况，也一定不会出现多个'*'相邻的情况(无意义)
// 请实现一个支持 '.' 和 '*' 的正则表达式匹配
// 返回p的整个字符串能不能匹配出s的整个字符串
// 测试链接 : https://leetcode.cn/problems/regular-expression-matching/

// 问题描述：
// 实现支持 '.' 和 '*' 的正则表达式匹配。
// '.' 匹配任意单个字符。
// '*' 匹配零个或多个前面的那个元素。
// 匹配需要覆盖整个输入字符串（而不是部分字符串）。
// s 只包含小写字母 a-z。
// p 包含小写字母 a-z、'.' 或 '*'。
// 保证 '*' 不会是 p 的第一个字符，并且 '*' 总是跟随一个有效的字符或 '.'。

class Solution {
private:
    // isMatch1 的辅助函数：纯递归方法
    // s, p: 输入字符串
    // i: 字符串 s 中的当前索引
    // j: 模式 p 中的当前索引
    // 如果 s[i...] 能够匹配 p[j...]，则返回 true
    bool f1(const std::string& s, const std::string& p, int i, int j) {
        int n = s.length(); // 字符串 s 的长度
        int m = p.length(); // 模式 p 的长度

        // 递归基 1：模式 p 已经遍历完
        if (j == m) {
            // 只有当字符串 s 也同时遍历完时，才算匹配成功
            return i == n;
        }

        // 递归基 2：字符串 s 已经遍历完 (但 p 还有剩余)
        if (i == n) {
            // 剩下的模式 p 必须能够匹配空字符串，这意味着它必须由类似 'x*y*z*' 的形式组成
            // 检查 p 剩余的部分是否能有效地变为空（例如 "a*b*c*"）
            // 需要成对的 (字符, '*')
            // 检查 p[j]* 是否可以变成空串（即 * 匹配 0 次），然后递归检查 p[j+2...] 是否能匹配空串
            return (j + 1 < m && p[j + 1] == '*' && f1(s, p, i, j + 2));
        }

        // === 递归步骤 ===

        // 情况 1：模式 p 中的下一个字符 (p[j+1]) 不是 '*'
        if (j + 1 == m || p[j + 1] != '*') {
            // 当前字符必须匹配 (s[i] == p[j] 或者 p[j] == '.')
            // 并且剩余的字符串也必须匹配
            if (i < n && (s[i] == p[j] || p[j] == '.')) {
                return f1(s, p, i + 1, j + 1); // 移动到下一个字符进行比较
            } else {
                return false; // 当前字符不匹配
            }
        }
        // 情况 2：模式 p 中的下一个字符 (p[j+1]) 是 '*'
        else {
            // 这代表模式中的 'p[j]*' 组合

            // 选项 A：'*' 匹配零次 p[j]。
            // 跳过模式中的 p[j] 和 p[j+1]，检查模式的剩余部分 p[j+2...] 是否能匹配 s[i...]。
            bool res_zero = f1(s, p, i, j + 2);

            // 选项 B：'*' 匹配一次或多次 p[j]。
            // 这要求当前字符 s[i] 和 p[j] 必须匹配。
            // 如果匹配，我们消耗 s[i]（移动到 i+1），但保持在相同的模式位置 j
            // （因为 '*' 允许用同一个 p[j] 匹配 s 中的更多字符）。
            bool res_one_or_more = false;
            if (i < n && (s[i] == p[j] || p[j] == '.')) {
                 res_one_or_more = f1(s, p, i + 1, j); // 字符串指针后移，模式指针不动
            }

            // 如果选项 A 或选项 B 中任何一个能导致匹配成功，则整体结果为 true。
            return res_zero || res_one_or_more;
        }
    }


    // isMatch2 的辅助函数：带记忆化搜索的递归方法 (自顶向下 DP)
    // dp[i][j]: 匹配 s[i...] 和 p[j...] 的结果
    // 0: 未计算, 1: true (匹配成功), 2: false (匹配失败)
    bool f2(const std::string& s, const std::string& p, int i, int j, std::vector<std::vector<int>>& dp) {
        // 如果该状态已经计算过，直接返回结果
        if (dp[i][j] != 0) {
            return dp[i][j] == 1;
        }

        int n = s.length();
        int m = p.length();
        bool ans; // 用于存储当前状态的计算结果

        // 递归基 1：模式 p 耗尽
        if (j == m) {
            ans = (i == n); // 仅当 s 也耗尽时为 true
        }
        // 递归基 2：字符串 s 耗尽 (但 p 未耗尽)
        else if (i == n) {
             // 检查 p[j...] 是否能匹配空字符串 (必须是 x*y*z* 的形式)
             // 即 p[j]* 可以匹配 0 次，然后看 p[j+2...] 能否匹配空串
             ans = (j + 1 < m && p[j + 1] == '*' && f2(s, p, i, j + 2, dp));
        }
        // 递归步骤
        else {
            // 情况 1：p 的下一个字符不是 '*'
            if (j + 1 == m || p[j + 1] != '*') {
                 // 当前字符匹配且后续递归匹配成功
                 if (s[i] == p[j] || p[j] == '.') {
                      ans = f2(s, p, i + 1, j + 1, dp);
                 } else {
                      ans = false; // 当前字符不匹配
                 }
            }
            // 情况 2：p 的下一个字符是 '*'
            else {
                 // 选项 A: '*' 匹配 0 次 (跳过 p[j]p[j+1])
                 bool res_zero = f2(s, p, i, j + 2, dp);
                 // 选项 B: '*' 匹配 1 次或多次 (要求当前字符匹配，然后消耗 s[i]，模式 p 停留在 j)
                 bool res_one_or_more = false;
                 if (s[i] == p[j] || p[j] == '.') {
                     res_one_or_more = f2(s, p, i + 1, j, dp);
                 }
                 // 两种选项任一成功即可
                 ans = res_zero || res_one_or_more;
            }
        }

        // 存储计算结果到 dp 表 (1 代表 true, 2 代表 false)
        dp[i][j] = ans ? 1 : 2;
        return ans;
    }


public:
    // 版本 1：纯递归 (可能因重复计算导致超时 - Time Limit Exceeded)
    bool isMatch1(std::string s, std::string p) {
        return f1(s, p, 0, 0);
    }

    // 版本 2：记忆化搜索 (自顶向下动态规划)
    bool isMatch2(std::string s, std::string p) {
        int n = s.length();
        int m = p.length();
        // dp[i][j] == 0: 未计算, 1: true, 2: false
        // 大小为 (n+1) x (m+1)，因为索引可以达到 n 和 m
        std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));
        return f2(s, p, 0, 0, dp);
    }

    // 版本 3：迭代动态规划 (自底向上 DP)
    bool isMatch3(std::string s, std::string p) {
        int n = s.length();
        int m = p.length();

        // dp[i][j] = true 表示 s 的后缀 s[i...] 可以匹配 p 的后缀 p[j...]
        // 大小为 (n+1) x (m+1)
        std::vector<std::vector<bool>> dp(n + 1, std::vector<bool>(m + 1, false));

        // 基础情况：空字符串 s (i=n) 可以匹配空模式 p (j=m)
        dp[n][m] = true;

        // 自底向上填充 DP 表
        // i 从 n (空字符串) 遍历到 0 (整个字符串 s)
        // j 从 m (空模式) 遍历到 0 (整个模式 p) - 注意 j 的循环从 m-1 开始，因为 dp[...][m] 已经处理
        for (int i = n; i >= 0; --i) {
            for (int j = m - 1; j >= 0; --j) {
                // 处理 s 为空串 (i=n) 的情况 (除了 dp[n][m] 之外)
                // 这部分逻辑与 Java 版本在循环结构上略有不同，但目的是相同的：正确初始化最后一行
                if (i == n) {
                    // 如果 p[j+1] 是 '*'，那么 p[j]* 可以匹配空串，结果取决于 p[j+2...] 是否能匹配空串
                    dp[n][j] = (j + 1 < m && p[j + 1] == '*' && dp[n][j + 2]);
                    continue; // 处理完 i=n 的情况，继续下一个 j
                }

                // 当 s 和 p 都不是空串时 (i < n, j < m)

                // 情况 1：p 的下一个字符不是 '*'
                if (j + 1 == m || p[j + 1] != '*') {
                    // 当前字符必须匹配 (s[i] == p[j] 或 p[j] == '.')
                    // 并且 s[i+1...] 必须能匹配 p[j+1...] (即 dp[i+1][j+1] 为 true)
                    dp[i][j] = (s[i] == p[j] || p[j] == '.') && dp[i + 1][j + 1];
                }
                // 情况 2：p 的下一个字符是 '*' (即模式为 p[j]p[j+1] 形式，代表 x*)
                else {
                    // 选项 A：x* 匹配零次。
                    // 结果取决于 s[i...] 是否能匹配 p[j+2...] (即 dp[i][j+2])
                    bool res_zero = dp[i][j + 2];

                    // 选项 B：x* 匹配一次或多次。
                    // 这需要当前字符匹配 (s[i] == p[j] 或 p[j] == '.')
                    // 并且 x* (即 p[j]p[j+1]...) 需要能匹配 s 的剩余部分 s[i+1...] (即 dp[i+1][j] 为 true)
                    bool res_one_or_more = false;
                     if (s[i] == p[j] || p[j] == '.') {
                        // dp[i+1][j] 表示 s[i+1...] 是否能匹配 p[j...]
                        res_one_or_more = dp[i + 1][j];
                    }
                    // 两种选项任一成功即可
                    dp[i][j] = res_zero || res_one_or_more;
                }
            }
        }

        // 最终结果：整个字符串 s (s[0...]) 是否能匹配整个模式 p (p[0...])
        return dp[0][0];
    }


    // 默认调用的函数（通常选择最高效的版本）
     bool isMatch(std::string s, std::string p) {
        // return isMatch1(s, p); // 递归 - 很可能超时
        // return isMatch2(s, p); // 记忆化搜索 - 可行
        return isMatch3(s, p); // 迭代DP - 可行且通常更优
    }
};
