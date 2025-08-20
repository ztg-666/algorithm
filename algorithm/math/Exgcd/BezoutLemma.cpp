#include <bits/stdc++.h>

// 裴蜀定理模版题
// 给定长度为n的一组整数值[a1, a2, a3...]，你找到一组数值[x1, x2, x3...]
// 要让a1*x1 + a2*x2 + a3*x3...得到的结果为最小正整数
// 返回能得到的最小正整数是多少
// 1 <= n <= 20
// 1 <= ai <= 10^5
// 测试链接 : https://www.luogu.com.cn/problem/P4549

using namespace std;

// gcd函数：求最大公约数（辗转相除法）
// 裴蜀定理：一组数 a1, a2, ..., an 的最大公约数
//          就是能通过整数组合得到的最小正整数
int gcd(int a, int b) {
    return b == 0 ? a : gcd(b, a % b);
}

int main() {
    ios::sync_with_stdio(false); // 关闭 iostream 与 stdio 的同步，提高效率
    cin.tie(nullptr);            // 解除 cin 与 cout 的绑定，加快输入输出

    int n;
    cin >> n; // 读入整数个数 n

    int ans = 0; // 初始最大公约数为 0
    // 方便第一个数参与 gcd 计算：gcd(x, 0) = x

    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;                // 读入当前数
        ans = gcd(abs(x), ans);  // 与当前答案取 gcd
        // abs(x) 确保负数也能正确处理
    }

    // 输出最终结果：所有数的 gcd
    // 根据裴蜀定理，这个 gcd 就是能整数组合得到的最小正整数
    cout << ans << "\n";

    return 0;
}
