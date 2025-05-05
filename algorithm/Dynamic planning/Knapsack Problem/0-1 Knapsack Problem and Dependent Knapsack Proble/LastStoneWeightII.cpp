#include <vector>      // 包含 vector 头文件
#include <numeric>     // 包含 accumulate（用于求和）的头文件
#include <algorithm>   // 包含 max 的头文件

using namespace std; // 使用标准命名空间

// 最后一块石头的重量 II
// 有一堆石头，用整数数组 stones 表示
// 其中 stones[i] 表示第 i 块石头的重量。
// 每一回合，从中选出任意两块石头，然后将它们一起粉碎
// 假设石头的重量分别为 x 和 y，且 x <= y
// 那么粉碎的可能结果如下：
// 如果 x == y，那么两块石头都会被完全粉碎；
// 如果 x != y，那么重量为 x 的石头将会完全粉碎，而重量为 y 的石头新重量为 y-x
// 最后，最多只会剩下一块 石头，返回此石头 最小的可能重量
// 如果没有石头剩下，就返回 0
// 测试链接 : https://leetcode.cn/problems/last-stone-weight-ii/

class Solution {
private:
    /**
     * @brief 找到 'stones' 中小于或等于 't' 但尽可能接近 't' 的最大子集和。
     *
     * 此函数使用动态规划和空间优化实现了 0/1 背包问题的逻辑。
     * 它确定了 'stones' 集合中，总和不超过目标容量 't' 的子集可能的最大总和。
     *
     * @param stones 一个常量引用，指向表示石头重量的非负整数向量。
     * @param t 目标容量（允许的最大和）。
     * @return 小于或等于 't' 的最大子集和。
     */
    int findMaxSubsetSumNearTarget(const vector<int>& stones, int t) {
        // dp[j] 将存储容量为 'j' 时可实现的最大子集和。
        // 初始化大小为 t+1 的 dp 数组，所有元素为 0。
        vector<int> dp(t + 1, 0);

        // 遍历每个石头的重量。
        for (int stone_weight : stones) {
            // 从目标 't' 向下迭代到当前石头的重量。
            // 这种反向迭代对于空间优化的 0/1 背包至关重要，
            // 可以避免在同一次外层循环迭代中多次使用同一个物品。
            for (int j = t; j >= stone_weight; --j) {
                // 对于当前容量 'j'，决定是否包含当前石头。
                // 如果我们不包含该石头，最大和保持为 dp[j]（使用之前的石头计算得出）。
                // 如果我们包含该石头（仅当 j >= stone_weight 时可能），最大和将是
                // 该石头的值加上剩余容量（j - stone_weight）可实现的最大和。
                // 我们取这两种可能性的最大值。
                dp[j] = max(dp[j], dp[j - stone_weight] + stone_weight);
            }
        }
        // 目标容量 't' 的最终答案存储在 dp[t] 中。
        return dp[t];
    }

public:
    /**
     * @brief 计算重复粉碎石头后，最后剩下的石头可能的最小重量。
     *
     * 这个问题可以被重新定义为：将石头集合划分为两个子集，
     * 使得它们总和的绝对差最小化。
     * 设所有石头的总重量为 'S'。我们想找到一个子集 P，使其总和 ('sum(P)')
     * 尽可能接近 S/2。另一个子集 N 的总和将是 'S - sum(P)'。
     * 两者之差为 |sum(P) - sum(N)| = |sum(P) - (S - sum(P))| = |2*sum(P) - S|。
     * 为了最小化这个差值，我们需要找到小于或等于 S/2 的最大可能 sum(P)。
     * 设这个最大子集和（<= S/2）为 'max_near_half_sum'。
     * 那么最小差值就是 S - 2 * max_near_half_sum。
     *
     * @param stones 一个表示石头重量的整数向量。
     * @return 最后一块石头的最小可能重量，如果没有石头剩下则返回 0。
     */
    int lastStoneWeightII(vector<int>& stones) {
        // 计算所有石头重量的总和。
        int total_sum = accumulate(stones.begin(), stones.end(), 0);

        // 计算目标和，即总和的一半（整数除法）。
        // 我们的目标是找到一个尽可能接近此目标但不超过它的子集和。
        int target = total_sum / 2;

        // 找到小于或等于目标的最大子集和。
        // 这等价于01背包问题：从stones中选物品，每个物品重量和价值都是stone_weight，
        // 背包容量为target，求能达到的最大价值。
        int max_near_half_sum = findMaxSubsetSumNearTarget(stones, target);

        // 最后一块石头的最小可能重量是总和减去两倍的最大可能子集和（该子集和小于或等于总和的一半）。
        // 这代表了将石头分成两组时可实现的最小差值 (total_sum - max_near_half_sum) - max_near_half_sum。
        return total_sum - 2 * max_near_half_sum;
    }
};


#include <iostream>
int main() {
    Solution sol;
    vector<int> stones1 = {2, 7, 4, 1, 8, 1};
    cout << "{2, 7, 4, 1, 8, 1}: " << sol.lastStoneWeightII(stones1) << endl; // 输出: 1

    vector<int> stones2 = {31, 26, 33, 21, 40};
    cout << "{31, 26, 33, 21, 40}: " << sol.lastStoneWeightII(stones2) << endl; // 输出: 5

    vector<int> stones3 = {1, 2};
     cout << "{1, 2}: " << sol.lastStoneWeightII(stones3) << endl; // 输出: 1

    return 0;
}

