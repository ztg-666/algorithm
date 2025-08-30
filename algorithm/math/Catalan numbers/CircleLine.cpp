#include <bits/stdc++.h>

// 圆上连线
// 圆上有2n个点，这些点成对连接起来，形成n条线段，任意两条线段不能相交，返回连接的方法数
// 注意！答案不对 10^9 + 7 取模！而是对 10^8 + 7 取模！
// 1 <= n <= 2999
// 测试链接 : https://www.luogu.com.cn/problem/P1976

using namespace std;

const int MOD = 100000007;   // 注意！这里是 1e8+7，不是 1e9+7
const int MAXN = 1000001;    // 阶乘和逆元数组的最大范围

long long fac[MAXN];   // 阶乘表 fac[i] = i! % MOD
long long inv[MAXN];   // 阶乘逆元表 inv[i] = (i!)^(-1) % MOD

// 快速幂: 计算 (x^p) % MOD
// 用于计算逆元 (费马小定理: a^(MOD-2) ≡ a^(-1) mod MOD)
long long power(long long x, long long p) {
    long long ans = 1;
    while (p > 0) {
        if (p & 1) ans = ans * x % MOD; // 如果 p 的当前二进制位是 1，就乘一次
        x = x * x % MOD;                // 每次循环平方底数
        p >>= 1;                        // 指数右移一位
    }
    return ans;
}

// 预处理阶乘表 fac[] 和阶乘逆元表 inv[]
// fac[i] = i! % MOD
// inv[i] = (i!)^(-1) % MOD
void build(int n) {
    fac[0] = inv[0] = 1;   // 0! = 1, 逆元也定义为 1
    fac[1] = 1;
    for (int i = 2; i <= n; i++) {
        fac[i] = 1LL * i * fac[i - 1] % MOD;  // fac[i] = i! % MOD
    }
    inv[n] = power(fac[n], MOD - 2);  // n! 的逆元，通过快速幂求出
    for (int i = n - 1; i >= 1; i--) {
        // inv[i] = (i!)^(-1) = (i+1)!^(-1) * (i+1)
        inv[i] = 1LL * (i + 1) * inv[i + 1] % MOD;
    }
}

// 组合数 C(n,k) = n! / (k! * (n-k)!) mod MOD
long long C(int n, int k) {
    return 1LL * fac[n] * inv[k] % MOD * inv[n - k] % MOD;
}

// 卡特兰数公式1: Catalan(n) = C(2n,n) - C(2n,n-1)
// 圆上 2n 个点成对不相交连线的方案数
long long compute(int n) {
    build(2 * n);  // 预处理到 2n 阶乘和逆元
    return (C(2 * n, n) - C(2 * n, n - 1) + MOD) % MOD;
    // 注意加上 MOD 防止结果为负数
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;                // 输入点对数 n
    cout << compute(n) << "\n"; // 输出结果
    return 0;
}
