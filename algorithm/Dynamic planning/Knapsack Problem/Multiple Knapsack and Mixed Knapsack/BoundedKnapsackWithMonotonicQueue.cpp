#include <iostream> // 用于标准输入输出流 (cin, cout)
#include <vector>   // 用于动态数组 (std::vector)
#include <algorithm> // 用于 std::max, std::fill 等算法
#include <cstdio>    // 用于 scanf/printf，提供类似 StreamTokenizer 的高速输入输出

// 多重背包单调队列优化
// 宝物筛选
// 一共有n种货物, 背包容量为t
// 每种货物的价值(v[i])、重量(w[i])、数量(c[i])都给出
// 请返回选择货物不超过背包容量的情况下，能得到的最大的价值
// 测试链接 : https://www.luogu.com.cn/problem/P1776

const int MAXN_ITEMS = 101;    // 最大物品种类数
const int MAX_CAPACITY = 40001; // 最大背包容量

// 使用 vector 代替原始数组，尽管大小是基于约束固定的

// dp 数组对于容量是 0-based 索引，大小为 MAX_CAPACITY
std::vector<int> v(MAXN_ITEMS);      // v[i] = 物品 i 的价值
std::vector<int> w(MAXN_ITEMS);      // w[i] = 物品 i 的重量
std::vector<int> c(MAXN_ITEMS);      // c[i] = 物品 i 的数量
std::vector<int> dp(MAX_CAPACITY);   // dp[j] = 容量为 j 时的最大价值
std::vector<int> queue_arr(MAX_CAPACITY); // 存储单调队列中的容量值

int q_l, q_r; // 单调队列的左指针和右指针 (队首和队尾)
int n_item_types, t_capacity; // 物品种类数量, 背包总容量

// 辅助函数，计算在单调队列中使用的值

// 计算公式为：dp_val[j] - (j / w_item_i) * v_item_i
int calculate_value_for_queue(int item_idx, int current_capacity_j) {
    if (w[item_idx] == 0) { // 如果物品重量为0，避免除以零
        // 这种情况需要根据问题的具体说明来仔细处理。
        // 假设根据典型的背包问题，w[i] >= 1。
        // 如果 v[i]>0 且 w[i]=0，若 c[i]>0 则价值可能是无限大。
        // 这里我们遵循公式，如果 w[i]=0 可能会引发问题。
        // 如果 w[i] 可以为0，问题定义需要澄清。
        // 此翻译中，基于 j / w[i]，我们假设 w[i] >= 1。
        return dp[current_capacity_j]; // 或者其他处理方式
    }
    return dp[current_capacity_j] - (current_capacity_j / w[item_idx]) * v[item_idx];
}

// 空间优化的动态规划 (DP) 结合单调队列

int solve_knapsack() {
    // 初始化 dp 数组为 0 (如果未更改，全局 vector 初始化时已完成)
    // std::fill(dp.begin(), dp.begin() + t_capacity + 1, 0); // 如果需要显式初始化

    for (int i = 1; i <= n_item_types; ++i) { // 遍历每一种物品
        for (int mod = 0; mod < w[i] && mod <= t_capacity; ++mod) { // 遍历余数 (同余类)
            q_l = 0; // 重置队列头指针
            q_r = 0; // 重置队列尾指针

            // 第一个循环：为当前余数组，从右到左初始化队列，最多包含 c[i] 个元素


            for (int current_j = t_capacity - mod, count = 0; count < c[i] && current_j >= 0; ++count,current_j -= w[i]) {
                 // `current_j` 是当前考虑加入队列的容量值
                while (q_l < q_r && calculate_value_for_queue(i, queue_arr[q_r - 1]) <= calculate_value_for_queue(i, current_j)) {
                    q_r--; // 维护队列单调性：队尾元素若劣于或等于新元素，则出队
                }
                queue_arr[q_r++] = current_j; // 新元素入队

            }

            // 主要处理循环：对于当前余数组，从右到左遍历 j

            for (int current_j = t_capacity - mod,enter_idx = t_capacity - mod - w[i] * c[i]; current_j >= mod; current_j -= w[i],enter_idx -= w[i]) {

                // 将 `enter_idx` (滑入窗口的元素) 加入队列
                if (enter_idx >= 0) { // 确保 enter_idx 是一个有效的容量
                    while (q_l < q_r && calculate_value_for_queue(i, queue_arr[q_r - 1]) <= calculate_value_for_queue(i, enter_idx)) {
                        q_r--; // 维护单调性
                    }
                    queue_arr[q_r++] = enter_idx; // 入队
                }

                // 使用队列头部的最优值计算 dp[current_j]
                // dp[j] = value2(i, queue[l]) + j / w[i] * v[i]
                // 即 (dp[queue[l]] - queue[l]/w[i]*v[i]) + j/w[i]*v[i]

                 // dp[current_j] = calculate_value_for_queue(i, queue_arr[q_l]) + (current_j / w[i]) * v[i];
                 // 确保我们总是取最优解（dp[current_j] 可能来自前一个物品在不同 mod 组但相同容量下的更优解，或不选当前物品）
                 // 但在此特定算法中，dp[current_j] 的更新是基于上一轮迭代（i-1）的值，所以直接赋值是常见的。
                 // 为确保稳健性，尤其是在 dp 数组被多个 mod 循环更新时，取 max 更安全。
                 dp[current_j] = std::max(dp[current_j], calculate_value_for_queue(i, queue_arr[q_l]) + (current_j / w[i]) * v[i]);

                // 从队首弹出：如果 queue_arr[q_l] == current_j。
                // 这个条件意味着，如果用于计算 dp[current_j] 的最优选择（来自 dp[i-1]层，通过 value2(i, current_j) 体现）
                // 恰好是 dp[i-1][current_j] （即不选任何物品 i），并且这个 current_j （作为先前状态）是队首元素，
                // 那么它现在已经被处理完毕，可以出队。
                // 这不是典型的基于窗口大小的弹出规则，而是与元素如何添加和使用相关的特定算法变体。
                // 实际的窗口约束（最多 c[i] 个物品）是通过添加 enter_idx = current_j - w[i]*c[i] 来隐式管理的。
                // 对于 dp[current_j]，考虑的最旧元素是 current_j - c[i]*w[i]。
                // 一个标准的弹出规则可能是：if (queue_arr[q_l] < current_j - c[i]*w[i]) q_l++;

                if (queue_arr[q_l] == current_j) {
                    q_l++;
                }
            }
        }
    }
    return dp[t_capacity]; // 返回最大总容量下的最大价值
}


int main() {
    // 快速 I/O (可选, scanf/printf 已经比较快)
    // std::ios_base::sync_with_stdio(false);
    // std::cin.tie(NULL);


    // 使用 scanf 可能比 cin 更快，接近 StreamTokenizer 的速度
    while (scanf("%d %d", &n_item_types, &t_capacity) == 2) { // 当成功读取两个整数时
        // 如果是多组测试用例，需要为每个测试用例初始化/重置 dp 数组

        // 全局 dp 数组会被零初始化。如果是多组数据，确保 dp 被重置。
        std::fill(dp.begin(), dp.begin() + t_capacity + 1, 0); // 将 dp 数组中直到 t_capacity 的部分填充为 0


        for (int i = 1; i <= n_item_types; ++i) {
            scanf("%d %d %d", &v[i], &w[i], &c[i]); // 读取物品的价值、重量和数量
        }

        printf("%d\n", solve_knapsack()); // 输出结果
    }

    return 0;
}

