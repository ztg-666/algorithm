#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

// 跑路
// 一共有n个节点，编号1~n，一共有m条有向边，每条边1公里
// 有一个空间跑路器，每秒你都可以直接移动2^k公里，每秒钟可以随意决定k的值
// 题目保证1到n之间一定可以到达，返回1到n最少用几秒
// 2 <= n <= 50
// m <= 10^4
// k <= 64
// 测试链接 : https://www.luogu.com.cn/problem/P1613

using namespace std;

const int MAXN = 61;    // 最大节点数
const int MAXP = 64;    // 最大幂次（k的最大值）
const int NA = INT_MAX; // 表示不可达

// st[i][j][p] : 表示从节点i到节点j的距离是否恰好为2^p
// 这是一个三维数组，用于记录二进制提升的状态
bool st[MAXN][MAXN][MAXP + 1];

// time_arr[i][j] : 从节点i到节点j的最短时间
int time_arr[MAXN][MAXN];

int n, m; // n为节点数，m为边数

// 初始化函数：将所有状态重置
void build() {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            st[i][j][0] = false;    // 初始时没有直接连接
            time_arr[i][j] = NA;    // 初始时所有距离都是不可达
        }
    }
}

// 核心计算函数：使用二进制提升 + Floyd算法
int compute() {
    // 第一阶段：使用二进制提升计算所有2^p距离的可达性
    // 类似于倍增算法的思想

    // 先枚举次方 p (从1到MAXP)
    for (int p = 1; p <= MAXP; p++) {
        // 再枚举跳板节点 jump
        for (int jump = 1; jump <= n; jump++) {
            // 最后枚举每一组起点和终点 (i,j)
            for (int i = 1; i <= n; i++) {
                for (int j = 1; j <= n; j++) {
                    // 如果i能通过2^(p-1)的距离到达jump，
                    // 且jump能通过2^(p-1)的距离到达j，
                    // 那么i就能通过2^p的距离到达j
                    if (st[i][jump][p - 1] && st[jump][j][p - 1]) {
                        st[i][j][p] = true;      // 标记可达
                        time_arr[i][j] = 1;      // 时间为1秒（一次跳跃）
                    }
                }
            }
        }
    }

    // 如果通过一次跳跃无法从1到达n，则需要多次跳跃
    if (time_arr[1][n] != 1) {
        // 第二阶段：使用Floyd算法计算最短路径
        // 这里计算的是最少需要多少秒（多少次跳跃）

        // 先枚举中转点 jump
        for (int jump = 1; jump <= n; jump++) {
            // 最后枚举每一组起点和终点 (i,j)
            for (int i = 1; i <= n; i++) {
                for (int j = 1; j <= n; j++) {
                    // 如果i能到达jump，且jump能到达j
                    if (time_arr[i][jump] != NA && time_arr[jump][j] != NA) {
                        // 更新i到j的最短时间
                        time_arr[i][j] = min(time_arr[i][j],
                                           time_arr[i][jump] + time_arr[jump][j]);
                    }
                }
            }
        }
    }

    return time_arr[1][n]; // 返回从节点1到节点n的最短时间
}

int main() {
    // 优化输入输出速度
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 读取节点数
    cin >> n;

    // 初始化数据结构
    build();

    // 读取边数
    cin >> m;

    // 读取每条边的信息
    for (int i = 1; i <= m; i++) {
        int u, v;
        cin >> u >> v;
        st[u][v][0] = true;     // 标记u到v距离为2^0=1（直接相连）
        time_arr[u][v] = 1;     // u到v的时间为1秒
    }

    // 计算并输出结果
    cout << compute() << endl;

    return 0;
}
