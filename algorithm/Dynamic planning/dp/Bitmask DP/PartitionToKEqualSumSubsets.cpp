#include <vector>      // C++标准库中的动态数组
#include <numeric>     // 用于 std::accumulate (求和)
#include <algorithm>   // 用于 std::sort (排序)
// #include <vector> // 重复包含了，确保它存在即可

// 划分为k个相等的子集
// 给定一个整数数组 nums 和一个正整数 k，
// 找出是否有可能把这个数组分成 k 个非空子集，其总和都相等。
// 测试链接 : https://leetcode.cn/problems/partition-to-k-equal-sum-subsets/

class Solution {
private:
    // canPartitionKSubsets1 的辅助函数 (状压dp的解法)
    // nums: 输入的数字
    // limit: 每个子集的目标和
    // status: 位掩码，表示可用的数字 (1 表示可用, 0 表示已使用)
    // cur: 正在形成的子集的当前和
    // rest: 还需要形成的子集的数量
    // dp: 记忆化表格; dp[status] 存储状态为 'status' 的数字是否可以被划分
    //     dp[status] = 0 (未计算), 1 (true), -1 (false)
    bool f1_helper(const std::vector<int>& nums, int limit, int status, int cur, int rest, std::vector<int>& dp) {
        if (rest == 0) { // 所有 k 个子集都已成功形成
            return status == 0; // 如果所有数字都已使用则为 true，否则为 false
        }
        if (dp[status] != 0) { // 如果此状态已计算过
            return dp[status] == 1;
        }

        bool ans = false;
        // 遍历所有数字，看它们是否可以被添加到当前子集中
        for (size_t i = 0; i < nums.size(); ++i) {
            if ((status & (1 << i)) != 0) { // 如果第 i 个数字可用
                if (cur + nums[i] <= limit) { // 如果添加 nums[i] 不超过目标和
                    if (cur + nums[i] == limit) { // 当前子集已完成
                        ans = f1_helper(nums, limit, status ^ (1 << i), 0, rest - 1, dp);
                    } else { // 当前子集尚未完成，添加 nums[i] 并继续
                        ans = f1_helper(nums, limit, status ^ (1 << i), cur + nums[i], rest, dp);
                    }
                    if (ans) { // 如果找到了一个有效的划分
                        break;
                    }
                }
            }
        }
        dp[status] = ans ? 1 : -1; // 将结果存储在记忆化表格中
        return ans;
    }

    // canPartitionKSubsets2 的辅助函数 (纯暴力的递归 + 剪枝策略)
    // group: vector，其中 group[j] 是第 j 个子集的当前和
    // target: 每个子集的目标和
    // nums: 输入的数字，已排序 (升序，但从最大的开始处理)
    // index: 当前在 nums 中要考虑的索引 (从 nums.size()-1 向下处理到 0)
    bool f2_helper(std::vector<int>& group, int target, const std::vector<int>& nums, int index) {
        if (index < 0) { // 所有数字都已放入子集
            return true;
        }

        int num_to_place = nums[index]; // 当前要放置的数字 (从大到小)

        // 尝试将 num_to_place 放入 k 个组中的一个
        for (size_t i = 0; i < group.size(); ++i) {
            if (group[i] + num_to_place <= target) {
                group[i] += num_to_place; // 将数字放入组 i
                if (f2_helper(group, target, nums, index - 1)) { // 为下一个数字递归
                    return true;
                }
                group[i] -= num_to_place; // 回溯：从组 i 中移除数字

                // 剪枝：如果将 num_to_place 放入 group[i] (在添加 num_to_place 之前的和为 group[i])
                // 未能得到解，并且 group[i+1] 的和与 group[i] 相同，
                // 那么尝试将 num_to_place 放入 group[i+1] 是多余的。跳到下一个不同的组和。

                while (i + 1 < group.size() && group[i] == group[i + 1]) {
                    i++;
                }

                // 如果 group[i] 为 0 (意味着 num_to_place 被放入一个空组并且失败了),
                // 那么就跳出循环。没有必要为 num_to_place 尝试其他空组，因为它们是对称的。
                // if (group[i] == 0) break;
            }
        }
        return false; // num_to_place 无法放入任何组以形成有效的划分
    }

public:
    // 状压dp的解法
    // 这是最正式的解
    bool canPartitionKSubsets1(const std::vector<int>& nums, int k) {
        int sum = std::accumulate(nums.begin(), nums.end(), 0); // 计算总和

        if (k <= 0 || sum % k != 0) { // k 必须为正，并且总和必须能被 k 整除
            return false;
        }
        int n = nums.size();
        if (n < k) { // 数字数量不足以形成 k 个非空子集
            return false;
        }

        int target_sum_per_subset = sum / k; // 每个子集的目标和
        // dp[status] = 0 (未计算), 1 (true), -1 (false)
        std::vector<int> dp(1 << n, 0);

        // 初始调用: 所有数字都可用 (status = (1<<n)-1), 当前和为 0, 需要形成 k 个子集
        return f1_helper(nums, target_sum_per_subset, (1 << n) - 1, 0, k, dp);
    }

    // 纯暴力的递归（不做任何动态规划），利用良好的剪枝策略，可以做到非常好的效率
    // 但这并不是正式的解，如果数据设计的很苛刻，是通过不了的
    bool canPartitionKSubsets2(std::vector<int>& nums, int k) { // nums 是非 const 的，允许排序
        int sum = std::accumulate(nums.begin(), nums.end(), 0); // 计算总和

        if (k <= 0 || sum % k != 0) {
            return false;
        }
        int n = nums.size();
        if (n < k) {
            return false;
        }

        // 对 nums 进行排序。辅助函数 f2_helper 将从最大值开始处理。
        // 排序有助于剪枝更有效。
        std::sort(nums.begin(), nums.end());

        std::vector<int> group(k, 0); // group[j] 存储第 j 个子集的当前和
        int target_sum_per_subset = sum / k; // 每个子集的目标和

        // 从最大的数字开始 (升序排序后的索引 n-1)
        return f2_helper(group, target_sum_per_subset, nums, n - 1);
    }
};
