#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

// 哪些点可以改造成重心
// 给定一棵n个点的树，你可以随便删掉一条边，然后随便加上一条边
// 通过这种方式可以让很多点变成重心
// 打印所有能变成重心的点
// 测试链接 : https://www.luogu.com.cn/problem/CF708C
// 测试链接 : https://codeforces.com/problemset/problem/708/C

using namespace std;

const int MAXN = 400001;

int n;
// 邻接表存储树结构
int head[MAXN];        // 每个节点的第一条边
int next_edge[MAXN << 1];  // 下一条边的索引
int to[MAXN << 1];     // 边的终点
int cnt;               // 边的计数器

// siz[i]: 以i为根的子树大小
int siz[MAXN];

// maxsub[i]: i节点的子树中，最大子树对应的子节点编号
int maxsub[MAXN];

// inner1[i]: i节点内部，大小<=n/2的最大子树的大小
int inner1[MAXN];

// inner2[i]: i节点内部，大小<=n/2的第二大子树的大小
int inner2[MAXN];

// 注意: inner1[i]和inner2[i]代表的子树必须来自i的不同子节点

// choose[i]: inner1[i]对应的子树是i的哪个子节点，记录节点编号
int choose[MAXN];

// outer[i]: i节点外部，大小<=n/2的最大子树的大小
int outer[MAXN];

/**
 * 初始化所有数组
 */
void build() {
    cnt = 1;  // 边的索引从1开始，0表示空
    memset(head, 0, sizeof(head));
    memset(maxsub, 0, sizeof(maxsub));
    memset(choose, 0, sizeof(choose));
    memset(inner1, 0, sizeof(inner1));
    memset(inner2, 0, sizeof(inner2));
    memset(outer, 0, sizeof(outer));
}

/**
 * 添加一条有向边 u -> v
 */
void addEdge(int u, int v) {
    next_edge[cnt] = head[u];  // 新边指向之前的第一条边
    to[cnt] = v;               // 边的终点
    head[u] = cnt++;           // 更新u的第一条边为新边
}

/**
 * 第一次DFS：计算子树大小和内部的最大、次大子树信息
 * @param u 当前节点
 * @param f 父节点
 */
void dfs1(int u, int f) {
    siz[u] = 1;  // 初始化子树大小为1（包含自己）

    // 遍历u的所有邻接节点
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        int v = to[e];
        if (v != f) {  // 不回到父节点
            dfs1(v, u);  // 递归处理子节点
            siz[u] += siz[v];  // 累加子树大小

            // 更新最大子树对应的子节点
            if (siz[maxsub[u]] < siz[v]) {
                maxsub[u] = v;
            }

            // 计算当前子树中可用的最大大小（<=n/2）
            int innersiz = siz[v] <= n / 2 ? siz[v] : inner1[v];

            // 更新inner1和inner2（维护最大和次大）
            if (inner1[u] < innersiz) {
                choose[u] = v;        // 记录最大子树对应的子节点
                inner2[u] = inner1[u]; // 原来的最大变成次大
                inner1[u] = innersiz; // 更新最大
            } else if (inner2[u] < innersiz) {
                inner2[u] = innersiz; // 更新次大
            }
        }
    }
}

/**
 * 第二次DFS：计算每个节点外部的最大子树信息
 * @param u 当前节点
 * @param f 父节点
 */
void dfs2(int u, int f) {
    // 遍历u的所有子节点
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        int v = to[e];
        if (v != f) {  // 不回到父节点
            // 计算v节点外部的最大子树大小
            if (n - siz[v] <= n / 2) {
                // 如果v的补集（除v子树外的所有节点）大小<=n/2
                outer[v] = n - siz[v];
            } else if (choose[u] != v) {
                // 如果v不是u的最大内部子树，则可以选择u的最大内部子树
                outer[v] = max(outer[u], inner1[u]);
            } else {
                // 如果v是u的最大内部子树，则只能选择u的次大内部子树
                outer[v] = max(outer[u], inner2[u]);
            }
            dfs2(v, u);  // 递归处理子节点
        }
    }
}

/**
 * 检查节点u是否可以通过删除一条边、添加一条边的操作变成重心
 * @param u 待检查的节点
 * @return 如果可以变成重心返回true，否则返回false
 */
bool check(int u) {
    // 情况1：u有一个子树大小>n/2
    if (siz[maxsub[u]] > n / 2) {
        // 检查是否可以通过删除这个大子树的一部分来满足重心条件
        return siz[maxsub[u]] - inner1[maxsub[u]] <= n / 2;
    }

    // 情况2：u的父亲方向的子树大小>n/2
    if (n - siz[u] > n / 2) {
        // 检查是否可以通过从外部删除一部分来满足重心条件
        return n - siz[u] - outer[u] <= n / 2;
    }

    // 情况3：u本身就满足重心条件
    return true;
}

int main() {
    // 优化输入输出速度
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n;
    build();

    // 读取树的边并建立邻接表
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        addEdge(u, v);  // 添加有向边u->v
        addEdge(v, u);  // 添加有向边v->u（无向图）
    }

    // 第一次DFS：计算子树信息
    dfs1(1, 0);

    // 第二次DFS：计算外部信息
    dfs2(1, 0);

    // 检查每个节点是否可以变成重心
    for (int i = 1; i <= n; i++) {
        cout << (check(i) ? "1 " : "0 ");
    }
    cout << endl;

    return 0;
}
