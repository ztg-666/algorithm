#include <vector>
#include <algorithm> // For std::max

// 买卖股票的最佳时机含手续费
// 给定一个整数数组 prices，其中 prices[i]表示第 i 天的股票价格
// 整数 fee 代表了交易股票的手续费用
// 你可以无限次地完成交易，但是你每笔交易都需要付手续费
// 如果你已经购买了一个股票，在卖出它之前你就不能再继续购买股票了。
// 返回获得利润的最大值
// 注意：这里的一笔交易指买入持有并卖出股票的整个过程，每笔交易你只需要为支付一次手续费
// 测试链接 : https://leetcode.cn/problems/best-time-to-buy-and-sell-stock-with-transaction-fee/

class Solution {
public:
    int maxProfit(std::vector<int>& prices, int fee) {
        if (prices.empty()) {
            return 0;
        }

        // prepare: 表示在当前天，如果持有股票（之前某天买入，并支付了手续费）时的最大现金。
        //          初始化为第0天买入股票并支付手续费后的现金。
        //          更准确地说，是（之前某次卖出后的现金 - 当前买入价 - 手续费）的最大值。
        int prepare = -prices[0] - fee;

        // done: 表示在当前天，如果不持有股票（之前某天卖出）时的最大现金。
        //       初始化为0，因为开始时没有交易，现金为0。
        int done = 0;

        for (size_t i = 1; i < prices.size(); ++i) {
            // 今天不持有股票（done）的最大现金:
            // 1. 昨天就不持有股票 (previous done)
            // 2. 昨天持有股票，今天卖出 (previous prepare + prices[i])
            //    注意：手续费在买入时已经扣除或者说包含在prepare的定义里
            done = std::max(done, prepare + prices[i]);

            // 今天持有股票（prepare）的最大现金:
            // 1. 昨天就持有股票 (previous prepare)
            // 2. 昨天不持有股票，今天买入 (previous done - prices[i] - fee)
            prepare = std::max(prepare, done - prices[i] - fee);
        }

        // 最终返回不持有股票状态下的最大现金，因为持有股票意味着亏损（未实现的收益）
        return done;
    }
};

