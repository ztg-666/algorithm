#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

// 休息k分钟最大会议和
// 给定一个长度为n的数组arr，表示从早到晚发生的会议，各自召开的分钟数
// 当选择一个会议并参加之后，必须休息k分钟
// 返回能参加的会议时长最大累加和
// 比如，arr = { 200, 5, 6, 14, 7, 300 }，k = 15
// 最好的选择为，选择200分钟的会议，然后必须休息15分钟
// 那么接下来的5分钟、6分钟、14分钟的会议注定错过
// 然后放弃7分钟的会议，而选择参加300分钟的会议
// 最终返回500
// 1 <= n、arr[i]、k <= 10^6


// 方法1：不减少枚举的可能性
// 时间复杂度O(n^2)
// 思路：对每个位置，暴力计算休息k分钟后能到达的下一个位置
long long best1(const std::vector<int>& arr, int k) {
    int n = arr.size();
    // dp[i]表示从第i个会议开始到结束能获得的最大时长
    std::vector<long long> dp(n + 1, 0);

    // 从后往前计算dp值
    for (int i = n - 1; i >= 0; i--) {
        int j, sum = 0;
        // 从第i+1个会议开始累加时间，直到累加和>=k（休息时间）
        for (j = i + 1; j < n && sum < k; j++) {
            sum += arr[j];
        }
        // 对于第i个会议，有两种选择：
        // 1. 不参加第i个会议：dp[i+1]
        // 2. 参加第i个会议：arr[i] + dp[j]（j是休息k分钟后能参加的下一个会议位置）
        dp[i] = std::max(dp[i + 1], dp[j] + arr[i]);
    }
    return dp[0]; // 返回从第0个会议开始的最大时长
}

// 方法2：利用预处理结构减少枚举的可能性
// 时间复杂度O(n)
// 思路：预先计算每个位置休息k分钟后能到达的下一个位置，避免重复计算
long long best2(const std::vector<int>& arr, int k) {
    int n = arr.size();
    // jump[i]表示从第i个会议开始，休息k分钟后能参加的下一个会议的位置
    std::vector<int> jump(n);

    // 使用滑动窗口预处理jump数组
    // 窗口[l...r)，左闭右开，sum是窗口累加和
    for (int i = 0, l = 1, r = 1, sum = 0; i < n - 1; i++, l++) {
        // 扩展窗口右边界，直到累加和>=k
        while (r < n && sum < k) {
            sum += arr[r++];
        }
        jump[i] = r; // 记录休息k分钟后能到达的位置
        sum -= arr[l]; // 窗口左边界右移，减去左边界的值
    }
    jump[n - 1] = n; // 最后一个会议后面没有会议了

    // 使用预处理的jump数组进行动态规划
    std::vector<long long> dp(n + 1, 0);
    for (int i = n - 1; i >= 0; i--) {
        // 对于第i个会议，有两种选择：
        // 1. 不参加第i个会议：dp[i+1]
        // 2. 参加第i个会议：arr[i] + dp[jump[i]]
        dp[i] = std::max(dp[i + 1], dp[jump[i]] + arr[i]);
    }
    return dp[0];
}

// 生成随机数组用于测试
// n: 数组长度, v: 数组元素的最大值
std::vector<int> randomArray(int n, int v) {
    std::vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % v + 1; // 生成1到v之间的随机数
    }
    return arr;
}

int main() {
    srand(time(nullptr)); // 初始化随机数种子

    const int n = 1000;      // 最大数组长度
    const int v = 3000;      // 最大数组元素值
    const int testTime = 10000; // 测试次数

    std::cout << "" << std::endl;

    // 进行大量随机测试，验证两种算法结果是否一致
    for (int i = 1; i <= testTime; i++) {
        int size = rand() % n + 1;  // 随机数组长度
        std::vector<int> arr = randomArray(size, v); // 生成随机数组
        int k = rand() % v + 1;     // 随机休息时间

        // 用两种方法计算结果
        long long ans1 = best1(arr, k);
        long long ans2 = best2(arr, k);

        // 如果结果不一致，说明算法有问题
        if (ans1 != ans2) {
            std::cout << "!" << std::endl;
            return 1;
        }
    }

    std::cout << "" << std::endl;
    return 0;
}

/*
问题描述：
给定一个长度为n的数组arr，表示从早到晚发生的会议，各自召开的分钟数
当选择一个会议并参加之后，必须休息k分钟
返回能参加的会议时长最大累加和

算法思路：
这是一个动态规划问题。对于每个会议，我们都有两种选择：
1. 参加这个会议：获得会议时长，但需要休息k分钟
2. 不参加这个会议：直接考虑下一个会议

关键点：
- 如果参加了第i个会议，需要找到休息k分钟后能参加的下一个会议位置
- best1用暴力方法每次都重新计算这个位置
- best2用滑动窗口预处理出所有位置，避免重复计算

时间复杂度：
- best1: O(n^2) - 每个位置都要重新计算下一个可达位置
- best2: O(n) - 预处理 + 动态规划都是线性时间
*/
