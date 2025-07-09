#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

// 删增边使其重心唯一
// 一共有n个节点，编号1~n，有n-1条边形成一棵树
// 现在希望重心是唯一的节点，调整的方式是先删除一条边、然后增加一条边
// 如果树上只有一个重心，需要删掉连接重心的任意一条边，再把这条边加上(否则无法通过已经实测)
// 如果树上有两个重心，调整的方式是先删除一条边、然后增加一条边，使重心是唯一的
// 如果方案有多种，打印其中一种即可
// 比如先删除节点3和节点4之间的边，再增加节点4和节点7之间的边，那么打印:
// "3 4"
// "4 7"
// 测试链接 : https://codeforces.com/problemset/problem/1406/C

using namespace std;

const int MAXN = 100001;

int n;                      // 节点数
int head[MAXN];            // 邻接表的头指针数组
int next_edge[MAXN << 1];  // 存储下一条边的位置
int to[MAXN << 1];         // 存储边的终点
int cnt;                   // 边的计数器

// size[i] : 从1号节点开始dfs的过程中，以i为头的子树的节点数
int size_arr[MAXN];

// maxsub[i] : 如果节点i做整棵树的根，最大子树的大小
int maxsub[MAXN];

// 收集所有的重心（最多2个）
int centers[2];

// 最大子树上的叶节点
int leaf;

// 叶节点的父亲节点
int leafFather;

// 初始化邻接表
void build() {
    cnt = 1;                              // 边的编号从1开始
    memset(head, 0, sizeof(head));       // 头指针数组初始化为0
}

// 添加一条从u到v的边
void addEdge(int u, int v) {
    next_edge[cnt] = head[u];            // 新边的next指向当前u的第一条边
    to[cnt] = v;                         // 新边的终点是v
    head[u] = cnt++;                     // u的头指针指向新边，边数加1
}

// 深度优先搜索，计算每个节点的子树大小和最大子树大小
void dfs(int u, int f) {
    size_arr[u] = 1;                     // 当前节点的子树大小初始化为1（自己）
    maxsub[u] = 0;                       // 最大子树大小初始化为0

    // 遍历当前节点u的所有邻接边
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        int v = to[e];                   // 获取边的终点
        if (v != f) {                    // 如果不是父节点（避免回溯）
            dfs(v, u);                   // 递归处理子节点
            size_arr[u] += size_arr[v];  // 累加子树大小
            maxsub[u] = max(maxsub[u], size_arr[v]); // 更新最大子树大小
        }
    }
    // 考虑去掉以u为根的子树后，剩余部分的大小
    maxsub[u] = max(maxsub[u], n - size_arr[u]);
}

// 随意找一个叶节点和该叶节点的父亲节点
// 这里找的是centers[1]子树中的任意一个叶节点
void find(int u, int f) {
    // 遍历当前节点的所有邻接边
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        if (to[e] != f) {               // 如果不是父节点
            find(to[e], u);             // 递归查找子节点
            return;                     // 找到后立即返回
        }
    }
    // 如果没有子节点，说明当前节点是叶节点
    leaf = u;                           // 记录叶节点
    leafFather = f;                     // 记录叶节点的父节点
}

// 返回重心的数量，并将重心存储在centers数组中
int centerCnt() {
    int m = 0;                          // 重心计数器
    for (int i = 1; i <= n; i++) {
        // 重心的定义：最大子树大小不超过n/2
        if (maxsub[i] <= n / 2) {
            centers[m++] = i;           // 将重心存储到centers数组中
        }
    }
    return m;                           // 返回重心数量
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int testCase;
    cin >> testCase;

    while (testCase--) {
        cin >> n;
        build();                        // 初始化邻接表

        // 读取n-1条边，构建树
        for (int i = 1; i < n; i++) {
            int u, v;
            cin >> u >> v;
            addEdge(u, v);              // 添加双向边
            addEdge(v, u);
        }

        dfs(1, 0);                      // 从节点1开始DFS，计算子树大小

        if (centerCnt() == 1) {
            // 如果只有一个重心，删除并重新添加连接重心的任意一条边
            cout << centers[0] << " " << to[head[centers[0]]] << "\n";
            cout << centers[0] << " " << to[head[centers[0]]] << "\n";
        } else {
            // 如果有两个重心，找到其中一个重心子树中的叶节点
            find(centers[1], centers[0]);
            // 删除叶节点和其父节点的边
            cout << leafFather << " " << leaf << "\n";
            // 将叶节点连接到另一个重心
            cout << centers[0] << " " << leaf << "\n";
        }
    }

    return 0;
}
