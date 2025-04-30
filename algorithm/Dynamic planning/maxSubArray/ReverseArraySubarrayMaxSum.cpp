#include <vector>
#include <numeric> // 用于 std::accumulate（尽管对于Kadane算法并非严格必需）
#include <algorithm> // 用于 std::max, std::reverse
#include <limits>    // 用于 std::numeric_limits
#include <iostream>  // 用于在 main 函数中使用 std::cout
#include <random>    // 用于生成随机数
#include <ctime>     // 用于为随机数生成器设置种子
// 可以翻转1次的情况下子数组最大累加和
// 给定一个数组nums，
// 现在允许你随意选择数组连续一段进行翻转，也就是子数组逆序的调整
// 比如翻转[1,2,3,4,5,6]的[2~4]范围，得到的是[1,2,5,4,3,6]
// 返回必须随意翻转1次之后，子数组的最大累加和
// 对数器验证
// 辅助函数：反转子数组 nums[l...r]
void reverseSubarray(std::vector<int>& nums, int l, int r) {
    // 使用 <algorithm> 中的 std::reverse
    if (l < r && l >= 0 && r < nums.size()) {
         std::reverse(nums.begin() + l, nums.begin() + r + 1);
    }
    // 手动实现（类似于Java代码）：
    // while (l < r) {
    //     int tmp = nums[l];
    //     nums[l++] = nums[r];
    //     nums[r--] = tmp;
    // }
}

// 辅助函数：计算最大子数组和（Kadane算法）
int maxSum(const std::vector<int>& nums) {
    if (nums.empty()) {
        // 定义空数组的处理方式，例如返回0或抛出异常
        // 如果所有数字都是负数，返回0可能会产生误导。
        // 如果最大和可以为负数，返回最小的可能整数可能更好。
        return 0; // 或者根据需要进行处理
    }
    int n = nums.size();
    // 用第一个元素初始化，处理所有数字都是负数的情况
    int max_so_far = nums[0];
    int current_max = nums[0];

    for (int i = 1; i < n; ++i) {
        current_max = std::max(nums[i], current_max + nums[i]);
        max_so_far = std::max(max_so_far, current_max);
    }
    // 如果你绝对需要在所有数字都是负数时返回0（常见的Kadane变体）：
    // max_so_far = std::max(0, max_so_far);
    return max_so_far;
}


// 暴力方法（由于循环内有反转和maxSum操作，时间复杂度为O(n^3)）
// 用于验证目的
int maxSumReverse1(std::vector<int>& nums) { // 接受非常量引用，因为它会修改数组
    if (nums.empty()) return 0; // 或者进行适当的处理
    int n = nums.size();
    // 用原始数组的最大和初始化 overallMaxSum
    int overallMaxSum = maxSum(nums);

    for (int l = 0; l < n; ++l) {
        for (int r = l; r < n; ++r) {
            // 反转子数组 [l, r]
            reverseSubarray(nums, l, r);
            // 计算修改后数组的最大子数组和
            overallMaxSum = std::max(overallMaxSum, maxSum(nums));
            // 反转回来，恢复原始状态以进行下一次迭代
            reverseSubarray(nums, l, r);
        }
    }
    return overallMaxSum;
}

// 优化方法（时间复杂度为O(n)）
// 基于预先计算从右侧开始的最大和
// 并从左侧迭代，考虑分割点。
int maxSumReverse2(const std::vector<int>& nums) {
    int n = nums.size();
    if (n == 0) return 0;
    if (n == 1) return nums[0];

    // start[i]：恰好从索引 i 开始的最大子数组和
    std::vector<int> start(n);
    start[n - 1] = nums[n - 1];
    for (int i = n - 2; i >= 0; --i) {
        start[i] = std::max(nums[i], nums[i] + start[i + 1]);
    }

    // 在尚未考虑分割的情况下，用可能的最佳和初始化整体答案
    // 这可以是 'start' 中的最大值，或者是完整的Kadane算法运行的结果
    int ans = start[0]; // 初始最佳猜测（从0开始的最大和）
    for(int s : start) { // 在 start 数组中找到最大值
        ans = std::max(ans, s);
    }


    // end：恰好结束于索引 i-1 的最大子数组和（Kadane算法中的 'current_max'）
    int end = nums[0];
    // maxEnd：到目前为止 'end' 所取得的最大值（从索引 0 到 i-1）
    int maxEnd = nums[0];

    // 遍历潜在的分割点 'i'
    // 考虑反转的子数组恰好在 'i' 之前结束的情况。
    // 最佳和可能是（在 i 之前结束的最大和）+（从 i 开始的最大和）。
    for (int i = 1; i < n; ++i) {
        // 检查在 'i' 处分割数组是否会产生更好的和
        // maxEnd 表示在 i-1 处或之前结束的最佳和
        // start[i] 表示恰好从 i 开始的最佳和
        // 反转某些子数组可能会将这两部分连接起来。
        ans = std::max(ans, maxEnd + start[i]);

        // 更新左侧部分（在索引 i 处结束）的Kadane算法变量
        end = std::max(nums[i], end + nums[i]);
        maxEnd = std::max(maxEnd, end);
    }

    // 最后，确保 'ans' 也考虑了可能完全存在于数组中而无需分割的最大子数组和
    // （即标准的Kadane算法结果，在循环之后由 maxEnd 捕获）
    ans = std::max(ans, maxEnd);

    return ans;
}

// 用于测试：生成随机数组
std::vector<int> randomArray(int n, int v, std::mt19937& rng) {
    std::vector<int> ans(n);
    // 创建一个范围在 [-v, v] 的分布
    std::uniform_int_distribution<int> dist(-v, v);
    for (int i = 0; i < n; ++i) {
        ans[i] = dist(rng);
    }
    return ans;
}

// 用于测试的主函数（日志检查器）
int main() {
    int n = 50;        // 最大长度
    int v = 200;       // 最大绝对值
    int testTime = 20000; // 测试次数

    // 为随机数生成器设置种子
    std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));

    std::cout << "start..." << std::endl;
    for (int i = 0; i < testTime; ++i) {
        // 生成一个介于 1 和 n 之间的随机长度
        std::uniform_int_distribution<int> len_dist(1, n);
        int len = len_dist(rng);

        // 生成一个随机数组
        std::vector<int> arr = randomArray(len, v, rng);

        // 为暴力方法创建一个副本，因为它会修改数组
        std::vector<int> arr_copy = arr;

        // 运行两种方法
        int ans1 = maxSumReverse1(arr_copy); // 使用副本
        int ans2 = maxSumReverse2(arr);      // 使用原始数组（常量引用）

        // 比较结果
        if (ans1 != ans2) {
            std::cout << "error！" << std::endl;
            std::cout << "array: ";
            for (int x : arr) {
                std::cout << x << " ";
            }
            std::cout << std::endl;
            std::cout << " (ans1): " << ans1 << std::endl;
            std::cout << " (ans2): " << ans2 << std::endl;
            break; // 一旦发现错误就停止
        }

        if ((i + 1) % 1000 == 0) { // 进度指示器
             std::cout << "finish " << (i + 1) << "/" << testTime << " times." << std::endl;
        }
    }
    std::cout << "end." << std::endl;

    return 0;
}