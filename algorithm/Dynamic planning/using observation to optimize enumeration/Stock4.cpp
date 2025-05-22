#include <vector>
#include <algorithm> // For std::max

// 买卖股票的最佳时机 IV
// 给你一个整数数组 prices 和一个整数 k ，其中 prices[i] 是某支给定的股票在第 i 天的价格
// 设计一个算法来计算你所能获取的最大利润。你最多可以完成 k 笔交易
// 也就是说，你最多可以买 k 次，卖 k 次
// 注意：你不能同时参与多笔交易，你必须在再次购买前出售掉之前的股票
// 测试链接 : https://leetcode.cn/problems/best-time-to-buy-and-sell-stock-iv/

class Solution {
public:
    // 辅助函数：当交易次数无限制时的最大利润 (来自 "买卖股票的最佳时机 II")
    // Helper function: max profit with unlimited transactions (from "Best Time to Buy and Sell Stock II")
    int free_transactions(const std::vector<int>& prices) {
        int ans = 0;
        if (prices.size() < 2) {
            return 0;
        }
        for (size_t i = 1; i < prices.size(); ++i) {
            ans += std::max(prices[i] - prices[i - 1], 0);
        }
        return ans;
    }

    // 方法1: O(k * N^2) - 朴素DP，可能会超时
    // Method 1: O(k * N^2) - Naive DP, likely to TLE
    int maxProfit1(int k, std::vector<int>& prices) {
        int n = prices.size();

        // 剪枝：如果k足够大，相当于交易次数无限制
        // Pruning: if k is large enough, it's like unlimited transactions
        if (k >= n / 2) {
            return free_transactions(prices);
        }

        // dp[i][j]: 进行至多 i 笔交易，在第 j 天结束时的最大利润
        // dp[i][j]: max profit using at most i transactions up to day j
        std::vector<std::vector<int>> dp(k + 1, std::vector<int>(n, 0));

        for (int i = 1; i <= k; ++i) { // 交易次数 i
            for (int j = 1; j < n; ++j) { // 第 j 天
                // 选择1: 第 i 笔交易不在第 j 天卖出 (或者说不考虑在第j天操作第i笔交易的卖出)
                // 利润与在第 j-1 天结束时相同
                dp[i][j] = dp[i][j - 1];

                // 选择2: 第 i 笔交易在第 j 天卖出
                // 枚举买入时机 p < j
                // 利润 = (i-1)笔交易在第 p 天结束的最大利润 + (prices[j] - prices[p])
                for (int p = 0; p < j; ++p) { // 买入时机 p
                    dp[i][j] = std::max(dp[i][j], dp[i - 1][p] + prices[j] - prices[p]);
                }
            }
        }
        return dp[k][n - 1];
    }

    // 方法2: O(k * N) - 优化DP
    // Method 2: O(k * N) - Optimized DP
    int maxProfit2(int k, std::vector<int>& prices) {
        int n = prices.size();
        if (n == 0 || k == 0) {
            return 0;
        }
        if (k >= n / 2) {
            return free_transactions(prices);
        }

        // dp[i][j]: 进行至多 i 笔交易，在第 j 天结束时的最大利润
        std::vector<std::vector<int>> dp(k + 1, std::vector<int>(n, 0));

        for (int i = 1; i <= k; ++i) { // 交易次数 i
            // best = max_{p<j} (dp[i-1][p] - prices[p])
            // 对于第i笔交易，考虑在之前的某天p买入，而在p天之前已经完成了i-1笔交易
            // 初始化：如果第i笔交易的买入点是第0天，则此值为 dp[i-1][0] - prices[0]
            // dp[i-1][0] (i-1次交易，截至第0天) 总是0
            int best = -prices[0];

            for (int j = 1; j < n; ++j) { // 第 j 天
                // 选择1: 不在第 j 天卖出第 i 笔交易
                dp[i][j] = dp[i][j - 1];

                // 选择2: 在第 j 天卖出第 i 笔交易
                // 利润 = max_{p<j} (dp[i-1][p] - prices[p]) + prices[j]
                dp[i][j] = std::max(dp[i][j], best + prices[j]);

                // 更新 best 以备下一天 (j+1) 使用
                // 考虑在当前第 j 天买入，作为第 i 笔交易的买入点
                best = std::max(best, dp[i - 1][j] - prices[j]);
            }
        }
        return dp[k][n - 1];
    }

    // 方法3: O(k * N) 时间, O(N) 空间 - 空间优化的DP

    int maxProfit3(int k, std::vector<int>& prices) {
        int n = prices.size();

        if (k >= n / 2) {
            return free_transactions(prices);
        }

        // dp[j] 将存储使用当前交易次数 i，在第 j 天结束时的最大利润
        std::vector<int> dp(n, 0);

        for (int i = 1; i <= k; ++i) { // 当前是第 i 笔交易
            // best = max_{p<j} (dp_for_prev_tx[p] - prices[p])
            // dp_for_prev_tx[p] 指的是 dp[i-1][p]
            // 初始化时，考虑第0天买入: dp[i-1][0] - prices[0]. dp[i-1][0]总是0.
            int best = -prices[0]; // n > 0 已由前置检查保证

            // tmp 用于存储 dp[i-1][j] 的值
            // 因为 dp[j] 会被覆盖更新为 dp[i][j]
            // 但更新 best 时需要 dp[i-1][j]
            int tmp;

            // 内部循环从 j=1 开始。dp[0] (即 dp[i][0]) 始终为0，因为它代表在第0天结束、进行i次交易的利润。
            // 在 j 循环开始前，dp 数组中存储的是第 i-1 次交易的结果。
            // 所以 best 的初始化使用了 dp[i-1][0] (隐式地为0) - prices[0]。
            // dp[0] 在内层循环中不会被 `dp[j] = std::max(dp[j-1], ...)` 修改，因此它保持为0 (dp[i][0]=0)。

            for (int j = 1; j < n; ++j) { // 第 j 天
                tmp = dp[j]; // 保存 dp[i-1][j]

                // dp[j] (更新为 dp[i][j]) 取以下两者最大值:
                // 1. dp[i][j-1]: 即当前的 dp[j-1]，它是在本次i交易迭代中，j-1时刻算好的值
                // 2. best + prices[j]: 第i笔交易在第j天卖出
                dp[j] = std::max(dp[j - 1], best + prices[j]);

                // 为下一天 (j+1) 更新 best
                // 考虑在当前第 j 天买入作为第 i 笔交易的买入点
                // (dp[i-1][j] - prices[j])
                best = std::max(best, tmp - prices[j]);
            }
        }
        return dp[n - 1];
    }
};
