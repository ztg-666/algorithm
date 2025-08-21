#include <bits/stdc++.h>

// 扩展中国剩余定理模版
// 给出n个同余方程，求满足同余方程的最小正数解x
// 一共n个同余方程，x ≡ ri(% mi)
// 1 <= n <= 10^5
// 0 <= ri、mi <= 10^12
// 所有mi不一定互质
// 所有mi的最小公倍数 <= 10^18
// 测试链接 : https://www.luogu.com.cn/problem/P4777
// 测试链接 : https://www.luogu.com.cn/problem/P1495

using namespace std;

const int MAXN = 100001;
long long m[MAXN], r[MAXN];   // m[i]: 模数，r[i]: 余数
long long d, x, y, px, py;   // 扩展欧几里得中的全局变量

// --------------------------- 扩展欧几里得算法 ---------------------------
// 功能: 求解 a*x + b*y = gcd(a, b)
// 输入: a, b
// 输出: gcd 保存在 d 中, 同时求得一组整数解 (x, y)
void exgcd(long long a, long long b) {
    if (b == 0) {
        d = a;   // gcd(a,0) = a
        x = 1;   // 此时 a*1 + b*0 = a
        y = 0;
    } else {
        exgcd(b, a % b);
        // 递归返回时，更新 x, y
        px = x;
        py = y;
        x = py;
        y = px - py * (a / b);
    }
}

// --------------------------- 龟速乘 ---------------------------
// 功能: 计算 (a * b) % mod，避免 long long 溢出
// 原理: 类似快速幂，每次把 b 按二进制拆分
long long multiply(long long a, long long b, long long mod) {
    a = (a % mod + mod) % mod;  // 保证非负
    b = (b % mod + mod) % mod;
    long long ans = 0;
    while (b) {
        if (b & 1) ans = (ans + a) % mod; // 如果 b 的当前位是 1，加上 a
        a = (a + a) % mod;                // a 翻倍
        b >>= 1;                          // b 右移一位
    }
    return ans;
}

// --------------------------- 扩展中国剩余定理 EXCRT ---------------------------
// 功能: 求解一组模数不一定互质的同余方程组
// 输入: n 个同余方程
//      x ≡ r[i] (mod m[i])，1 <= i <= n
// 输出: 最小非负整数解，如果无解返回 -1
// 注意: lcm 可能变大，但题目保证 ≤ 1e18
long long excrt(int n) {
    long long tail = 0, lcm = 1; // 初始解为 x ≡ 0 (mod 1)，即 x = 0
                                 // 当前解形式: x = lcm * k + tail
    for (int i = 1; i <= n; i++) {
        long long b = m[i];   // 当前方程的模数
        // 我们要求: lcm * k + tail ≡ r[i] (mod m[i])
        // 移项得到: lcm * k ≡ (r[i] - tail) (mod m[i])
        long long c = ((r[i] - tail) % b + b) % b; // c = (r[i] - tail) mod b

        exgcd(lcm, b); // 计算 gcd(lcm, b)，以及对应的系数 x, y

        if (c % d != 0) {
            return -1; // 无解情况：常数项不能整除 gcd
        }

        // 计算 lcm * k ≡ c (mod b) 的特解
        // 通解: k = x0 + (b/d)*n
        long long x0 = multiply(x, c / d, b / d);

        // ans = lcm * x + tail
        // ans = lcm * (x0 + (b/d)*n) + tail
        // ans = lcm * (b/d) * n + lcm * x0 + tail
        // lcm' = lcm * (b/d)
        // tail' = (lcm * x0 + tail) % lcm'
        long long tmp = lcm * (b / d); // 新的 lcm
        tail = (tail + multiply(x0, lcm, tmp)) % tmp;
        lcm = tmp; // 更新最小公倍数
    }
    return tail;
}

// --------------------------- 主函数 ---------------------------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n; // 输入方程个数
    for (int i = 1; i <= n; i++) {
        cin >> m[i] >> r[i]; // 输入模数和余数
    }

    cout << excrt(n) << "\n"; // 输出最小解
    return 0;
}
