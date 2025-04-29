#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
// 环形数组中不能选相邻元素的最大累加和
// 给定一个数组nums，长度为n
// nums是一个环形数组，下标0和下标n-1是连在一起的
// 可以随意选择数字，但是不能选择相邻的数字
// 返回能得到的最大累加和
// 测试链接 : https://leetcode.cn/problems/house-robber-ii/
class HouseRobberII {
public:
    int rob(const vector<int>& nums) {
        int n = nums.size();
        if (n == 1) {
            return nums[0];
        }
        return max(best(nums, 1, n - 1), nums[0] + best(nums, 2, n - 2));
    }

    // nums[l....r]范围上，没有环形的概念
    // 返回 : 可以随意选择数字，但不能选择相邻数字的情况下，最大累加和
    int best(const vector<int>& nums, int l, int r) {
        if (l > r) {
            return 0;
        }
        if (l == r) {
            return nums[l];
        }
        if (l + 1 == r) {
            return max(nums[l], nums[r]);
        }
        int prepre = nums[l];
        int pre = max(nums[l], nums[l + 1]);
        for (int i = l + 2, cur; i <= r; i++) {
            cur = max(pre, nums[i] + max(0, prepre));
            prepre = pre;
            pre = cur;
        }
        return pre;
    }
};

int main() {
    vector<int> nums = {2, 3, 2};

    HouseRobberII solution;

    // 计算环形数组的最大子数组和
    cout << "sum:" << solution.rob(nums) << endl;

    return 0;
}