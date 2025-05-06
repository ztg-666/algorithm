#include <string>       // 引入字符串类
#include <vector>       // 引入向量（动态数组）类
#include <functional>   // 如果使用 std::function 进行递归可能需要，但这里未使用

// 通配符匹配（和题目4高度相似，只是边界条件不同而已，而且更简单）
// 给你字符串s、字符串p
// s中一定不含有'?'、'*'字符，p中可能含有'?'、'*'字符
// '?' 表示可以变成任意字符，数量1个
// '*' 表示可以匹配任何字符串
// 请实现一个支持 '?' 和 '*' 的通配符匹配
// 返回p的整个字符串能不能匹配出s的整个字符串
// 测试链接 : https://leetcode.cn/problems/wildcard-matching/

// 问题描述：
// 实现支持 '?' 和 '*' 的通配符模式匹配。
// '?' 匹配任意单个字符。
// '*' 匹配任意字符序列（包括空序列）。
// 匹配需要覆盖整个输入字符串（而不是部分字符串）。
// s 只包含小写字母 a-z。
// p 包含小写字母 a-z、'?' 或 '*'。

class Solution {
private:
    // isMatch1 的辅助函数：纯递归方法
    // s, p: 输入字符串
    // i: 字符串 s 中的当前索引
    // j: 模式 p 中的当前索引
    // 返回 s[i...] 是否能匹配 p[j...]
    bool f1(const std::string& s, const std::string& p, int i, int j) {
        int n = s.length(); // 字符串 s 的长度
        int m = p.length(); // 模式 p 的长度

        // 递归基 1：字符串 s 已经遍历完
        if (i == n) {
            // 如果模式 p 也同时遍历完，则匹配成功
            if (j == m) {
                return true;
            } else {
                // 如果 p 未遍历完，剩下的 p 必须都能被 '*' 消掉才能匹配空字符串 s
                // 具体地：检查当前 p[j] 是否是 '*'，并且 p[j+1...] 也能匹配空字符串
                return p[j] == '*' && f1(s, p, i, j + 1);
            }
        }
        // 递归基 2：模式 p 已经遍历完（但 s 还有剩余字符）
        else if (j == m) {
            // s 还有剩余字符，但 p 已经没了，无法匹配
            return false;
        }
        // 递归步骤：s 和 p 都还有剩余字符
        else {
            // 情况 1：当前模式字符 p[j] 不是 '*'
            if (p[j] != '*') {
                // 当前字符必须匹配（直接相等或 p[j] 是 '?'）
                // 并且 剩余的字符串 s[i+1...] 和模式 p[j+1...] 也必须匹配
                return (s[i] == p[j] || p[j] == '?') && f1(s, p, i + 1, j + 1);
            }
            // 情况 2：当前模式字符 p[j] 是 '*'
            else {
                // '*' 可以匹配 s 中的零个或多个字符。有两种可能性：
                // 选项 A：'*' 匹配 s 中的一个或多个字符。
                //         此时，我们消耗 s 中的当前字符 s[i]（移动到 i+1），
                //         但模式 p 的 '*' 保持不变（停留在 j），因为它可能还需要匹配 s 后面的更多字符。
                //         递归检查 s[i+1...] 是否能匹配 p[j...]。
                bool match_one_or_more = f1(s, p, i + 1, j);

                // 选项 B：'*' 匹配 s 中的零个字符（即匹配空序列）。
                //         此时，我们不消耗 s 中的字符（保持在 i），
                //         但模式 p 需要跳过这个 '*'（移动到 j+1）。
                //         递归检查 s[i...] 是否能匹配 p[j+1...]。
                bool match_zero = f1(s, p, i, j + 1);

                // 如果这两种选项中的任何一种能够成功匹配，则最终结果为 true。
                return match_one_or_more || match_zero;
            }
        }
    }


    // isMatch2 的辅助函数：带记忆化搜索的递归方法 (自顶向下 DP)
    // dp[i][j]: 匹配 s[i...] 和 p[j...] 的结果
    // 0: 未计算, 1: true (匹配成功), 2: false (匹配失败)
    bool f2(const std::string& s, const std::string& p, int i, int j, std::vector<std::vector<int>>& dp) {
        // 检查记忆化表，如果已经计算过，直接返回结果
        if (dp[i][j] != 0) {
            return dp[i][j] == 1;
        }

        int n = s.length();
        int m = p.length();
        bool ans; // 用于存储当前状态的计算结果

        // 递归基 1：字符串 s 耗尽
        if (i == n) {
            if (j == m) {
                ans = true; // p 也耗尽，匹配成功
            } else {
                // 剩余的 p 必须能被 '*' 匹配掉
                ans = p[j] == '*' && f2(s, p, i, j + 1, dp);
            }
        }
        // 递归基 2：模式 p 耗尽 (但 s 未耗尽)
        else if (j == m) {
            ans = false; // s 还有字符，p 没了，失败
        }
        // 递归步骤
        else {
            // 情况 1：p[j] 不是 '*'
            if (p[j] != '*') {
                // 当前字符匹配 且 后续匹配成功
                ans = (s[i] == p[j] || p[j] == '?') && f2(s, p, i + 1, j + 1, dp);
            }
            // 情况 2：p[j] 是 '*'
            else {
                 // 选项 A: '*' 匹配一个或多个字符 -> f2(i+1, j)
                 // 选项 B: '*' 匹配零个字符 -> f2(i, j+1)
                ans = f2(s, p, i + 1, j, dp) || f2(s, p, i, j + 1, dp);
            }
        }

        // 将计算结果存入记忆化表 (1 代表 true, 2 代表 false)
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
        // 大小为 (n+1) x (m+1)
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

        // 填充 DP 表的最后一行 (i = n，即 s 为空串时)
        // 模式 p[j...] 必须能够匹配空字符串 s。
        // 这只有在 p 从 j 开始的所有字符都是 '*' 时才可能。
        for (int j = m - 1; j >= 0; --j) {
            // 如果 p[j] 是 '*'，它就可以匹配空串，
            // 那么 p[j...] 能否匹配空串就取决于 p[j+1...] 能否匹配空串 (dp[n][j+1])。
            if (p[j] == '*') {
                dp[n][j] = dp[n][j + 1];
            } else {
                // 如果 p[j] 不是 '*'，它无法匹配空串，因此 dp[n][j] 必须是 false。
                // 因为默认是 false，这里可以不写，或者显式写 dp[n][j] = false;
                // 或者像Java代码那样，只要遇到非'*'就停止填充 true
                 break; // 更直接的写法：一旦遇到非'*', 后续的 dp[n][...] 都无法匹配空串了
            }
        }
        // (上述循环逻辑等价于 Java 版本中的初始化循环)


        // 自底向上填充 DP 表的其余部分
        // i 从 n-1 (s 的最后一个字符) 遍历到 0 (s 的第一个字符)
        for (int i = n - 1; i >= 0; --i) {
            // j 从 m-1 (p 的最后一个字符) 遍历到 0 (p 的第一个字符)
            for (int j = m - 1; j >= 0; --j) {
                // 情况 1：当前模式字符 p[j] 不是 '*'
                if (p[j] != '*') {
                    // 必须当前字符匹配 (s[i] == p[j] 或 p[j] == '?')
                    // 并且 后续字符串也要匹配 (dp[i+1][j+1])
                    dp[i][j] = (s[i] == p[j] || p[j] == '?') && dp[i + 1][j + 1];
                }
                // 情况 2：当前模式字符 p[j] 是 '*'
                else {
                    // 选项 A：'*' 匹配零个字符。
                    //         结果取决于 s[i...] 是否能匹配 p[j+1...] (即 dp[i][j+1])。
                    bool match_zero = dp[i][j + 1];

                    // 选项 B：'*' 匹配一个或多个字符。
                    //         结果取决于 s[i+1...] 是否能匹配 p[j...] (即 dp[i+1][j])。
                    //         (因为 '*' 消耗了 s[i]，我们检查 '*' 是否能继续匹配 s[i+1...] )
                    bool match_one_or_more = dp[i + 1][j];

                    // 只要有一种选项能匹配成功，dp[i][j] 就为 true。
                    dp[i][j] = match_zero || match_one_or_more;
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
