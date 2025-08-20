#include <bits/stdc++.h>

// 无法组成的最大值
// 一共有a、b两种面值的硬币，a和b一定互质，每种硬币都有无限个
// 返回a和b无法组成的钱数中，最大值是多少
// 题目的输入保证存在最大的无法组成的钱数
// 1 <= a、b <= 10^9
// 测试链接 : https://www.luogu.com.cn/problem/P3951

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long a, b;
    cin >> a >> b;
    cout << a * b - a - b << "\n";

    return 0;
}
