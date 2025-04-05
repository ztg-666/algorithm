#include <vector>
#include <algorithm> // 用于 std::max 和 std::min

// 定义静态 deque 数组的最大大小
const int MAXN = 100001;

// 静态数组用于存储索引的 deque
int maxDeque[MAXN];
int minDeque[MAXN];

// deque 的头和尾指针
int maxh, maxt, minh, mint;

// 辅助函数：检查添加 'number' (arr[r]) 到当前窗口是否保持条件 max - min <= limit。
bool ok(const std::vector<int>& arr, int limit, int number) {
    // 确定如果添加 'number' 后的潜在最大值。
    // 如果 maxDeque 不为空，比较当前最大值 (arr[maxDeque[maxh]]) 与 'number'。
    // 如果 maxDeque 为空，最大值就是 'number'。
    int current_max = (maxh < maxt) ? arr[maxDeque[maxh]] : number;
    int max_val = std::max(current_max, number);

    // 确定如果添加 'number' 后的潜在最小值。
    // 如果 minDeque 不为空，比较当前最小值 (arr[minDeque[minh]]) 与 'number'。
    // 如果 minDeque 为空，最小值就是 'number'。
    int current_min = (minh < mint) ? arr[minDeque[minh]] : number;
    int min_val = std::min(current_min, number);

    // 检查差值是否违反限制。
    return max_val - min_val <= limit;
}

// 辅助函数：将索引 'r' 添加到两个单调 deque，保持 maxDeque 的递减顺序和 minDeque 的递增顺序。
void push(const std::vector<int>& arr, int r) {
    // 维护 maxDeque 的递减顺序（存储值的索引）
    while (maxh < maxt && arr[maxDeque[maxt - 1]] <= arr[r]) {
        maxt--; // 从尾部移除较小或相等的元素
    }
    maxDeque[maxt++] = r; // 将索引 r 添加到尾部

    // 维护 minDeque 的递增顺序（存储值的索引）
    while (minh < mint && arr[minDeque[mint - 1]] >= arr[r]) {
        mint--; // 从尾部移除较大或相等的元素
    }
    minDeque[mint++] = r; // 将索引 r 添加到尾部
}

// 辅助函数：如果索引 'l' 是即将移出窗口的元素，则从 deque 的头部移除它。
void pop(int l) {
    // 如果最左边的元素（索引 l）是 maxDeque 中的最大值，则移除它。
    if (maxh < maxt && maxDeque[maxh] == l) {
        maxh++; // 将头部指针向前移动
    }
    // 如果最左边的元素（索引 l）是 minDeque 中的最小值，则移除它。
    if (minh < mint && minDeque[minh] == l) {
        minh++; // 将头部指针向前移动
    }
}
// 绝对差不超过限制的最长连续子数组
// 给你一个整数数组 nums ，和一个表示限制的整数 limit
// 请你返回最长连续子数组的长度
// 该子数组中的任意两个元素之间的绝对差必须小于或者等于 limit
// 如果不存在满足条件的子数组，则返回 0
// 测试链接 : https://leetcode.cn/problems/longest-continuous-subarray-with-absolute-diff-less-than-or-equal-to-limit/
// 主函数，用于查找最长子数组
int longestSubarray(const std::vector<int>& nums, int limit) {
    // 初始化 deque 指针
    maxh = maxt = minh = mint = 0;
    int n = nums.size();
    int ans = 0;
    int r = 0; // 滑动窗口的右指针

    // 使用左指针遍历数组
    for (int l = 0; l < n; ++l) {
        // 尽可能向右扩展窗口，直到条件不再满足或右指针超出边界。
        // ok() 检查添加 nums[r] 是否保持条件 max - min <= limit。
        // push() 将索引 r 添加到 deque。
        while (r < n && ok(nums, limit, nums[r])) {
            push(nums, r++);
        }

        // 此时，窗口 [l, r-1] 是从 l 开始的最大有效窗口。
        // 更新迄今为止找到的最大长度。
        ans = std::max(ans, r - l);

        // 准备滑动窗口：从 deque 中移除索引 l 处的元素。
        pop(l);
    }
    return ans;
}



// 示例用法（可选）：
#include <iostream>
int main() {
    std::vector<int> nums1 = {8, 2, 4, 7};
    int limit1 = 4;
    std::cout << "Result 1: " << longestSubarray(nums1, limit1) << std::endl; // 输出: 2 ([4, 7] -> |7-4|=3 <= 4)

    std::vector<int> nums2 = {10, 1, 2, 4, 7, 2};
    int limit2 = 5;
    std::cout << "Result 2: " << longestSubarray(nums2, limit2) << std::endl; // 输出: 4 ([2, 4, 7, 2] -> max=7, min=2, |7-2|=5 <= 5)

    std::vector<int> nums3 = {4, 2, 2, 2, 4, 4, 2, 2};
    int limit3 = 0;
    std::cout << "Result 3: " << longestSubarray(nums3, limit3) << std::endl; // 输出: 3 ([2, 2, 2] 或 [4, 4])

    return 0;
}
