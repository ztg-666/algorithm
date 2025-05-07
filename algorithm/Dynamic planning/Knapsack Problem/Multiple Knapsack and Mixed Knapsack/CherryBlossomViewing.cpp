#include <iostream>
#include <vector>
#include <string>    // 对于此问题并非严格必要，但经常包含
#include <algorithm> // 用于 std::max, std::fill
#include <numeric>   // 用于 std::fill (虽然 <algorithm> 通常足够)

// 观赏樱花
// 给定一个背包的容量t，一共有n种货物，并且给定每种货物的信息
// 花费(cost)、价值(val)、数量(cnt)
// 如果cnt == 0，代表这种货物可以无限选择
// 如果cnt > 0，那么cnt代表这种货物的数量
// 挑选货物的总容量在不超过t的情况下，返回能得到的最大价值
// 背包容量不超过1000，每一种货物的花费都>0
// 测试链接 : https://www.luogu.com.cn/problem/P1833

const int MAX_DERIVED_ITEMS_COUNT = 100001;

// MAX_KNAPSACK_CAPACITY: 最大背包容量 (Java中的MAXW, 用于dp数组大小)
// P1833的题目描述通常说明 "时间差不超过 1000 分钟".
const int MAX_KNAPSACK_CAPACITY = 1001;

// ENOUGH_ITEMS_FOR_UNBOUNDED: 用于数量为0的物品
// 如果一个物品是无限的，容量为T，花费为C，最大有效数量是T/C。
// T <= 1000, C >= 1, 所以最大有效数量是1000。
// 1001 是对这些物品进行二进制拆分的一个安全上限。
const int ENOUGH_ITEMS_FOR_UNBOUNDED = 1001;

// 全局数组，用于存储派生物品的价值和重量（花费），以及DP表
int derived_v[MAX_DERIVED_ITEMS_COUNT]; // 派生物品的价值 (1基索引)
int derived_w[MAX_DERIVED_ITEMS_COUNT]; // 派生物品的重量/花费 (1基索引)
int dp[MAX_KNAPSACK_CAPACITY];          // DP表: dp[j] 是容量为j时的最大价值

// 用于存储输入和中间值的变量
int hour1, minute1, hour2, minute2;
int knapsack_capacity_t; // 总可用时间（分钟），作为背包容量
int num_item_types_n;    // 原始物品种类的数量
int num_derived_items_m; // 二进制拆分后派生物品的数量

// 对派生物品执行0/1背包算法，计算最大价值
// 等同于Java的compute()方法
int solve_01_knapsack() {
    // 为当前测试用例初始化dp数组为0。
    // dp[j] 将存储容量为j时可实现的最大价值。
    // 需要填充从dp[0]到dp[knapsack_capacity_t]（含）。
    std::fill(dp, dp + knapsack_capacity_t + 1, 0);

    // 遍历每个派生出来的物品 (这些现在是0/1物品)
    // 派生物品存储在索引1到num_derived_items_m
    for (int i = 1; i <= num_derived_items_m; ++i) {
        // 向下迭代容量，以确保每个物品最多被考虑一次 (0/1背包特性)
        for (int j = knapsack_capacity_t; j >= derived_w[i]; --j) {
            dp[j] = std::max(dp[j], dp[j - derived_w[i]] + derived_v[i]);
        }
    }
    return dp[knapsack_capacity_t]; // 完整背包容量下的最大价值
}

int main() {
    // C++的快速I/O设置
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    char colon_char; // 用于消耗时间输入（如"HH:MM"）中的':'字符

    // 通过尝试读取测试用例输入的开头部分。
    // 如果 `std::cin` 失败（例如，在文件尾或由于格式错误的输入），循环将终止。
    while (std::cin >> hour1 >> colon_char >> minute1 >> hour2 >> colon_char >> minute2) {

        // 计算背包容量 (总可用时间，单位：分钟)
        // 标准的时间差计算，假设在同一天且 H1:M1 <= H2:M2。
        if (minute1 > minute2) { // 如果开始分钟数 > 结束分钟数，则从小时借位
            hour2--;
            minute2 += 60;
        }
        knapsack_capacity_t = (hour2 - hour1) * 60 + (minute2 - minute1);


        std::cin >> num_item_types_n; // 读取不同物品种类的数量

        num_derived_items_m = 0; // 为这个新的测试用例重置派生物品的计数

        // 读取每个原始物品类型，然后执行二进制拆分
        for (int i = 0; i < num_item_types_n; ++i) {
            int cost, val, count_orig; // 花费(时间), 价值, 原始数量
            std::cin >> cost >> val >> count_orig;

            if (count_orig == 0) { // 这表示一个无限数量的物品 (完全背包)
                //将其转换为一个具有足够大数量的有限物品，以便进行二进制拆分。
                count_orig = ENOUGH_ITEMS_FOR_UNBOUNDED;
            }

            // 二进制拆分逻辑：
            // 将一个数量为 'count_orig' 的物品分解为几个0/1物品。
            // 每个新物品代表取k个原始物品（其中k是2的幂或一个余数）。
            int current_count_for_splitting = count_orig;
            for (int k = 1; k <= current_count_for_splitting; k <<= 1) { // k = 1, 2, 4, 8...
                num_derived_items_m++;
                // 假设由于题目约束，num_derived_items_m 不会超过 MAX_DERIVED_ITEMS_COUNT。
                derived_v[num_derived_items_m] = k * val;
                derived_w[num_derived_items_m] = k * cost;
                current_count_for_splitting -= k; // 为本次拆分的下一个k以及最终的余数减少计数
            }
            if (current_count_for_splitting > 0) { // 如果按2的幂拆分后仍有剩余
                num_derived_items_m++;
                derived_v[num_derived_items_m] = current_count_for_splitting * val;
                derived_w[num_derived_items_m] = current_count_for_splitting * cost;
            }
        }

        // 对所有派生出来的物品解决0/1背包问题，并打印结果
        std::cout << solve_01_knapsack() << "\n";
    }

    return 0;
}

