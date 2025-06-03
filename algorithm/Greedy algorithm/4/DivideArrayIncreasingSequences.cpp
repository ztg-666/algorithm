#include <vector>

// 将数组分成几个递增序列
// 给你一个有序的正数数组 nums 和整数 K
// 判断该数组是否可以被分成一个或几个 长度至少 为 K 的 不相交的递增子序列
// 数组中的所有数字，都要被，若干不相交的递增子序列包含
// 测试链接 : https://leetcode.cn/problems/divide-array-into-increasing-sequences/


using namespace std;

class Solution {
public:
    bool canDivideIntoSubsequences(vector<int>& nums, int k) {
        // 处理空数组边界情况（题目保证非空，但保留安全检查）
        if (nums.empty()) return false;

        int cnt = 1;          // 当前连续相同元素的计数
        int maxCnt = 1;       // 全局最大连续相同元素计数

        /* 遍历数组统计最大连续重复次数：
           1. 数组已排序（题目条件隐含）
           2. 连续相同元素的数量决定了最少需要的子序列数 */
        for (int i = 1; i < nums.size(); ++i) {
            if (nums[i] == nums[i-1]) {
                ++cnt;
                maxCnt = max(maxCnt, cnt); // 更新全局最大连续数
            } else {
                cnt = 1; // 遇到不同元素，重置当前计数器
            }
        }
        // 最终检查最后一组连续元素的计数
        maxCnt = max(maxCnt, cnt);

        /* 关键判断逻辑：
           - 最少需要maxCnt个子序列（每个子序列包含一个重复元素）
           - 每个子序列最小长度 = 总长度 / 子序列数
           - 当且仅当最小长度≥k时满足条件 */
        return nums.size() / maxCnt >= k;
    }
};
