#include <vector>
#include <algorithm>
#include <climits>
#include<iostream>
// 分割数组的最大值(画匠问题)
// 给定一个非负整数数组 nums 和一个整数 m
// 你需要将这个数组分成 m 个非空的连续子数组。
// 设计一个算法使得这 m 个子数组各自和的最大值最小。
class SplitArrayLargestSum {
public:
    // 时间复杂度O(n * log(sum))，额外空间复杂度O(1)
    static int splitArray(const std::vector<int>& nums, int k) {
        // 计算数组所有元素的总和
        long long sum = 0;
        for (int num : nums) {
            sum += num;
        }

        long long ans = 0;
        // 二分查找的左右边界
        for (long long l = 0, r = sum, m, need; l <= r;) {
            // 计算中间值 m
            m = l + ((r - l) >> 1);
            // 计算需要划分成多少部分才能使每部分的和不超过 m
            need = f(nums, m);
            if (need <= k) {
                // 如果划分的部分数量达标，记录当前的 m 作为可能的答案，并尝试更小的 m
                ans = m;
                r = m - 1;
            } else {
                // 如果划分的部分数量不达标，尝试更大的 m
                l = m + 1;
            }
        }
        // 返回最小的最大子数组和
        return static_cast<int>(ans);
    }

    // 必须让数组arr每一部分的累加和 <= limit，请问划分成几个部分才够!
    // 返回需要的部分数量
    static int f(const std::vector<int>& arr, long long limit) {
        int parts = 1; // 初始时认为至少需要一个部分
        int sum = 0;   // 当前部分的累加和

        for (int num : arr) {
            if (num > limit) {
                // 如果单个元素大于 limit，则无法划分
                return INT_MAX;
            }
            if (sum + num > limit) {
                // 如果当前部分的累加和加上当前元素超过 limit，则需要新开一个部分
                parts++;
                sum = num; // 新部分的初始累加和为当前元素
            } else {
                // 否则，继续累加当前元素到当前部分
                sum += num;
            }
        }
        // 返回需要的总部分数量
        return parts;
    }
};

int main() {
    // 示例数据
    std::vector<int> nums = {3, 6, 7, 11};
    int k = 8;

    // 调用splitArray函数
    int result = SplitArrayLargestSum::splitArray(nums, k);

    // 输出结果
    std::cout << "The minimum largest sum of any partition is: " << result << std::endl;

    return 0;
}
