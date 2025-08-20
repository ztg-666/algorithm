#include <bits/stdc++.h>
using namespace std;

long long d, x, y, px, py;

// 扩展欧几里得算法
// 功能：求解 gcd(a, b)，并找到一组整数解 (x, y)，满足：a*x + b*y = gcd(a, b)
// 注意：a 和 b 必须非负
void exgcd(long long a, long long b) {
    if (b == 0) {
        // 递归边界：gcd(a, 0) = a，此时解为 (x=1, y=0)
        d = a;
        x = 1;
        y = 0;
    } else {
        // 递归：转为 gcd(b, a % b)
        exgcd(b, a % b);

        // 保存上一次递归的解
        px = x;
        py = y;

        // 更新当前解 (核心公式)
        // 新的 x = y'
        // 新的 y = x' - y' * (a / b)
        x = py;
        y = px - py * (a / b);
    }
}

// 快速幂 (num^pow % mod)
// 时间复杂度 O(log pow)
long long power(long long num, long long pow, long long mod) {
    long long ans = 1;
    while (pow > 0) {
        if (pow & 1) {
            ans = (ans * num) % mod; // 如果当前二进制位为 1，则累乘
        }
        num = (num * num) % mod; // 平方
        pow >>= 1;               // 右移，相当于除以 2
    }
    return ans;
}

// 费马小定理计算逆元
// 前提：mod 是质数，且 num 与 mod 互质
// 根据定理：num^(mod-1) ≡ 1 (mod mod)
// 推出：num^(mod-2) ≡ num^(-1) (mod mod)
long long fermat(long long num, long long mod) {
    return power(num, mod - 2, mod);
}

int main() {
    // 扩展欧几里得算法例子
    int a = 220;
    int b = 170;
    exgcd(a, b);

    // 打印 gcd 以及一组 (x, y)
    cout << "gcd(" << a << ", " << b << ") = " << d << endl;
    cout << "x = " << x << ", y = " << y << endl;

    // 逆元测试：比较 exgcd 和 费马小定理 的结果是否一致
    cout << "逆元测试开始" << endl;
    long long mod = 1000000007;  // 常用大质数
    long long test = 10000000;   // 测试范围：1 ~ 1000万

    for (long long num = 1; num <= test; num++) {
        exgcd(num, mod); // 通过扩展欧几里得算法求解 num 的逆元
        long long inv = (x % mod + mod) % mod; // 保证结果为非负数

        // 对比费马小定理的逆元结果
        if (inv != fermat(num, mod)) {
            cout << "发现错误！num = " << num << endl;
            break;
        }
    }
    cout << "逆元测试结束" << endl;

    return 0;
}
