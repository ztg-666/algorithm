#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
#include <cstdio>

// 最优比率生成树
// 一共有n个村庄，每个村庄由(x, y, z)表示
// 其中(x,y)代表村庄在二维地图中的位置，z代表其海拔高度
// 任意两个村庄之间的距离就是二维地图中的欧式距离
// 任意两个村庄之间的修路花费就是海拔差值的绝对值
// 现在想把所有村庄连通起来，希望修路的条数尽量少，同时希望让
// 总花费 / 总距离，这个比值尽量小，返回最小的比值是多少，结果保留小数点后3位其余部分舍弃
// 2 <= n <= 10^3
// 0 <= x、y <= 10^4
// 0 <= z <= 10^7
// 测试链接 : http://poj.org/problem?id=2728

using namespace std;

const int MAXN = 1001;
const double sml = 1e-6;

int x[MAXN], y[MAXN], z[MAXN];  // 存储每个村庄的坐标(x, y)和海拔z
double dist[MAXN][MAXN];  // 存储两个村庄之间的欧几里得距离
double cost[MAXN][MAXN];  // 存储两个村庄之间的海拔差值
bool visit[MAXN];  // 标记村庄是否已被访问
double value[MAXN];  // 用于记录当前最优的值
int n;  // 村庄数量

// 用于计算最小生成树的函数，使用了Prim算法，返回最小生成树的权值和
double prim(double x) {
    for (int i = 1; i <= n; i++) {
        visit[i] = false;
        value[i] = cost[1][i] - x * dist[1][i];  // 初始时设置与1号村庄的距离
    }
    visit[1] = true;
    double sum = 0;

    // 最小生成树的边数是n-1，所以执行n-1次
    for (int i = 1; i <= n - 1; i++) {
        double minDist = INT_MAX;
        int next = 0;

        // 在未访问的村庄中找出最小的值
        for (int j = 1; j <= n; j++) {
            if (!visit[j] && value[j] < minDist) {
                minDist = value[j];
                next = j;
            }
        }

        // 将最小的村庄加到最小生成树中
        sum += minDist;
        visit[next] = true;

        // 更新最小生成树能连接到其他村庄的距离
        for (int j = 1; j <= n; j++) {
            if (!visit[j] && value[j] > cost[next][j] - x * dist[next][j]) {
                value[j] = cost[next][j] - x * dist[next][j];
            }
        }
    }

    return sum;  // 返回最小生成树的总权值
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    while (cin >> n && n != 0) {
        for (int i = 1; i <= n; i++) {
            cin >> x[i] >> y[i] >> z[i];  // 读取每个村庄的坐标和海拔
        }

        // 计算所有村庄之间的距离和修路花费
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (i != j) {
                    dist[i][j] = sqrt((x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j]));  // 欧几里得距离
                    cost[i][j] = abs(z[i] - z[j]);  // 海拔差值
                }
            }
        }

        double l = 0, r = 100, x, ans = 0;

        // 使用二分法查找最优比值
        while (r - l >= sml) {
            x = (l + r) / 2;
            if (prim(x) <= 0) {
                ans = x;  // 如果最小生成树的权值 <= 0，说明当前比值可行
                r = x - sml;  // 继续寻找更小的比值
            } else {
                l = x + sml;  // 否则，尝试更大的比值
            }
        }

        printf("%.3f\n", ans);  // 输出结果，保留三位小数
    }

    return 0;
}
