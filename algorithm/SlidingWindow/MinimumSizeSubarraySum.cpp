#include <iostream>
#include <vector>
#include <climits>
// 累加和大于等于target的最短子数组长度
// 给定一个含有 n 个正整数的数组和一个正整数 target
// 找到累加和 >= target 的长度最小的子数组并返回其长度
// 如果不存在符合条件的子数组返回0
class MinimumSizeSubarraySum {
public:
    static int minSubArrayLen(int target, std::vector<int>& nums) {
        // 初始化答案为最大整数值，表示尚未找到符合条件的子数组
        int ans = INT_MAX;

        // 使用双指针法，l为左指针，r为右指针，sum为当前窗口内元素的和
        for (int l = 0, r = 0, sum = 0; r < nums.size(); r++) {
            // 将右指针指向的元素加入当前窗口的和中
            sum += nums[r];

            // 检查是否可以通过移除左指针指向的元素来缩小窗口，同时保持窗口内的和大于等于目标值
            while (sum - nums[l] >= target) {
                // 从窗口中移除左指针指向的元素
                sum -= nums[l++];
            }

            // 如果当前窗口的和大于等于目标值，更新最小子数组长度
            if (sum >= target) {
                ans = std::min(ans, r - l + 1);
            }
        }

        // 如果ans仍为INT_MAX，说明没有找到符合条件的子数组，返回0；否则返回找到的最小子数组长度
        return ans == INT_MAX ? 0 : ans;
    }
};

int main() {
    // 测试用例
    std::vector<int> nums = {2, 3, 1, 2, 4, 3};
    int target = 7;

    // 调用minSubArrayLen方法
    int result = MinimumSizeSubarraySum::minSubArrayLen(target, nums);

    // 输出结果
    std::cout << "The minimum length of subarray with sum at least " << target << " is: " << result << std::endl;

    return 0;
}
