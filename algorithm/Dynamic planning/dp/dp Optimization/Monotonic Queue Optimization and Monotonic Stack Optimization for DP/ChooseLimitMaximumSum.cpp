#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>

// 不超过连续k个元素的最大累加和
// 给定一个长度为n的数组arr，你可以随意选择数字
// 要求选择的方案中，连续选择的个数不能超过k个
// 返回能得到的最大累加和
// 1 <= n、k <= 10^5
// 0 <= arr[i] <= 10^9
// 测试链接 : https://www.luogu.com.cn/problem/P2627

using namespace std;

const int MAXN = 100001;

int arr[MAXN];          // 原始数组，1-indexed
long long sum[MAXN];    // 前缀和数组，sum[i] = arr[1] + arr[2] + ... + arr[i]
long long dp[MAXN];     // dp[i]表示从数组前i个元素中选择（满足连续不超过k个）的最大累加和
int queue_arr[MAXN];    // 单调队列数组，存储索引
int l, r;               // 单调队列的左右指针
int n, k;               // n为数组长度，k为连续选择的最大个数限制

/**
 * 计算不选择第i个位置数字时的"价值"
 * 这个函数的含义：如果我们不选择第i个位置的数字，
 * 那么dp[i-1] - sum[i]就表示前i-1个位置的最优解减去前i个位置的前缀和
 * 当我们后续加上某个区间的和时，这个值会起到调节作用
 */
long long value(int i) {
    return i == 0 ? 0 : (dp[i - 1] - sum[i]);
}

long long compute() {
    // 第一步：计算前缀和数组
    // sum[i] = arr[1] + arr[2] + ... + arr[i]
    for (int i = 1; i <= n; i++) {
        sum[i] = sum[i - 1] + arr[i];
    }

    // 第二步：初始化单调队列
    l = r = 0;                // 队列为空
    queue_arr[r++] = 0;       // 将索引0加入队列（表示选择0个元素的情况）

    // 第三步：动态规划 + 单调队列优化
    for (int i = 1; i <= n; i++) {
        // 维护单调队列：保持队列中元素的value值单调递减
        // 如果新元素的value值 >= 队尾元素的value值，则移除队尾元素
        // 这样可以保证队首元素总是value值最大的
        while (l < r && value(queue_arr[r - 1]) <= value(i)) {
            r--;
        }
        queue_arr[r++] = i;   // 将当前索引加入队列

        // 维护滑动窗口：如果队首元素超出了k的范围，则移除
        // i - k - 1 表示距离当前位置i超过k个位置的索引
        if (l < r && queue_arr[l] == i - k - 1) {
            l++;
        }

        // 计算dp[i]：选择从某个位置开始到当前位置i的最优解
        // queue_arr[l] 是当前窗口内value值最大的索引
        // value(queue_arr[l]) + sum[i] 表示选择从queue_arr[l]+1到i这个区间的累加和
        dp[i] = value(queue_arr[l]) + sum[i];
    }

    return dp[n];  // 返回考虑所有n个元素的最大累加和
}

int main() {
    // 优化输入输出速度
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 读取数组长度n和连续选择限制k
    cin >> n >> k;

    // 读取数组元素（1-indexed）
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }

    // 输出结果
    cout << compute() << endl;

    return 0;
}
