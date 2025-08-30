#include <bits/stdc++.h>

// 生成字符串
// 有n个1和m个0，要组成n+m长度的数列，保证任意前缀上，1的数量 >= 0的数量
// 返回有多少种排列方法，答案对 20100403 取模
// 1 <= m <= n <= 10^6
// 测试链接 : https://www.luogu.com.cn/problem/P1641

using namespace std;

const int MOD = 20100403;      // 题目要求的模数
const int MAXN = 2000005;      // n+m 最大可达 2*10^6，所以预处理到这个范围

long long fac[MAXN];           // 阶乘数组 fac[i] = i! % MOD
long long inv[MAXN];           // 阶乘逆元数组 inv[i] = (i!)^-1 % MOD

// 快速幂：计算 x^p % MOD
long long power(long long x, long long p) {
    long long ans = 1;
    while (p > 0) {
        if (p & 1) ans = ans * x % MOD;  // 如果当前二进制位是 1，乘上 x
        x = x * x % MOD;                // 平方
        p >>= 1;                        // 移动到下一位
    }
    return ans;
}

// 预处理阶乘和逆元
void build(int n) {
    fac[0] = inv[0] = 1; // 0! = 1
    fac[1] = 1;
    for (int i = 2; i <= n; i++) {
        fac[i] = fac[i - 1] * i % MOD;  // fac[i] = i! % MOD
    }
    // 计算 (n!)^-1 % MOD，使用费马小定理：a^(p-2) ≡ a^-1 (mod p)
    // 其实这里 MOD=20100403 是质数，所以能用费马小定理。
    inv[n] = power(fac[n], MOD - 2);
    // 从 n-1 往下推，递推公式：inv[i] = inv[i+1] * (i+1) % MOD
    for (int i = n - 1; i >= 1; i--) {
        inv[i] = inv[i + 1] * (i + 1) % MOD;
    }
}

// 计算组合数 C(n,k) = n! / (k! * (n-k)!)
long long C(int n, int k) {
    if (k < 0 || k > n) return 0;  // 不合法的情况
    return fac[n] * inv[k] % MOD * inv[n - k] % MOD;
}

// 主逻辑：Catalan 数公式变形
// 合法排列数 = C(n+m, m) - C(n+m, m-1)
long long compute(int n, int m) {
    build(n + m);
    // 减去不合法的情况 (即某前缀 0 比 1 多)
    return (C(n + m, m) - C(n + m, m - 1) + MOD) % MOD;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    cout << compute(n, m) << "\n";
    return 0;
}
