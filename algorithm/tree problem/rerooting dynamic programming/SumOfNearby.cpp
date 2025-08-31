#include <iostream>
#include <vector>
#include <cstring>

// 每个节点距离k以内的权值和(递归版)
// 给定一棵n个点的树，每个点有点权
// 到达每个节点的距离不超过k的节点就有若干个
// 把这些节点权值加起来，就是该点不超过距离k的点权和
// 打印每个节点不超过距离k的点权和
// 注意k并不大
// 测试链接 : https://www.luogu.com.cn/problem/P3047

using namespace std;

const int MAXN = 100001;  // 最大节点数
const int MAXK = 21;      // 最大距离k

int n, k;                 // n: 节点数, k: 距离限制
int head[MAXN];           // 邻接表头指针
int next_edge[MAXN << 1]; // 边的下一条边索引
int to[MAXN << 1];        // 边的终点
int cnt;                  // 边的计数器

// sum[u][i] : 以u为头的子树内，距离为i的节点权值和
int sum[MAXN][MAXK];

// dp[u][i] : 以u做根，整棵树上，距离为i的节点权值和
int dp[MAXN][MAXK];

/**
 * 初始化数据结构
 */
void build() {
    cnt = 1;  // 边计数器从1开始，0表示空边
    memset(head, 0, sizeof(head));     // 初始化邻接表头指针
    memset(sum, 0, sizeof(sum));       // 初始化子树距离权值和数组
    memset(dp, 0, sizeof(dp));         // 初始化全树距离权值和数组
}

/**
 * 添加无向边
 * @param u 起点
 * @param v 终点
 */
void addEdge(int u, int v) {
    next_edge[cnt] = head[u];  // 新边的下一条边是当前头指针指向的边
    to[cnt] = v;               // 新边的终点是v
    head[u] = cnt++;           // 更新头指针为新边，边计数器自增
}

/**
 * 第一次DFS：计算每个节点的子树内各距离的权值和
 * @param u 当前节点
 * @param f 父节点，用于避免回溯
 */
void dfs1(int u, int f) {
    // 先递归处理所有子节点
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        int v = to[e];
        if (v != f) {  // 避免回到父节点
            dfs1(v, u);
        }
    }

    // 回溯时，统计子树内各距离的权值和
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        int v = to[e];
        if (v != f) {
            // sum[u][j] += sum[v][j-1]
            // 子节点v距离为j-1的节点，在u看来距离为j
            for (int j = 1; j <= k; j++) {
                sum[u][j] += sum[v][j - 1];
            }
        }
    }
}

/**
 * 第二次DFS：换根DP，计算以每个节点为根时的全树距离权值和
 * @param u 当前节点
 * @param f 父节点
 */
void dfs2(int u, int f) {
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        int v = to[e];
        if (v != f) {
            // 计算以v为根时的dp值
            dp[v][0] = sum[v][0];  // 距离为0的只有v本身

            // 距离为1的包括：v子树内距离为1的 + u的权值
            dp[v][1] = sum[v][1] + dp[u][0];

            // 距离为i的包括：
            // 1. v子树内距离为i的节点
            // 2. 通过u能到达的距离为i-1的节点
            // 3. 减去v子树内距离为i-2的节点（这些节点在计算dp[u][i-1]时被重复计算）
            for (int i = 2; i <= k; i++) {
                dp[v][i] = sum[v][i] + dp[u][i - 1] - sum[v][i - 2];
            }

            // 递归处理子节点
            dfs2(v, u);
        }
    }
}

int main() {
    // 优化输入输出速度
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 读取节点数和距离限制
    cin >> n >> k;
    build();

    // 读取n-1条边，构建树
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        addEdge(u, v);  // 添加边u->v
        addEdge(v, u);  // 添加边v->u（无向图）
    }

    // 读取每个节点的权值（作为距离为0的权值和）
    for (int i = 1; i <= n; i++) {
        cin >> sum[i][0];
    }

    // 第一次DFS：以节点1为根，计算每个节点子树内的距离权值和
    dfs1(1, 0);

    // 初始化根节点1的dp值
    for (int i = 0; i <= k; i++) {
        dp[1][i] = sum[1][i];
    }

    // 第二次DFS：换根DP，计算每个节点作为根时的全树距离权值和
    dfs2(1, 0);

    // 输出每个节点距离不超过k的节点权值和
    for (int i = 1; i <= n; i++) {
        int ans = 0;
        // 累加距离0到k的所有权值和
        for (int j = 0; j <= k; j++) {
            ans += dp[i][j];
        }
        cout << ans << "\n";
    }

    return 0;
}
