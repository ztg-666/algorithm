#include<bits/stdc++.h>

// 好子集的数目
// 给你一个整数数组 nums，好子集的定义如下：
// nums的某个子集，所有元素的乘积可以表示为一个或多个互不相同质数的乘积
// 比如nums = [1, 2, 3, 4]
// [2, 3]，[1, 2, 3]，[1, 3] 是好子集
// 乘积分别为6=2*3，6=2*3，3=3
// [1, 4]和[4]不是好子集，因为乘积分别为4=2*2和4=2*2
// 返回nums中不同的好子集的数目，答案对 1000000007 取模
// 如果两个子集删除的下标不同，那么它们被视为不同的子集
// 测试链接 : https://leetcode.cn/problems/the-number-of-good-subsets/

class Solution {
public:
    // MAXV是nums中元素的最大可能值
    int MAXV = 30;
    // LIMIT是质数掩码的最大值 + 1。小于等于30的质数有10个 (2,3,5,7,11,13,17,19,23,29)
    // 所以掩码最大为 2^10 - 1，LIMIT = 2^10 = 1024
    int LIMIT = (1 << 10);
    int MOD = 1000000007;

    // 预计算的0-30数字的质因子掩码
    // 每个数字 x (0 <= x <= 30) 对应一个掩码 own[x]
    // 如果 x 的质因子包含 p，则掩码中对应 p 的位为1
    // 如果 x 本身是0, 1, 或者包含重复的质因子 (如 4=2*2, 9=3*3)，则 own[x] = 0，表示它不能作为好子集中的一个有效乘积贡献者
    // 质数顺序 (从低位到高位):
    // 位置: 0  1  2  3  4   5   6   7   8   9
    // 质数: 2, 3, 5, 7, 11, 13, 17, 19, 23, 29
    std::vector<int> own = {
        0b0000000000, // 0: 无效
        0b0000000000, // 1: 不贡献质因子
        0b0000000001, // 2: prime mask for 2
        0b0000000010, // 3: prime mask for 3
        0b0000000000, // 4 = 2*2 (无效)
        0b0000000100, // 5: prime mask for 5
        0b0000000011, // 6 = 2*3
        0b0000001000, // 7: prime mask for 7
        0b0000000000, // 8 = 2*2*2 (无效)
        0b0000000000, // 9 = 3*3 (无效)
        0b0000000101, // 10 = 2*5
        0b0000010000, // 11: prime mask for 11
        0b0000000000, // 12 = 2*2*3 (无效)
        0b0000100000, // 13: prime mask for 13
        0b0000001001, // 14 = 2*7
        0b0000000110, // 15 = 3*5
        0b0000000000, // 16 = 2*2*2*2 (无效)
        0b0001000000, // 17: prime mask for 17
        0b0000000000, // 18 = 2*3*3 (无效)
        0b0010000000, // 19: prime mask for 19
        0b0000000000, // 20 = 2*2*5 (无效)
        0b0000001010, // 21 = 3*7
        0b0000010001, // 22 = 2*11
        0b0100000000, // 23: prime mask for 23
        0b0000000000, // 24 = 2*2*2*3 (无效)
        0b0000000000, // 25 = 5*5 (无效)
        0b0000100001, // 26 = 2*13
        0b0000000000, // 27 = 3*3*3 (无效)
        0b0000000000, // 28 = 2*2*7 (无效)
        0b1000000000, // 29: prime mask for 29
        0b0000000111  // 30 = 2*3*5
    };


    // 方法一：递归 + 记忆化搜索 (动态规划)
    // f1_recursive(i, s, cnt, dp) 表示：
    // 考虑数字 1 到 i (含i)，能够形成质因子掩码为 s 的不同子集的方法数。
    // cnt 是输入数组 nums 中各数字的频率。
    // dp 是记忆化表格。
    int f1_recursive(int i, int s, const std::vector<int>& cnt, std::vector<std::vector<int>>& dp) {
        if (s < 0) return 0; // s不应该为负，防御性编程

        // 基本情况：没有更多大于1的数字可选 (i=0)
        // 如果目标掩码 s 也是 0 (表示空质因子集)，则有1种方法 (即不选择任何数字形成这个空集)。
        // 否则，如果 s 非0，则无法形成，0种方法。
        if (i == 0) {
            return (s == 0) ? 1 : 0;
        }

        // 基本情况：只考虑数字1 (i=1)
        // dp[1][s] 表示只用数字1能凑出掩码s的方法数
        if (i == 1) {
            if (dp[i][s] != -1) return dp[i][s];
            long long current_ans = 0;
            if (s == 0) { // 数字1不贡献任何质因子，所以只能形成掩码0
                current_ans = 1; // 初始为1 (代表由大于1的数构成的空质数集，现在轮到1来贡献)
                // 每个数字1可以选或不选，有 cnt[1] 个1，所以有 2^cnt[1] 种组合
                for (int k = 0; k < cnt[1]; ++k) {
                    current_ans = (current_ans * 2) % MOD;
                }
            } else {
                current_ans = 0; // 仅用数字1无法形成非零质数掩码
            }
            dp[i][s] = static_cast<int>(current_ans);
            return dp[i][s];
        }

        // 如果已计算过，直接返回结果
        if (dp[i][s] != -1) {
            return dp[i][s];
        }

        // 选项1: 不使用当前数字 'i'
        // 方法数等于用 1 到 i-1 的数字形成掩码 s 的方法数
        long long current_ans = f1_recursive(i - 1, s, cnt, dp);

        // 选项2: 使用当前数字 'i'
        int prime_mask_of_i = own[i]; // 数字 i 的质因子掩码
        int count_of_i = cnt[i];      // 数字 i 的出现次数

        // 数字 i 必须是有效的 (own[i] != 0，即没有重复质因子且不为0或1) 且 nums 中存在 (count_of_i > 0)
        if (prime_mask_of_i != 0 && count_of_i > 0) {
            // 数字 i 的质因子集合必须是目标掩码 s 的子集
            // (s & prime_mask_of_i) == prime_mask_of_i 确保 s 包含了 i 的所有质因子
            if ((s & prime_mask_of_i) == prime_mask_of_i) {
                // 如果使用数字 i，则需要用 1 到 i-1 的数字形成剩余的质因子掩码 (s ^ prime_mask_of_i)
                long long ways_with_i_val = f1_recursive(i - 1, s ^ prime_mask_of_i, cnt, dp);
                // 有 count_of_i 种方式选择一个数字 i
                current_ans = (current_ans + ways_with_i_val * count_of_i) % MOD;
            }
        }

        dp[i][s] = static_cast<int>(current_ans);
        return dp[i][s];
    }

    // 解法1的入口函数
    int numberOfGoodSubsets1(const std::vector<int>& nums) {
        // 统计 nums 中 [1, MAXV] 范围内各数字的出现次数
        std::vector<int> cnt(MAXV + 1, 0);
        for (int num : nums) {
            if (num >= 1 && num <= MAXV) {
                cnt[num]++;
            }
        }

        // 初始化DP表，dp[i][s] 表示用数字 1..i 凑成掩码 s 的方法数
        // -1 表示尚未计算
        std::vector<std::vector<int>> dp(MAXV + 1, std::vector<int>(LIMIT, -1));

        long long total_good_subsets = 0;
        // 遍历所有可能的非空质数掩码 s (s=0 表示空集，不符合“一个或多个质数”的要求)
        // f1_recursive(MAXV, s, cnt, dp) 计算使用数字 1..MAXV 得到掩码 's' 的方法数
        // 这些方法数已经考虑了数字1的各种组合 (通过 f1_recursive 中 i=1 的基准情况)
        for (int s = 1; s < LIMIT; ++s) {
            total_good_subsets = (total_good_subsets + f1_recursive(MAXV, s, cnt, dp)) % MOD;
        }

        if (total_good_subsets < 0) total_good_subsets += MOD; // 确保结果非负
        return static_cast<int>(total_good_subsets);
    }

    // 方法二：迭代动态规划 (空间优化)
    int numberOfGoodSubsets2(const std::vector<int>& nums) {
        // 统计 nums 中 [1, MAXV] 范围内各数字的出现次数
        std::vector<int> cnt(MAXV + 1, 0);
        for (int num : nums) {
            if (num >= 1 && num <= MAXV) {
                cnt[num]++;
            }
        }

        // dp[mask] 表示形成质因子掩码为 'mask' 的好子集的方法数
        // 这个方法数是基于使用数字 2..MAXV，并且已经乘上了由数字1贡献的 2^cnt[1] 因子
        std::vector<long long> dp(LIMIT, 0);

        // 初始化 dp[0]。dp[0] 最初代表由大于1的数字构成的空集 (乘积为1)，有1种方法。
        dp[0] = 1;
        // 每个数字 '1' 可以被包含或不包含在子集中。
        // 如果有 cnt[1] 个 '1'，它们可以形成 2^cnt[1] 种不同的子集 (包括空集)。
        // 这些组合将与由其他数字 (2 到 MAXV) 构成的任何好子集相乘。
        for (int k = 0; k < cnt[1]; ++k) {
            dp[0] = (dp[0] * 2) % MOD;
        }
        // 此刻，dp[0] = 2^cnt[1]。这代表只选择1的子集 (对应空质数掩码) 的方法数。
        // 后续计算中，dp[mask] 会自动包含这个因子。

        // 遍历数字从 2 到 MAXV (数字1的影响已在 dp[0] 中处理)
        for (int i = 2; i <= MAXV; ++i) {
            int prime_mask_of_i = own[i]; // 数字 i 的质因子掩码
            int count_of_i = cnt[i];      // 数字 i 的出现次数

            // 如果数字 i 不存在，或者是无效数字 (如4, 9等，own[i]==0)，则跳过
            if (count_of_i == 0 || prime_mask_of_i == 0) {
                continue;
            }

            // 遍历所有可能的目标状态 (掩码 s)
            // 必须从大到小遍历 (LIMIT-1 down to 0)，以确保在计算 dp[status] 时，
            // dp[status ^ prime_mask_of_i] 的值是基于上一个数字 (i-1) 或更早数字的计算结果，
            // 而不是当前数字 i 更新后的结果。这类似于0-1背包问题的空间优化。
            for (int status = LIMIT - 1; status >= 0; --status) {
                // 检查是否可以通过包含数字 'i' 来达到掩码 'status'。
                // 条件是：
                // 1. 'prime_mask_of_i' 必须是 'status' 的子掩码 (即 'status' 包含 'i' 的所有质因子)。
                //    (status & prime_mask_of_i) == prime_mask_of_i
                // 2. 之前的状态 dp[status ^ prime_mask_of_i] > 0，即存在用 <i 的数构成 (status ^ prime_mask_of_i) 的方法。
                if ((status & prime_mask_of_i) == prime_mask_of_i) {
                    // ways_from_prev_mask 是不包含当前数字 i 时，形成 (status ^ prime_mask_of_i) 掩码的方法数。
                    // 这个值已经包含了 2^cnt[1] 的因子。
                    long long ways_from_prev_mask = dp[status ^ prime_mask_of_i];
                    if (ways_from_prev_mask > 0) {
                         // 更新 dp[status]：原有方法数 + (通过添加数字i形成status的方法数)
                         // 添加数字i的方法数 = (形成前置掩码的方法数) * (选择一个i的方法数即count_of_i)
                         dp[status] = (dp[status] + ways_from_prev_mask * count_of_i) % MOD;
                    }
                }
            }
        }

        long long ans = 0;
        // 对所有 s > 0 的 dp[s] 求和。
        // s=0 (即 dp[0]) 对应的是那些只包含数字1或者其非1部分乘积为1的子集，
        // 这些子集的乘积不能表示为“一个或多个互不相同质数”的乘积，因此不是好子集。
        for (int s = 1; s < LIMIT; ++s) {
            ans = (ans + dp[s]) % MOD;
        }

        if (ans < 0) ans += MOD; // 确保模结果为正
        return static_cast<int>(ans);
    }
};


// 主函数，用于测试
int main() {
    Solution sol;
    std::vector<int> nums1 = {1, 2, 3, 4};
    // 测试用例 1
    std::cout << " {1, 2, 3, 4} (sol1) : " << sol.numberOfGoodSubsets1(nums1) << std::endl; // 预期: 6
    std::cout << " {1, 2, 3, 4} (sol2) : " << sol.numberOfGoodSubsets2(nums1) << std::endl; // 预期: 6

    std::vector<int> nums2 = {4, 6, 8, 12};
    // 测试用例 2
    std::cout << " {4, 6, 8, 12} (sol1) : " << sol.numberOfGoodSubsets1(nums2) << std::endl; // 预期: 1 ([6])
    std::cout << " {4, 6, 8, 12} (sol2) : " << sol.numberOfGoodSubsets2(nums2) << std::endl; // 预期: 1

    std::vector<int> nums3 = {1,1,1};
    // 测试用例 3
    std::cout << " {1,1,1} (sol1) : " << sol.numberOfGoodSubsets1(nums3) << std::endl; // 预期: 0
    std::cout << " {1,1,1} (sol2) : " << sol.numberOfGoodSubsets2(nums3) << std::endl; // 预期: 0

    std::vector<int> nums_lc = {5,10,1,26,6,2,21,23};
    // LeetCode 示例测试用例
    std::cout << " {5,10,1,26,6,2,21,23} (sol1) : " << sol.numberOfGoodSubsets1(nums_lc) << std::endl; // 预期: 101
    std::cout << " {5,10,1,26,6,2,21,23} (sol2) : " << sol.numberOfGoodSubsets2(nums_lc) << std::endl; // 预期: 101
    return 0;
}
