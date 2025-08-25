#include <iostream>
#include <vector>

// 分割的方法数
// 有同学找到了测试链接，题意几乎一样，而且数据量小很多
// 唯一的区别是：
// 课上讲的题意，单独的3可以分裂成(1, 2)、(2, 1)，一共两种方式
// 测试链接题意，单独的3可以分裂成(0, 3)、(1, 2)、(2, 1)、(3, 0)，一共四种方式
// 也就是对单独的v来说，课上讲的题意，分裂方式为v-1种。测试链接题意，分裂方式为v+1种
// 别的没有任何区别，实现代码中唯一有注释的那行，是仅有的改动
// 测试链接 : https://leetcode.cn/problems/find-the-count-of-monotonic-pairs-ii/

using namespace std;

const int MOD = 1000000007; // 大质数，用于取模防止溢出

// ----------------------------
// 快速幂：计算 (x^p) % MOD
// ----------------------------
long long power(long long x, long long p) {
    long long ans = 1;
    while (p > 0) {
        if ((p & 1) == 1) {       // 如果p的当前二进制位是1
            ans = (ans * x) % MOD; // 累乘x
        }
        x = (x * x) % MOD; // x翻倍
        p >>= 1;           // p右移一位
    }
    return ans;
}

// --------------------------------------------
// 计算组合数 C(n, k) = n! / (k! * (n-k)!) % MOD
// 使用逆元计算：k! 和 (n-k)! 的逆元
// --------------------------------------------
int c(int n, int k) {
    long long fac = 1;   // 保存n!
    long long inv1 = 1;  // 保存k!的逆元
    long long inv2 = 1;  // 保存(n-k)!的逆元

    // 逐步计算 1 ~ n 的阶乘
    for (int i = 1; i <= n; i++) {
        fac = (fac * i) % MOD; // fac = i! (mod MOD)

        // 当i等于k时，保存k!的逆元
        if (i == k) {
            inv1 = power(fac, MOD - 2);
        }
        // 当i等于(n-k)时，保存(n-k)!的逆元
        if (i == n - k) {
            inv2 = power(fac, MOD - 2);
        }
    }

    // 组合数公式：n! * (k!)^-1 * ((n-k)!)^-1
    return (int) ((((fac * inv1) % MOD) * inv2) % MOD);
}

// --------------------------------------------
// 计算分裂方法数
// arr: 输入数组
// 题意修改：对于arr[0]，其可分裂方式是 arr[0] + 1 种
// --------------------------------------------
int countOfPairs(vector<int>& arr) {
    int n = arr.size();
    int k = arr[0] + 1; // 修改后的初始值

    // 遍历数组，若前一个数比后一个数大，则减少k
    for (int i = 1; i < n && k > 0; i++) {
        if (arr[i - 1] > arr[i]) {
            k -= arr[i - 1] - arr[i]; // 递减调整
        }
    }

    // 如果k <= 0，说明无合法方案
    if (k <= 0) {
        return 0;
    }

    // 计算组合数 C(k + n - 1, n)
    // 对应「球盒模型」(隔板法)
    return c(k + n - 1, n);
}

// ----------------------------
// 主函数：简单测试
// ----------------------------
int main() {
    vector<int> arr = {3, 2, 5, 4}; // 测试样例
    cout << countOfPairs(arr) << endl; // 输出结果

    return 0;
}
