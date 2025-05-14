#include <vector>      // 包含 <vector> 头文件，用于使用 std::vector
#include <numeric>     // 包含 <numeric> 头文件 (此版本并非严格需要，但通常与 vector 一起使用以进行如 accumulate 等操作)
#include <algorithm>   // 包含 <algorithm> 头文件 (用于 std::min)
#include <limits>      // 包含 <limits> 头文件 (用于 std::numeric_limits)

// 合并石头的最低成本
// 有 n 堆石头排成一排，第 i 堆中有 stones[i] 块石头
// 每次 移动 需要将 连续的 k 堆石头合并为一堆，而这次移动的成本为这 k 堆中石头的总数
// 返回把所有石头合并成一堆的最低成本
// 如果无法合并成一堆返回-1
// 测试链接 : https://leetcode.cn/problems/minimum-cost-to-merge-stones/

class Solution {
public:
    int mergeStones(std::vector<int>& stones, int k) {
        int n = stones.size(); // 石头堆的数量

        // 如果只有一堆石头，不需要合并，成本为 0。
        if (n == 1) {
            return 0;
        }

        // 如果 (n-1) 不能被 (k-1) 整除，则无法将 n 堆石头合并成 1 堆。
        // 每次合并操作将 k 堆石头变成 1 堆，即减少了 (k-1) 堆。
        // 要从 n 堆变成 1 堆，总共需要减少 (n-1) 堆。
        if ((n - 1) % (k - 1) != 0) {
            return -1;
        }

        // presum[i] 将存储从索引 0 到 i-1 的石头总数。
        // 区间 [l, r] (含两端) 的石头总和是 presum[r+1] - presum[l]。
        std::vector<int> presum(n + 1, 0);
        for (int i = 0; i < n; ++i) {
            presum[i + 1] = presum[i] + stones[i];
        }

        // dp[l][r] 将存储合并区间 [l, r] 内的石头
        // 到规则允许的最少堆数的最小成本。
        // 如果区间 [l,r] 可以合并成一堆 (即 (r-l) % (k-1) == 0)，
        // 那么 dp[l][r] 包含这最后一次合并的成本 (即 stones[l...r] 的总和)。
        // 初始化 dp 表。dp[i][i] = 0 被隐式处理，因为“合并”
        // 单个堆的成本是 0。下面的循环从长度至少为 2 的区间开始。
        std::vector<std::vector<int>> dp(n, std::vector<int>(n, 0));

        // 从右到左遍历石堆区间的左边界 'l'。
        // 考虑的最小子问题长度是 2 (当 r = l+1 时)。
        for (int l = n - 2; l >= 0; --l) {
            // 遍历石堆区间的右边界 'r'。
            for (int r = l + 1; r < n; ++r) {
                // 将当前区间 [l, r] 的最小成本初始化为一个大值。
                dp[l][r] = INT_MAX;

                // 遍历可能的分割点 'm'。
                // 其逻辑是，我们将 stones[l...m] 合并为一堆 (如果可能)，
                // 然后将其与 stones[m+1...r] 合并后的 (k-1) 堆 (如果可能) 进行合并。
                // 步骤 m += (k-1) 至关重要。它确保了 stones[l...m] 段
                // 的长度 (m-l+1) 满足 ((m-l+1)-1) 是 (k-1) 的倍数。
                // 这意味着 stones[l...m] 确实可以最终合并成一堆 (作为一次 k 堆合并的左部)。
                // 此时 dp[l][m] 的成本是将其合并成最少堆数的成本，如果 (m-l)%(k-1)==0，则包含了合并成1堆的成本。
                for (int m = l; m < r; m += (k - 1)) {
                    // dp[l][m] 是解决子区间 [l,m] 并将其合并成符合规则的堆数的最小成本。
                    // dp[m+1][r] 是解决子区间 [m+1,r] 并将其合并成符合规则的堆数的最小成本。
                    // 由于循环顺序，这些子问题已经被解决了。
                    // 它们的值是有限的 (如果没有合并操作则为0，或者为正成本的总和)。
                    if (dp[l][m] != INT_MAX && dp[m + 1][r] != INT_MAX) {
                         dp[l][r] = std::min(dp[l][r], dp[l][m] + dp[m + 1][r]);
                    }
                }

                // 如果当前区间 [l, r] 可以最终合并成一堆
                // (即区间内堆数 (r-l+1) 减 1 是 (k-1) 的倍数，意味着从 (r-l+1) 堆变成 1 堆是可能的)，
                // 那么加上将区间 [l,r] 内所有石头作为最后一步合并成一堆的成本。
                // 前面的循环 dp[l][r] = min(dp[l][m] + dp[m+1][r]) 已经计算了将 l..r 合并成 (k-1)x + 1 堆的成本，
                // 如果 l..r 本身可以合并成1堆，那么这个成本是 l..r 中所有石头的总和。
                if ((r - l) % (k - 1) == 0) {
                    // 如果 dp[l][r] 仍然是其初始的 MAX_VALUE，这意味着没有找到有效的分割 'm'
                    // 来产生子成本。然而，'m' 的循环和子问题的解决应该确保
                    // 如果可以合并，dp[l][r] 会被更新为一个有限值。
                    // （例如 m=l 时，dp[l][l]=0，依赖 dp[l+1][r]）。
                    if (dp[l][r] != INT_MAX) {
                        dp[l][r] += (presum[r + 1] - presum[l]);
                    }
                }
            }
        }

        // 最终答案是合并从索引 0 到 n-1 的所有石头的最小成本。
        return dp[0][n - 1];
    }
};
