#include <iostream>
#include <vector>
#include <algorithm> // 用于 std::max
#include <cstring>   // 用于 std::fill (对于 char 或字节单位的 0 也可以使用 memset)
// 多重背包不进行枚举优化
// 宝物筛选
// 一共有n种货物, 背包容量为t
// 每种货物的价值(v[i])、重量(w[i])、数量(c[i])都给出
// 请返回选择货物不超过背包容量的情况下，能得到的最大的价值
// 测试链接 : https://www.luogu.com.cn/problem/P1776
// 全局常量和数组，与 Java 结构对应
const int MAXN = 101;   // 最大物品种类数 + 1 (用于从 1 开始的索引)
const int MAXW = 40001; // 最大背包容量 + 1

int v[MAXN]; // 物品 i 的价值
int w[MAXN]; // 物品 i 的重量
int c[MAXN]; // 物品 i 的数量

int dp[MAXW]; // 用于 compute2 (空间优化) 的 DP 数组

int n; // 物品种类数
int t; // 背包容量

// 函数 compute2: 使用一维 DP 数组解决有界背包问题。
// 这是 Java main 方法调用的函数，也是预期使用的函数。
// 时间复杂度: O(n * t * 每种物品类型有效使用的平均数量)
// (不考虑 j/w[i] 限制的最坏情况: O(n * t * max_c_i))
// 空间复杂度: O(t) 用于 dp 数组
int compute2() {
    // 为当前测试用例初始化 dp 数组为 0。
    // dp[j_cap] 存储容量为 j_cap 时的最大价值。
    // 基本情况: dp[0] = 0 (没有物品，没有容量，没有价值)。所有其他 dp[j_cap] 从 0 开始。
    // 如果 'main' 在一次执行中处理多个测试用例，此重置至关重要。
    std::fill(dp, dp + t + 1, 0); // 清除 dp 数组从索引 0 到当前容量 't'。

    for (int i = 1; i <= n; i++) { // 遍历每种类型的物品 (从 1 到 n)
        // 对于每种物品类型，我们考虑取 k_count 件。
        // 'j_cap' (容量) 的循环必须向后进行。这确保了在为物品 'i' 计算 dp[j_cap] 时，
        // dp[j_cap - w[i] * k_count] 项引用的是由先前物品 (1 到 i-1) 导出的状态，
        // 防止当前物品 'i' 在其自身的处理阶段被使用超过 c[i] 次。
        for (int j_cap = t; j_cap >= 0; j_cap--) { // 容量从 t 向下迭代到 0 (与原始 Java 中相同)
                                                  // 注意: j_cap 可以优化到最小为 w[i]，
                                                  // 因为如果 j_cap < w[i]，则无法拿取物品 'i'。
                                                  // 然而，k_count 循环条件 (w[i] * k_count <= j_cap)
                                                  // 已经处理了这个问题，所以 j_cap >= 0 是正确的。
            for (int k_count = 1; k_count <= c[i] && w[i] * k_count <= j_cap; k_count++) {
                // dp[j_cap] 当前是使用物品 1..i-1 达到容量 j_cap 的最大价值，或者
                // 是使用物品 1..i (其中物品 i 的数量少于 k_count) 达到容量 j_cap 的最大价值。
                // 我们尝试通过拿取 k_count 件类型为 'i' 的物品来改进它。
                dp[j_cap] = std::max(dp[j_cap], dp[j_cap - w[i] * k_count] + k_count * v[i]);
            }
        }
    }
    return dp[t]; // 背包满容量 't' 时的最大价值
}

// 函数 compute1: 使用二维 DP 表解决有界背包问题。
// 为与 Java 源代码保持一致而包含。
// 时间复杂度: 与 compute2 相同。
// 空间复杂度: O(n * t) 用于 dp_table。
int compute1() {
    // dp_table[i][j_cap] = 使用前 'i' 种物品类型，容量为 'j_cap' 时的最大价值
    // std::vector 对于数值类型默认初始化为 0。
    std::vector<std::vector<int>> dp_table(n + 1, std::vector<int>(t + 1, 0));

    for (int i = 1; i <= n; i++) { // 遍历每种类型的物品 (从 1 到 n)
        for (int j_cap = 0; j_cap <= t; j_cap++) { // 遍历每个容量从 0 到 t
            // 选项 1: 不拿取任何类型为 'i' 的物品。
            // 价值与使用物品 1 到 i-1，容量为 j_cap 时相同。
            dp_table[i][j_cap] = dp_table[i - 1][j_cap];

            // 选项 2: 拿取 k_count 件类型为 'i' 的物品。
            // 遍历要拿取的类型为 'i' 的物品数量。
            for (int k_count = 1; k_count <= c[i] && w[i] * k_count <= j_cap; k_count++) {
                dp_table[i][j_cap] = std::max(dp_table[i][j_cap],
                                           dp_table[i - 1][j_cap - w[i] * k_count] + k_count * v[i]);
            }
        }
    }
    return dp_table[n][t]; // 使用所有 'n' 种物品类型，容量为 't' 时的最大价值
}

int main() {
    // 可选: 为了更快的 C++ I/O，类似于 Java 的 BufferedReader/PrintWriter 设置。
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    // std::cout.tie(NULL); // 通常不需要，除非与会刷新 cout 的 C 风格 I/O 混合使用。

    // Java 代码使用了 StreamTokenizer。`std::cin` 是标准的 C++ 等效物。
    // Java 中的循环 `while (in.nextToken() != StreamTokenizer.TT_EOF)`
    // 在 C++ 中转换为 `while (std::cin >> n >> t)`，用于读取直到文件末尾
    // 或无效输入。
    while (std::cin >> n >> t) { // 读取物品种类数和背包容量
        for (int i = 1; i <= n; i++) { // 读取每种物品类型的价值、重量和数量
            std::cin >> v[i] >> w[i] >> c[i];
        }

        // 调用 compute2 函数 (空间优化的 DP)，与 Java main 方法中一样
        std::cout << compute2() << "\n"; // 使用 "\n" 而不是 std::endl 以获得可能更快的输出
    }

    return 0;
}

