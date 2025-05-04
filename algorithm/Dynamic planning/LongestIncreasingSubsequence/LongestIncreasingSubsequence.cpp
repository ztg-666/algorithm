#include <vector>
#include <algorithm>

using namespace std;
// 最长递增子序列和最长不下降子序列
// 给定一个整数数组nums
// 找到其中最长严格递增子序列长度、最长不下降子序列长度
// 测试链接 : https://leetcode.cn/problems/longest-increasing-subsequence/
class Solution {
public:
    // 解法一：动态规划（DP）
    // 时间复杂度为 O(n^2)，适用于一般情况下的最长递增子序列（LIS）求解
    int lengthOfLIS1(vector<int>& nums) {
        int n = nums.size();                  // 获取数组长度
        vector<int> dp(n, 1);                 // dp[i] 表示以 nums[i] 结尾的 LIS 长度，默认初始化为 1
        int ans = 0;                          // 存储最终结果

        for (int i = 0; i < n; ++i) {         // 遍历每一个元素
            for (int j = 0; j < i; ++j) {     // 检查之前的所有元素
                if (nums[j] < nums[i]) {      // 如果 nums[j] < nums[i]，说明可以构成递增序列
                    dp[i] = max(dp[i], dp[j] + 1);  // 更新 dp[i]
                }
            }
            ans = max(ans, dp[i]);            // 更新全局最大值
        }

        return ans;                           // 返回最长递增子序列的长度
    }

    // 解法二：贪心 + 二分查找（最优解）
    // 时间复杂度为 O(n * logn)，适用于大规模数据
    int lengthOfLIS2(vector<int>& nums) {
        int n = nums.size();                  // 获取数组长度
        vector<int> ends(n, 0);               // ends 数组用于维护当前“候选”的 LIS 尾部元素
        int len = 0;                          // 记录当前 LIS 的长度

        for (int i = 0; i < n; ++i) {         // 遍历所有元素
            int find = bs1(ends, len, nums[i]);  // 使用 bs1 查找 nums[i] 应该插入的位置
            if (find == -1) {                 // 如果找不到合适位置，则扩展 LIS 长度
                ends[len++] = nums[i];
            } else {                          // 否则替换对应位置上的值
                ends[find] = nums[i];
            }
        }

        return len;                           // 返回最长递增子序列的长度
    }

private:
    // 二分查找函数（用于最长递增子序列）
    // 查找第一个大于等于 num 的位置
    int bs1(vector<int>& ends, int len, int num) {
        int l = 0, r = len - 1, m, ans = -1;
        while (l <= r) {
            m = (l + r) / 2;
            if (ends[m] >= num) {
                ans = m;
                r = m - 1;
            } else {
                l = m + 1;
            }
        }
        return ans;
    }

    // 二分查找函数（用于最长不下降子序列）
    // 查找第一个大于 num 的位置
    int bs2(vector<int>& ends, int len, int num) {
        int l = 0, r = len - 1, m, ans = -1;
        while (l <= r) {
            m = (l + r) / 2;
            if (ends[m] > num) {
                ans = m;
                r = m - 1;
            } else {
                l = m + 1;
            }
        }
        return ans;
    }
};
#include <iostream>

int main() {
    Solution solution;

    // 测试数据
    vector<int> nums = {10, 9, 2, 5, 3, 7, 101, 18};

    // 调用动态规划解法 O(n^2)
    int result1 = solution.lengthOfLIS1(nums);
    cout << "Length of LIS (DP method): " << result1 << endl;

    // 调用贪心 + 二分查找最优解 O(n logn)
    int result2 = solution.lengthOfLIS2(nums);
    cout << "Length of LIS (Greedy + BS method): " << result2 << endl;

    return 0;
}