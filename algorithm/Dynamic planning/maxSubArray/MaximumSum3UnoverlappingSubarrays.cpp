#include <vector>
#include <numeric>      // 可能用于求和，并非严格必需
#include <algorithm>    // 用于std::max（尽管此处未直接使用）
#include <limits>       // 用于初始最大值（可选）

// 三个无重叠子数组的最大和
// 给你一个整数数组 nums 和一个整数 k
// 找出三个长度为 k 、互不重叠、且全部数字和（3 * k 项）最大的子数组
// 并返回这三个子数组
// 以下标的数组形式返回结果，数组中的每一项分别指示每个子数组的起始位置
// 如果有多个结果，返回字典序最小的一个
// 测试链接 : https://leetcode.cn/problems/maximum-sum-of-3-non-overlapping-subarrays/
class Solution {
public:
    /**
     * @brief 找到长度为 k 的三个不重叠子数组，使它们的和最大。
     *
     * 给定一个整数数组 nums 和一个整数 k，找到三个长度为 k 的不重叠子数组，
     * 使得这 3 * k 个元素的和最大。返回这三个子数组的起始索引，以向量形式表示。
     * 如果存在多个答案，返回字典序最小的那个。
     *
     * @param nums 输入的整数数组。
     * @param k 每个子数组的长度。
     * @return 一个包含三个起始索引 [a, b, c] 的向量。
     */
    std::vector<int> maxSumOfThreeSubarrays(const std::vector<int>& nums, int k) {
        int n = nums.size();
        // 基本检查：需要至少 3*k 个元素才能有三个不重叠的子数组
        if (n < 3 * k) {
            return {}; // 如果不可能，返回空向量
        }

        // --- 步骤 1：计算所有长度为 k 的子数组的和 ---
        // sums[i] 将存储子数组 nums[i...i+k-1] 的和
        // 我们只需要起始索引从 0 到 n-k 的子数组的和。
        // 使用 long long 以防止求和过程中可能出现的整数溢出。
        std::vector<long long> sums(n - k + 1);
        long long current_sum = 0;

        // 计算第一个窗口（索引 0 到 k-1）的和
        for (int i = 0; i < k; ++i) {
            current_sum += nums[i];
        }
        sums[0] = current_sum;

        // 使用滑动窗口计算后续窗口的和
        for (int i = 1; i <= n - k; ++i) {
            // 减去滑出的元素，加上滑入的元素
            current_sum = current_sum - nums[i - 1] + nums[i + k - 1];
            sums[i] = current_sum;
        }
        // `sums` 向量现在大小为 n-k+1，包含了从索引 0 到 n-k 起始的窗口的和。

        // --- 步骤 2：计算前缀最佳索引 ---
        // prefix[i] 存储在起始索引范围 [0...i] 内长度为 k 的子数组的最大和的*起始索引*。
        // 如果和相等，选择较小的索引（为了字典序）。
        std::vector<int> prefix(n - k + 1);
        prefix[0] = 0; // 在 [0..0] 范围内最佳（唯一）的子数组从 0 开始。
        for (int i = 1; i <= n - k; ++i) {
            // 比较从 i 开始的子数组的和与在 [0..i-1] 范围内最佳子数组的和
            if (sums[i] > sums[prefix[i - 1]]) {
                prefix[i] = i; // 新的最佳子数组从 i 开始
            } else {
                prefix[i] = prefix[i - 1]; // 之前的最佳子数组仍然是最佳的
            }
        }

        // --- 步骤 3：计算后缀最佳索引 ---
        // suffix[i] 存储在起始索引范围 [i...n-k] 内长度为 k 的子数组的最大和的*起始索引*。
        // 如果和相等，选择较小的索引（为了字典序）。
        std::vector<int> suffix(n - k + 1);
        suffix[n - k] = n - k; // 在 [n-k..n-k] 范围内最佳（唯一）的子数组从 n-k 开始。
        for (int i = n - k - 1; i >= 0; --i) {
            // 比较从 i 开始的和与在 [i+1..n-k] 范围内最佳的和
            // 这里使用 '>='：如果 sums[i] == sums[suffix[i+1]]，我们优先选择较小的索引 i。
            if (sums[i] >= sums[suffix[i + 1]]) {
                suffix[i] = i; // 找到新的最佳（或和相等且索引较小）子数组从 i 开始
            } else {
                suffix[i] = suffix[i + 1]; // 之前的最佳子数组仍然是最佳的
            }
        }

        // --- 步骤 4：找到三个子数组的最优组合 ---
        // 遍历中间子数组的所有可能起始位置 'i'。
        // 中间子数组是 nums[i...i+k-1]。
        // 'i' 的范围必须从 k（以允许左边的子数组从 0 开始）
        // 到 n-2k（以允许右边的子数组在 n-1 结束）。
        int a = -1, b = -1, c = -1; // 初始化结果索引
        long long max_total_sum = -1; // 初始化目前找到的最大和

        for (int i = k; i <= n - 2 * k; ++i) {
            // 左边的子数组必须在 [0...i-k] 范围内开始。最佳起始索引是 prefix[i-k]。
            int left_start_idx = prefix[i - k];
            // 右边的子数组必须在 [i+k...n-k] 范围内开始。最佳起始索引是 suffix[i+k]。
            int right_start_idx = suffix[i + k];

            // 计算这个组合的总和
            long long current_total_sum = sums[left_start_idx] + sums[i] + sums[right_start_idx];

            // 检查这个和是否比当前的最大和更好
            // 如果这是第一个有效和或者严格大于当前最大和，就更新。
            // > 比较确保了字典序：因为我们从最小到最大遍历 'i'（中间索引），
            // 当我们第一次遇到最大和时，它将具有最小可能的 'a'（由于前缀逻辑），最小的 'b'（当前的 'i'），
            // 以及最小的 'c'（对于那个 'b' 的后缀逻辑）。
            if (a == -1 || current_total_sum > max_total_sum) {
                max_total_sum = current_total_sum;
                a = left_start_idx;
                b = i;
                c = right_start_idx;
            }
        }

        // 返回三个子数组的起始索引
        return {a, b, c};
    }
};

// 示例用法（可选 - 需要包含 <iostream> 和 <vector>）

#include <iostream>

int main() {
    Solution sol;
    std::vector<int> nums1 = {1, 2, 1, 2, 6, 7, 5, 1};
    int k1 = 2;
    std::vector<int> result1 = sol.maxSumOfThreeSubarrays(nums1, k1);
    std::cout << "Result 1: [" << result1[0] << ", " << result1[1] << ", " << result1[2] << "]" << std::endl; // 预期：[0, 3, 5]

    std::vector<int> nums2 = {1, 2, 1, 2, 1, 2, 1, 2, 1};
    int k2 = 2;
    std::vector<int> result2 = sol.maxSumOfThreeSubarrays(nums2, k2);
    std::cout << "Result 2: [" << result2[0] << ", " << result2[2] << ", " << result2[4] << "]" << std::endl; // 预期：[0, 2, 4]（字典序最小）

    return 0;
}
