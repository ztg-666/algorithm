#include <iostream>  // 用于标准输入输出流 (cin, cout)
#include <vector>    // 用于使用动态数组 std::vector
#include <algorithm> // 用于 std::max, std::fill 等算法
#include <numeric>   // 如果 <algorithm> 不够用，可能需要它（通常足够）

// 完全背包(模版)
// 给定一个正数t，表示背包的容量
// 有m种货物，每种货物可以选择任意个
// 每种货物都有体积costs[i]和价值values[i]
// 返回在不超过总容量的情况下，怎么挑选货物能达到价值最大
// 返回最大的价值
// 测试链接 : https://www.luogu.com.cn/problem/P1616


// 如果需要，根据题目约束调整 MAXT，10,000,001 很大
const int MAXM = 10001;     // 最大物品种类数 + 1
const int MAXT = 10000001;  // 最大背包容量 + 1 (根据题目 P1616 的数据范围调整)

int t, m;                     // t: 背包容量, m: 物品种类数
std::vector<int> cost(MAXM);  // cost[i]: 第 i 种物品的成本（体积/重量）
std::vector<int> val(MAXM);   // val[i]: 第 i 种物品的价值
// 使用 long long (64位整数) 作为 dp 表的类型，因为总价值可能很大，超出 int 范围
std::vector<long long> dp_1d(MAXT); // 用于空间优化版的一维 DP 表

// 函数：二维动态规划解法
// dp[i][j]: 表示只考虑前 'i' 种物品，在容量不超过 'j' 的情况下能获得的最大价值
long long compute1() {
    // 初始化二维 DP 表。需要 m+1 行和 t+1 列。
    // 使用 long long 存储价值。
    // 在这里要非常小心内存限制！MAXM * MAXT 可能非常巨大。
    // std::vector<std::vector<long long>> dp(m + 1, std::vector<long long>(t + 1, 0LL)); // 0LL 表示 long long 类型的 0

    // 上述内存分配对于典型的竞赛限制来说可能太大了。
    // 例如：10000 * 10000000 = 10^11 个元素 -> 如果是 long long 类型，大约需要 800 GB 内存！
    // 因此，对于给定的约束，这种方法通常是不可行的。
    // 如果要实现，逻辑如下：


    std::vector<std::vector<long long>> dp(m + 1, std::vector<long long>(t + 1, 0LL));
    for (int i = 1; i <= m; ++i) { // 遍历物品种类
        for (int j = 0; j <= t; ++j) { // 遍历容量
            // 决策 1：完全不使用第 'i' 种物品来达到容量 'j'
            dp[i][j] = dp[i - 1][j];

            // 决策 2：尝试使用第 'i' 种物品 *至少一次* 来达到容量 'j'
            // 如果物品 'i' 能装下 (j >= cost[i])，我们就有可能提高价值。
            // 与 0/1 背包的关键区别在于状态转移方程中引用的是 dp[i][...] 而不是 dp[i-1][...]：
            // dp[i][j - cost[i]] 表示在考虑第 i 种物品时，容量为 j - cost[i] 的最大价值，
            // 这已经隐含了物品 i 可能已经被使用过（用来达到 j - cost[i] 这个状态）的可能性。
            if (j >= cost[i]) {
                // 注意：是 dp[i][...] 而不是 dp[i-1][...]
                dp[i][j] = std::max(dp[i][j], dp[i][j - cost[i]] + (long long)val[i]);
            }
        }
    }
    return dp[m][t];

}


// 函数：空间优化的动态规划解法（一维 DP 表）
// dp[j]: 表示在处理过的物品种类中，对于容量 'j' 能获得的最大价值
// 这是解决完全背包问题的标准且高效的方法。
long long compute2() {
    // 用零初始化一维 DP 表。
    // dp_1d[0] 默认是 0 (或者显式初始化)。
    std::fill(dp_1d.begin(), dp_1d.begin() + t + 1, 0LL); // 0LL 代表 long long 类型的零

    // 遍历每一种物品 'i'
    for (int i = 1; i <= m; ++i) {
        // **关键点**：对于完全背包问题，内层循环遍历容量 'j' 时需要 *正序* 遍历，
        // 从当前物品的成本 cost[i] 开始，直到总容量 t。
        // 正序遍历 (j 递增) 是完全背包优化的核心。
        // 它允许一个物品在计算同一个容量 'j' 时被多次使用，
        // 因为在计算 dp_1d[j] 时，所依赖的 dp_1d[j - cost[i]] 可能已经是
        // 使用了物品 'i' 之后的结果。
        for (int j = cost[i]; j <= t; ++j) {
            // 更新 dp_1d[j]:
            // 比较 当前容量 'j' 的最大价值 (dp_1d[j])
            // 与 通过将物品 'i' 加入到剩余容量 (j - cost[i]) 的最大价值中
            //   所获得的新价值 (dp_1d[j - cost[i]] + val[i])。
            // 在加法之前将 val[i] 强制转换为 long long，以防止潜在的溢出，
            // 特别是当 val[i] 和 dp_1d[j - cost[i]] 都很大时。
            dp_1d[j] = std::max(dp_1d[j], dp_1d[j - cost[i]] + (long long)val[i]);
        }
    }
    // 最终答案是背包容量为 't' 时的最大价值。
    return dp_1d[t];
}

int main() {
    // 加速 C++ 输入输出流
    std::ios_base::sync_with_stdio(false); // 关闭 C++ 标准流与 C 标准流的同步
    std::cin.tie(NULL); // 解除 cin 和 cout 的绑定
    std::cout.tie(NULL); // 通常也解除 cout 的绑定

    // 循环读取输入直到文件结束 (EOF)，类似于 Java 的循环
    // `std::cin >> t >> m` 在读取失败时（如遇到 EOF）会返回 false
    while (std::cin >> t >> m) { // 读取背包容量 t 和物品种类数 m
        // 读取物品详情 (使用从 1 开始的索引)
        for (int i = 1; i <= m; ++i) {
            std::cin >> cost[i] >> val[i]; // 读取成本和价值
        }

        // 调用空间优化的计算函数并打印结果
        std::cout << compute2() << "\n"; // 使用 "\n" 进行换行，通常比 std::endl 更快

        // 如果你被迫使用 compute1（并且假设有无限内存）：
        // std::cout << compute1() << "\n";
    }

    return 0; // 程序正常退出
}
