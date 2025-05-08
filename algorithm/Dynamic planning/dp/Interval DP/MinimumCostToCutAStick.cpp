#include <vector>
#include <algorithm> // 用于 std::sort, std::min
#include <limits>    // 用于 std::numeric_limits

// 切棍子的最小成本
// 有一根长度为n个单位的木棍，棍上从0到n标记了若干位置
// 给你一个整数数组cuts，其中cuts[i]表示你需要将棍子切开的位置
// 你可以按顺序完成切割，也可以根据需要更改切割的顺序
// 每次切割的成本都是当前要切割的棍子的长度，切棍子的总成本是历次切割成本的总和
// 对棍子进行切割将会把一根木棍分成两根较小的木棍
// 这两根木棍的长度和就是切割前木棍的长度
// 返回切棍子的最小总成本
// 测试链接 : https://leetcode.cn/problems/minimum-cost-to-cut-a-stick/

class MinimumCostToCutAStick {
public:
    // minCost1 的递归辅助函数 (记忆化搜索)
    // arr: 扩展后的切割点数组 [0, c1_sorted, c2_sorted, ..., cm_sorted, n]
    //      arr[0] 是木棍段的起点，arr[m+1] 是终点。
    //      arr[1]...arr[m] 是输入 'cuts' 数组中实际的切割点。
    // l, r: 'arr' 中的索引 (对于原始切割点是1-based)，定义了当前要进行的一组切割。
    //       这些切割点是 arr[l], arr[l+1], ..., arr[r]。
    //       它们将在当前从 arr[l-1] 到 arr[r+1] 的木棍段上进行。
    // dp: 记忆化表格。dp[l][r] 存储了在由 arr[l-1] 和 arr[r+1] 定义的木棍段内，
    //     完成从 arr[l] 到 arr[r] 所有切割的最小成本。
    static int f(const std::vector<int>& arr, int l, int r, std::vector<std::vector<int>>& dp) {
        if (l > r) { // 基本情况：在范围 [l, r] 内没有切割点需要处理
            return 0;
        }

        if (dp[l][r] != -1) { // 检查记忆化表格
            return dp[l][r];
        }

        int min_sum_of_subproblem_costs = INT_MAX; // 初始化子问题成本之和为最大值
        // 遍历当前范围 [l, r] 中所有可能的第一个切割点 'k'。
        // arr[k] 是在木棍段 arr[l-1]...arr[r+1] 中进行的第一个切割的位置。
        for (int k = l; k <= r; ++k) {
            // 如果 arr[k] 是第一个切割点，子问题的成本为：
            //   f(arr, l, k - 1, dp): 在木棍段 arr[l-1]...arr[k] 上进行切割 arr[l]...arr[k-1] 的最小成本。
            //   f(arr, k + 1, r, dp): 在木棍段 arr[k]...arr[r+1] 上进行切割 arr[k+1]...arr[r] 的最小成本。
            // 注意：问题定义每次切割的成本是被切割木棍的长度。
            // 递归调用 f(arr, l, k-1, dp) 和 f(arr, k+1, r, dp) 会在其各自的第一次切割时，
            // 进一步加上它们对应木棍段的长度。
            min_sum_of_subproblem_costs = std::min(min_sum_of_subproblem_costs,
                                                   f(arr, l, k - 1, dp) + f(arr, k + 1, r, dp));
        }

        // 在当前木棍段 (从 arr[l-1] 到 arr[r+1]) 进行 *第一次* 切割 (从 l 到 r 中选择一个 k) 的成本
        // 是这个木棍段的长度。
        int cost_of_this_first_cut = arr[r + 1] - arr[l - 1];

        dp[l][r] = cost_of_this_first_cut + min_sum_of_subproblem_costs;
        return dp[l][r];
    }

    // 方法1: 记忆化搜索
    static int minCost1(int n, std::vector<int>& cuts) { // cuts 通过非常量引用传递以允许排序
        int m = cuts.size();
        if (m == 0) {
            return 0; // 没有切割点，没有成本
        }
        std::sort(cuts.begin(), cuts.end()); // 对切割点进行排序

        // 创建一个扩展数组 'arr'，包含 0 和 n 作为边界
        std::vector<int> arr(m + 2);
        arr[0] = 0; // 原始木棍的起点
        for (int i = 0; i < m; ++i) {
            arr[i + 1] = cuts[i]; // 切割点在 'arr' 中是 1-indexed 的
        }
        arr[m + 1] = n; // 原始木棍的终点

        // dp[i][j] 将存储在最初由 arr[i-1] 和 arr[j+1] 定义的木棍段上，
        // 完成从 arr[i] 到 arr[j] 所有切割的最小成本。
        // dp 表中与切割点相关的索引范围是 1 到 m。
        std::vector<std::vector<int>> dp(m + 2, std::vector<int>(m + 2, -1));

        // 我们需要在初始木棍段从 arr[0] (0) 到 arr[m+1] (n) 上，
        // 完成从 arr[1] 到 arr[m] (即排序后的输入切割点) 的所有切割。
        return f(arr, 1, m, dp);
    }

    // 方法2: 严格位置依赖的动态规划
    static int minCost2(int n, std::vector<int>& cuts) { // cuts 通过非常量引用传递以允许排序
        int m = cuts.size();
        if (m == 0) {
            return 0; // 没有切割点，没有成本
        }
        std::sort(cuts.begin(), cuts.end()); // 对切割点进行排序

        std::vector<int> arr(m + 2);
        arr[0] = 0;
        for (int i = 0; i < m; ++i) {
            arr[i + 1] = cuts[i];
        }
        arr[m + 1] = n;

        // dp[l][r] 存储在最初由 arr[l-1] 和 arr[r+1] 定义的木棍段内，
        // 完成从 arr[l] 到 arr[r] 所有切割的最小成本。
        // 切割点的索引 l, r 的范围是 1 到 m。
        // 用 0 初始化：dp[x][y] 其中 x > y 意味着成本为 0 (没有切割)，这是正确的。
        std::vector<std::vector<int>> dp(m + 2, std::vector<int>(m + 2, 0));

        // 循环填充 dp 表。
        // l: 'arr' 中切割点的起始索引 (从 1 到 m)
        // r: 'arr' 中切割点的结束索引 (从 1 到 m)

        // 按照子问题中切割数量 (len_cuts = r - l + 1) 的顺序迭代
        // l 从 m 向下到 1
        // r 从 l 向上到 m

        // 基本情况：考虑对角线，当 l == r 时 (num_cuts = 1)
        // dp[i][i] 表示在木棍段 arr[i-1]...arr[i+1] 上进行单一切割 arr[i]。
        // 成本是 (arr[i+1] - arr[i-1]) + (子问题的成本，对于空范围是0)。
        // 如果空范围的 dp 值正确初始化为 0，则通用循环结构也能处理此情况。
        // 然而，通用递推式可以处理它。如果 l==r，那么 k=l。
        // min_sum = dp[l][l-1] + dp[l+1][l] = 0 + 0 = 0。
        // dp[l][l] = (arr[l+1] - arr[l-1]) + 0。这是正确的。
        // 因此，如果 l>r 的空范围的 dp 表值为 0，则并非严格需要 dp[i][i] 的显式基本情况。

        // 这隐含地意味着我们正在按子问题长度 (r-l+1) 递增的顺序进行计算。
        for (int l_idx = m; l_idx >= 1; --l_idx) {
            for (int r_idx = l_idx; r_idx <= m; ++r_idx) {
                // if (l_idx > r_idx) { // 如果 r_idx 从 l_idx 开始，这不应该发生
                //     dp[l_idx][r_idx] = 0; // 或者跳过
                //     continue;
                // }
                // 上面的 if(l_idx > r_idx) 判断在当前循环下不会为真，因为r_idx从l_idx开始。
                // dp[l_idx][r_idx] 默认是0，当l_idx > r_idx时，表示空区间，成本为0，这是正确的。

                int min_sum_of_subproblem_costs = INT_MAX;
                if (l_idx == r_idx) { // 此范围内只有一个切割点 arr[l_idx]。
                    min_sum_of_subproblem_costs = 0; // 左侧 (l_idx 到 l_idx-1) 和 右侧 (l_idx+1 到 l_idx) 都是空范围。
                } else {
                    // k_idx 是从可用切割点 arr[l_idx]...arr[r_idx] 中选出的，
                    // 在木棍段 arr[l_idx-1]...arr[r_idx+1] 上进行的第一个切割。
                    for (int k_idx = l_idx; k_idx <= r_idx; ++k_idx) {
                        // dp[l_idx][k_idx - 1] 处理左子问题。如果 k_idx=l_idx，则 k_idx-1 < l_idx，
                        // 因此 dp[l_idx][k_idx-1] (例如 dp[l_idx][l_idx-1]) 由于 l>r 的初始化为 0。
                        // dp[k_idx + 1][r_idx] 处理右子问题。如果 k_idx=r_idx，则 k_idx+1 > r_idx，
                        // 因此 dp[k_idx+1][r_idx] (例如 dp[r_idx+1][r_idx]) 为 0。
                        min_sum_of_subproblem_costs = std::min(min_sum_of_subproblem_costs,
                                                               dp[l_idx][k_idx - 1] + dp[k_idx + 1][r_idx]);
                    }
                }

                int cost_of_this_first_cut = arr[r_idx + 1] - arr[l_idx - 1];
                dp[l_idx][r_idx] = cost_of_this_first_cut + min_sum_of_subproblem_costs;
            }
        }
        return dp[1][m]; // 返回完成所有切割点（1到m）的最小总成本
    }
};
