#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

// 树的直径模版(树型dp)
// 给定一棵树，边权可能为负，求直径长度
// 测试链接 : https://www.luogu.com.cn/problem/U81904

using namespace std;

const int MAXN = 500001;  // 最大节点数

int n;                    // 树的节点数
int head[MAXN];          // 邻接表的头节点数组
int next_edge[MAXN << 1]; // 邻接表中下一条边的索引
int to[MAXN << 1];       // 边的终点
int weight[MAXN << 1];   // 边的权重
int cnt;                 // 边的计数器

// dist[u]: 从节点u开始必须往下走，能走出的最大距离
// 表示以u为根的子树中，从u出发到叶子节点的最大权重路径
int dist[MAXN];

// ans[u]: 路径必须包含点u的情况下，最大路径和
// 表示以u为根节点时，通过u的最长路径长度
int ans[MAXN];

// 初始化函数
void build() {
    cnt = 1;  // 边的编号从1开始
    for (int i = 1; i <= n; i++) {
        head[i] = 0;    // 初始化邻接表头节点
        dist[i] = 0;    // 初始化距离数组
        ans[i] = 0;     // 初始化答案数组
    }
}

// 添加边的函数（邻接表实现）
void addEdge(int u, int v, int w) {
    next_edge[cnt] = head[u];  // 新边的下一条边指向原来的第一条边
    to[cnt] = v;               // 设置边的终点
    weight[cnt] = w;           // 设置边的权重
    head[u] = cnt++;           // 更新头节点并增加边计数
}

// 深度优先搜索 + 动态规划
void dp(int u, int f) {
    // 第一遍遍历：先递归处理所有子节点
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        int v = to[e];
        if (v != f) {  // 避免回到父节点
            dp(v, u);  // 递归处理子节点
        }
    }

    // 第二遍遍历：根据子节点的结果更新当前节点
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        int v = to[e];
        if (v != f) {  // 避免回到父节点
            // 更新通过节点u的最长路径
            // ans[u]表示经过u的最长路径，由u的两个不同子树中的最长路径组成
            ans[u] = max(ans[u], dist[u] + dist[v] + weight[e]);

            // 更新从节点u出发的最长路径
            // dist[u]表示从u出发到子树中任意节点的最长路径
            dist[u] = max(dist[u], dist[v] + weight[e]);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);  // 优化输入输出速度
    cin.tie(nullptr);

    cin >> n;  // 读入节点数
    build();   // 初始化

    // 读入n-1条边（树的边数）
    for (int i = 1; i < n; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        // 由于是无向图，需要添加双向边
        addEdge(u, v, w);
        addEdge(v, u, w);
    }

    // 从节点1开始进行深度优先搜索
    dp(1, 0);  // 0表示没有父节点

    // 找出所有节点中ans值的最大值，即树的直径
    int diameter = INT_MIN;
    for (int i = 1; i <= n; i++) {
        diameter = max(diameter, ans[i]);
    }

    cout << diameter << endl;  // 输出树的直径

    return 0;
}
