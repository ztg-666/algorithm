#include <vector>

// 火柴拼正方形
// 你将得到一个整数数组 matchsticks
// 其中 matchsticks[i] 是第 i 个火柴棒的长度
// 你要用 所有的火柴棍 拼成一个正方形
// 你 不能折断 任何一根火柴棒，但你可以把它们连在一起，而且每根火柴棒必须 使用一次
// 如果你能拼出正方形，则返回 true ，否则返回 false
// 测试链接 : https://leetcode.cn/problems/matchsticks-to-square/

class Solution {
private:
    // nums: 火柴棒数组 (matchsticks)
    // limit: 每条边规定的长度 (target side length)
    // status: 位掩码，表示哪些火柴还可以选 (1 表示可选, 0 表示已用)
    // cur_sum: 当前正在构建的边的已累加长度 (current sum for the side being built)
    // sides_left: 还需要构建的边的数量 (number of sides remaining to be built)
    // dp: 记忆化表. dp[status] 存储当可用火柴状态为 status 时，能否完成任务
    //     dp[status] == 0: 未计算
    //     dp[status] == 1: 可以完成 (true)
    //     dp[status] == -1: 不可以完成 (false)
    // 返回: 能否用光所有火柴去解决剩下的所有边
    bool solve(const std::vector<int>& nums, int limit, int status, int cur_sum, int sides_left, std::vector<int>& dp) {
        if (sides_left == 0) {
            // 所有边都已尝试构建
            // 如果所有火柴都用完了 (status == 0), 则表示成功
            return status == 0;
        }

        // 如果当前状态 (status) 已经计算过，直接返回结果

        if (dp[status] != 0) {
            return dp[status] == 1;
        }

        // 默认为无法完成
        bool possible = false;

        // 遍历所有火柴
        for (int i = 0; i < nums.size(); ++i) {
            // 检查第 i 根火柴是否可用 (status 的第 i 位是否为 1)
            if ((status & (1 << i)) != 0) {
                // 如果将这根火柴用于当前边，长度不超过 limit
                if (cur_sum + nums[i] <= limit) {
                    if (cur_sum + nums[i] == limit) {
                        // 当前火柴恰好完成了当前边
                        // 递归调用: 尝试用剩余火柴 (status ^ (1 << i)) 构建剩余的边 (sides_left - 1)
                        // 新的边从长度 0 开始 (cur_sum = 0)
                        if (solve(nums, limit, status ^ (1 << i), 0, sides_left - 1, dp)) {
                            possible = true;
                            break; // 找到解法，终止当前循环
                        }
                    } else {
                        // 当前火柴未完成当前边，继续构建当前边
                        // 递归调用: 尝试用剩余火柴 (status ^ (1 << i)) 继续构建当前边 (sides_left 不变)
                        // 当前边的长度增加 (cur_sum + nums[i])
                        if (solve(nums, limit, status ^ (1 << i), cur_sum + nums[i], sides_left, dp)) {
                            possible = true;
                            break; // 找到解法，终止当前循环
                        }
                    }
                }
            }
        }

        // 存储当前 status 的计算结果
        dp[status] = possible ? 1 : -1;
        return possible;
    }

public:
    bool makesquare(std::vector<int>& matchsticks) {
        int n = matchsticks.size();
        // matchsticks.length >= 1.
        // 题目也说明了 "你将得到一个整数数组 matchsticks", 暗示不会是空数组。

        int total_sum = 0;
        for (int length : matchsticks) {
            total_sum += length;
        }

        // 根据matchsticks[i] >= 1, 所以 total_sum >= n >= 1.
        // 因此 total_sum 必为正数。
        // 如果总长度不能被4整除，则不可能拼成正方形
        if (total_sum % 4 != 0) {
            return false;
        }

        int side_length = total_sum / 4;

        // dp 数组大小为 2^n，表示所有火柴使用状态的组合
        // 初始化为0 (0: 未计算, 1: true, -1: false)
        std::vector<int> dp(1 << n, 0);

        // 初始调用递归函数:
        // matchsticks: 火柴数组
        // side_length: 正方形的边长
        // (1 << n) - 1: 初始状态，所有火柴都可用 (所有位都为1)
        // 0: 当前边的初始累加长度为0
        // 4: 需要拼凑4条边
        // dp: 记忆化表
        return solve(matchsticks, side_length, (1 << n) - 1, 0, 4, dp);
    }
};
