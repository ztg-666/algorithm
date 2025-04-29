#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
// 数组中不能选相邻元素的最大累加和
// 给定一个数组，可以随意选择数字
// 但是不能选择相邻的数字，返回能得到的最大累加和
// 测试链接 : https://leetcode.cn/problems/house-robber/
class HouseRobber {
public:
    // 动态规划
    int rob1(const vector<int>& nums) {
        int n = nums.size();
        if (n == 1) {
            return nums[0];
        }
        if (n == 2) {
            return max(nums[0], nums[1]);
        }
        // dp[i] : nums[0...i]范围上可以随意选择数字，但是不能选相邻数，能得到的最大累加和
        vector<int> dp(n);
        dp[0] = nums[0];
        dp[1] = max(nums[0], nums[1]);
        for (int i = 2; i < n; i++) {
            dp[i] = max(dp[i - 1], max(nums[i], dp[i - 2] + nums[i]));
        }
        return dp[n - 1];
    }

    // 空间压缩
    int rob2(const vector<int>& nums) {
        int n = nums.size();
        if (n == 1) {
            return nums[0];
        }
        if (n == 2) {
            return max(nums[0], nums[1]);
        }
        int prepre = nums[0];
        int pre = max(nums[0], nums[1]);
        for (int i = 2, cur; i < n; i++) {
            cur = max(pre, max(nums[i], prepre + nums[i]));
            prepre = pre;
            pre = cur;
        }
        return pre;
    }
};

int main() {
    vector<int> nums = {2, 7, 9, 3, 1};

    HouseRobber solution;

    // 使用动态规划的方式
    cout << "dynamic" << solution.rob1(nums) << endl;

    // 使用空间压缩的方式
    cout << "space" << solution.rob2(nums) << endl;

    return 0;
}
