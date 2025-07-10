#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>

// 树上点差分模版(递归版)
// 有n个节点形成一棵树，一开始所有点权都是0
// 给定很多操作，每个操作(a,b)表示从a到b路径上所有点的点权增加1
// 所有操作完成后，返回树上的最大点权
// 测试链接 : https://www.luogu.com.cn/problem/P3128

using namespace std;

const int MAXN = 50001;     // 最大节点数
const int LIMIT = 16;       // 二进制跳跃表的最大层数 (log2(50000) < 16)

int power;                  // 实际需要的跳跃表层数
int num[MAXN];             // 每个节点被路径经过的次数
int head[MAXN];            // 邻接表头指针
int next_edge[MAXN << 1];  // 邻接表的下一条边
int to[MAXN << 1];         // 边的终点
int cnt;                   // 边的计数器
int deep[MAXN];            // 每个节点的深度
int stjump[MAXN][LIMIT];   // 倍增跳跃表，stjump[i][j]表示从节点i向上跳2^j步到达的节点

// 计算log2(n)，用于确定跳跃表的层数
int log2(int n) {
    int ans = 0;
    while ((1 << ans) <= (n >> 1)) {
        ans++;
    }
    return ans;
}

// 初始化建图
void build(int n) {
    power = log2(n);                    // 计算需要的跳跃表层数
    memset(num, 0, sizeof(num));        // 初始化路径经过次数数组
    cnt = 1;                            // 边的编号从1开始
    memset(head, 0, sizeof(head));      // 初始化邻接表头指针
}

// 添加边 (u, v)
void addEdge(int u, int v) {
    next_edge[cnt] = head[u];   // 新边的下一条边指向原来的第一条边
    to[cnt] = v;                // 新边的终点是v
    head[u] = cnt++;            // 更新u的第一条边为新边，并递增边计数器
}

// 第一次DFS：计算深度和构建倍增跳跃表
void dfs1(int u, int f) {
    deep[u] = deep[f] + 1;      // 当前节点深度 = 父节点深度 + 1
    stjump[u][0] = f;           // 跳跃表第0层：直接父节点

    // 构建倍增跳跃表
    for (int p = 1; p <= power; p++) {
        // stjump[u][p] = 从u向上跳2^p步到达的节点
        // 等于从u跳2^(p-1)步，再从那个节点跳2^(p-1)步
        stjump[u][p] = stjump[stjump[u][p - 1]][p - 1];
    }

    // 遍历所有子节点
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        if (to[e] != f) {           // 不回到父节点
            dfs1(to[e], u);         // 递归处理子节点
        }
    }
}

// 使用倍增算法求LCA
int lca(int a, int b) {
    // 确保a的深度大于等于b的深度
    if (deep[a] < deep[b]) {
        swap(a, b);
    }

    // 第一步：将a上提到与b相同的深度
    for (int p = power; p >= 0; p--) {
        if (deep[stjump[a][p]] >= deep[b]) {
            a = stjump[a][p];
        }
    }

    // 如果此时a == b，说明b是a的祖先
    if (a == b) {
        return a;
    }

    // 第二步：同时上提a和b，直到它们的父节点相同
    for (int p = power; p >= 0; p--) {
        if (stjump[a][p] != stjump[b][p]) {
            a = stjump[a][p];
            b = stjump[b][p];
        }
    }

    // 返回LCA（此时a和b的父节点相同）
    return stjump[a][0];
}

// 第二次DFS：计算每个节点被路径经过的次数
void dfs2(int u, int f) {
    // 先递归处理所有子节点
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        int v = to[e];
        if (v != f) {
            dfs2(v, u);
        }
    }

    // 将所有子节点的路径经过次数累加到当前节点
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        int v = to[e];
        if (v != f) {
            num[u] += num[v];
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    cin >> n >> m;      // n个节点，m条路径

    build(n);           // 初始化

    // 读入树的n-1条边
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        addEdge(u, v);  // 添加无向边
        addEdge(v, u);
    }

    dfs1(1, 0);         // 以节点1为根，构建倍增跳跃表

    // 处理m条路径
    for (int i = 1; i <= m; i++) {
        int u, v;
        cin >> u >> v;
        int lca_node = lca(u, v);               // 求u和v的LCA
        int lcafather = stjump[lca_node][0];    // LCA的父节点

        // 差分数组技巧：
        // 路径u到v经过的节点在树上形成u->lca<-v的路径
        // 对起点和终点+1，对lca和lca父节点-1
        num[u]++;
        num[v]++;
        num[lca_node]--;
        num[lcafather]--;
    }

    dfs2(1, 0);         // 通过DFS计算每个节点的实际被经过次数

    // 找出被经过次数最多的节点
    int max_val = 0;
    for (int i = 1; i <= n; i++) {
        max_val = max(max_val, num[i]);
    }

    cout << max_val << endl;

    return 0;
}
