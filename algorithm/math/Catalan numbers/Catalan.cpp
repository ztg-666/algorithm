#include <bits/stdc++.h>

// 卡特兰数模版
// 进栈顺序规定为1、2、3..n，返回有多少种不同的出栈顺序
// 测试题目的数据量很小，得到的卡特兰数没有多大，不需要取模处理
// 但是请假设，当n比较大时，卡特兰数是很大的，答案对 1000000007 取模
// 测试链接 : https://www.luogu.com.cn/problem/P1044

using namespace std;

const int MOD = 1000000007;  // 取模常数
const int MAXN = 1000001;    // 最大支持的 n

long long fac[MAXN];   // 阶乘表 fac[i] = i!
long long inv1[MAXN];  // 阶乘逆元表 inv1[i] = (i!)^(-1) mod MOD
long long inv2[MAXN];  // 连续数逆元表 inv2[i] = i^(-1) mod MOD

// 快速幂: 计算 x^p mod MOD
long long power(long long x, long long p) {
    long long ans = 1;
    while (p > 0) {
        if (p & 1) ans = ans * x % MOD;
        x = x * x % MOD;
        p >>= 1;
    }
    return ans;
}

// 生成阶乘表和阶乘逆元表
// fac[i] = i!
// inv1[i] = (i!)^(-1) mod MOD
// 利用逆元递推：inv1[i] = (i+1) * inv1[i+1] mod MOD
void build1(int n) {
    fac[0] = inv1[0] = 1;
    fac[1] = 1;
    for (int i = 2; i <= n; i++) {
        fac[i] = 1LL * i * fac[i - 1] % MOD;
    }
    inv1[n] = power(fac[n], MOD - 2);  // n! 的逆元
    for (int i = n - 1; i >= 1; i--) {
        inv1[i] = 1LL * (i + 1) * inv1[i + 1] % MOD;
    }
}

// 生成连续数逆元表
// inv2[i] = i^(-1) mod MOD
// 用公式：inv2[i] = (MOD - MOD/i) * inv2[MOD % i] mod MOD
void build2(int n) {
    inv2[1] = 1;
    for (int i = 2; i <= n + 1; i++) {
        inv2[i] = (MOD - 1LL * (MOD / i) * inv2[MOD % i] % MOD) % MOD;
    }
}

// 组合数 C(n, k) = n! / (k! * (n-k)!)
long long C(int n, int k) {
    return 1LL * fac[n] * inv1[k] % MOD * inv1[n - k] % MOD;
}

// 公式1: Catalan(n) = C(2n, n) - C(2n, n-1)
// 常用公式之一，结果 mod MOD
long long compute1(int n) {
    build1(2 * n);
    return (C(2 * n, n) - C(2 * n, n - 1) + MOD) % MOD;
}

// 公式2: Catalan(n) = C(2n, n) / (n+1)
// 通过乘法逆元实现除法
long long compute2(int n) {
    build1(2 * n);
    return C(2 * n, n) * power(n + 1, MOD - 2) % MOD;
}

// 公式3: Catalan(n) 递推式
// f[n] = f[n-1] * (4n - 2) / (n+1)
// 需要用到连续数逆元 inv2[]
long long compute3(int n) {
    build2(n);
    vector<long long> f(n + 1, 0);
    f[0] = f[1] = 1;
    for (int i = 2; i <= n; i++) {
        f[i] = f[i - 1] * (4LL * i - 2) % MOD * inv2[i + 1] % MOD;
    }
    return f[n];
}

// 公式4: Catalan(n) 经典递推
// f[n] = sum(f[l] * f[n-1-l])  (l=0..n-1)
// 时间复杂度 O(n^2)，只能算比较小的 n
long long compute4(int n) {
    vector<long long> f(n + 1, 0);
    f[0] = f[1] = 1;
    for (int i = 2; i <= n; i++) {
        for (int l = 0, r = i - 1; l < i; l++, r--) {
            f[i] = (f[i] + f[l] * f[r] % MOD) % MOD;
        }
    }
    return f[n];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    // 默认输出公式1结果
    cout << compute1(n) << "\n";

    // 也可以改成以下任意一种:
    // cout << compute2(n) << "\n";
    // cout << compute3(n) << "\n";
    // cout << compute4(n) << "\n";
    return 0;
}
