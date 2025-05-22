#include <vector>
#include <string>
#include <numeric>
using ll = long long;
// DI序列的有效排列
// 给定一个长度为n的字符串s，其中s[i]是:
// "D"意味着减少，"I"意味着增加
// 有效排列是对有n+1个在[0,n]范围内的整数的一个排列perm，使得对所有的i：
// 如果 s[i] == 'D'，那么 perm[i] > perm[i+1]
// 如果 s[i] == 'I'，那么 perm[i] < perm[i+1]
// 返回有效排列的perm的数量
// 因为答案可能很大，答案对 1000000007 取模
// 测试链接 : https://leetcode.cn/problems/valid-permutations-for-di-sequence/

// DI序列有效排列问题：根据D/I模式生成符合条件的排列数
class Solution {
private:
    /**
     * 递归解法辅助函数（带记忆化搜索）
     * @param s DI模式字符串
     * @param i 当前处理位置（0-based）
     * @param less 当前可用数字中比前一个数字小的数量（关键状态参数）
     * @param n 排列总长度（s.length()+1）
     * @return 从当前位置开始的有效排列数
     */
    int f(const std::string& s, int i, int less, int n) {
        if (i == n) return 1; // 递归终止条件：完成所有位置的填充

        int ans = 0;
        if (i == 0 || s[i-1] == 'D') { // 处理递减情况
            // 遍历所有可能的较小值数量（必须严格小于当前less）
            for (int nextLess = 0; nextLess < less; ++nextLess) {
                ans += f(s, i+1, nextLess, n);
            }
        } else { // 处理递增情况
            // nextLess的起始值为less，通过k控制循环次数
            for (int nextLess = less, k = 1; k <= n - i - less; k++, nextLess++) {
                ans += f(s, i+1, nextLess, n);
            }
        }
        return ans;
    }

public:
    // 方法1：递归解法（时间复杂度O(n^3)，会超时，仅用于理解问题）
    int numPermsDISequence1(std::string s) {
        return f(s, 0, s.length()+1, s.length()+1);
    }

    // 方法2：动态规划基础版（时间复杂度O(n^3)）
    int numPermsDISequence2(std::string s) {
        const ll mod = 1000000007;
        const ll n = s.length() + 1;
        // dp[i][less] 表示处理到第i位时，可用较小数数量为less的方案数
        std::vector<std::vector<ll>> dp(n+1, std::vector<ll>(n+1, 0));

        // 初始化：当处理完所有位置时，方案数为1
        for (ll less = 0; less <= n; ++less) dp[n][less] = 1;

        // 逆向填充DP表
        for (ll i = n-1; i >= 0; --i) {
            for (ll less = 0; less <= n; ++less) {
                if (i == 0 || s[i-1] == 'D') { // 递减情况
                    for (ll nextLess = 0; nextLess < less; ++nextLess) {
                        dp[i][less] = (dp[i][less] + dp[i+1][nextLess]) % mod;
                    }
                } else { // 递增情况
                    for (ll nextLess = less, k = 1; k <= n - i - less; ++k, ++nextLess) {
                        dp[i][less] = (dp[i][less] + dp[i+1][nextLess]) % mod;
                    }
                }
            }
        }
        return dp[0][n]; // 返回初始状态的方案数
    }

    // 方法3：优化后的动态规划（时间复杂度O(n^2)）
    int numPermsDISequence3(std::string s) {
        const int mod = 1000000007;
        const int n = s.length() + 1;
        std::vector<std::vector<ll>> dp(n+1, std::vector<ll>(n+1, 0));

        // 初始化基准情况
        for (int less = 0; less <= n; ++less) dp[n][less] = 1;

        // 逆向DP + 前缀和优化
        for (int i = n-1; i >= 0; --i) {
            if (i == 0 || s[i-1] == 'D') {
                // 递减情况：使用前缀和优化累加过程
                dp[i][1] = dp[i+1][0];
                for (int less = 2; less <= n; ++less) {
                    // dp[i][less] = sum(dp[i+1][0..less-1])
                    dp[i][less] = (dp[i][less-1] + dp[i+1][less-1]) % mod;
                }
            } else {
                // 递增情况：使用后缀和优化累加过程
                dp[i][n-i-1] = dp[i+1][n-i-1];
                for (int less = n-i-2; less >= 0; --less) {
                    // dp[i][less] = sum(dp[i+1][less..n-i-1])
                    dp[i][less] = (dp[i][less+1] + dp[i+1][less]) % mod;
                }
            }
        }
        return dp[0][n];
    }
};
