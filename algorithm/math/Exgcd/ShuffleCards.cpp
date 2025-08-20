#include <bits/stdc++.h>

// 洗牌
// 一共有n张牌，n一定是偶数，每张牌的牌面从1到n，洗牌规则如下
// 比如n = 6，牌面最初排列为1 2 3 4 5 6
// 先分成左堆1 2 3，右堆4 5 6，然后按照右堆第i张在前，左堆第i张在后的方式依次放置
// 所以洗一次后，得到 4 1 5 2 6 3
// 如果再洗一次，得到 2 4 6 1 3 5
// 如果再洗一次，得到 1 2 3 4 5 6
// 想知道n张牌洗m次的之后，第l张牌，是什么牌面
// 1 <= n <= 10^10，n为偶数
// 0 <= m <= 10^10
// 测试链接 : https://www.luogu.com.cn/problem/P2054

using namespace std;

// -------------------- 扩展欧几里得算法 --------------------
// 用于求解 a*x + b*y = gcd(a, b) 的一组整数解
// 并且返回 gcd(a,b)；递归更新 x, y 的值
long long d, x, y, px, py;

void exgcd(long long a, long long b) {
    if (b == 0) {
        d = a;
        x = 1;
        y = 0;
    } else {
        exgcd(b, a % b);
        px = x;
        py = y;
        x = py;
        y = px - py * (a / b);
    }
}

// -------------------- 龟速乘 --------------------
// 目的：计算 (a * b) % mod，防止溢出
// 原理：使用加法和位运算模拟乘法
long long multiply(long long a, long long b, long long mod) {
    a = (a % mod + mod) % mod;
    b = (b % mod + mod) % mod;
    long long ans = 0;
    while (b != 0) {
        if (b & 1) {
            ans = (ans + a) % mod;
        }
        a = (a + a) % mod;
        b >>= 1;
    }
    return ans;
}

// -------------------- 快速幂 (带模) --------------------
// 计算 (a^b) % mod，内部使用龟速乘防止溢出
long long power(long long a, long long b, long long mod) {
    long long ans = 1;
    while (b > 0) {
        if (b & 1) {
            ans = multiply(ans, a, mod);
        }
        a = multiply(a, a, mod);
        b >>= 1;
    }
    return ans;
}

// -------------------- 核心计算 --------------------
// n: 牌数 (偶数)，m: 洗牌次数，l: 查询位置
long long compute(long long n, long long m, long long l) {
    long long mod = n + 1;

    // 计算 2^m % (n+1)
    long long p = power(2, m, mod);

    // 求 p 在 mod 下的逆元
    // 使用扩展欧几里得算法：p * x ≡ 1 (mod mod)
    exgcd(p, mod);
    long long x0 = (x % mod + mod) % mod;  // 保证是正数

    // 返回 (l * x0) % mod
    return multiply(x0, l, mod);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n, m, l;
    cin >> n >> m >> l;
    cout << compute(n, m, l) << "\n";

    return 0;
}
