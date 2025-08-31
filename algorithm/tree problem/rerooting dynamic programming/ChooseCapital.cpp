#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

// 翻转道路数量最少的首都
// 给定一棵n个点的树，但是给定的每条边都是有向的
// 需要选择某个城市为首都，要求首都一定可以去往任何一个城市
// 这样一来，可能需要翻转一些边的方向才能做到，现在想翻转道路的数量尽量少
// 打印最少翻转几条道路就可以拥有首都
// 如果有若干点做首都时，翻转道路的数量都是最少的，那么打印这些点
// 测试链接 : https://www.luogu.com.cn/problem/CF219D

using namespace std;

const int MAXN = 200001;  // 最大节点数

// 全局变量
int n;                          // 节点数
int head[MAXN];                 // 邻接表头节点
int next_edge[MAXN << 1];       // 邻接表中下一条边的索引
int to[MAXN << 1];              // 边的终点
int weight[MAXN << 1];          // 边的权重(0表示正向边，1表示需要翻转的边)
int cnt;                        // 边的计数器

// reverse_edges[u] : 以u为根时，u到所有子节点需要逆转的边数
int reverse_edges[MAXN];

// dp[u] : 以u为根时，到达全树所有节点需要逆转的边数
int dp[MAXN];

/**
 * 初始化所有数组
 */
void build() {
    cnt = 1;  // 边的索引从1开始，0表示链表结束
    // 初始化邻接表头节点
    fill(head + 1, head + n + 1, 0);
    // 初始化逆转边数数组
    fill(reverse_edges + 1, reverse_edges + n + 1, 0);
    // 初始化dp数组
    fill(dp + 1, dp + n + 1, 0);
}

/**
 * 添加有向边
 * @param u 起点
 * @param v 终点
 * @param w 权重(0表示正向边，1表示需要翻转的边)
 */
void addEdge(int u, int v, int w) {
    next_edge[cnt] = head[u];  // 将当前边连接到u的邻接表
    to[cnt] = v;               // 设置边的终点
    weight[cnt] = w;           // 设置边的权重
    head[u] = cnt++;           // 更新u的头节点并递增边计数器
}

/**
 * 第一次DFS：计算以节点1为根时，每个节点到其子树需要翻转的边数
 * @param u 当前节点
 * @param f 父节点
 */
void dfs1(int u, int f) {
    // 遍历u的所有邻接边
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        int v = to[e];        // 边的终点
        int w = weight[e];    // 边的权重

        if (v != f) {         // 如果不是父节点，则继续递归
            dfs1(v, u);       // 递归处理子节点
            // 累加子节点需要翻转的边数 + 当前边的权重
            reverse_edges[u] += reverse_edges[v] + w;
        }
    }
}

/**
 * 第二次DFS：换根DP，计算以每个节点为根时需要翻转的边数
 * @param u 当前节点
 * @param f 父节点
 */
void dfs2(int u, int f) {
    // 遍历u的所有邻接边
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        int v = to[e];        // 边的终点
        int w = weight[e];    // 边的权重

        if (v != f) {         // 如果不是父节点
            if (w == 0) {
                // 边的方向是 u -> v (正向边)
                // 当根从u换到v时，这条边需要翻转，所以+1
                dp[v] = dp[u] + 1;
            } else {
                // 边的方向是 v -> u (反向边)
                // 当根从u换到v时，这条边不需要翻转了，所以-1
                dp[v] = dp[u] - 1;
            }
            dfs2(v, u);       // 递归处理子节点
        }
    }
}

int main() {
    // 优化输入输出速度
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 读取输入直到EOF
    while (cin >> n) {
        build();  // 初始化数据结构

        // 读取n-1条边，构建树
        for (int i = 1; i < n; i++) {
            int u, v;
            cin >> u >> v;
            // 添加双向边：
            // u -> v 权重为0 (正向边)
            // v -> u 权重为1 (反向边，需要翻转)
            addEdge(u, v, 0);
            addEdge(v, u, 1);
        }

        // 第一次DFS：计算以节点1为根时的翻转边数
        dfs1(1, 0);
        dp[1] = reverse_edges[1];  // 设置节点1的dp值

        // 第二次DFS：换根DP计算所有节点的dp值
        dfs2(1, 0);

        // 找出最小的翻转边数
        int min_val = INT_MAX;
        for (int i = 1; i <= n; i++) {
            if (min_val > dp[i]) {
                min_val = dp[i];
            }
        }

        // 输出最小翻转边数
        cout << min_val << "\n";

        // 输出所有达到最小翻转边数的节点
        for (int i = 1; i <= n; i++) {
            if (min_val == dp[i]) {
                cout << i << " ";
            }
        }
        cout << "\n";
    }

    return 0;
}
