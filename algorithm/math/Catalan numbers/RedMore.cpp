#include <bits/stdc++.h>

// 任意前缀上红多于黑
// 有n个红和n个黑，要组成2n长度的数列，保证任意前缀上，红的数量 >= 黑的数量
// 返回有多少种排列方法，答案对 100 取模
// 1 <= n <= 100
// 测试链接 : https://www.luogu.com.cn/problem/P1722

using namespace std;

const int MOD = 100;  // 注意：题目要求结果对 100 取模，而不是常见的 1e9+7

// 计算卡特兰数的方法数：
// 问题转化为：有 n 个红球和 n 个黑球，组成长度 2n 的序列，
// 要求任何前缀中红球数 ≥ 黑球数。
// 这个数量正好是第 n 个卡特兰数 Catalan(n)。
//
// 卡特兰数的递推公式：
//   f[0] = 1
//   f[n] = Σ (f[l] * f[n-1-l])，其中 l 从 0 到 n-1
// 这就是著名的“二叉树括号化方案数”递推。
//
// 由于 MOD=100，不是质数，不能用逆元公式（公式1、2、3）。
// 所以必须采用递推公式4，直接用 DP 计算。
long long compute(int n) {
    vector<long long> f(n + 1, 0);
    f[0] = f[1] = 1;  // 初始化：f[0] = 1, f[1] = 1

    // 按照递推关系计算 f[i]
    for (int i = 2; i <= n; i++) {
        for (int l = 0, r = i - 1; l < i; l++, r--) {
            // f[i] = sum_{l=0}^{i-1} f[l] * f[i-1-l]
            f[i] = (f[i] + f[l] * f[r] % MOD) % MOD;
        }
    }
    return f[n];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;              // 输入 n
    cout << compute(n) << "\n";  // 输出答案（对 100 取模）
    return 0;
}
