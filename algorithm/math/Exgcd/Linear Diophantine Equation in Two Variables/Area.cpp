#include <bits/stdc++.h>

// 机器人的移动区域
// 二维网格中只有x和y的值都为整数的坐标，才叫格点
// 某个机器人从格点(0,0)出发，每次机器人都走直线到达(x + dx, y + dy)的格点
// 一共移动n次，每次的(dx, dy)都给定，途中路线不会交叉，输入保证机器人最终回到(0,0)
// 机器人走的路线所围成的区域一定是多边形，输入保证机器人一定沿着逆时针方向行动
// 返回多边形的内部一共几个格点，多边形的边上一共几个格点，多边形的面积
// 3 <= n <= 100
// -100 <= dx、dy <= 100
// 测试链接 : http://poj.org/problem?id=1265

using namespace std;

// 计算两个 long long 数的最大公约数（GCD）
long long gcd_ll(long long a, long long b) {
    return b == 0 ? a : gcd_ll(b, a % b);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int cases;
    cin >> cases;  // 输入测试用例数量
    for (int t = 1; t <= cases; t++) {
        int n;
        cin >> n;  // 输入多边形的边数（以向量形式给出）

        long long edges = 0;   // 多边形边上整点的数量
        long long area2 = 0;   // 使用鞋带公式时，先存储面积的 2 倍（避免浮点误差）
        int x = 0, y = 0;      // 当前点的坐标（初始为原点）

        // 读取 n 个向量（dx, dy），表示多边形的边
        for (int i = 1; i <= n; i++) {
            int dx, dy;
            cin >> dx >> dy;

            // 每条边上整点数 = gcd(|dx|, |dy|)
            // 这是因为步长是 (dx/g, dy/g)，一共有 g+1 个点，其中端点可能和别的边重合
            edges += gcd_ll(abs(dx), abs(dy));

            // 累加鞋带公式的一部分
            // 当前边从 (x, y) -> (x+dx, y+dy)
            // 鞋带公式贡献：x * (y+dy) - (x+dx) * y
            area2 += 1LL * x * (y + dy) - 1LL * (x + dx) * y;

            // 更新当前位置
            x += dx;
            y += dy;
        }

        // 面积（取绝对值，area2 可能为负）
        double area = abs(area2) / 2.0;

        // 根据 Pick 定理：面积 = 内部整点数 + 边上整点数/2 - 1
        // => 内部整点数 = 面积 - 边上整点数/2 + 1
        long long inners = (long long)(area) - edges / 2 + 1;

        // 输出结果
        cout << "Scenario #" << t << ":\n";
        cout << inners << " " << edges << " ";
        cout << fixed << setprecision(1) << area << "\n\n";
    }

    return 0;
}
