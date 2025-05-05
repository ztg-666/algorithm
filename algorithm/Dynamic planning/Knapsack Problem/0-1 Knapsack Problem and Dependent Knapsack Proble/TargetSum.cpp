#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;
// 目标和
// 给你一个非负整数数组 nums 和一个整数 target 。
// 向数组中的每个整数前添加 '+' 或 '-' ，然后串联起所有整数
// 可以构造一个表达式
// 例如nums=[2, 1]，可以在2之前添加'+' ，在1之前添加'-'
// 然后串联起来得到表达式 "+2-1" 。
// 返回可以通过上述方法构造的，运算结果等于 target 的不同表达式的数目
// 测试链接 : https://leetcode.cn/problems/target-sum/
class TargetSum {
public:
    // 方法一：暴力递归（指数时间复杂度）
    // 通过递归遍历所有可能的+/-组合
    int findTargetSumWays1(vector<int>& nums, int target) {
        return f1(nums, target, 0, 0);
    }

    // 递归辅助函数
    // i: 当前处理元素索引，sum: 当前累计和
    int f1(vector<int>& nums, int target, int i, int sum) {
        if (i == nums.size()) {
            return sum == target ? 1 : 0; // 终止条件：达到数组末尾时判断是否命中目标
        }
        // 递归计算+/-当前元素两种情况
        return f1(nums, target, i + 1, sum + nums[i])
             + f1(nums, target, i + 1, sum - nums[i]);
    }

    // 方法二：记忆化搜索（优化重复计算）
    // 使用嵌套哈希表存储中间结果：dp[元素索引][当前和] = 路径数
    unordered_map<int, unordered_map<int, int>> dp;

    int findTargetSumWays2(vector<int>& nums, int target) {
        dp.clear();
        return f2(nums, target, 0, 0);
    }

    int f2(vector<int>& nums, int target, int i, int sum) {
        if (i == nums.size()) {
            return sum == target ? 1 : 0;
        }
        // 命中缓存直接返回
        if (dp.count(i) && dp[i].count(sum)) {
            return dp[i][sum];
        }
        // 计算结果并缓存
        int res = f2(nums, target, i + 1, sum + nums[i])
                + f2(nums, target, i + 1, sum - nums[i]);
        dp[i][sum] = res;
        return res;
    }

    // 方法三：动态规划（处理负数偏移量）
    // 时间复杂度O(n*sum)，空间复杂度O(n*sum)
    int findTargetSumWays3(vector<int>& nums, int target) {
        int sum = 0;
        for (int num : nums) sum += num;
        if (abs(target) > sum) return 0; // 可行性检查

        int offset = sum; // 偏移量处理负数索引
        int size = 2 * sum + 1; // 可能和的取值范围[-sum, sum]
        vector<vector<int>> dp(nums.size() + 1, vector<int>(size, 0));

        // 初始化边界条件
        dp[nums.size()][target + offset] = 1; // 达到目标值时计数为1

        // 逆向递推（从最后一个元素开始）
        for (int i = nums.size() - 1; i >= 0; --i) {
            for (int j = -sum; j <= sum; ++j) {
                // 计算加减当前元素的两种可能性
                int left = j - nums[i] + offset;  // 选择-号
                int right = j + nums[i] + offset; // 选择+号

                // 累加有效范围内的值
                if (left >= 0) dp[i][j + offset] += dp[i + 1][left];
                if (right < size) dp[i][j + offset] += dp[i + 1][right];
            }
        }
        return dp[0][offset]; // 返回初始状态的解
    }

    // 方法四：转换为0-1背包问题（最优解法）
    // 时间复杂度O(n*sum)，空间复杂度O(sum)
    int subsets(vector<int>& nums, int t) {
        if (t < 0) return 0;
        vector<int> dp(t + 1, 0);
        dp[0] = 1; // 空集的方案数为1

        // 标准背包问题模板
        for (int num : nums) {
            // 逆向遍历避免重复计算
            for (int j = t; j >= num; --j) {
                dp[j] += dp[j - num];
            }
        }
        return dp[t];
    }

    int findTargetSumWays4(vector<int>& nums, int target) {
        int sum = 0;
        for (int num : nums) sum += num;
        // 可行性检查：1.绝对值限制 2.奇偶性检查
        if (sum < abs(target) || ((sum + target) & 1)) return 0;

        // 将问题转换为寻找正数子集和s
        int s = (sum + target) / 2;
        return subsets(nums, s);
    }
};

// 测试入口
int main() {
    TargetSum ts;
    vector<int> nums = {1, 1, 1, 1, 1}; // 测试用例
    int target = 3; // 预期输出：5

    // 验证四种方法的结果一致性
    cout << "1: " << ts.findTargetSumWays1(nums, target) << endl;
    cout << "2: " << ts.findTargetSumWays2(nums, target) << endl;
    cout << "3: " << ts.findTargetSumWays3(nums, target) << endl;
    cout << "4: " << ts.findTargetSumWays4(nums, target) << endl;

    return 0;
}
