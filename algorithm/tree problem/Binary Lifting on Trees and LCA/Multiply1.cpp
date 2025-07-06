#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
using namespace std;

const int MAXN = 500001;    // 最大节点数
const int LIMIT = 20;       // 倍增的最大层数，2^20 > 500000

// 全局变量
int power;                  // 根据节点个数n计算出的2的幂次，用于倍增
int head[MAXN];            // 链式前向星的头节点数组
int next_edge[MAXN << 1];  // 链式前向星的next数组，存储下一条边的编号
int to[MAXN << 1];         // 链式前向星的to数组，存储边的终点
int cnt;                   // 边的计数器
int deep[MAXN];            // deep[i]表示节点i的深度（距离根节点的距离）
int stjump[MAXN][LIMIT];   // stjump[i][p]表示节点i往上跳2^p步到达的节点编号

/**
 * 计算2的对数，用于确定倍增需要的层数
 * 返回满足 2^ans <= n/2 的最大ans值
 */
int log2(int n) {
    int ans = 0;
    while ((1 << ans) <= (n >> 1)) {
        ans++;
    }
    return ans;
}

/**
 * 初始化函数
 * n: 节点个数
 */
void build(int n) {
    power = log2(n);                    // 计算倍增需要的层数
    cnt = 1;                           // 边的编号从1开始
    memset(head, 0, sizeof(head));     // 初始化头节点数组
}

/**
 * 添加边，使用链式前向星存储图
 * u: 边的起点
 * v: 边的终点
 */
void addEdge(int u, int v) {
    next_edge[cnt] = head[u];  // 新边的下一条边指向u的原来的第一条边
    to[cnt] = v;               // 新边的终点是v
    head[u] = cnt++;           // u的第一条边更新为新边，边的编号递增
}

/**
 * 深度优先搜索，预处理每个节点的深度和倍增数组
 * u: 当前节点
 * f: u的父节点
 */
void dfs(int u, int f) {
    deep[u] = deep[f] + 1;     // 当前节点的深度等于父节点深度+1
    stjump[u][0] = f;          // 节点u往上跳2^0=1步到达的节点是父节点f

    // 预处理倍增数组：stjump[u][p] = stjump[stjump[u][p-1]][p-1]
    // 节点u往上跳2^p步 = 先跳2^(p-1)步，再跳2^(p-1)步
    for (int p = 1; p <= power; p++) {
        stjump[u][p] = stjump[stjump[u][p - 1]][p - 1];
    }

    // 遍历u的所有邻接节点
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        if (to[e] != f) {          // 如果邻接节点不是父节点
            dfs(to[e], u);         // 递归处理子节点
        }
    }
}

/**
 * 求两个节点的最近公共祖先（LCA）
 * a, b: 要查询LCA的两个节点
 * 返回值: a和b的最近公共祖先节点编号
 */
int lca(int a, int b) {
    // 确保a的深度不小于b的深度
    if (deep[a] < deep[b]) {
        swap(a, b);
    }

    // 第一步：将a跳到与b相同的深度
    for (int p = power; p >= 0; p--) {
        // 如果a往上跳2^p步后的深度仍然不小于b的深度
        if (deep[stjump[a][p]] >= deep[b]) {
            a = stjump[a][p];      // 执行跳跃
        }
    }

    // 如果此时a和b相同，说明b就是a的祖先
    if (a == b) {
        return a;
    }

    // 第二步：同时向上跳跃，找到最近公共祖先
    for (int p = power; p >= 0; p--) {
        // 如果a和b往上跳2^p步后到达的节点不同
        if (stjump[a][p] != stjump[b][p]) {
            a = stjump[a][p];      // a向上跳2^p步
            b = stjump[b][p];      // b向上跳2^p步
        }
    }

    // 此时a和b都是LCA的直接子节点，所以LCA是a的父节点
    return stjump[a][0];
}

int main() {
    // 优化输入输出速度
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, root;
    cin >> n >> m >> root;     // 读入节点数、查询数、根节点

    build(n);                  // 初始化

    // 读入n-1条边，构建树
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        addEdge(u, v);         // 添加边u->v
        addEdge(v, u);         // 添加边v->u（无向图）
    }

    dfs(root, 0);              // 从根节点开始DFS预处理

    // 处理m个查询
    for (int i = 1; i <= m; i++) {
        int a, b;
        cin >> a >> b;
        cout << lca(a, b) << '\n';  // 输出a和b的LCA
    }

    return 0;
}
