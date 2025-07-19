#include <vector>
#include <algorithm>

// 达标子数组的个数
// 给定一个长度为n的数组nums，给定两个整数lower和upper
// 子数组达标的条件是累加和在[lower, upper]范围上
// 返回nums中有多少个达标子数组
// 1 <= n <= 10^5
// nums[i]可能是任意整数
// -10^5 <= lower <= upper <= +10^5
// 测试链接 : https://leetcode.cn/problems/count-of-range-sum/

using namespace std;

class Solution {
private:
    static const int MAXN = 100001;
    long long sum[MAXN];    // 前缀和数组
    long long help[MAXN];   // 归并排序辅助数组
    int low, up;           // 目标范围的下界和上界

    /**
     * 归并过程，统计跨越左右两部分的有效区间数量
     * @param l 左边界
     * @param m 中间位置
     * @param r 右边界
     * @return 跨越左右两部分的有效区间数量
     */
    int merge(int l, int m, int r) {
        // 归并分治的统计过程
        int ans = 0;
        int wl = l, wr = l;  // 滑动窗口的左右指针
        long long max_val, min_val;

        // 遍历右半部分的每个前缀和
        for (int i = m + 1; i <= r; i++) {
            // 对于右半部分的前缀和sum[i]，要找左半部分中满足条件的前缀和sum[j]
            // 使得 low <= sum[i] - sum[j] <= up
            // 即 sum[i] - up <= sum[j] <= sum[i] - low
            max_val = sum[i] - low;  // sum[j]的上界
            min_val = sum[i] - up;   // sum[j]的下界

            // 维护有效窗口[wl, wr)，左闭右开
            // wr指向第一个大于max_val的位置
            while (wr <= m && sum[wr] <= max_val) {
                wr++;
            }
            // wl指向第一个大于等于min_val的位置
            while (wl <= m && sum[wl] < min_val) {
                wl++;
            }
            // [wl, wr)区间内的所有前缀和都满足条件
            ans += wr - wl;
        }

        // 正常的归并排序合并过程，保持前缀和数组有序
        int p1 = l;      // 左半部分指针
        int p2 = m + 1;  // 右半部分指针
        int i = l;       // 辅助数组指针

        // 合并两个有序数组
        while (p1 <= m && p2 <= r) {
            help[i++] = (sum[p1] <= sum[p2]) ? sum[p1++] : sum[p2++];
        }
        // 处理剩余元素
        while (p1 <= m) {
            help[i++] = sum[p1++];
        }
        while (p2 <= r) {
            help[i++] = sum[p2++];
        }
        // 将排序结果拷贝回原数组
        for (i = l; i <= r; i++) {
            sum[i] = help[i];
        }
        return ans;
    }

    /**
     * 分治函数，统计区间[l,r]内的有效区间数量
     * @param l 左边界
     * @param r 右边界
     * @return 有效区间数量
     */
    int f(int l, int r) {
        // 递归基：只有一个元素
        if (l == r) {
            // 检查单个前缀和是否在目标范围内
            return (low <= sum[l] && sum[l] <= up) ? 1 : 0;
        }

        // 分治：将问题分解为三部分
        int m = (l + r) / 2;
        // 1. 左半部分内部的有效区间
        // 2. 右半部分内部的有效区间
        // 3. 跨越左右两部分的有效区间
        return f(l, m) + f(m + 1, r) + merge(l, m, r);
    }

public:
    /**
     * 计算区间和在[lower, upper]范围内的子数组个数
     * @param nums 输入数组
     * @param lower 下界
     * @param upper 上界
     * @return 符合条件的子数组个数
     */
    int countRangeSum(vector<int>& nums, int lower, int upper) {
        int n = nums.size();

        // 构建前缀和数组
        // sum[i] = nums[0] + nums[1] + ... + nums[i]
        sum[0] = nums[0];
        for (int i = 1; i < n; i++) {
            sum[i] = sum[i - 1] + nums[i];
        }

        // 保存目标范围
        low = lower;
        up = upper;

        // 开始分治统计
        return f(0, n - 1);
    }
};
