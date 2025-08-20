#include <iostream>
#include <cstdio>

// 青蛙的约会
// 有一个周长为l的环，从环的0位置开始，规定只能沿着顺时针方向不停转圈
// 青蛙A在环的x1位置，每秒跳m个单位，青蛙B在x2位置，每秒跳n个单位
// 只有在某时刻，青蛙A和青蛙B来到环的同一个位置，才算相遇
// 如果两只青蛙相遇不了，打印"Impossible"
// 如果可以相遇，打印两只青蛙至少多久才能相遇
// 1 <= l <= 3 * 10^9
// 1 <= x1、x2、m、n <= 2 * 10^9
// x1 != x2
// 测试链接 : https://www.luogu.com.cn/problem/P1516

using namespace std;

// 扩展欧几里得算法
// 功能：给定 a、b，返回 gcd(a, b)，并且求出一组 (x, y)
// 满足 a * x + b * y = gcd(a, b)
long long d, x, y, px, py;

void exgcd(long long a, long long b) {
    if (b == 0) {
        d = a;   // gcd = a
        x = 1;   // 当 b = 0 时，a * 1 + b * 0 = a
        y = 0;
    } else {
        exgcd(b, a % b); // 递归
        px = x;
        py = y;
        // 更新 (x, y)，使其满足 a * x + b * y = gcd(a, b)
        x = py;
        y = px - py * (a / b);
    }
}

int main() {
    long long x1, x2, m, n, l;
    cin >> x1 >> x2 >> m >> n >> l;

    // 我们要求青蛙 A 和 B 相遇的最小时间 t
    // A 每秒移动 m 单位，B 每秒移动 n 单位，环长为 l
    // 初始位置差 = |x1 - x2|
    // 要求解： (m - n) * t ≡ (x2 - x1) (mod l)

    long long a, c;
    if (x1 < x2) {
        a = m - n;     // 系数 a = m - n
        c = x2 - x1;   // 常数 c = x2 - x1
    } else {
        a = n - m;     // 系数 a = n - m
        c = x1 - x2;   // 常数 c = x1 - x2
    }

    // 处理成 a >= 0 的情况，方便后续运算
    if (a < 0) {
        a = -a;     // 把 a 变为正数
        c = l - c;  // 同余方程右边要调整
    }

    // 解方程 a * t ≡ c (mod l)
    // 即 a * t + l * k = c
    // 使用扩展欧几里得算法
    exgcd(a, l);

    if (c % d != 0) {
        // gcd(a, l) 不整除 c → 无解
        cout << "Impossible" << endl;
    } else {
        // 有解
        // 特解：t0 = x * (c / d)
        long long x0 = x * (c / d);
        // 通解步长 = l / gcd(a, l)
        long long xd = l / d;

        // 调整 t0，保证 t0 >= 1 且是最小的正整数解
        if (x0 < 0) {
            // 如果 t0 是负数，加上若干个 xd 直到变正
            x0 += ((1 - x0 + xd - 1) / xd) * xd;
        } else {
            // 如果 t0 是正数，可能太大了，可以减去若干个 xd，缩小到最小正整数解
            x0 -= ((x0 - 1) / xd) * xd;
        }
        cout << x0 << endl;
    }

    return 0;
}
