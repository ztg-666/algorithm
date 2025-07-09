#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

// 造公园
// 一共n个节点，编号1~n，有m条边连接，边权都是1
// 所有节点可能形成多个连通区，每个连通区保证是树结构
// 有两种类型的操作
// 操作 1 x   : 返回x到离它最远的点的距离
// 操作 2 x y : 如果x和y已经连通，那么忽略
//              如果不连通，那么执行连通操作，把x和y各自的区域连通起来
//              并且要保证连通成的大区域的直径长度最小
// 测试链接 : https://www.luogu.com.cn/problem/P2195

using namespace std;

const int MAXN = 300001;

// 邻接表存储图结构
int head[MAXN];          // 头节点数组，存储每个节点的第一条边
int next_edge[MAXN << 1]; // 下一条边的索引
int to[MAXN << 1];       // 边的终点
int cnt;                 // 边的计数器

// 并查集相关
int father[MAXN];        // 父节点数组，用于并查集操作

// 树型DP相关
// dist[u]: 从节点u开始向下走能达到的最大距离
int dist[MAXN];

// diameter[i]: 如果i是连通分量的根节点，diameter[i]表示该连通分量的直径长度
//              如果i不是根节点，diameter[i]的值无效
// 这里使用了并查集+集合打标签的技巧
int diameter[MAXN];

// 初始化函数
void build(int n) {
    cnt = 1;                           // 边计数器从1开始（0表示空）
    memset(head, 0, sizeof(head));     // 清空邻接表头节点
    for (int i = 1; i <= n; i++) {
        father[i] = i;                 // 初始化并查集，每个节点的父节点是自己
    }
    memset(dist, 0, sizeof(dist));      // 清空距离数组
    memset(diameter, 0, sizeof(diameter)); // 清空直径数组
}

// 添加边（无向边需要添加两次）
void addEdge(int u, int v) {
    next_edge[cnt] = head[u];  // 新边的下一条边指向u的原第一条边
    to[cnt] = v;               // 新边的终点是v
    head[u] = cnt++;           // u的第一条边更新为新边，计数器递增
}

// 并查集的查找操作（带路径压缩）
int find(int i) {
    if (i != father[i]) {
        father[i] = find(father[i]);  // 路径压缩：直接连接到根节点
    }
    return father[i];
}

// 使用树型DP求树的直径
void dp(int u, int f) {
    // 第一次遍历：递归处理所有子节点
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        int v = to[e];
        if (v != f) {  // 避免回到父节点
            dp(v, u);
        }
    }

    // 第二次遍历：更新当前节点的直径和距离
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        int v = to[e];
        if (v != f) {
            // 更新以u为根的子树的直径
            // 直径可能是：
            // 1. 子树v的直径 diameter[v]
            // 2. 经过u的路径：dist[u] + dist[v] + 1
            diameter[u] = max(diameter[u], max(diameter[v], dist[u] + dist[v] + 1));

            // 更新从u向下的最大距离
            dist[u] = max(dist[u], dist[v] + 1);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, q;
    cin >> n >> m >> q;

    build(n);

    // 处理初始的m条边
    for (int i = 1; i <= m; i++) {
        int u, v;
        cin >> u >> v;
        addEdge(u, v);    // 添加边u->v
        addEdge(v, u);    // 添加边v->u（无向边）
        u = find(u);      // 找到u的根节点
        v = find(v);      // 找到v的根节点
        father[u] = v;    // 合并两个连通分量
    }

    // 对每个连通分量计算直径
    for (int i = 1; i <= n; i++) {
        if (i == father[i]) {  // 如果i是根节点
            dp(i, 0);          // 从i开始计算该连通分量的直径
        }
    }

    // 处理q个查询
    for (int i = 1; i <= q; i++) {
        int op;
        cin >> op;
        if (op == 1) {
            // 查询操作：输出节点x所在连通分量的直径
            int x;
            cin >> x;
            x = find(x);           // 找到x的根节点
            cout << diameter[x] << "\n";  // 输出该连通分量的直径
        } else {
            // 连接操作：连接节点x和y
            int x, y;
            cin >> x >> y;
            x = find(x);           // 找到x的根节点
            y = find(y);           // 找到y的根节点
            if (x != y) {          // 如果x和y不在同一连通分量中
                father[x] = y;     // 合并连通分量，x的根变为y

                // 更新合并后连通分量的直径
                // 新直径可能是：
                // 1. 原x连通分量的直径 diameter[x]
                // 2. 原y连通分量的直径 diameter[y]
                // 3. 通过新连接的边形成的路径：
                //    从x连通分量最远点 -> x连通分量中心 -> 新边 -> y连通分量中心 -> y连通分量最远点
                //    长度为 (diameter[x] + 1) / 2 + 1 + (diameter[y] + 1) / 2
                diameter[y] = max({(diameter[x] + 1) / 2 + (diameter[y] + 1) / 2 + 1,
                                  diameter[x], diameter[y]});
            }
        }
    }

    return 0;
}
