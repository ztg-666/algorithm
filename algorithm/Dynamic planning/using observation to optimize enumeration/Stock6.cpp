#include <vector>    // 引入 std::vector 所需的头文件
#include <algorithm> // 引入 std::max 所需的头文件

// 买卖股票的最佳时机含冷冻期
// 给定一个整数数组prices，其中第 prices[i] 表示第 i 天的股票价格
// 设计一个算法计算出最大利润
// 在满足以下约束条件下，你可以尽可能地完成更多的交易（多次买卖一支股票）:
// 卖出股票后，你无法在第二天买入股票 (即冷冻期为 1 天)
// 注意：你不能同时参与多笔交易（你必须在再次购买前出售掉之前的股票）
// 测试链接 : https://leetcode.cn/problems/best-time-to-buy-and-sell-stock-with-cooldown/

class Solution {
public:
    // 方法1: 使用DP数组
    int maxProfit1(std::vector<int>& prices) {
        int n = prices.size();
        if (n < 2) {
            return 0; // 天数不足以完成一次交易和冷冻期
        }

        // prepare_dp[i]: 在第i天结束时，持有股票状态下的最大利润。
        // (第 i 天的利润，以“持有”状态结束，已经购买了股票)
        std::vector<int> prepare_dp(n);

        // done_dp[i]: 在第i天结束时，不持有股票状态下的最大利润。
        // (第 i 天的利润，以“不持有”状态结束，可能是在卖出后或休息)
        // std::vector 初始化元素为 0, 所以 done_dp[0] 将为 0。
        std::vector<int> done_dp(n);


        // 第 0 天和第 1 天的基本情况
        // 对于第 0 天:
        // 如果在第 0 天买入: prepare_dp[0] = -prices[0]
        // 如果什么都不做 (不持有): done_dp[0] = 0 (vector 默认初始化)

        // 对于第 1 天:
        // prepare_dp[1]: 第 1 天持有股票
        //   - 第 0 天买入, 第 1 天持有: -prices[0]
        //   - 第 1 天买入: -prices[1] (意味着在第 -1 天处于“完成”状态，第 0 天冷冻，然后在第 1 天买入)
        prepare_dp[1] = std::max(-prices[0], -prices[1]);

        // done_dp[1]: 第 1 天不持有股票
        //   - 直到第 1 天什么都没做: 0
        //   - 第 0 天买入, 第 1 天卖出: prices[1] - prices[0]
        done_dp[1] = std::max(0, prices[1] - prices[0]);
        // done_dp[0] 是 0, 将在循环中用于计算 prepare_dp[2]

        for (int i = 2; i < n; ++i) {
            // done_dp[i]: 第 i 天不持有股票的最大利润
            //   1. 第 i-1 天就不持有 (休息): done_dp[i-1]
            //   2. 第 i-1 天持有并在第 i 天卖出: prepare_dp[i-1] + prices[i]
            done_dp[i] = std::max(done_dp[i - 1], prepare_dp[i - 1] + prices[i]);

            // prepare_dp[i]: 第 i 天持有股票的最大利润
            //   1. 第 i-1 天就持有 (继续持有): prepare_dp[i-1]
            //   2. 第 i-2 天不持有 (done_dp[i-2]), 第 i-1 天是冷冻期, 在第 i 天买入: done_dp[i-2] - prices[i]
            prepare_dp[i] = std::max(prepare_dp[i - 1], done_dp[i - 2] - prices[i]);
        }

        // 最大利润将是最后一天不持有股票时的利润。
        return done_dp[n - 1];
    }

    // 方法2: 空间优化的DP (变量滚动更新)
    int maxProfit2(std::vector<int>& prices) {
        int n = prices.size();
        if (n < 2) {
            return 0;
        }

        // prepare_val 对应于迭代 i（计算第 i 天）开始时的 prepare_dp[i-1]，
        // 然后更新为 prepare_dp[i]。
        // 初始化为 prepare_dp[1] 的值。
        int prepare_val = std::max(-prices[0], -prices[1]);

        // done2_val 对应于 done_dp[i-2]。
        // 初始化为 done_dp[0] 的值。
        int done2_val = 0;

        // done1_val 对应于 done_dp[i-1]。
        // 初始化为 done_dp[1] 的值。
        int done1_val = std::max(0, prices[1] - prices[0]);

        // 针对第 i 天的循环，从 2 到 n-1。
        // 循环结束后，done1_val 将持有 done_dp[n-1]。
        for (int i = 2; i < n; ++i) {
            // current_done_val 将是 done_dp[i]
            int current_done_val = std::max(done1_val, prepare_val + prices[i]);

            // 更新 prepare_val 为 prepare_dp[i]
            // 使用旧的 prepare_val (prepare_dp[i-1]) 和 done2_val (done_dp[i-2])
            prepare_val = std::max(prepare_val, done2_val - prices[i]);

            // 为下一次迭代移动 done 状态
            done2_val = done1_val;    // done_dp[i-1] 变为下一次迭代的 done_dp[i-2]
            done1_val = current_done_val; // done_dp[i] 变为下一次迭代的 done_dp[i-1]
        }

        return done1_val; // 这是 done_dp[n-1]
    }
};
