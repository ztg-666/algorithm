#include <bits/stdc++.h>
using namespace std;

// 计算最大公约数（辗转相除法）
// gcd(a, b) 表示 a 和 b 的最大公约数
long long gcd(long long a, long long b) {
    return b == 0 ? a : gcd(b, a % b);
}

int main() {
    ios::sync_with_stdio(false); // 提升 cin/cout 的输入输出效率
    cin.tie(nullptr);            // 取消 cin 与 cout 的绑定，加快输入输出

    int cases;
    cin >> cases; // 读取测试用例数量

    for (int t = 1; t <= cases; t++) {
        long long x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2; // 读取两个点的坐标

        /*
         * 在一个二维整数格点坐标系中，要求 (x1,y1) 到 (x2,y2) 之间的整点（包含端点）数量。
         *
         * 结论公式：
         *   整点数量 = gcd(|x1 - x2|, |y1 - y2|) + 1
         *
         * 原理：
         *   - 一条线段上整点的间隔是均匀分布的。
         *   - 差值 Δx = |x1 - x2|, Δy = |y1 - y2|
         *   - 线段上整点间隔数 = gcd(Δx, Δy)
         *   - 因为包含两个端点，所以总点数 = gcd(Δx, Δy) + 1
         */
        long long ans = gcd(abs(x1 - x2), abs(y1 - y2)) + 1;

        // 输出结果，按照格式 "Case t: ans"
        cout << "Case " << t << ": " << ans << "\n";
    }

    return 0;
}
