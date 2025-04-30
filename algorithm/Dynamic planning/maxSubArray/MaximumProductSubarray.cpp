#include <vector>       // 需要使用 std::vector
#include <algorithm>    // 需要使用 std::min 和 std::max
#include <limits>       // 可能有用，但这里用 double 处理溢出情况
#include <cmath>        // 对于混合类型使用 std::max/min 时可能需要，但 <algorithm> 就足够了
#include <iostream>     // 需要使用 std::cout 进行输出
// 乘积最大子数组
// 给你一个整数数组 nums
// 请你找出数组中乘积最大的非空连续子数组
// 并返回该子数组所对应的乘积
// 测试链接 : https://leetcode.cn/problems/maximum-product-subarray/
// ---------------- 函数定义开始 ----------------
int maxProductStandalone(const std::vector<int>& nums) {
    // 如果输入向量可能为空，添加一个检查：
    if (nums.empty()) {
        return 0; // 或者根据要求抛出一个异常。
    }

    // 像在Java代码中一样，使用 double 类型进行计算以避免中间结果溢出。
    double ans = static_cast<double>(nums[0]);          // 存储到目前为止找到的最大乘积。
    double min_prod_ending_here = static_cast<double>(nums[0]); // 以当前索引结尾的子数组的最小乘积。
    double max_prod_ending_here = static_cast<double>(nums[0]); // 以当前索引结尾的子数组的最大乘积。

    // 从第二个元素开始遍历数组。
    for (size_t i = 1; i < nums.size(); ++i) {
        double current_num = static_cast<double>(nums[i]);

        // 存储之前的最大乘积，因为在最大乘积本身可能已更新后，新的最小乘积计算需要用到它。
        // 或者简单地根据 *上一次* 迭代的值来计算新的值。
        double prev_min = min_prod_ending_here;
        double prev_max = max_prod_ending_here;

        // 计算以索引 i 结尾的新的最小乘积。
        // 它可以是当前数字本身，或者是当前数字与之前的最小或最大乘积的乘积。
        // （一个负数可能会使 prev_max * current_num 成为新的最小值）。
        min_prod_ending_here = std::min({current_num,
                                          prev_min * current_num,
                                          prev_max * current_num});

        // 计算以索引 i 结尾的新的最大乘积。
        // 类似的逻辑：它可以是当前数字，或者是与之前的最小或最大乘积的乘积。
        // （一个负数可能会使 prev_min * current_num 成为新的最大值）。
        max_prod_ending_here = std::max({current_num,
                                          prev_min * current_num,
                                          prev_max * current_num});

        // 更新到目前为止找到的整体最大乘积。
        ans = std::max(ans, max_prod_ending_here);
    }

    return static_cast<int>(ans);
}
// ---------------- 函数定义结束 ----------------


// -------- 主函数 main 开始 --------
int main() {
    // 示例测试用例 1
    std::vector<int> nums1 = {2, 3, -2, 4};
    int result1 = maxProductStandalone(nums1);
    std::cout << "Input: {2, 3, -2, 4}, Max Product: " << result1 << std::endl; // 预期输出: 6 (子数组 {2, 3})

    // 示例测试用例 2
    std::vector<int> nums2 = {-2, 0, -1};
    int result2 = maxProductStandalone(nums2);
    std::cout << "Input: {-2, 0, -1}, Max Product: " << result2 << std::endl; // 预期输出: 0 (子数组 {0})

    // 示例测试用例 3 (包含负数相乘得正数)
    std::vector<int> nums3 = {-2, 3, -4};
    int result3 = maxProductStandalone(nums3);
    std::cout << "Input: {-2, 3, -4}, Max Product: " << result3 << std::endl; // 预期输出: 24 (子数组 {-2, 3, -4})

    // 示例测试用例 4 (全负数)
    std::vector<int> nums4 = {-1, -2, -3};
    int result4 = maxProductStandalone(nums4);
    std::cout << "Input: {-1, -2, -3}, Max Product: " << result4 << std::endl; // 预期输出: 6 (子数组 {-2, -3})

    // 示例测试用例 5 (单个元素)
    std::vector<int> nums5 = {-5};
    int result5 = maxProductStandalone(nums5);
    std::cout << "Input: {-5}, Max Product: " << result5 << std::endl; // 预期输出: -5 (子数组 {-5})

    // 示例测试用例 6 (空数组)
    std::vector<int> nums6 = {};
    int result6 = maxProductStandalone(nums6);
    std::cout << "Input: {}, Max Product: " << result6 << std::endl; // 预期输出: 0 (根据函数实现)

    return 0; // 表示程序成功执行
}
// -------- 主函数 main 结束 --------
