#include <vector>

// 定义静态 deque 数组的最大大小
const int MAXN = 100001;
int deque_arr[MAXN]; // 重命名为 'deque_arr' 以避免与 std::deque 冲突（如果包含）
int h, t; // 静态数组 deque 的头和尾指针
// 滑动窗口最大值（单调队列经典用法模版）
// 给你一个整数数组 nums，有一个大小为 k 的滑动窗口从数组的最左侧移动到数组的最右侧
// 你只可以看到在滑动窗口内的 k 个数字。滑动窗口每次只向右移动一位。
// 返回 滑动窗口中的最大值 。
// 函数用于查找每个滑动窗口的最大值
std::vector<int> maxSlidingWindow(const std::vector<int>& arr, int k) {
    h = t = 0; // 初始化头和尾指针
    int n = arr.size();

    if (n == 0 || k == 0) {
        return {}; // 处理空输入或零窗口大小
    }

    // 如果 k 大于数组大小，则调整 k
    k = std::min(k, n);

    // 处理前 k-1 个元素以初始化 deque
    for (int i = 0; i < k - 1; ++i) {
        // 维护 deque 中的单调递减顺序（索引指向值）
        while (h < t && arr[deque_arr[t - 1]] <= arr[i]) {
            t--; // 从尾部移除较小的元素
        }
        deque_arr[t++] = i; // 将当前索引添加到尾部
    }

    // 计算结果向量的大小
    int m = n - k + 1;
    std::vector<int> ans(m);

    // 从左到右滑动窗口
    for (int l = 0, r = k - 1; r < n; ++l, ++r) {
        // 将新元素（索引 r）添加到 deque
        while (h < t && arr[deque_arr[t - 1]] <= arr[r]) {
            t--; // 从尾部移除较小的元素
        }
        deque_arr[t++] = r; // 将当前索引 r 添加到尾部

        // 当前窗口 [l, r] 的最大元素位于 deque 的头部
        ans[l] = arr[deque_arr[h]];

        // 如果索引 l 位于 deque 的头部（已超出窗口），则移除该元素
        if (deque_arr[h] == l) {
            h++; // 将头部指针向前移动
        }
    }

    return ans;
}

// 示例用法（可选）：
#include <iostream>
int main() {
    std::vector<int> nums = {1, 3, -1, -3, 5, 3, 6, 7};
    int k = 3;
    std::vector<int> result = maxSlidingWindow(nums, k);
    std::cout << "Max sliding window: ";
    for (int val : result) {
        std::cout << val << " ";
    }
    std::cout << std::endl; // 输出: Max sliding window: 3 3 5 5 6 7
    return 0;
}
