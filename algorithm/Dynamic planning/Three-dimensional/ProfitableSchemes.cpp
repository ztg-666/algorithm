#include <vector>
#include <numeric>   // 可能有用，但使用向量构造函数进行初始化通常更好
#include <algorithm> // 用于 std::max


// 盈利计划(多维费用背包)
// 集团里有 n 名员工，他们可以完成各种各样的工作创造利润
// 第 i 种工作会产生 profit[i] 的利润，它要求 group[i] 名成员共同参与
// 如果成员参与了其中一项工作，就不能参与另一项工作
// 工作的任何至少产生 minProfit 利润的子集称为 盈利计划
// 并且工作的成员总数最多为 n
// 有多少种计划可以选择？因为答案很大，答案对 1000000007 取模
// 测试链接 : https://leetcode.cn/problems/profitable-schemes/


class Solution {
private:
    const int MOD = 1000000007;

    // --- 方法 1：普通递归法 ---
    // 似乎与动态规划逻辑略有不同。这个 C++ 版本遵循从动态规划状态推导出来的逻辑。
    // i：当前工作索引
    // r：剩余员工数量
    // s：达到最小利润目标所需的剩余利润
    int solve_recursive(const std::vector<int>& g, const std::vector<int>& p, int i, int r, int s) {
        // 基本情况：没有更多工作可供考虑
        if (i == g.size()) {
            // 如果所需的剩余利润为 0 或更少，则达到了目标。
            return s <= 0 ? 1 : 0;
        }

        // 选项 1：不选择工作 i
        // 使用 long long 类型存储中间和，以防止在取模前可能出现的溢出
        long long p1 = solve_recursive(g, p, i + 1, r, s);

        // 选项 2：选择工作 i（如果可行）
        long long p2 = 0;
        if (g[i] <= r) {
            // 计算新的所需剩余利润。
            // 如果 s - p[i] 变为负数，意味着已经达到了利润目标。
            // 我们使用 std::max(0,...) 将所需的剩余利润上限设为 0。
            p2 = solve_recursive(g, p, i + 1, r - g[i], std::max(0, s - p[i]));
        }

        // 返回方案总数，对 MOD 取模
        return (p1 + p2) % MOD;
    }


    // --- 方法 2：记忆化搜索法---
    std::vector<std::vector<std::vector<int>>> memo;
    int num_jobs_memo; // 存储工作数量，避免在递归中传递它

    // i：当前工作索引
    // r：剩余员工数量
    // s：所需的剩余利润（下限为 0）
    int solve_memo(const std::vector<int>& g, const std::vector<int>& p, int i, int r, int s) {
         // 基本情况：没有更多工作
        if (i == num_jobs_memo) {
            return s <= 0? 1 : 0; // 检查是否达到了利润目标
        }

        // 使用上限为 0 的利润 's' 作为记忆化搜索的索引（始终 >= 0）
        int current_s = std::max(0, s);

        // 检查记忆化表
        if (memo[i][r][current_s] != -1) {
            return memo[i][r][current_s];
        }

        // 选项 1：不选择工作 i
        long long p1 = solve_memo(g, p, i + 1, r, current_s); // 传递上限为 0 的 's'

        // 选项 2：选择工作 i（如果可行）
        long long p2 = 0;
        if (g[i] <= r) {
            // 传递正确更新（且上限为 0）的所需剩余利润
            p2 = solve_memo(g, p, i + 1, r - g[i], std::max(0, current_s - p[i]));
        }

        // 将结果存储在记忆化表中并返回
        long long ans = (p1 + p2) % MOD;
        memo[i][r][current_s] = static_cast<int>(ans);
        return memo[i][r][current_s];
    }

public:
    // --- 递归方法的接口 ---
    int profitableSchemes1(int n, int minProfit, const std::vector<int>& group, const std::vector<int>& profit) {
        // 初始调用：从工作 0 开始，有 'n' 名员工，需要达到 'minProfit' 的利润
        return solve_recursive(group, profit, 0, n, minProfit);
    }

    // --- 记忆化搜索方法的接口 ---
    int profitableSchemes2(int n, int minProfit, const std::vector<int>& group, const std::vector<int>& profit) {
        num_jobs_memo = group.size();
        // 用 -1 初始化记忆化表（大小：工作数量 x (n + 1) x (minProfit + 1)）
        memo.assign(num_jobs_memo,
                    std::vector<std::vector<int>>(n + 1,
                    std::vector<int>(minProfit + 1, -1)));

        // 初始调用：从工作 0 开始，有 'n' 名员工，需要达到 'minProfit' 的利润
        // solve_memo 函数会在内部处理状态中对's' 进行上限设置。
        return solve_memo(group, profit, 0, n, minProfit);
    }

    // --- 方法 3：空间优化的自底向上动态规划法 ---
    int profitableSchemes(int n, int minProfit, const std::vector<int>& group, const std::vector<int>& profit) {
        int num_jobs = group.size();
        // dp[r][s]：使用最多 r 名员工且实现利润 >= s 的方案数量
        // 维度：(n + 1) 名员工，(minProfit + 1) 所需利润
        std::vector<std::vector<int>> dp(n + 1, std::vector<int>(minProfit + 1, 0));

        // 基本情况：实现利润要求为 0（或更低）的情况
        // 总是有一种方案（即不选择任何工作）。这适用于任何员工数量。
        for (int r = 0; r <= n; ++r) {
            dp[r][0] = 1;
        }

        // 遍历每个工作
        for (int i = 0; i < num_jobs; ++i) {
            int current_group = group[i];
            int current_profit = profit[i];

            // 反向遍历员工数量 (r) 和所需利润 (s)
            // 这确保我们使用的是 *上一个* 工作迭代的结果
            for (int r = n; r >= current_group; --r) {
                for (int s = minProfit; s >= 0; --s) {
                    // 计算在选择工作 i *之前* 所需的利润状态
                    // 如果我们现在需要 's' 的利润，而工作 'i' 能带来 'current_profit' 的利润，
                    // 那么我们之前必须需要 's - current_profit' 的利润。
                    // 将这个之前所需的利润下限设为 0。
                    int prev_s = std::max(0, s - current_profit);

                    // 将达到状态 (r - current_group, prev_s) 的方案数量相加
                    // 这代表了我们 *确实* 选择了工作 i 的方案。
                    // dp[r][s] 已经保存了 *不* 选择工作 i 的方案数量（来自上一次迭代）。
                    dp[r][s] = (dp[r][s] + dp[r - current_group][prev_s]) % MOD;
                }
            }
        }

        // 使用最多 'n' 名员工且达到 'minProfit' 利润要求的方案数量。
        return dp[n][minProfit];
    }
};

// 示例用法（可选）
#include <iostream>

int main() {
    Solution sol;
    std::vector<int> group1 = {2, 2};
    std::vector<int> profit1 = {2, 3};
    int n1 = 5, minProfit1 = 3;
    std::cout << "Result 1 (DP): " << sol.profitableSchemes(n1, minProfit1, group1, profit1) << std::endl; // 输出: 2
    std::cout << "Result 1 (Memo): " << sol.profitableSchemes2(n1, minProfit1, group1, profit1) << std::endl; // 输出: 2
    // std::cout << "Result 1 (Rec): " << sol.profitableSchemes1(n1, minProfit1, group1, profit1) << std::endl; // 输出: 2（可能较慢）


    std::vector<int> group2 = {2, 3, 5};
    std::vector<int> profit2 = {6, 7, 8};
    int n2 = 10, minProfit2 = 5;
    std::cout << "Result 2 (DP): " << sol.profitableSchemes(n2, minProfit2, group2, profit2) << std::endl; // 输出: 7
    std::cout << "Result 2 (Memo): " << sol.profitableSchemes2(n2, minProfit2, group2, profit2) << std::endl; // 输出: 7
    // std::cout << "Result 2 (Rec): " << sol.profitableSchemes1(n2, minProfit2, group2, profit2) << std::endl; // 输出: 7（可能较慢）

    return 0;
}
