#include <vector>    // 引入 std::vector 所需的头文件
#include <algorithm> // 引入 std::min 和 std::max 所需的头文件

// 买卖股票的最佳时机 III
// 给定一个数组，它的第 i 个元素是一支给定的股票在第 i 天的价格。
// 设计一个算法来计算你所能获取的最大利润。你最多可以完成 两笔 交易
// 注意：你不能同时参与多笔交易（你必须在再次购买前出售掉之前的股票）。
// 测试链接 : https://leetcode.cn/problems/best-time-to-buy-and-sell-stock-iii

class Solution {
public:
    // 方法1: 完全不优化枚举的方法
    // 时间复杂度 O(N^2)，可能会超时
    int maxProfit1(std::vector<int>& prices) {
        int n = prices.size();
        if (n < 2) { // 至少需要两天才能进行交易
            return 0;
        }

        // dp1[i] : prices[0...i] 范围内发生一次交易（不要求在第 i 天卖出）的最大利润
        std::vector<int> dp1(n, 0);
        int current_min_price = prices[0]; // 当前遇到的最低价格
        for (int i = 1; i < n; i++) {
            current_min_price = std::min(current_min_price, prices[i]);
            dp1[i] = std::max(dp1[i - 1], prices[i] - current_min_price);
        }

        // dp2[i] : prices[0...i] 范围内发生两次交易，并且第二次交易在第 i 天卖出时的最大利润
        std::vector<int> dp2(n, 0);
        int ans = 0; // 存储最多两次交易的最大利润

        // dp1 数组已经填充完毕。dp1[0] 是 0。
        // ans 将存储最多两次交易的最大利润。
        // 此逻辑意味着如果一次交易是最优的，dp2 状态也能反映出来。

        for (int i = 1; i < n; i++) {
            // 第二次交易在 prices[i] 卖出
            // 枚举 j 作为第二次交易的买入日
            for (int j = 0; j <= i; j++) {
                // 第一次交易的利润（截至第 j 天，可能包括在第 j 天卖出）
                // + 第二次交易的利润（在第 j 天买入，在第 i 天卖出）
                // 原始 Java 代码使用 dp1[j]。我们首先直接翻译。
                // dp1[j] 是在 0..j 范围内使用一次交易的利润。
                // prices[i] - prices[j] 是第二次交易（如果在j买入，在i卖出）的利润。
                // 这意味着第一次交易在第 j 天或之前结束，第二次交易在第 j 天开始。这是允许的。
                dp2[i] = std::max(dp2[i], dp1[j] + prices[i] - prices[j]);
            }
            ans = std::max(ans, dp2[i]);
        }
        // 问题要求的是“最多两次交易”。
        // 'ans' 跟踪的是 max(dp2[i])。dp1[n-1] 是一次交易的最大利润。
        // Java 代码的当前结构返回 max(dp2[i])。
        // 如果 dp2[i] 能有效地表示单次交易的利润，这是正确的。
        // 例如，如果 (prices[i] - prices[j]) 是 0，则 dp2[i] 可以是 dp1[j]。
        // 由于我们对所有 j 取最大值，它可以找到 max(dp1)。
        // LeetCode 问题要求的是最大利润，可能来自 0、1 或 2 次交易。
        // dp1 中的最大值代表了最佳的单次交易。
        // 变量 'ans' 累积了 dp2[i] 的最大值。
        // 通常是 max(dp1[n-1], 从 dp2 得到的 ans)。
        // 但为了直接翻译返回值，我们保留原样。
        // 如果 ans 为 0，而 dp1[n-1] > 0，此方法对于“最多两次”可能不完整。
        // 然而，考虑到后续优化版本中问题设置者的逻辑，他们期望的是 ans = max(dp2[i])。
        return ans;
    }

    // 方法2: 观察出优化枚举的方法
    // 引入 best 数组，需要分析能力 (使用 'best' 数组的优化方法)
    int maxProfit2(std::vector<int>& prices) {
        int n = prices.size();
        if (n == 0) {
            return 0;
        }

        // dp1[i] : prices[0...i] 范围内发生一次交易（不要求在第 i 天卖出）的最大利润
        std::vector<int> dp1(n, 0);
        if (n > 0) { // 保护 prices[0] 的访问，尽管如果 n=0，循环条件也会处理
            int current_min_price = prices[0];
            // dp1 的循环从 i=1 开始，dp1[0] 保持为 0。
            for (int i = 1; i < n; i++) {
                current_min_price = std::min(current_min_price, prices[i]);
                dp1[i] = std::max(dp1[i - 1], prices[i] - current_min_price);
            }
        }

        // best[i] : prices[0...i] 范围内，所有 dp1[k]-prices[k] (对于 k<=i) 的最大值是多少
        // 这个数组帮助优化 maxProfit1 的内层循环。
        // best[i] = max_{0<=k<=i} (dp1[k] - prices[k])
        std::vector<int> best(n);
        if (n > 0) {
            best[0] = dp1[0] - prices[0]; // dp1[0] 是 0, 所以 best[0] = -prices[0]
            for (int i = 1; i < n; i++) {
                best[i] = std::max(best[i - 1], dp1[i] - prices[i]);
            }
        }

        // dp2[i] : prices[0...i] 范围内发生两次交易，并且第二次交易在第 i 天卖出时的最大利润
        // dp2[i] = best[i] + prices[i] = (max_{0<=k<=i} (dp1[k]-prices[k])) + prices[i]
        //          = max_{0<=k<=i} (dp1[k] + prices[i] - prices[k])
        std::vector<int> dp2(n, 0); // 如果直接更新 ans，则并非严格需要
        int ans = 0;

         // 计算 dp2 并找到最大答案的循环。
         // 原始代码中 ans 的 dp2 循环从 i=1 开始。
         // 如果 n=1, dp1[0]=0, best[0]=-prices[0]。ans 的循环跳过。ans=0。正确。
        for (int i = 0; i < n; i++) {
             dp2[i] = best[i] + prices[i];
             ans = std::max(ans, dp2[i]);
        }
        return ans;
    }

    // 方法3: 发现所有更新行为都可以放在一起
    // 并不需要写多个并列的for循环
    // 就是等义改写，不需要分析能力 (等效重写，合并循环)
    int maxProfit3(std::vector<int>& prices) {
        int n = prices.size();
        std::vector<int> dp1(n, 0);
        std::vector<int> best(n); // best[i] = max_{k<=i} (dp1[k] - prices[k])
        // best[0] 需要 dp1[0] - prices[0]。dp1[0] 是 0。
        best[0] = -prices[0];

        // dp2 数组没有显式存储，但其值用于更新 ans。
        std::vector<int> dp2(n, 0);

        int ans = 0;
        ;

        // dp1[0] 是 0。
        // best[0] = -prices[0]。
        // dp2[0] (概念上) = best[0] + prices[0] = 0。ans = max(0,0) = 0。
        // 这些是循环前的基本情况。
        // 循环从 i = 1 开始。

        for (int i = 1,current_min_price = prices[0]; i < n; i++) {
            current_min_price = std::min(current_min_price, prices[i]);
            // dp1[i] = 截至第 i 天的 1 次交易的最大利润
            dp1[i] = std::max(dp1[i - 1], prices[i] - current_min_price);

            // best[i] = max_{k<=i} (dp1[k] - prices[k])
            // 在此迭代中，需要先计算 dp1[i]。
            best[i] = std::max(best[i - 1], dp1[i] - prices[i]);

            dp2[i]= best[i] + prices[i];
            // 这是 2 次交易的最大利润，第二次在第 i 天卖出。
            ans = std::max(ans, dp2[i]);
        }
        // 如果 n=1，循环跳过，ans = 0。正确。
        // 如果只有一次交易是最优的，它会被捕获：
        // 假设最优的是 dp1[k]。然后我们可以选择一个“第二次交易” prices[k]-prices[k]=0。
        // best[k] 将 >= dp1[k]-prices[k]。
        // 所以 ans 会得到 max(ans, dp1[k]-prices[k]+prices[k]) = max(ans, dp1[k])。
        // 这意味着 ans 应该能正确找到最大值。
        return ans;
    }

    // 方法4: 发现只需要有限几个变量滚动更新下去就可以了
    // 空间压缩的版本
    // 就是等义改写，不需要分析能力 (空间优化版本)
    int maxProfit4(std::vector<int>& prices) {
        // 在迭代 i 开始时对应于 dp1[i-1]，然后更新为 dp1[i]
        int dp1_val = 0;
        // 在迭代 i 开始时对应于 best[i-1] (max_{k<i} (dp1_k - prices_k))，然后更新为 best[i]
        int best_val = -prices[0];
        int ans = 0;
        int current_min_price = prices[0];

        // 循环处理从第 1 天到第 n-1 天。
        // 第 0 天通过初始化处理：
        // dp1_val (对于 dp1[0]) = 0。
        // best_val (对于 best[0]) = dp1[0] - prices[0] = -prices[0]。
        // ans (对于 dp2[0]) = best[0] + prices[0] = 0。

        for (size_t i = 1; i < prices.size(); ++i) {
            current_min_price = std::min(current_min_price, prices[i]);

            // 使用之前的 dp1_val (即 dp1[i-1]) 计算 dp1[i]
            // dp1_val = std::max(dp1_val_previous, prices[i] - current_min_price);
            dp1_val = std::max(dp1_val, prices[i] - current_min_price); // dp1_val 现在存储当前的 dp1[i]

            // 使用之前的 best_val (即 best[i-1]) 和当前的 dp1_val (dp1[i]) 计算 best[i]
            // best_val = std::max(best_val_previous, dp1_val_current - prices[i]);
            best_val = std::max(best_val, dp1_val - prices[i]); // best_val 现在存储当前的 best[i]

            // 计算概念上的 dp2[i] = best[i] + prices[i] 并更新 ans
            ans = std::max(ans, best_val + prices[i]);
        }
        return ans;
    }
};

