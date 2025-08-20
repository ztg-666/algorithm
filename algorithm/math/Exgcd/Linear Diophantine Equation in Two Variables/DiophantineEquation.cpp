#include <iostream>
#include <cstdio>

// 二元一次不定方程模板
// 给定 a, b, c，求解方程 ax + by = c
// 如果方程无解，打印 -1
// 如果方程无正整数解，但是有整数解
//   打印这些整数解中，x 的最小正数值，y 的最小正数值
// 如果方程有正整数解
//   打印正整数解的数量，同时打印：
//   所有正整数解中，x 的最小正数值，y 的最小正数值，x 的最大正数值，y 的最大正数值
// 1 <= a, b, c <= 1e9
// 测试链接 : https://www.luogu.com.cn/problem/P5656

using namespace std;

// 扩展欧几里得算法相关变量
long long d, x, y, px, py;

// 扩展欧几里得算法
// 作用：求解 gcd(a, b)，同时得到一组整数解 (x, y)，满足 ax + by = gcd(a, b)
void exgcd(long long a, long long b) {
    if (b == 0) {  
        // 递归边界：gcd(a, 0) = a，此时解为 (x=1, y=0)
        d = a;
        x = 1;
        y = 0;
    } else {
        // 递归求解 gcd(b, a % b)
        exgcd(b, a % b);
        // 记录上一次递归得到的解
        px = x;
        py = y;
        // 更新当前解
        x = py;
        y = px - py * (a / b);
    }
}

// 输入数据和中间变量
long long a, b, c, xd, yd, times;

int main() {
    int cases;
    scanf("%d", &cases);  // 输入测试组数
    for (int t = 1; t <= cases; t++) {
        scanf("%lld %lld %lld", &a, &b, &c);  // 输入 a, b, c

        // 扩展欧几里得，得到 gcd(a, b) 和一组解
        exgcd(a, b);

        // 如果 c 不能被 gcd(a, b) 整除，则无解
        if (c % d != 0) {
            printf("-1\n");
        } else {
            // 否则，将解 (x, y) 扩展为 ax + by = c 的解
            x *= c / d;
            y *= c / d;

            // 通解形式：
            // x = x0 + k * (b/d)
            // y = y0 - k * (a/d)
            xd = b / d;
            yd = a / d;

            // 调整 x，使得 x 尽量靠近正数范围
            if (x < 0) {
                // 如果 x 为负数，就往正方向平移若干倍 xd
                times = (xd - x) / xd;
                x += xd * times;
                y -= yd * times;
            } else {
                // 如果 x 为正数，就尽量减小 x，让它仍为正
                times = (x - 1) / xd;
                x -= xd * times;
                y += yd * times;
            }

            // 判断 y 是否为正
            if (y <= 0) {
                // 如果 y <= 0，说明无正整数解
                // 输出最小正 x 和最小正 y（通过平移得到）
                printf("%lld ", x);
                printf("%lld\n", y + yd * ((yd - y) / yd));
            } else {
                // 有正整数解
                // 计算解的个数：
                // 通过调整 k，可以让 y 在正数范围内
                // (y - 1)/yd + 1 就是满足 y>0 的解的个数
                printf("%lld ", ((y - 1) / yd + 1));

                // 打印最小正数解 (x, y)
                printf("%lld ", x);
                printf("%lld ", (y - (y - 1) / yd * yd));

                // 打印最大正数解 (x, y)
                printf("%lld ", (x + (y - 1) / yd * xd));
                printf("%lld\n", y);
            }
        }
    }
    return 0;
}
