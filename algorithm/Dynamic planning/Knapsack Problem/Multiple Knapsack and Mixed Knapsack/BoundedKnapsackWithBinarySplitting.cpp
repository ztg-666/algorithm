#include <iostream>
#include <vector>
#include <algorithm> // 用于 std::max 和 std::fill


// 多重背包通过二进制分组转化成01背包(模版)
// 宝物筛选
// 一共有n种货物, 背包容量为t
// 每种货物的价值(v[i])、重量(w[i])、数量(c[i])都给出
// 请返回选择货物不超过背包容量的情况下，能得到的最大的价值
// 测试链接 : https://www.luogu.com.cn/problem/P1776


// 派生出的物品数量 m 可以是 sum(log_2(count_i))，大约为 100 * log_2(40000) ~ 100 * 15.3 ~ 1530，甚至可能达到 100 * 16 = 1600。
// 如果遇到最坏情况的拆分，1001 对于 P1776 的通用约束可能太小。

const int MAX_DERIVED_ITEMS_COUNT = 1001; // 二进制拆分后的最大物品数量
const int MAX_CAPACITY = 40001;        // 最大背包容量

// 全局数组，用于存储派生物品的价值和重量，以及DP表
int v[MAX_DERIVED_ITEMS_COUNT]; // 派生物品的价值 (通过 m 进行 1-索引)
int w[MAX_DERIVED_ITEMS_COUNT]; // 派生物品的重量 (通过 m 进行 1-索引)
int dp[MAX_CAPACITY];          // DP 表：dp[j] 是容量为 j 时的最大价值

int n; // 原始物品的种类数
int t; // 背包容量
int m; // 二进制拆分后的派生物品数量

// 对派生物品执行0/1背包算法

int solve_knapsack() {
    // 为当前测试用例初始化 dp 数组为 0。
    // dp[j] 将存储容量为 j 时可获得的最大价值。
    // 我们需要填充从 dp[0] 到 dp[t] (含)。
    std::fill(dp, dp + t + 1, 0);

    // 遍历每个派生物品 (这些现在是 0/1 物品)
    // 派生物品存储在 v 和 w 数组的索引 1 到 m 中
    for (int i = 1; i <= m; ++i) {
        // 向下迭代容量，以确保每个物品最多只被考虑一次（0/1背包特性）
        for (int j = t; j >= w[i]; --j) {
            dp[j] = std::max(dp[j], dp[j - w[i]] + v[i]);
        }
    }
    return dp[t]; // 背包满容量 t 时的最大价值
}

int main() {
    // 快速 I/O 设置
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);


    while (std::cin >> n >> t) { // 读取原始物品种类数 (n) 和容量 (t)
        m = 0; // 为每个新的测试用例重置派生物品的计数

        // 读取每个原始物品类型并执行二进制拆分
        for (int i = 1; i <= n; ++i) {
            int item_value, item_weight, item_count;
            std::cin >> item_value >> item_weight >> item_count;

            // 二进制拆分逻辑：
            // 将一个数量为 'item_count' 的物品分解为数量为 1, 2, 4, ... 和一个剩余部分的物品。
            // 这些中的每一个都成为一个新的“派生”物品，只能选择一次 (0/1 属性)。
            for (int k = 1; k <= item_count; k <<= 1) { // k 取值为 1, 2, 4, 8, ...

                // 此处 C++ 代码也直接赋值，假设 m 不会超过 MAX_DERIVED_ITEMS_COUNT - 1。
                // 如果 MAX_DERIVED_ITEMS_COUNT 设置不当，这里可能导致数组越界。
                v[++m] = k * item_value;
                w[m] = k * item_weight;
                item_count -= k;
            }
            if (item_count > 0) { // 如果在 2 的幂之后有任何剩余
                // 处理剩余部分
                v[++m] = item_count * item_value;
                w[m] = item_count * item_weight;
            }
        }

        // 用派生物品解决 0/1 背包问题并打印结果
        std::cout << solve_knapsack() << "\n";
    }

    return 0;
}

