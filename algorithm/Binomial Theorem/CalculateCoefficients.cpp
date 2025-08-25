#include <bits/stdc++.h>

// 计算系数
// 多项式为，(ax + by)的k次方，其中a、b、k为常数
// 计算这个多项式展开后，x的n次方 * y的m次方，这一项的系数
// 0 <= k <= 1000
// 0 <= a、b <= 10^6
// n + m == k
// 测试链接 : https://www.luogu.com.cn/problem/P1313

using namespace std;

const int MAXK = 1001;  // k 的最大值（题目保证 k ≤ 1000）
const int MOD = 10007;  // 模数（题目指定）

long long fac[MAXK + 1]; // 阶乘表：fac[i] = i! % MOD
long long inv[MAXK + 1]; // 阶乘逆元表：inv[i] = (i!)^-1 % MOD

int a, b, k, n, m; // 输入参数

// 快速幂：计算 (x^p) % MOD
long long power(long long x, int p) {
    long long ans = 1;
    while (p > 0) {
        if (p & 1) ans = (ans * x) % MOD; // 如果 p 的当前二进制位为 1，则乘一次
        x = (x * x) % MOD; // 每次平方
        p >>= 1;           // p 右移一位
    }
    return ans;
}

// 预处理：构建阶乘表 fac[] 和阶乘逆元表 inv[]
void build() {
    fac[0] = 1; // 0! = 1
    for (int i = 1; i <= MAXK; i++) {
        fac[i] = (fac[i - 1] * i) % MOD; // 递推计算 i! % MOD
    }
    // 使用费马小定理计算阶乘的逆元
    // inv[MAXK] = (fac[MAXK])^(MOD-2) % MOD
    inv[MAXK] = power(fac[MAXK], MOD - 2);
    for (int i = MAXK - 1; i >= 0; i--) {
        // 递推计算 inv[i] = inv[i+1] * (i+1) % MOD
        inv[i] = (inv[i + 1] * (i + 1)) % MOD;
    }
}

// 组合数公式：C(n,k) = n! / (k! * (n-k)!) % MOD
long long C(int n, int k) {
    if (k < 0 || k > n) return 0; // 不合法情况
    return fac[n] * inv[k] % MOD * inv[n - k] % MOD;
}

// 计算 (ax + by)^k 展开后 x^n * y^m 项的系数
long long compute() {
    build(); // 先预处理阶乘和逆元表
    // 系数公式：C(k, n) * a^n * b^m
    return power(a, n) * power(b, m) % MOD * C(k, n) % MOD;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 输入 a, b, k, n, m
    cin >> a >> b >> k >> n >> m;

    // 输出结果
    cout << compute() << "\n";
    return 0;
}
