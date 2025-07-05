#include <vector>
#include <iostream>

// 出现次数大于n/k的数
// 给定一个大小为n的数组nums，给定一个较小的正数k
// 水王数是指在数组中出现次数大于n/k的数
// 返回所有的水王数，如果没有水王数返回空列表
// 测试链接 : https://leetcode.cn/problems/majority-element-ii/

class Solution {
public:

    std::vector<int> majorityElement(std::vector<int>& nums) {
        // 调用通用版本的 majority 函数，k=3
        return majority(nums, 3);
    }

    // 通用函数：找出数组中出现次数大于 n/k 的所有数
    // nums: 输入数组
    // k: 除数
    std::vector<int> majority(std::vector<int>& nums, int k) {
        if (k < 2) {
            // k<2 没有意义，或者说所有数都满足，根据题意返回空或所有数
            // LeetCode 的 k 是固定的，这里做个健壮性处理
            return {};
        }

        // 候选人数组，需要 k-1 个候选人
        // cands[i][0] 是候选的数, cands[i][1] 是这个数的"血量"或"票数"
        // 使用 vector<vector<int>> 来模拟 Java 的 int[][]
        // 大小为 (k-1) x 2，并全部初始化为 0
        int num_candidates = k - 1;
        std::vector<std::vector<int>> cands(num_candidates, std::vector<int>(2, 0));

        // 第一阶段：选出候选人
        for (int num : nums) {
            update(cands, num);
        }

        // 第二阶段：验证候选人并收集结果
        std::vector<int> ans;
        collect(cands, nums, k, ans);
        return ans;
    }

private:
    // 更新候选人列表的逻辑
    // cands: 候选人列表
    // num: 当前遍历到的数字
    void update(std::vector<std::vector<int>>& cands, int num) {
        // 1. 如果 num 已经是候选人，票数增加
        for (auto& cand : cands) {
            if (cand[0] == num && cand[1] > 0) {
                cand[1]++;
                return;
            }
        }

        // 2. 如果 num 不是候选人，寻找一个空位（票数为0的候选人）
        for (auto& cand : cands) {
            if (cand[1] == 0) {
                cand[0] = num;
                cand[1] = 1;
                return;
            }
        }

        // 3. 如果没有空位，所有候选人票数减一
        for (auto& cand : cands) {
            if (cand[1] > 0) {
                cand[1]--;
            }
        }
    }

    // 收集最终结果的逻辑
    // cands: 候选人列表
    // nums: 原始数组
    // divisor_k: 原始的 k 值，用于计算 n/k
    // ans: 结果列表
    void collect(const std::vector<std::vector<int>>& cands, const std::vector<int>& nums, int divisor_k, std::vector<int>& ans) {
        int n = nums.size();
        for (const auto& cand : cands) {
            // 只验证票数大于0的候选人
            if (cand[1] > 0) {
                int current_candidate = cand[0];
                int real_count = 0;
                // 重新计数，获取真实出现次数
                for (int num : nums) {
                    if (current_candidate == num) {
                        real_count++;
                    }
                }
                // 如果真实次数大于 n/k，则加入答案
                if (real_count > n / divisor_k) {
                    ans.push_back(current_candidate);
                }
            }
        }
    }
};

