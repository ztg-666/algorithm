#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
// 子数组最大累加和
// 给你一个整数数组 nums
// 返回非空子数组的最大累加和
// 测试链接 : https://leetcode.cn/problems/maximum-subarray/
class MaximumSubarray {
public:
    // 动态规划
    int maxSubArray1(const vector<int>& nums) {
        int n = nums.size();
        // dp[i] : 子数组必须以i位置的数做结尾，往左能延伸出来的最大累加和
        vector<int> dp(n);
        dp[0] = nums[0];
        int ans = nums[0];
        for (int i = 1; i < n; i++) {
            dp[i] = max(nums[i], dp[i - 1] + nums[i]);
            ans = max(ans, dp[i]);
        }
        return ans;
    }

    // 空间压缩
    int maxSubArray2(const vector<int>& nums) {
        int n = nums.size();
        int ans = nums[0];
        for (int i = 1, pre = nums[0]; i < n; i++) {
            pre = max(nums[i], pre + nums[i]);
            ans = max(ans, pre);
        }
        return ans;
    }

    // 如下代码为附加问题的实现
    // 子数组中找到拥有最大累加和的子数组
    // 并返回如下三个信息:
    // 1) 最大累加和子数组的开头left
    // 2) 最大累加和子数组的结尾right
    // 3) 最大累加和子数组的累加和sum
    // 如果不止一个子数组拥有最大累加和，那么找到哪一个都可以
    void extra(const vector<int>& nums, int& left, int& right, int& sum) {
        sum = INT_MIN;
        int l = 0, r = 0, pre = INT_MIN;
        for (r = 0; r < nums.size(); r++) {
            if (pre >= 0) {
                // 吸收前面的累加和有利可图
                // 那就不换开头
                pre += nums[r];
            } else {
                // 吸收前面的累加和已经无利可图
                // 那就换开头
                pre = nums[r];
                l = r;
            }
            if (pre > sum) {
                sum = pre;
                left = l;
                right = r;
            }
        }
    }
};

int main() {
    vector<int> nums = {-2, 1, -3, 4, -1, 2, 1, -5, 4};

    MaximumSubarray solution;

    // 使用动态规划的方式
    cout << "dynamic: " << solution.maxSubArray1(nums) << endl;

    // 使用空间压缩的方式
    cout << "space" << solution.maxSubArray2(nums) << endl;

    // 使用附加问题的方式找最大子数组
    int left, right, sum;
    solution.extra(nums, left, right, sum);
    cout << "sum" << sum << endl;
    cout << "start" << left << endl;
    cout << "end" << right << endl;

    return 0;
}
