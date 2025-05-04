#include <vector>
using namespace std;

const int MAXN = 100001; // 定义数组最大长度
// 使数组K递增的最少操作次数
// 给你一个下标从0开始包含n个正整数的数组arr，和一个正整数k
// 如果对于每个满足 k <= i <= n-1 的下标 i
// 都有 arr[i-k] <= arr[i] ，那么称 arr 是K递增的
// 每一次操作中，你可以选择一个下标i并将arr[i]改成任意正整数
// 请你返回对于给定的 k ，使数组变成K递增的最少操作次数
// 测试链接 : https://leetcode.cn/problems/minimum-operations-to-make-the-array-k-increasing/
class Solution {
public:
    int nums[MAXN];  // 用于存储每组子序列的临时数组
    int ends[MAXN];  // 用于最长不下降子序列的辅助数组

    // 主函数：计算使数组 k-递增所需的最少操作次数
    int kIncreasing(vector<int>& arr, int k) {
        int n = arr.size();       // 获取数组总长度
        int ans = 0;              // 初始化最少操作次数为0

        // 将原数组按k分组处理，每一组进行独立分析
        for (int i = 0; i < k; ++i) {
            int size = 0;         // 当前分组的有效元素个数

            // 按照间隔k提取当前分组的元素
            for (int j = i; j < n; j += k) {
                nums[size++] = arr[j];
            }

            // 最少修改次数 = 当前分组长度 - 最长不下降子序列长度
            ans += size - lengthOfNoDecreasing(size);
        }

        return ans; // 返回最终结果
    }

private:
    // 计算最长不下降子序列（LIS）的长度
    int lengthOfNoDecreasing(int size) {
        int len = 0; // 当前最长不下降子序列的长度

        for (int i = 0; i < size; ++i) {
            // 使用二分查找确定当前元素在ends数组中的位置
            int find = bs(len, nums[i]);

            if (find == -1) {
                // 如果找不到合适的位置，则扩展序列长度
                ends[len++] = nums[i];
            } else {
                // 否则更新对应位置的值为更小的nums[i]
                ends[find] = nums[i];
            }
        }

        return len; // 返回最长不下降子序列的长度
    }

    // 二分查找：寻找第一个大于等于当前num的位置
    int bs(int len, int num) {
        int l = 0;             // 左边界
        int r = len - 1;       // 右边界
        int ans = -1;          // 默认找不到满足条件的位置

        while (l <= r) {
            int m = (l + r) / 2; // 中间索引

            if (num < ends[m]) {
                ans = m;      // 找到候选位置，继续向左找更优解
                r = m - 1;
            } else {
                l = m + 1;    // 向右继续查找
            }
        }

        return ans; // 返回找到的位置，若未找到返回-1
    }
};
