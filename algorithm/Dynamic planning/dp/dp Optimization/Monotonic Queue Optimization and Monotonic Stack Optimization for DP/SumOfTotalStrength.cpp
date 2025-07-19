#include <vector>
#include <stack>

// 巫师力量和
// 题目可以简化为如下的描述
// 给定一个长度为n的数组arr，下标从0开始
// 任何一个子数组的指标为，子数组累加和 * 子数组中最小值
// 返回arr中所有子数组指标的累加和，答案对 1000000007 取模
// 1 <= n <= 10^5
// 1 <= arr[i] <= 10^9
// 测试链接 : https://leetcode.cn/problems/sum-of-total-strength-of-wizards/

using namespace std;

class Solution {
public:
    static const int MOD = 1000000007;

    int totalStrength(vector<int>& arr) {
        int n = arr.size();

        // 构建前缀和的前缀和数组（二重前缀和）
        // sumsum[i] = sum(prefixSum[0] + prefixSum[1] + ... + prefixSum[i])
        // 其中prefixSum[i] = arr[0] + arr[1] + ... + arr[i]
        long long pre = arr[0] % MOD;  // 当前前缀和
        vector<long long> sumsum(n);
        sumsum[0] = pre;

        // 构建二重前缀和数组
        for (int i = 1; i < n; i++) {
            pre = (pre + arr[i]) % MOD;  // 更新前缀和
            sumsum[i] = (sumsum[i - 1] + pre) % MOD;  // 累加前缀和得到二重前缀和
        }

        // 单调栈，用于找到每个元素作为最小值的范围
        vector<int> stack;
        long long ans = 0;

        // 遍历数组，使用单调栈找到每个元素的贡献
        for (int i = 0; i < n; i++) {
            // 当前元素比栈顶元素小时，栈顶元素的右边界确定
            while (!stack.empty() && arr[stack.back()] >= arr[i]) {
                int m = stack.back();  // 当前要计算的元素下标
                stack.pop_back();
                int l = stack.empty() ? -1 : stack.back();  // 左边界
                // 计算以arr[m]为最小值的所有子数组的贡献
                ans = (ans + sum(arr, sumsum, l, m, i)) % MOD;
            }
            stack.push_back(i);
        }

        // 处理栈中剩余的元素（右边界为数组末尾）
        while (!stack.empty()) {
            int m = stack.back();
            stack.pop_back();
            int l = stack.empty() ? -1 : stack.back();
            // 右边界为n（数组长度）
            ans = (ans + sum(arr, sumsum, l, m, n)) % MOD;
        }

        return (int)ans;
    }

private:
    // 计算以arr[m]为最小值的所有子数组的指标贡献
    // l: 左边界（不包含），m: 最小值位置，r: 右边界（不包含）
    long long sum(vector<int>& arr, vector<long long>& sumsum, int l, int m, int r) {
        // 计算左半部分的贡献
        // left表示从m到r-1的所有前缀和乘以(m-l)
        long long left = sumsum[r - 1];
        if (m - 1 >= 0) {
            left = (left - sumsum[m - 1] + MOD) % MOD;
        }
        left = (left * (m - l)) % MOD;

        // 计算右半部分的贡献
        // right表示从l+1到m-1的所有前缀和乘以(r-m)
        long long right = 0;
        if (m - 1 >= 0) {
            right = (right + sumsum[m - 1]) % MOD;
        }
        if (l - 1 >= 0) {
            right = (right - sumsum[l - 1] + MOD) % MOD;
        }
        right = (right * (r - m)) % MOD;

        // 返回最终贡献：(left - right) * arr[m]
        // 这里的公式来自于对所有以arr[m]为最小值的子数组求和的数学推导
        return (long long)(((left - right + MOD) % MOD * arr[m]) % MOD);
    }
};

/*
算法思路：
1. 对于每个元素arr[i]，找到它作为最小值的所有子数组
2. 使用单调栈找到每个元素的左右边界
3. 利用二重前缀和快速计算区间内所有子数组的和
4. 对于每个元素作为最小值的贡献，使用数学公式进行计算

时间复杂度：O(n)
空间复杂度：O(n)
*/
