#include <bits/stdc++.h>

// 猜数字
// 给定两个长度为n数组，一组为r1，r2，r3...，另一组为m1，m2，m3...
// 其中第二组数字两两互质，求最小正数解x
// 要求x满足，mi | (x - ri)，即(x - ri)是mi的整数倍
// 1 <= n <= 10
// -10^9 <= ri <= +10^9
// 1 <= mi <= 6 * 10^3
// 所有mi的乘积 <= 10^18
// 测试链接 : https://www.luogu.com.cn/problem/P3868

using namespace std;

const int MAXN = 11;
long long m[MAXN], r[MAXN];   // m[i]: 模数, r[i]: 余数
long long d, x, y, px, py;   // 扩展欧几里得算法的全局变量

// ---------------- 扩展欧几里得 ----------------
// 功能: 求解 a*x + b*y = gcd(a, b)
// 输入: a, b
// 输出: gcd(a, b)，同时得到一组整数解 (x, y)
void exgcd(long long a, long long b) {
    if (b == 0) {
        d = a;   // gcd(a,0) = a
        x = 1;   // 此时解为 (1,0)
        y = 0;
    } else {
        exgcd(b, a % b); // 递归
        // 回溯时更新 x, y
        px = x;
        py = y;
        x = py;
        y = px - py * (a / b);
    }
}

// ---------------- 龟速乘 ----------------
// 功能: 计算 (a * b) % mod，防止 long long 溢出
// 原理: 类似快速幂，每次按二进制拆分 b
long long multiply(long long a, long long b, long long mod) {
    a = (a % mod + mod) % mod;  // 处理成非负数
    b = (b % mod + mod) % mod;
    long long ans = 0;
    while (b) {
        if (b & 1) ans = (ans + a) % mod; // 当前位为1，加上对应的 a
        a = (a + a) % mod;                // a 翻倍
        b >>= 1;                          // b 右移一位
    }
    return ans;
}

// ---------------- 中国剩余定理 (CRT, 模数两两互质) ----------------
// 输入: n 个同余方程
//       x ≡ r[i] (mod m[i])
// 输出: 最小非负整数解
long long crt(int n) {
    long long lcm = 1; // 模数乘积
    for (int i = 1; i <= n; i++) {
        lcm *= m[i];
    }
    long long ai, ci, ans = 0;
    for (int i = 1; i <= n; i++) {
        ai = lcm / m[i];     // 除去当前模数的部分
        exgcd(ai, m[i]);     // 求 ai 在 mod m[i] 下的逆元
        ci = multiply(r[i], multiply(ai, x, lcm), lcm);
        ans = (ans + ci) % lcm; // 累加到答案
    }
    return ans;
}

// ---------------- 扩展中国剩余定理 (EXCRT, 模数不一定互质) ----------------
// 输入: n 个同余方程
//       x ≡ r[i] (mod m[i])
// 输出: 最小非负整数解 (可能为 0)
//       如果无解返回 -1
long long excrt(int n) {
    long long tail = 0, lcm = 1; // 当前解: x = lcm * k + tail
    for (int i = 1; i <= n; i++) {
        long long b = m[i];
        // 要解: lcm * k + tail ≡ r[i] (mod m[i])
        // 化简: lcm * k ≡ (r[i] - tail) (mod m[i])
        long long c = ((r[i] - tail) % b + b) % b;

        exgcd(lcm, b); // gcd(lcm, b) = d，同时得到系数 x, y
        if (c % d != 0) return -1; // 无解

        // 求特解: k = x * (c/d) (mod b/d)
        long long x0 = multiply(x, c / d, b / d);

        // 更新解: x = lcm * k + tail
        long long tmp = lcm * (b / d); // 新的 lcm
        tail = (tail + multiply(x0, lcm, tmp)) % tmp;
        lcm = tmp;
    }
    return tail;
}

// ---------------- 主函数 ----------------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;                        // 输入方程个数
    for (int i = 1; i <= n; i++) cin >> r[i]; // 输入余数
    for (int i = 1; i <= n; i++) cin >> m[i]; // 输入模数

    // 将余数规范化到 [0, m[i]) 区间
    for (int i = 1; i <= n; i++) {
        r[i] = (r[i] % m[i] + m[i]) % m[i];
    }

    // 本题保证模数互质，因此用 CRT 或 EXCRT 都行
    cout << excrt(n) << "\n";

    return 0;
}
