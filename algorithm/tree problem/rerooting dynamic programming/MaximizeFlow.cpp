#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

// 选择节点做根使流量和最大(递归版)
// 给定一棵n个点的树，边的边权代表流量限制
// 从边上流过的流量，不能超过流量限制
// 现在想知道以某个节点做根时，流到所有叶节点的流量，最大是多少
// 测试链接 : http://poj.org/problem?id=3585

using namespace std;

const int MAXN = 200001;

int n;                              // 树的节点数
int head[MAXN];                     // 邻接表的头指针数组
int next_edge[MAXN << 1];          // 邻接表中指向下一条边的指针
int to[MAXN << 1];                 // 边的终点
int weight[MAXN << 1];             // 边的权重（流量限制）
int cnt;                           // 边的计数器

// degree[u] : 节点u的度数（连接的边数）
int degree[MAXN];

// flow[u] : 以u为根的子树中，从u流向所有叶节点的最大流量
int flow[MAXN];

// dp[u] : 以u为根的整棵树中，从u流向所有叶节点的最大流量
int dp[MAXN];

// 初始化函数，重置所有数组
void build() {
    cnt = 1;                       // 边的编号从1开始
    memset(head, 0, sizeof(head));
    memset(degree, 0, sizeof(degree));
    memset(flow, 0, sizeof(flow));
    memset(dp, 0, sizeof(dp));
}

// 添加一条从u到v权重为w的边
void addEdge(int u, int v, int w) {
    next_edge[cnt] = head[u];      // 新边指向原来的第一条边
    to[cnt] = v;                   // 设置边的终点
    weight[cnt] = w;               // 设置边的权重
    head[u] = cnt++;               // 更新头指针并递增边计数器
}

// 第一次DFS：计算每个节点作为子树根时的最大流量
void dfs1(int u, int f) {
    // 先递归处理所有子节点
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        int v = to[e];
        if (v != f) {              // 避免回到父节点
            dfs1(v, u);
        }
    }

    // 计算从当前节点u流向子树中所有叶节点的最大流量
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        int v = to[e];
        if (v != f) {              // 避免回到父节点
            if (degree[v] == 1) {  // 如果v是叶节点
                flow[u] += weight[e];  // 直接加上边的容量
            } else {               // 如果v不是叶节点
                // 取子树流量和边容量的最小值
                flow[u] += min(flow[v], weight[e]);
            }
        }
    }
}

// 第二次DFS：计算每个节点作为整棵树根时的最大流量
void dfs2(int u, int f) {
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        int v = to[e];
        if (v != f) {              // 避免回到父节点
            if (degree[u] == 1) {  // 如果u是叶节点
                dp[v] = flow[v] + weight[e];
            } else {               // 如果u不是叶节点
                // uOut : 从u流向除了v子树以外的所有叶节点的流量
                int uOut = dp[u] - min(flow[v], weight[e]);
                // v作为根时的流量 = v的子树流量 + 从u流向v的流量
                dp[v] = flow[v] + min(uOut, weight[e]);
            }
            dfs2(v, u);            // 递归处理子节点
        }
    }
}

int main() {
    // 优化输入输出速度
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int testCase;
    cin >> testCase;               // 读取测试用例数

    for (int t = 1; t <= testCase; t++) {
        cin >> n;                  // 读取节点数
        build();                   // 初始化

        // 读取n-1条边，构建树
        for (int i = 1; i < n; i++) {
            int u, v, w;
            cin >> u >> v >> w;
            addEdge(u, v, w);      // 添加u到v的边
            addEdge(v, u, w);      // 添加v到u的边（无向图）
            degree[u]++;           // 增加u的度数
            degree[v]++;           // 增加v的度数
        }

        // 第一次DFS：以节点1为根，计算每个节点的子树流量
        dfs1(1, 0);
        dp[1] = flow[1];           // 节点1作为根的流量就是其子树流量

        // 第二次DFS：计算每个节点作为根时的最大流量
        dfs2(1, 0);

        // 找出所有节点中作为根时流量的最大值
        int ans = 0;
        for (int i = 1; i <= n; i++) {
            ans = max(ans, dp[i]);
        }

        cout << ans << "\n";       // 输出答案
    }

    return 0;
}
