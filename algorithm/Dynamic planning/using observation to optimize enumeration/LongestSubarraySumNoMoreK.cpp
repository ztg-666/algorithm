#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 累加和不大于k的最长子数组
// 给定一个无序数组arr，长度为n，其中元素可能是正、负、0
// 给定一个整数k，求arr所有的子数组中累加和不大于k的最长子数组长度
// 要求时间复杂度为O(n)
// 测试链接 : https://www.nowcoder.com/practice/3473e545d6924077a4f7cbc850408ade

// 问题：寻找累加和不超过k的最长子数组长度
// 方法1：前缀和+二分查找（O(nlogn)）
// 方法2：滑动窗口优化（O(n)）
const int MAXN = 100001; // 最大数组长度限制

vector<int> nums(MAXN);     // 存储输入数组
vector<int> minSums(MAXN);  // minSums[i]表示从i开始的最小累加和
vector<int> minSumEnds(MAXN); // 对应最小累加和的结束位置
int n, k; // n:数组长度，k:目标累加和上限

// 方法1：前缀和+二分查找实现（时间复杂度O(nlogn)）
int compute1() {
    vector<int> sums(n + 1); // sums[i]表示前i个元素的最大前缀和
    for (int i = 0, sum = 0; i < n; ++i) {
        sum += nums[i];
        // 维护递增的前缀和序列（允许有重复值）
        sums[i + 1] = max(sum, sums[i]);
    }

    int ans = 0;
    for (int i = 0, sum = 0; i < n; ++i) {
        sum += nums[i];
        // 二分查找满足sum - sums[pre] <= k的最大pre（即sums[pre] >= sum - k）
        int l = 0, r = n, m, pre = -1;
        while (l <= r) {
            m = (l + r) / 2;
            if (sums[m] >= sum - k) {
                pre = m;    // 记录候选位置
                r = m - 1;  // 继续向左找更小的位置
            } else {
                l = m + 1;  // 向右查找
            }
        }
        if (pre != -1) {
            ans = max(ans, i - pre + 1); // 更新最大长度
        }
    }
    return ans;
}

// 方法2：滑动窗口优化实现（时间复杂度O(n)）
int compute2() {
    // 预处理每个位置开始的最小累加和及其结束位置
    minSums[n - 1] = nums[n - 1];
    minSumEnds[n - 1] = n - 1;
    for (int i = n - 2; i >= 0; --i) {
        // 如果后面的最小累加和为负，合并当前元素以获得更小和
        if (minSums[i + 1] < 0) {
            minSums[i] = nums[i] + minSums[i + 1];
            minSumEnds[i] = minSumEnds[i + 1];
        } else {
            // 否则独立成段
            minSums[i] = nums[i];
            minSumEnds[i] = i;
        }
    }

    int ans = 0;
    int sum = 0, end = 0; // sum:当前窗口累加和，end:窗口结束位置
    for (int i = 0; i < n; ++i) { // i:窗口起始位置
        // 尽可能扩展窗口到最大有效范围
        while (end < n && sum + minSums[end] <= k) {
            sum += minSums[end];
            end = minSumEnds[end] + 1; // 跳转到最小和区间的下一个位置
        }

        if (end > i) { // 有效窗口
            ans = max(ans, end - i); // 更新最大长度
            sum -= nums[i]; // 移动窗口起始位置前扣除当前元素
        } else { // 无效窗口（当前元素本身已超过k）
            end = i + 1; // 重置窗口从下一个位置开始
        }
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false); // 关闭同步提升IO速度
    cin.tie(0);                  // 解除cin与cout的绑定

    while (cin >> n >> k) {      // 循环处理多个测试用例
        for (int i = 0; i < n; ++i) {
            cin >> nums[i];      // 读取数组元素
        }
        // 选择更优的算法实现
        cout << compute2() << endl; // O(n)解法
        // cout << compute1() << endl; // O(nlogn)解法
    }
    return 0;
}
