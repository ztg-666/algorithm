// 买卖股票的最佳时机
// 给定一个数组 prices ，它的第 i 个元素 prices[i] 表示一支给定股票第 i 天的价格
// 你只能选择 某一天 买入这只股票，并选择在 未来的某一个不同的日子 卖出该股票
// 设计一个算法来计算你所能获取的最大利润
// 返回你可以从这笔交易中获取的最大利润
// 如果你不能获取任何利润，返回 0
// 测试链接 : https://leetcode.cn/problems/best-time-to-buy-and-sell-stock/

#include <vector>       // 使用 std::vector
#include <algorithm>    // 使用 std::min 和 std::max

class Solution {
public:
    int maxProfit(const std::vector<int>& prices) {
        // 根据 LeetCode 的约束 (1 <= prices.length <= 10^5),
        // prices 不会为空，所以 prices[0] 的访问是安全的。
        // 如果向量可能为空，则需要进行初始检查。
        if (prices.empty()) {
            return 0; // 或者根据空输入的具体要求进行处理
        }

        int max_profit = 0;             // 初始化最大利润为0
        int min_price_so_far = prices[0]; // 初始化到目前为止的最低价格为第一天的价格

        // 从第二天开始遍历
        for (size_t i = 1; i < prices.size(); ++i) {
            // 更新到目前为止（包括今天）找到的最低价格
            min_price_so_far = std::min(min_price_so_far, prices[i]);
            // 计算如果在今天以 prices[i] 的价格卖出（在 min_price_so_far 时买入）所能获得的利润
            // 注意：如果 prices[i] 是新的 min_price_so_far，那么 prices[i] - min_price_so_far 的结果是 0。
            max_profit = std::max(max_profit, prices[i] - min_price_so_far);
        }

        return max_profit;
    }
};

// 如何使用的示例（可选，用于测试）：

#include <iostream>
int main() {
    Solution sol;
    std::vector<int> prices1 = {7, 1, 5, 3, 6, 4};
    std::cout << " {7, 1, 5, 3, 6, 4} : " << sol.maxProfit(prices1) << std::endl; // 预期: 5

    std::vector<int> prices2 = {7, 6, 4, 3, 1};
    std::cout << " {7, 6, 4, 3, 1} : " << sol.maxProfit(prices2) << std::endl; // 预期: 0

    std::vector<int> prices3 = {1, 2, 3, 4, 5};
    std::cout << " {1, 2, 3, 4, 5} : " << sol.maxProfit(prices3) << std::endl; // 预期: 4

    std::vector<int> prices_single = {1};
    std::cout << " {1} : " << sol.maxProfit(prices_single) << std::endl; // 预期: 0

    std::vector<int> prices_empty = {};
    std::cout << " {} : " << sol.maxProfit(prices_empty) << std::endl; // 预期: 0 (如果启用了空检查)

    return 0;
}

