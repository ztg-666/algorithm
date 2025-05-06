#include <iostream>  // 用于标准输入输出流 (cin, cout)
#include <vector>    // 用于使用动态数组 std::vector
#include <algorithm> // 用于 std::min, std::max, std::fill 等算法
#include <limits>    // 用于 std::numeric_limits 获取类型的极限值 (如 int 的最大值)

// 购买足量干草的最小花费
// 有n个提供干草的公司，每个公司都有两个信息
// cost[i]代表购买1次产品需要花的钱
// val[i]代表购买1次产品所获得的干草数量
// 每个公司的产品都可以购买任意次
// 你一定要至少购买h数量的干草，返回最少要花多少钱
// 测试链接 : https://www.luogu.com.cn/problem/P2918

// 问题描述：
// 有 n 个供应干草的公司。
// cost[i]: 从公司 i 购买 1 次（一批）的花费。
// val[i]: 从公司 i 购买 1 次（一批）获得的干草量。
// 你可以从任何公司购买任意次数（批次）。
// 你需要购买 **至少** h 数量的干草。
// 找到达成这个目标的最小花费。
// 这是一个 无界背包（完全背包）问题的变种（为达到目标价值，最小化成本）。

const int MAXN = 101;   // 最大公司数量 + 1
// DP 计算可能需要的最大干草量。见 main() 函数解释为何是 h + maxv。
const int MAXM = 55001;


int n;                      // 公司数量
int h;                      // 目标最小干草量
int maxv;                   // 单次购买能获得的最大干草量
int m;                      // DP 计算的实际干草量上界 (h + maxv)
std::vector<int> val(MAXN); // val[i]: 公司 i 的干草量
std::vector<int> cost(MAXN); // cost[i]: 公司 i 的花费
std::vector<int> dp(MAXM);   // dp[j]: 获得**恰好** j 磅干草的最小花费

// 用 int 的最大值表示无穷大花费
const int INF = std::numeric_limits<int>::max();

// 函数：二维动态规划解法（概念性的，可能太慢或内存占用过大）
// dp[i][j]: 只使用 1 到 i 号公司，获得**恰好** j 磅干草的最小花费。
// 注意：这主要是为了理解逻辑。
int compute1() {
    // 初始化二维 DP 表。需要 n+1 行和 m+1 列。
    std::vector<std::vector<int>> dp_2d(n + 1, std::vector<int>(m + 1, INF));

    // 基本情况：获得 0 干草的花费是 0。
    dp_2d[0][0] = 0;
    // 注意：使用 0 个公司获得 >0 干草的花费是 INF（已初始化）。

    // 填充 DP 表
    for (int i = 1; i <= n; ++i) { // 遍历公司
        for (int j = 0; j <= m; ++j) { // 遍历目标干草量
            // 选项 1：不使用公司 i。花费与只使用 1 到 i-1 公司时相同。
            dp_2d[i][j] = dp_2d[i - 1][j];

            // 选项 2：使用公司 i（至少一次）。
            // 这只在 j >= val[i] 时可能。
            // 花费是 cost[i] + 获得剩余量 (j - val[i]) 的最小花费（*仍然可以使用公司 1 到 i*）。
            // 我们使用 dp_2d[i][...] 因为这是无界背包问题（公司 i 可以使用多次）。
            if (j >= val[i] && dp_2d[i][j - val[i]] != INF) { // 检查子问题是否可解（花费不是无穷大）
                 dp_2d[i][j] = std::min(dp_2d[i][j], dp_2d[i][j - val[i]] + cost[i]);
            }
        }
    }

    // 在所有干草量 >= h 的结果中找到最小花费
    int ans = INF;
    for (int j = h; j <= m; ++j) {
        ans = std::min(ans, dp_2d[n][j]);
    }
    return ans;
}


// 函数：空间优化的动态规划解法（一维 DP 表）
// dp[j]: 使用**到目前为止已考虑的**公司，获得**恰好** j 磅干草的最小花费。
// 这是标准且高效的方法。
int compute2() {
    // 初始化一维 DP 表。
    // dp[0] = 0 （获得 0 干草花费为 0）。
    // 对于 j > 0，dp[j] = INF。
    std::fill(dp.begin() + 1, dp.begin() + m + 1, INF); // 从索引 1 填充到 m
    dp[0] = 0; // 显式设置索引 0

    // 遍历每个公司 'i'
    for (int i = 1; i <= n; ++i) {
        // **关键点**：对于完全背包，内层循环遍历干草量 'j' 时需要 *正序* 遍历，
        // 从当前公司的干草量 val[i] 开始，直到计算上界 m。
        // 正序遍历 (j 递增) 是完全背包空间优化的关键。
        // 它允许在计算 dp[j] 时，考虑使用公司 'i' 多次的可能性，
        // 因为所依赖的 dp[j - val[i]] 可能已经是使用了公司 'i' 之后的结果。
        for (int j = val[i]; j <= m; ++j) {
            // 如果我们能够达到状态 (j - val[i]) （即其花费不是无穷大），
            // 那么我们就有可能通过从公司 i 购买一次来到达状态 j。
            if (dp[j - val[i]] != INF) {
                // 用目前找到的最小花费更新 dp[j]。
                dp[j] = std::min(dp[j], dp[j - val[i]] + cost[i]);
            }
        }
    }

    // 计算最终答案：
    // 我们需要获得 **至少** h 磅干草的最小花费。
    // 这意味着需要检查所有干草量从 h 到 m 的最小花费。
    int ans = INF;
    for (int j = h; j <= m; ++j) {
        ans = std::min(ans, dp[j]);
    }
    return ans;
}

int main() {
    // 加速 C++ 输入输出流
    std::ios_base::sync_with_stdio(false); // 关闭 C++ 标准流与 C 标准流的同步
    std::cin.tie(NULL); // 解除 cin 和 cout 的绑定
    std::cout.tie(NULL);

    // 循环读取输入直到文件结束 (EOF)
    while (std::cin >> n >> h) { // 读取公司数 n 和 目标干草量 h
        maxv = 0; // 对每个测试用例重置单次购买最大干草量
        // 读取公司详情 (物品使用从 1 开始的索引)
        for (int i = 1; i <= n; ++i) {
            std::cin >> val[i] >> cost[i]; // 读取干草量和花费
            maxv = std::max(maxv, val[i]); // 持续追踪最大的单次购买干草量
        }

        // *** 原始代码的核心洞察 ***
        // 为什么要计算 DP 直到 m = h + maxv？
        // 考虑最优解。假设最终购买的总干草量是 H_opt >= h，
        // 且是以最小花费 C_opt 实现的。
        // 假设这个最优解中的 *最后一次* 购买来自公司 k（获得 val[k] 干草量）。
        // 那么，达到 H_opt - val[k] 干草量的花费也必须是对于那个数量的最优花费。
        // 这个数量 H_opt - val[k] 可能小于 h。
        // 如果 H_opt - val[k] < h，那么 H_opt < h + val[k]。
        // 因为 val[k] <= maxv（任何单次购买量不会超过最大单次购买量），
        // 所以我们有 H_opt < h + maxv。
        // 因此，最优的总干草量 H_opt 必然小于 h + maxv。
        // 我们需要计算 DP 状态直到可能达到 h + maxv - 1，以便找到所有 >= h 的状态中的最小值。
        // 设置 m = h + maxv 确保我们覆盖了所有相关的状态。
        m = h + maxv;

        // 确保 dp 向量足够大（虽然根据题目限制，MAXM 应该够用）
        if (m >= MAXM) {
             // 处理潜在的边界情况，如果计算出的 m 因奇怪的输入而超过 MAXM
             // 根据题目约束这不应发生，但这是良好的编程习惯。
             // 可以调整 dp 大小或处理错误。对于此问题，假定 m < MAXM。
             // std::cerr << "计算得到的 m 超出了 MAXM!" << std::endl;
             // exit(1); // 或者如果允许，动态调整 dp 大小
        }


        // 调用空间优化的计算函数并打印结果
        std::cout << compute2() << "\n"; // 使用 "\n" 换行

        // 如果你想测试 compute1（并且内存足够）：
        // std::cout << compute1() << "\n";
    }

    return 0; // 程序正常退出
}
