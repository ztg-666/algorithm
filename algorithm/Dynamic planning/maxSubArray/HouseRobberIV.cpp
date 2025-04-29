#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
// 打家劫舍 IV
// 沿街有一排连续的房屋。每间房屋内都藏有一定的现金
// 现在有一位小偷计划从这些房屋中窃取现金
// 由于相邻的房屋装有相互连通的防盗系统，所以小偷不会窃取相邻的房屋
// 小偷的 窃取能力 定义为他在窃取过程中能从单间房屋中窃取的 最大金额
// 给你一个整数数组 nums 表示每间房屋存放的现金金额
// 第i间房屋中放有nums[i]的钱数
// 另给你一个整数k，表示小偷需要窃取至少 k 间房屋
// 返回小偷需要的最小窃取能力值
// 测试链接 : https://leetcode.cn/problems/house-robber-iv/
class HouseRobberIV {
public:
    int minCapability(const vector<int>& nums, int k) {
        int n = nums.size();
        int l = nums[0], r = nums[0];

        for (int i = 1; i < n; i++) {
            l = min(l, nums[i]);
            r = max(r, nums[i]);
        }

        int m, ans = 0;
        while (l <= r) {
            m = (l + r) / 2;
            if (mostRob1(nums, n, m) >= k) {
                ans = m;
                r = m - 1;
            } else {
                l = m + 1;
            }
        }
        return ans;
    }

    int mostRob1(const vector<int>& nums, int n, int ability) {
        if (n == 1) {
            return nums[0] <= ability? 1 : 0;
        }
        if (n == 2) {
            return (nums[0] <= ability || nums[1] <= ability)? 1 : 0;
        }
        vector<int> dp(n);
        dp[0] = nums[0] <= ability? 1 : 0;
        dp[1] = (nums[0] <= ability || nums[1] <= ability)? 1 : 0;

        for (int i = 2; i < n; i++) {
            dp[i] = max(dp[i - 1], (nums[i] <= ability? 1 : 0) + dp[i - 2]);
        }
        return dp[n - 1];
    }

    int mostRob2(const vector<int>& nums, int n, int ability) {
        if (n == 1) {
            return nums[0] <= ability? 1 : 0;
        }
        if (n == 2) {
            return (nums[0] <= ability || nums[1] <= ability)? 1 : 0;
        }
        int prepre = nums[0] <= ability? 1 : 0;
        int pre = (nums[0] <= ability || nums[1] <= ability)? 1 : 0;

        for (int i = 2, cur; i < n; i++) {
            cur = max(pre, (nums[i] <= ability? 1 : 0) + prepre);
            prepre = pre;
            pre = cur;
        }
        return pre;
    }

    int mostRob3(const vector<int>& nums, int n, int ability) {
        int ans = 0;
        for (int i = 0; i < n; i++) {
            if (nums[i] <= ability) {
                ans++;
                i++;
            }
        }
        return ans;
    }
};

int main() {
    vector<int> nums = {2, 3, 5, 9, 7};
    int k = 2;

    HouseRobberIV solution;

    // Calculate the minimum robbery capability
    cout << "min_ability " << solution.minCapability(nums, k) << endl;

    return 0;
}