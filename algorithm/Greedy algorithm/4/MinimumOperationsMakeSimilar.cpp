#include <vector>
#include <algorithm>

// 使数组相似的最少操作次数
// 给你两个正整数数组 nums 和 target ，两个数组长度相等
// 在一次操作中，你可以选择两个 不同 的下标 i 和 j
// 其中 0 <= i, j < nums.length ，并且：
// 令 nums[i] = nums[i] + 2 且
// 令 nums[j] = nums[j] - 2
// 如果两个数组中每个元素出现的频率相等，我们称两个数组是 相似 的
// 请你返回将 nums 变得与 target 相似的最少操作次数
// 测试数据保证nums一定能变得与target相似
// 测试链接 : https://leetcode.cn/problems/minimum-number-of-operations-to-make-arrays-similar/

using namespace std;

class Solution {
public:
    long long makeSimilar(vector<int>& nums, vector<int>& target) {
        int n = nums.size();
        // 分割并获取奇数数量（分割后：前奇后偶）
        int nums_odd = split(nums);
        int target_odd = split(target);

        /* 排序策略：
        1. 对奇数部分单独排序（nums[0..nums_odd-1]）
        2. 对偶数部分单独排序（nums[nums_odd..n-1]）
        排序后相同奇偶性的元素可以一一对应 */
        sort(nums.begin(), nums.begin() + nums_odd);
        sort(nums.begin() + nums_odd, nums.end());
        sort(target.begin(), target.begin() + target_odd);
        sort(target.begin() + target_odd, target.end());

        // 计算所有对应位置的绝对差值之和
        long long ans = 0;
        for (int i = 0; i < n; ++i) {
            // 使用long long防止大数溢出
            ans += abs(static_cast<long long>(nums[i]) - target[i]);
        }
        /* 每次操作可以消除4个单位差异：
           - 操作使一个数+2，另一个数-2
           - 每4个单位差异对应1次有效操作 */
        return ans / 4;
    }

private:
    // 分割函数：将奇数移动到数组前部，返回奇数数量
    int split(vector<int>& arr) {
        int odd_pos = 0; // 奇数存放位置指针
        for (int i = 0; i < arr.size(); ++i) {
            if (arr[i] % 2 != 0) {
                // 发现奇数时，与当前odd_pos位置元素交换
                swap(arr[i], arr[odd_pos++]);
            }
        }
        return odd_pos; // 返回奇数总数
    }
};
