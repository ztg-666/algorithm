#include <iostream>
#include <vector>
#include <climits>
using namespace std;

const int MAXN = 100001;
// 和至少为K的最短子数组
// 给定一个数组arr，其中的值有可能正、负、0
// 给定一个正数k
// 返回累加和>=k的所有子数组中，最短的子数组长度
class Solution {
public:
    long long sum[MAXN];    // 前缀和数组
    int deque[MAXN];        // 双端队列数组

    int shortestSubarray(vector<int>& arr, int K) {
        int h = 0, t = 0;      // 双端队列的头尾指针
        int n = arr.size();

        // 计算前缀和数组
        sum[0] = 0;
        for (int i = 0; i < n; ++i) {
            sum[i + 1] = sum[i] + arr[i];
        }

        int ans = INT_MAX; // 初始化答案为最大整数
        for (int i = 0; i <= n; ++i) {
            // 检查队列头部是否满足条件
            while (h != t && sum[i] - sum[deque[h]] >= K) {
                // 如果当前前缀和 - 头部前缀和 >= K，更新答案
                ans = min(ans, i - deque[h]);
                h++; // 移动头指针，收缩窗口
            }
            // 维护队列单调性（保持递增）
            while (h != t && sum[deque[t - 1]] >= sum[i]) {
                --t; // 弹出尾部不满足单调性的元素
            }
            deque[t++] = i; // 将当前索引加入队列
        }

        // 返回结果，如果没有找到满足条件的子数组，返回-1
        return ans != INT_MAX ? ans : -1;
    }
};

int main() {
    Solution solution;

    // 测试用例
    vector<int> arr1 = {2, -1, 2};
    int K1 = 3;
    cout << "Test Case 1: " << solution.shortestSubarray(arr1, K1) << endl; // 输出: 3

    vector<int> arr2 = {1, 2};
    int K2 = 4;
    cout << "Test Case 2: " << solution.shortestSubarray(arr2, K2) << endl; // 输出: -1

    vector<int> arr3 = {1, -1, 5, -2, 3};
    int K3 = 3;
    cout << "Test Case 3: " << solution.shortestSubarray(arr3, K3) << endl; // 输出: 1

    vector<int> arr4 = {84, -37, 32, 40, 95};
    int K4 = 167;
    cout << "Test Case 4: " << solution.shortestSubarray(arr4, K4) << endl; // 输出: 3

    return 0;
}
