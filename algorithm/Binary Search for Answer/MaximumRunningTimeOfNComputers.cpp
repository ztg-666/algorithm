#include <iostream>
#include <vector>
#include <algorithm>

// 同时运行N台电脑的最长时间
// 你有 n 台电脑。给你整数 n 和一个下标从 0 开始的整数数组 batteries
// 其中第 i 个电池可以让一台电脑 运行 batteries[i] 分钟
// 你想使用这些电池让 全部 n 台电脑 同时 运行。
// 一开始，你可以给每台电脑连接 至多一个电池
// 然后在任意整数时刻，你都可以将一台电脑与它的电池断开连接，并连接另一个电池，你可以进行这个操作 任意次
// 新连接的电池可以是一个全新的电池，也可以是别的电脑用过的电池
// 断开连接和连接新的电池不会花费任何时间。
// 注意，你不能给电池充电。
// 请你返回你可以让 n 台电脑同时运行的 最长 分钟数。
// 测试链接 : https://leetcode.cn/problems/maximum-running-time-of-n-computers/
class MaximumRunningTimeOfNComputers {
public:
    // 单纯的二分答案法
    // 提交时把函数名改为maxRunTime
    // 时间复杂度O(n * log(sum))，额外空间复杂度O(1)
    static long long maxRunTime1(int num, const std::vector<int>& arr) {
        long long sum = 0;
        // 计算所有电池电量的总和
        for (int x : arr) {
            sum += x;
        }
        long long ans = 0;
        // [0, sum]，不停二分
        for (long long l = 0, r = sum, m; l <= r;) {
            // 计算中间值 m
            m = l + ((r - l) >> 1);
            // 检查是否可以让 num 台电脑共同运行 m 分钟
            if (f1(arr, num, m)) {
                // 如果可以，尝试更大的 m
                ans = m;
                l = m + 1;
            } else {
                // 如果不可以，尝试更小的 m
                r = m - 1;
            }
        }
        return ans; // 返回可以让 num 台电脑共同运行的最长时间
    }

    // 让 num 台电脑共同运行 time 分钟，能不能做到
    static bool f1(const std::vector<int>& arr, int num, long long time) {
        // 碎片电量总和
        long long sum = 0;
        for (int x : arr) {
            if (x > time) {
                // 如果当前电池电量大于 time，减少 num
                num--;
            } else {
                // 如果当前电池电量小于等于 time，累加到 sum
                sum += x;
            }
            // 如果累加的碎片电量总和大于等于 num * time，返回 true
            if (sum >= (long long) num * time) {
                return true;
            }
        }
        return false; // 否则返回 false
    }

    // 二分答案法 + 增加分析(贪心)
    // 提交时把函数名改为maxRunTime
    // 时间复杂度O(n * log(max))，额外空间复杂度O(1)
    static long long maxRunTime2(int num, const std::vector<int>& arr) {
        int maxVal = 0;
        long long sum = 0;
        // 计算所有电池电量的总和和最大值
        for (int x : arr) {
            maxVal = std::max(maxVal, x);
            sum += x;
        }
        // 增加的逻辑
        if (sum > (long long) maxVal * num) {
            // 所有电池的最大电量是 maxVal
            // 如果此时 sum > (long long) maxVal * num，
            // 说明最终的供电时间一定在 >= maxVal，而如果最终的供电时间 >= maxVal
            // 说明对于最终的答案 X 来说，所有电池都是课上讲的"碎片拼接"的概念
            // 那么寻找 ? * num <= sum 的情况中，尽量大的 ? 即可
            // 即 sum / num
            return sum / num;
        }
        // 最终的供电时间一定在 < maxVal 范围上
        // [0, sum] 二分范围，可能定的比较粗，虽然不影响，但毕竟是有点慢
        // [0, maxVal] 二分范围！更精细的范围，二分次数会变少
        int ans = 0;
        for (int l = 0, r = maxVal, m; l <= r;) {
            // 计算中间值 m
            m = l + ((r - l) >> 1);
            // 检查是否可以让 num 台电脑共同运行 m 分钟
            if (f2(arr, num, m)) {
                // 如果可以，尝试更大的 m
                ans = m;
                l = m + 1;
            } else {
                // 如果不可以，尝试更小的 m
                r = m - 1;
            }
        }
        return ans; // 返回可以让 num 台电脑共同运行的最长时间
    }

    // 让 num 台电脑共同运行 time 分钟，能不能做到
    static bool f2(const std::vector<int>& arr, int num, int time) {
        // 碎片电量总和
        long long sum = 0;
        for (int x : arr) {
            if (x > time) {
                // 如果当前电池电量大于 time，减少 num
                num--;
            } else {
                // 如果当前电池电量小于等于 time，累加到 sum
                sum += x;
            }
            // 如果累加的碎片电量总和大于等于 num * time，返回 true
            if (sum >= (long long) num * time) {
                return true;
            }
        }
        return false; // 否则返回 false
    }
};

int main() {
    std::vector<int> batteries = {3, 3, 3}; // 示例输入
    int n = 2; // 示例输入

    // 调用 maxRunTime1 函数
    long long result1 = MaximumRunningTimeOfNComputers::maxRunTime1(n, batteries);
    std::cout << "The maximum running time using maxRunTime1 is: " << result1 << std::endl;

    // 调用 maxRunTime2 函数
    long long result2 = MaximumRunningTimeOfNComputers::maxRunTime2(n, batteries);
    std::cout << "The maximum running time using maxRunTime2 is: " << result2 << std::endl;

    return 0;
}