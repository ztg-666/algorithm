#include <vector>      // 引入 vector 容器
#include <numeric>     // 可能用于 std::accumulate 或 std::partial_sum，尽管这里用了手动求和
#include <algorithm>   // 引入 std::min, std::max 等算法
// 从栈中取出K个硬币的最大面值和
// 一张桌子上总共有 n 个硬币 栈 。每个栈有 正整数 个带面值的硬币
// 每一次操作中，你可以从任意一个栈的 顶部 取出 1 个硬币，从栈中移除它，并放入你的钱包里
// 给你一个列表 piles ，其中 piles[i] 是一个整数数组
// 分别表示第 i 个栈里 从顶到底 的硬币面值。同时给你一个正整数 k
// 请你返回在 恰好 进行 k 次操作的前提下，你钱包里硬币面值之和 最大为多少
// 测试链接 : https://leetcode.cn/problems/maximum-value-of-k-coins-from-piles/

// 问题描述：
// 给定 n 个硬币栈 (piles)，其中 piles[i] 是一个列表，表示第 i 个栈里从顶到底的硬币面值。
// 你必须执行 恰好 k 次操作。每次操作中，你可以从任意一个栈的顶部取出 1 个硬币。
// 返回你能收集到的硬币的最大总面值。

// 这个问题可以看作是背包问题的一个变种（特别是分组背包）。
// 每个栈 (pile) 就是一个“分组”。
// 第 'i' 组中的“物品”是：
// - 取走顶部 1 个硬币（成本 = 1 次操作，价值 = 第 1 个硬币的面值）
// - 取走顶部 2 个硬币（成本 = 2 次操作，价值 = 第 1 和第 2 个硬币的面值之和）
// - ...
// - 取走顶部 'x' 个硬币（成本 = x 次操作，价值 = 顶部 x 个硬币的面值之和）
// “背包容量”就是 'k'（允许的操作次数）。
// 对于每个分组（栈），你最多只能选择一个“物品”（即，你决定从该栈顶部连续取走多少个硬币，可以是 0 个，最多到栈的大小）。

class Solution {
public:
    // 方法一：使用二维动态规划
    // dp[i][j] = 表示考虑前 'i' 个栈，执行恰好 'j' 次操作（取走 j 个硬币）所能获得的最大价值
    int maxValueOfCoins1(const std::vector<std::vector<int>>& piles, int k) {
        int n = piles.size(); // 栈的数量

        // dp[i][j]：使用前 'i' 个栈，取走恰好 'j' 个硬币的最大价值
        // 用 0 初始化。维度是 (n+1) x (k+1)，以便对栈使用 1-based 索引。
        std::vector<std::vector<int>> dp(n + 1, std::vector<int>(k + 1, 0));

        // 遍历各个栈（对 dp 表使用 1-based 索引 'i'）
        for (int i = 1; i <= n; ++i) {
            // 获取当前栈（对输入 'piles' 使用 0-based 索引 'i-1'）
            const std::vector<int>& current_pile = piles[i - 1];
            int pile_size = current_pile.size(); // 当前栈的大小

            // 计算当前栈的前缀和，以便快速获取顶部 'x' 个硬币的总价值。
            // max_coins_from_pile: 我们可能从这个特定栈中取出的最大硬币数。
            int max_coins_from_pile = std::min((int)pile_size, k); // 最多取 pile_size 个，且不超过总次数 k
            std::vector<int> preSum(max_coins_from_pile + 1, 0); // 前缀和数组，大小为 max_coins_from_pile + 1
            int current_sum = 0;
            for (int coin_idx = 0; coin_idx < max_coins_from_pile; ++coin_idx) {
                current_sum += current_pile[coin_idx];
                preSum[coin_idx + 1] = current_sum; // preSum[x] 存储前 x 个硬币的和
            }

            // 遍历总共取的硬币数 'j'（从 0 到 k）
            for (int j = 0; j <= k; ++j) {
                // 基础情况：不从当前栈 'i' 取任何硬币。
                // 价值与只使用前 'i-1' 个栈取 'j' 个硬币时相同。
                dp[i][j] = dp[i - 1][j];

                // 尝试从当前栈 'i' 取 'num_take' 个硬币（从 1 到 max_coins_from_pile）。
                // num_take 代表从当前栈顶连续取出硬币的数量
                for (int num_take = 1; num_take <= max_coins_from_pile; ++num_take) {
                    // 检查是否有足够的操作次数 'j' 来取 'num_take' 个硬币。
                    if (j >= num_take) {
                        // 如果我们从栈 'i' 取 'num_take' 个硬币，计算得到的价值：
                        // 它是来自前面 i-1 个栈、使用剩余操作次数 (j - num_take) 的最大价值
                        // 加上从当前栈取的 'num_take' 个硬币的价值 (preSum[num_take])。
                        // 在所有可能的 num_take 中取最大值。
                        dp[i][j] = std::max(dp[i][j], dp[i - 1][j - num_take] + preSum[num_take]);
                    }
                }
            }
        }
        // 最终答案是使用所有 'n' 个栈，执行恰好 'k' 次操作的最大价值。
        return dp[n][k];
    }

    // 方法二：空间优化的动态规划（一维 DP）
    // dp[j] = 表示在考虑了目前处理过的所有栈后，执行恰好 'j' 次操作（取走 j 个硬币）所能获得的最大价值
    int maxValueOfCoins2(const std::vector<std::vector<int>>& piles, int k) {
        int n = piles.size(); // 栈的数量

        // dp[j]: 使用 'j' 次操作从当前已考虑的栈中能获得的最大价值。
        // 用 0 初始化。大小为 (k+1)。
        std::vector<int> dp(k + 1, 0);

        // 遍历每一个栈
        for (int i = 0; i < n; ++i) { // 使用 0-based 索引遍历栈
            const std::vector<int>& current_pile = piles[i];
            int pile_size = current_pile.size();

            // 计算当前栈的前缀和
            int max_coins_from_pile = std::min((int)pile_size, k);
            std::vector<int> preSum(max_coins_from_pile + 1, 0);
            int current_sum = 0;
            for (int coin_idx = 0; coin_idx < max_coins_from_pile; ++coin_idx) {
                current_sum += current_pile[coin_idx];
                preSum[coin_idx + 1] = current_sum;
            }

            // **关键：**逆序遍历总操作次数 'j'（从 k 到 1）。
            // 这对于一维优化至关重要，确保在计算 dp[j] 时，
            // 所依赖的 dp[j - num_take] 是 *在考虑从当前栈取硬币之前* 的状态（即上一轮或更早的状态）。
            for (int j = k; j >= 1; --j) {
                // 尝试从当前栈取 'num_take' 个硬币（从 1 到 max_coins_from_pile）。
                for (int num_take = 1; num_take <= max_coins_from_pile; ++num_take) {
                    // 检查是否有足够的操作次数 'j' 来取 'num_take' 个硬币。
                    if (j >= num_take) {
                         // 通过尝试从当前栈取 'num_take' 个硬币来更新 dp[j]。
                         // 比较当前的 dp[j]（在本次内层循环迭代中，*不* 从当前栈选择取num_take个硬币时的最大值）
                         // 与 通过从当前栈取 'num_take' 个硬币获得的价值 (preSum[num_take])
                         // 加上 使用剩余操作次数 (j - num_take) 从先前状态获得的最大价值 (dp[j - num_take])。
                        dp[j] = std::max(dp[j], dp[j - num_take] + preSum[num_take]);
                    }
                }
            }
        }
        // 最终答案是在考虑了所有栈之后，使用恰好 'k' 次操作的最大价值。
        return dp[k];
    }

    // 通常你会调用这两个方法中的一个，一般是空间优化的版本：
    int maxValueOfCoins(std::vector<std::vector<int>>& piles, int k) {
        // return maxValueOfCoins1(piles, k); // 二维 DP 版本
        return maxValueOfCoins2(piles, k); // 一维 DP (空间优化) 版本
    }
};
