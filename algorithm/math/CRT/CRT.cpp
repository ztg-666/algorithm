#include <bits/stdc++.h>

// 中国剩余定理模版
// 给出n个同余方程，求满足同余方程的最小正数解x
// 一共n个同余方程，x ≡ ri(% mi)
// 1 <= n <= 10
// 0 <= ri、mi <= 10^5
// 所有mi一定互质
// 所有mi整体乘积 <= 10^18
// 测试链接 : https://www.luogu.com.cn/problem/P1495


using namespace std;

const int MAXN = 11;
long long m[MAXN], r[MAXN];   // 存放模数 m[i] 和余数 r[i]
long long d, x, y, px, py;   // 扩展欧几里得算法的中间变量

// 扩展欧几里得算法：用来求 ax + by = gcd(a, b)
// 调用后 gcd 保存在 d 里，同时得到一组解 (x, y)
void exgcd(long long a, long long b) {
    if (b == 0) {
        d = a;   // gcd(a, 0) = a
        x = 1;
        y = 0;
    } else {
        exgcd(b, a % b);   // 递归
        px = x;            // 保存之前的解
        py = y;
        x = py;            // 更新 x
        y = px - py * (a / b);  // 更新 y
    }
}

// 龟速乘：防止 long long 溢出
// 计算 (a * b) % mod，复杂度 O(log b)
long long multiply(long long a, long long b, long long mod) {
    a = (a % mod + mod) % mod;
    b = (b % mod + mod) % mod;
    long long ans = 0;
    while (b) {
        if (b & 1) ans = (ans + a) % mod; // b 的二进制末位是 1
        a = (a + a) % mod;                // a 翻倍
        b >>= 1;                          // b 右移一位
    }
    return ans;
}

// 中国剩余定理 (CRT)
// 输入: n 个同余方程
//      x ≡ r[i] (mod m[i])   (1 <= i <= n)
// 条件: 所有 m[i] 互质
// 返回: 满足所有方程的最小非负整数解 x
long long crt(int n) {
    // 计算所有模数的乘积 lcm
    long long lcm = 1;
    for (int i = 1; i <= n; i++) {
        lcm = lcm * m[i];
    }

    long long ai, ci, ans = 0;
    for (int i = 1; i <= n; i++) {
        ai = lcm / m[i];   // ai = 其他模数的乘积 (M / m[i])

        // 求 ai 在 mod m[i] 下的逆元
        // 扩展欧几里得：解 ai * x + m[i] * y = 1
        exgcd(ai, m[i]);

        // ci = r[i] * ai * (ai的逆元 mod m[i])  (再对 lcm 取模)
        // 注意：这里用龟速乘防止溢出
        ci = multiply(r[i], multiply(ai, x, lcm), lcm);

        // 把结果加到答案里
        ans = (ans + ci) % lcm;
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;  // 输入方程个数
    for (int i = 1; i <= n; i++) {
        cin >> m[i] >> r[i];  // 输入 m[i], r[i]
    }

    cout << crt(n) << "\n";  // 输出最小解
    return 0;
}
