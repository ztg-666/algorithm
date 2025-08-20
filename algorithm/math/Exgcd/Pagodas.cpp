#include <bits/stdc++.h>

// 修理宝塔
// 一共有编号1~n的宝塔，其中a号和b号宝塔已经修好了
// Yuwgna和Iaka两个人轮流修塔，Yuwgna先手，Iaka后手，谁先修完所有的塔谁赢
// 每次可以选择j+k号或者j-k号塔进行修理，其中j和k是任意两个已经修好的塔
// 也就是输入n、a、b，如果先手赢打印"Yuwgna"，后手赢打印"Iaka"
// 2 <= n <= 2 * 10^4
// 测试链接 : https://acm.hdu.edu.cn/showproblem.php?pid=5512
// 测试链接 : https://vjudge.net/problem/HDU-5512

using namespace std;

// gcd函数：求最大公约数（欧几里得算法）
// 递归终止条件：当 b == 0 时，gcd(a, b) = a
int gcd(int a, int b) {
    return b == 0 ? a : gcd(b, a % b);
}

int main() {
    ios::sync_with_stdio(false); // 关闭C++和C的标准输入输出同步，提高效率
    cin.tie(nullptr);            // 解除 cin 与 cout 的绑定，加快输入输出

    int cases;
    cin >> cases; // 输入测试组数

    for (int t = 1; t <= cases; t++) {
        int n, a, b;
        cin >> n >> a >> b; // 输入塔的数量 n 和已经修好的两个塔 a, b

        cout << "Case #" << t << ": ";

        // 关键结论：
        // - 游戏等价于判断 (n / gcd(a, b)) 的奇偶性
        // - 如果是奇数，先手 Yuwgna 赢；如果是偶数，后手 Iaka 赢
        // 这是因为修塔的规则本质上让“步数”与 (n / gcd) 的奇偶性挂钩
        if (((n / gcd(a, b)) & 1) == 1) {
            // & 1 判断奇偶，==1 表示奇数
            cout << "Yuwgna\n";
        } else {
            cout << "Iaka\n";
        }
    }

    return 0;
}
