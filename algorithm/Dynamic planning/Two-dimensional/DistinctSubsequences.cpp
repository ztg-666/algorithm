#include <vector>   // 用于 std::vector
#include <string>   // 用于 std::string

// 不同的子序列
// 给你两个字符串s和t ，统计并返回在s的子序列中t出现的个数
// 答案对 1000000007 取模
// 测试链接 : https://leetcode.cn/problems/distinct-subsequences/
class Solution {
public:
    // 版本 1: 二维动态规划，不带取模
    // dp[i][j] : s[0...i-1] 的子序列中等于 t[0...j-1] 的个数
    int numDistinct1(std::string s, std::string t) {
        int n = s.length(); // s 的长度
        int m = t.length(); // t 的长度
        
        // 初始化所有值为 0
        std::vector dp(n + 1, std::vector<long long>(m + 1, 0));

        // 基本情况：空的目标字符串 (t 长度为 0) 是 s 任何前缀（包括空 s）的子序列，
        // 并且只有一种方式（不选择任何字符）。
        for (int i = 0; i <= n; ++i) {
            dp[i][0] = 1;
        }

        // 填充 DP 表
        for (int i = 1; i <= n; ++i) { // 遍历 s 的字符
            for (int j = 1; j <= m; ++j) { // 遍历 t 的字符
                // 情况 1: 不使用 s[i-1] 来匹配 t[j-1]。
                // 个数与使用 s[0...i-2] 匹配 t[0...j-1] 相同。
                dp[i][j] = dp[i - 1][j];

                // 情况 2: 如果 s[i-1] == t[j-1]，则可以 使用 s[i-1] 来匹配 t[j-1]。
                // 在这种情况下，我们加上使用 s[0...i-2] 匹配 t[0...j-2] 的个数。
                if (s[i - 1] == t[j - 1]) {
                     // 加上 使用 s[i-1] 匹配 t[j-1] 的情况的数量
                     dp[i][j] += dp[i - 1][j - 1];
                }
            }
        }

        // 返回完整字符串 s 和 t 的匹配个数。
        // 注意：如果返回类型要求是 int，进行强制类型转换。
        // 如果结果大于 INT_MAX 且未使用取模，可能会发生溢出。
        return (int)dp[n][m];
    }

    // ---

    // 版本 2: 空间优化的 一维动态规划 (对应 Java 的 numDistinct2)，不带取模
    int numDistinct2(std::string s, std::string t) {
        int n = s.length(); // s 的长度
        int m = t.length(); // t 的长度

        if (m == 0) return 1; // 空目标串匹配 1 次
        if (n < m) return 0;  // 源字符串太短，无法构成目标串

        // dp[j] 将存储构成 t[0...j-1] 的方式数量
        // 使用 long long 防止加法过程中溢出
        std::vector<long long> dp(m + 1, 0);

        // 基本情况：空目标串 (t[0...-1]) 可以用 1 种方式构成。
        dp[0] = 1;

        // 遍历源字符串 s 的每个字符
        for (int i = 1; i <= n; ++i) {
            // 反向遍历目标字符串 t
            // 确保计算 dp[j] 时使用的 dp[j-1] 是上一轮 i (即处理 s[0...i-2] 时) 的结果
            for (int j = m; j >= 1; --j) {
                // 如果当前字符匹配
                if (s[i - 1] == t[j - 1]) {
                    // 更新 dp[j]：等于不使用 s[i-1] 的方案数 (dp[j] 当前值)
                    // 加上 使用 s[i-1] 匹配 t[j-1] 的方案数 (依赖于上一轮的 dp[j-1])
                    dp[j] += dp[j - 1];
                }
                // else: 如果字符不匹配, s[i-1] 不能用来匹配 t[j-1]。
                // dp[j] 的值保持不变，因为它代表的是只用 s[0...i-2] 匹配 t[0...j-1] 的方案数。
            }
        }

        // 返回构成完整目标字符串 t[0...m-1] 的结果
        // 注意类型转换可能导致的溢出（在此版本中未处理）
         return (int)dp[m];
    }

    // ---

    // 版本 3: 空间优化的 一维动态规划 (带取模) (对应 Java 的 numDistinct3)
    int numDistinct3(std::string s, std::string t) {
        int n = s.length(); // s 的长度
        int m = t.length(); // t 的长度
        int mod = 1000000007; // 模数

        if (m == 0) return 1; // 空目标串匹配 1 次
        // 注意：如果 n < m，循环结束后 dp[m] 自然会是 0，可以不显式检查 n < m

        // dp[j] 将存储构成 t[0...j-1] 的方式数量 % mod
        // 使用 long long 作为 dp 数组元素类型，以防止加法过程中 *取模之前* 的溢出。
        std::vector<long long> dp(m + 1, 0);

        // 基本情况：空目标串可以用 1 种方式构成。
        dp[0] = 1;

        // 遍历源字符串 s 的每个字符
        for (int i = 1; i <= n; ++i) {
            // 反向遍历目标字符串 t
            for (int j = m; j >= 1; --j) {
                // 如果当前字符匹配
                if (s[i - 1] == t[j - 1]) {
                    // 加上使用 s[i-1] 匹配 t[j-1] 的方式数，并应用取模。
                    // dp[j] = (dp[j]_旧 + dp[j-1]_旧) % mod
                    dp[j] = (dp[j] + dp[j - 1]) % mod;
                }
                // else: 字符不匹配，dp[j] 保持不变
            }
        }

        // 最终结果已经是取模后的，转换回 int 类型。
        return (int)dp[m];
    }
};


#include<bits/stdc++.h>
int main() {
    Solution sol;
    
    // 测试用例1: 标准示例（LeetCode 115题示例）
    std::string s1 = "rabbbit";
    std::string t1 = "rabbit";
    std::cout << "Test Case 1:\n"
              << "s: " << s1 << "\nt: " << t1 << "\n"
              << "v1 result: " << sol.numDistinct1(s1, t1) << "\n"
              << "v2 result: " << sol.numDistinct2(s1, t1) << "\n"
              << "v3 result: " << sol.numDistinct3(s1, t1) << "\n\n";

    // 测试用例2: 另一个标准示例
    std::string s2 = "babgbag";
    std::string t2 = "bag";
    std::cout << "Test Case 2:\n"
              << "s: " << s2 << "\nt: " << t2 << "\n"
              << "v1 result: " << sol.numDistinct1(s2, t2) << "\n"
              << "v2 result: " << sol.numDistinct2(s2, t2) << "\n"
              << "v3 result: " << sol.numDistinct3(s2, t2) << "\n\n";

    // 测试用例3: 空目标字符串
    std::string s3 = "abc";
    std::string t3 = "";
    std::cout << "Test Case 3:\n"
              << "s: " << s3 << "\nt: (empty)\n"
              << "v1 result: " << sol.numDistinct1(s3, t3) << "\n"
              << "v2 result: " << sol.numDistinct2(s3, t3) << "\n"
              << "v3 result: " << sol.numDistinct3(s3, t3) << "\n\n";

    // 测试用例4: s比t短
    std::string s4 = "ab";
    std::string t4 = "abc";
    std::cout << "Test Case 4:\n"
              << "s: " << s4 << "\nt: " << t4 << "\n"
              << "v1 result: " << sol.numDistinct1(s4, t4) << "\n"
              << "v2 result: " << sol.numDistinct2(s4, t4) << "\n"
              << "v3 result: " << sol.numDistinct3(s4, t4) << "\n\n";

    // 测试用例5: 完全匹配
    std::string s5 = "abc";
    std::string t5 = "abc";
    std::cout << "Test Case 5:\n"
              << "s: " << s5 << "\nt: " << t5 << "\n"
              << "v1 result: " << sol.numDistinct1(s5, t5) << "\n"
              << "v2 result: " << sol.numDistinct2(s5, t5) << "\n"
              << "v3 result: " << sol.numDistinct3(s5, t5) << "\n";

    return 0;
}
