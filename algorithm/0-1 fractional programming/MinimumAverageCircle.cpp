#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
#include <cstdio>
#include <cstring>

// 最小圈
// 一共有n个节点，m条有向边，每条边有权值
// 这个有向带权图中，可能有若干环，规定环的平均值为
// 环中边的权值和 / 环中边的数量
// 返回所有环的平均值中最少的平均值，结果保留小数点后8位，其余部分舍弃
// 1 <= n <= 3000
// 1 <= m <= 10000
// -10^7 <= 边权 <= 10^7
// 测试链接 : https://www.luogu.com.cn/problem/P3199

using namespace std;

const int MAXN = 3001;      // 最大节点数
const int MAXM = 10001;     // 最大边数
const double MAXE = 1e7;    // 最大边权
const double sml = 1e-9;    // 精度

// 链式前向星
int head[MAXN], nxt[MAXM], to[MAXM];
double weight[MAXM];
int cnt;

// dfs判断负环，每个点的累积边权
double value[MAXN];
bool path[MAXN];

// 边的数量和节点数量
int n, m;

// 初始化
void prepare() {
    cnt = 1;
    memset(head, 0, sizeof(head));  // 初始化链式前向星头节点
}

// 添加边
void addEdge(int u, int v, double w) {
    nxt[cnt] = head[u];
    to[cnt] = v;
    weight[cnt] = w;
    head[u] = cnt++;
}


// 实际上是spfa的递归版，时间复杂度O(n * m)
bool dfs(int u, double x) {
    if (u == 0) {
        // 认为0号点是超级源点，具有通往所有点的有向边
        for (int i = 1; i <= n; i++) {
            if (dfs(i, x)) {
                return true;
            }
        }
    } else {
        path[u] = true;
        for (int e = head[u]; e != 0; e = nxt[e]) {
            int v = to[e];
            double w = weight[e] - x;
            // 只有v的累积边权变小，才会递归，非常强的剪枝
            // 如果递归路径回到v，并且此时是v的累积边权更小的情况，说明遇到了负环
            // 或者后续递归找到了负环，直接返回true
            if (value[v] > value[u] + w) {
                value[v] = value[u] + w;
                if (path[v] || dfs(v, x)) {
                    return true;
                }
            }
        }
        path[u] = false;
    }
    return false;
}

// 所有边都减去x，返回图中是否存在负环
bool check(double x) {
    memset(value, 0, sizeof(value));
    memset(path, false, sizeof(path));
    return dfs(0, x);
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // 输入节点和边的数量
    cin >> n >> m;
    prepare();

    // 读取所有边的信息
    for (int i = 1; i <= m; i++) {
        int u, v;
        double w;
        cin >> u >> v >> w;
        addEdge(u, v, w);
    }

    double l = -MAXE, r = MAXE, x, ans = 0;
    // 二分查找最小的平均值
    while (r - l >= sml) {
        x = (l + r) / 2;
        if (check(x)) {
            r = x - sml;  // 找到负环，减小x
        } else {
            ans = x;  // 没有负环，更新答案并增大x
            l = x + sml;
        }
    }

    // 输出最小平均值，保留8位小数
    printf("%.8f\n", ans);

    return 0;
}
