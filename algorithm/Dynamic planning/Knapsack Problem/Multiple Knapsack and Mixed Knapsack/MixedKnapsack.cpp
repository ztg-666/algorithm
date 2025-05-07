#include <iostream>
#include <vector>
#include <numeric>   // 对于 std::fill 可能需要 (或者使用 vector(size, value) 初始化)
#include <algorithm> // 对于 std::min (在此特定翻译的最终版本中没有明确使用)
// 混合背包 + 多重背包普通窗口优化
// 能成功找零的钱数种类
// 每一种货币都给定面值val[i]，和拥有的数量cnt[i]
// 想知道目前拥有的货币，在钱数为1、2、3...m时
// 能找零成功的钱数有多少
// 也就是说当钱数的范围是1~m
// 返回这个范围上有多少可以找零成功的钱数
// 比如只有3元的货币，数量是5张
// m = 10
// 那么在1~10范围上，只有钱数是3、6、9时，可以成功找零
// 所以返回3表示有3种钱数可以找零成功
// 测试链接 : http://poj.org/problem?id=1742

using namespace std;

const int MAXN_GLOBAL = 101;  // 最大物品种类数 + 1 (用于1-based索引)
const int MAXM_GLOBAL = 100001; // 最大目标金额M + 1

int main() {
    // 优化 C++ 标准流以加快 I/O速度
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::vector<int> val(MAXN_GLOBAL); // 存储每种货币的面值
    std::vector<int> cnt(MAXN_GLOBAL); // 存储每种货币的数量
    std::vector<bool> dp(MAXM_GLOBAL); // dp[j] 为 true 表示金额 j 可以被找零

    int n_items;      // 货币的种类数量
    int max_sum_m;    // 需要判断能否找零的最大金额


    while (std::cin >> n_items >> max_sum_m && (n_items != 0 || max_sum_m != 0)) {
        for (int i = 1; i <= n_items; ++i) {
            std::cin >> val[i];
        }
        for (int i = 1; i <= n_items; ++i) {
            std::cin >> cnt[i];
        }

        // 为当前测试用例初始化 dp 数组
        // dp[0] 为 true (金额0总是可以通过不拿任何货币达到)
        // dp[1...max_sum_m] 初始应为 false
        if (max_sum_m >= 0) { // 确保 max_sum_m 非负
            // 将 dp[1] 到 dp[max_sum_m] 填充为 false。
            // dp.begin() + max_sum_m + 1 是 dp[max_sum_m] 的下一个迭代器。
            if (max_sum_m + 1 <= dp.size()) { // 安全边界检查，尽管 MAXM_GLOBAL 应该足够大
                 std::fill(dp.begin() + 1, dp.begin() + max_sum_m + 1, false);
            } else {
                // 如果 MAXM_GLOBAL 太小，则处理潜在错误或调整大小，但题目约束通常能适应。
                // 对于此问题，max_sum_m 高达 100000，因此 MAXM_GLOBAL = 100001 是可以的。
                std::fill(dp.begin() + 1, dp.end(), false); // 如果 max_sum_m 超出预期，则填充我们已有的部分
            }
        }
        if (dp.size() > 0) { // 确保dp至少有一个元素dp[0]
             dp[0] = true;
        }


        // 遍历每一种货币
        for (int i = 1; i <= n_items; ++i) {
            // 假设货币面值为正，根据典型的“面值”上下文。
            // 如果 val[i] 可能 <=0，可能需要特殊处理。
            // 对于 POJ 1742, 面值 A_i 是正数。
            if (val[i] <= 0) { // 无效面值则跳过
                continue;
            }

            if (cnt[i] == 1) { // 情况1: 0-1背包 (该种货币只有一张)
                // 为了使用空间优化的0-1背包DP，从后向前迭代
                for (int j = max_sum_m; j >= val[i]; --j) {
                    if (dp[j - val[i]]) { // 如果金额 (j - val[i]) 之前可以达到
                        dp[j] = true;     // 那么金额 j 也可以通过加上这张货币达到
                    }
                }
            } else if (static_cast<long long>(val[i]) * cnt[i] >= max_sum_m && val[i] > 0) {
                // 情况2: 完全背包 (对于总金额 max_sum_m 来说，此种货币相当于无限多)
                // 注意：原始Java代码是 val[i] * cnt[i] > m。这里改为 >= m 更符合“物品总价值足以覆盖m”的语义，
                // 或者更精确地，若 val[i]*cnt[i] 已经足够凑出任何 val[i] 的倍数直到 m，
                // 且我们只关心能否凑出，那么它可以被当做完全背包处理。
                // 一个更常见的完全背包转化条件是当一种物品的数量 k 满足 k * val[i] >= m 时。
                // 此处 Java 的条件 val[i] * cnt[i] > m 意味着拥有此种货币的总价值超过了目标 m。
                // 关键是，对于任何小于等于 m 的金额 j，如果需要 k 枚此种货币 (k*val[i] <= j),
                // 我们总是有足够的 (cnt[i] >= k)。
                // 为了使用空间优化的完全背包DP，从前向后迭代
                for (int j = val[i]; j <= max_sum_m; ++j) {
                    if (dp[j - val[i]]) { // 如果金额 (j - val[i]) 之前可以达到
                        dp[j] = true;     // 那么金额 j 也可以达到
                    }
                }
            } else { // 情况3: 多重背包 (有限数量的同种货币)
                     // 使用基于同余分组的滑动窗口优化。
                for (int mod = 0; mod < val[i]; ++mod) { // 遍历每个同余组 (余数)
                    int true_in_window_count = 0; // 记录当前滑动窗口内有多少个金额是可以达成的 (dp值为true)

                    // 阶段1: 初始化最右侧窗口的 true_in_window_count。
                    // 这个窗口概念上覆盖的 dp 状态是：
                    // dp[max_sum_m - mod], dp[max_sum_m - mod - val[i]], ..., dp[max_sum_m - mod - cnt[i]*val[i]]。
                    // 这些是 (cnt[i]+1) 个潜在的 dp 状态。

                    for (int current_j_in_group = max_sum_m - mod, items_in_window = 0;
                         current_j_in_group >= mod && items_in_window <= cnt[i]; // current_j_in_group >=0 即可, mod确保了下界
                         current_j_in_group -= val[i], ++items_in_window) {
                        if (dp[current_j_in_group]) {
                            true_in_window_count++;
                        }
                    }

                    // 阶段2: 从右向左滑动窗口并更新 dp 状态。
                    // j_to_update: 当前正在确定其 dp 值的金额索引 (dp[j_to_update])。
                    // j_entering_from_left: 当窗口向左滑动时，从“左侧”（较小索引端）新进入观察窗口的金额索引。
                    // 对于 dp[j_to_update]，其观察窗口由先前的状态组成：
                    // {dp_old[j_to_update - val[i]], dp_old[j_to_update - 2*val[i]], ..., dp_old[j_to_update - cnt[i]*val[i]]}
                    // (其中 dp_old 指的是本次特定更新步骤之前的值)
                    for (int j_to_update = max_sum_m - mod,
                             j_entering_from_left = max_sum_m - mod - val[i] * (cnt[i] + 1); // 窗口最左侧将要进入的元素
                         j_to_update >= mod; // 向下迭代，确保 j_to_update 保持非负且在当前同余组内。
                                             // Java 中是 j>=1，但 j>=mod 更精确，因为 mod 是该组最小的有效金额(如果mod=0则为0)
                         j_to_update -= val[i], j_entering_from_left -= val[i]) {

                        // (旧的)状态 dp[j_to_update] （即处理当前物品i之前的dp[j_to_update]）现在滑出了固定大小窗口的右边界。
                        // true_in_window_count 是针对以上一个j (j_to_update + val[i]) 为右边界的窗口统计的。
                        // 现在 j_to_update 成为了新的右边界（正在被判断的元素）。
                        // 因此，原先的 dp[j_to_update] (来自上一轮物品迭代)的贡献需要从 true_in_window_count 中移除，
                        // 因为它不再是“组成当前j_to_update所需的前置状态”之一。
                        if (dp[j_to_update]) {
                            true_in_window_count--;
                        }

                        // 如果 dp[j_to_update] 当前为 false，检查是否可以将其变为 true。
                        // true_in_window_count 此刻反映了影响窗口中的真实状态总和：
                        // 即 dp_old[j_to_update - val[i]] 到 dp_old[j_to_update - cnt[i]*val[i]] 这 cnt[i] 个状态中是否有 true。
                        // (LHS上的 dp[j_to_update] 正在被更新)。
                        // 上面 if(dp[j_to_update]) 中的 dp[j_to_update] 指的是它在本次更新尝试之前的状态。
                        if (!dp[j_to_update]) { // 仅当当前为false时才尝试设置为true
                            if (true_in_window_count > 0) { // 如果窗口内（不含dp[j_to_update]自身）有任何可达状态
                                dp[j_to_update] = true;
                            }
                        }

                        // 加上从左边滑入窗口的元素。
                        // 这为下一次迭代 (当 j_to_update 减小时) 准备 true_in_window_count。
                        if (j_entering_from_left >= 0) { // 确保索引有效
                            if (dp[j_entering_from_left]) {
                                true_in_window_count++;
                            }
                        }
                    } // 结束j_to_update的循环
                } // 结束mod的循环
            } // 结束多重背包逻辑
        } // 结束物品i的循环

        int reachable_sums_count = 0;
        // 统计从1到max_sum_m有多少个金额是可以成功找零的
        for (int k = 1; k <= max_sum_m; ++k) {
            if (dp[k]) {
                reachable_sums_count++;
            }
        }
        std::cout << reachable_sums_count << std::endl;
    } // 结束while主循环

    return 0;
}
