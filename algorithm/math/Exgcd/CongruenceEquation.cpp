#include <bits/stdc++.h>

// 同余方程
// 求关于x的同余方程 ax ≡ 1(mod b) 的最小正整数解
// 题目保证一定有解，也就是a和b互质
// 2 <= a、b <= 2 * 10^9
// 测试链接 : https://www.luogu.com.cn/problem/P1082

using namespace std;

// 全局变量：保存 gcd 及对应的 x, y
long long d, x, y, px, py;

// 扩展欧几里得算法
// 功能：求解 a*x + b*y = gcd(a, b)
// 同时返回 gcd(a,b) 并通过递归更新 x, y 的值
void exgcd(long long a, long long b) {
    if (b == 0) {
        // 递归边界：gcd(a, 0) = a
        d = a;    // 最大公约数
        x = 1;    // 对应系数 x
        y = 0;    // 对应系数 y
    } else {
        // 递归调用：转化为 gcd(b, a % b)
        exgcd(b, a % b);

        // 记录上一次递归的结果
        px = x;
        py = y;

        // 根据递推公式更新 x, y
        // 新的 x = y'
        // 新的 y = x' - y' * (a / b)
        x = py;
        y = px - py * (a / b);
    }
}

int main() {
    ios::sync_with_stdio(false); // 关闭C++与C的IO同步，提高效率
    cin.tie(nullptr);            // 解除cin与cout绑定，加快输入输出

    long long a, b;
    cin >> a >> b;  // 输入同余方程的参数 a 和 b

    // 调用扩展欧几里得算法
    // 计算出一组 (x, y) 满足 a*x + b*y = gcd(a,b)
    exgcd(a, b);

    // 因为题目保证 a 和 b 互质，所以 gcd(a,b)=1
    // 此时 x 即为 a 在模 b 下的逆元（同余方程 ax ≡ 1 (mod b) 的解）
    // 使用 (x % b + b) % b 确保结果为最小正整数
    cout << ((x % b + b) % b) << "\n";

    return 0;
}
