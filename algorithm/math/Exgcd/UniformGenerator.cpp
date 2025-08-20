#include <bits/stdc++.h>

// 均匀生成器
// 如果有两个数字step和mod，那么可以由以下方式生成很多数字
// seed(1) = 0，seed(i+1) = (seed(i) + step) % mod
// 比如，step = 3、mod = 5
// seed(1) = 0，seed(2) = 3，seed(3) = 1，seed(4) = 4，seed(5) = 2
// 如果能产生0 ~ mod-1所有数字，step和mod的组合叫  "Good Choice"
// 如果无法产生0 ~ mod-1所有数字，step和mod的组合叫 "Bad Choice"
// 根据step和mod，打印结果
// 1 <= step、mod <= 10^5
// 测试链接 : http://poj.org/problem?id=1597

using namespace std;

// 求最大公约数（欧几里得算法）
// 使用递归实现，终止条件为 b == 0 时，gcd(a, b) = a
int gcd(int a, int b) {
    return b == 0 ? a : gcd(b, a % b);  // 递归求解最大公约数
}

int main() {
    ios::sync_with_stdio(false); // 提高输入输出效率
    cin.tie(nullptr);            // 解除 cin 与 cout 的绑定，提高效率

    int step, mod;

    // 读取输入，直到文件结束
    while (cin >> step >> mod) {
        // 输出格式：每行先输出step和mod，格式化为10个字符宽度
        // 例如: "        3        5"
        printf("%10d%10d    ", step, mod);

        // 判断step和mod是否互质（gcd(step, mod) == 1）
        // 如果互质，输出"Good Choice"，否则输出"Bad Choice"
        if (gcd(step, mod) == 1) {
            cout << "Good Choice" << endl; // 如果最大公约数为1，说明step和mod互质
        } else {
            cout << "Bad Choice" << endl;  // 否则，step和mod不互质
        }

        cout << endl;  // 每一组结果后输出换行符
    }

    return 0;
}
