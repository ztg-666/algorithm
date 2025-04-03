#include <vector>
#include <algorithm>
#include <iostream>
// 接雨水
// 给定 n 个非负整数表示每个宽度为 1 的柱子的高度图，计算按此排列的柱子，下雨之后能接多少雨水

class TrappingRainWater {
public:
    // 辅助数组的解法（不是最优解）
    // 时间复杂度O(n)，额外空间复杂度O(n)
    // 提交时改名为trap
    static int trap1(const std::vector<int>& nums) {
        int n = nums.size();
        std::vector<int> lmax(n);
        std::vector<int> rmax(n);

        // 初始化左边界最大值
        lmax[0] = nums[0];
        // 计算每个位置左边的最大值，记录在lmax[i]
        for (int i = 1; i < n; i++) {
            lmax[i] = std::max(lmax[i - 1], nums[i]);
        }

        // 初始化右边界最大值
        rmax[n - 1] = nums[n - 1];
        // 计算每个位置右边的最大值，记录在rmax[i]
        for (int i = n - 2; i >= 0; i--) {
            rmax[i] = std::max(rmax[i + 1], nums[i]);
        }

        int ans = 0;
        // 计算每个位置能存储的雨水量
        //   x              x
        //   0 1 2 3...n-2 n-1
        for (int i = 1; i < n - 1; i++) {
            ans += std::max(0, std::min(lmax[i - 1], rmax[i + 1]) - nums[i]);
        }

        return ans;
    }

    // 双指针的解法（最优解）
    // 时间复杂度O(n)，额外空间复杂度O(1)
    // 提交时改名为trap
    static int trap2(const std::vector<int>& nums) {
        int l = 1, r = nums.size() - 2;
        int lmax = nums[0], rmax = nums[nums.size() - 1];
        int ans = 0;

        // 使用双指针从两端向中间移动
        while (l <= r) {
            if (lmax <= rmax) {
                // 当左边的最大值小于等于右边的最大值时
                ans += std::max(0, lmax - nums[l]);
                lmax = std::max(lmax, nums[l++]);
            } else {
                // 当右边的最大值小于左边的最大值时
                ans += std::max(0, rmax - nums[r]);
                rmax = std::max(rmax, nums[r--]);
            }
        }

        return ans;
    }
};

int main() {
    // 示例数组
    std::vector<int> nums = {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1};

    // 使用辅助数组的方法
    int result1 = TrappingRainWater::trap1(nums);
    std::cout << "Trapped water using trap1: " << result1 << std::endl;

    // 使用双指针的方法
    int result2 = TrappingRainWater::trap2(nums);
    std::cout << "Trapped water using trap2: " << result2 << std::endl;

    return 0;
}
