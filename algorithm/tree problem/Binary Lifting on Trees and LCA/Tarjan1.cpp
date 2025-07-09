#include <iostream>
#include <vector>
#include <cstring>

// tarjan算法解法
// 测试链接 : https://www.luogu.com.cn/problem/P3379

using namespace std;

const int MAXN = 500001;

// ==================== 链式前向星建图 ====================
int headEdge[MAXN];     // 每个节点的邻接表头指针
int edgeNext[MAXN << 1]; // 边的下一条边的索引
int edgeTo[MAXN << 1];   // 边的终点
int tcnt;               // 边的计数器

// ==================== 查询存储 ====================
// 每个节点有哪些查询，也用链式前向星方式存储
int headQuery[MAXN];     // 每个节点的查询链表头指针
int queryNext[MAXN << 1]; // 查询的下一个查询的索引
int queryTo[MAXN << 1];   // 查询的另一个节点
int queryIndex[MAXN << 1]; // 查询的编号，用于知道答案应该填写在哪里
int qcnt;               // 查询计数器

// ==================== 算法辅助数组 ====================
bool visited[MAXN];     // 标记节点是否已被访问
int father[MAXN];       // 并查集的父节点数组
int ans[MAXN];          // 存储每个查询的答案

// ==================== 初始化函数 ====================
void build(int n) {
    tcnt = qcnt = 1;    // 从1开始计数（0作为空指针）
    memset(headEdge, 0, sizeof(headEdge));   // 初始化邻接表头指针
    memset(headQuery, 0, sizeof(headQuery)); // 初始化查询链表头指针
    memset(visited, false, sizeof(visited)); // 初始化访问标记
    // 初始化并查集，每个节点的父节点是自己
    for (int i = 1; i <= n; i++) {
        father[i] = i;
    }
}

// ==================== 建图函数 ====================
// 添加一条从u到v的边
void addEdge(int u, int v) {
    edgeNext[tcnt] = headEdge[u];  // 新边的下一条边指向原来的第一条边
    edgeTo[tcnt] = v;              // 新边的终点是v
    headEdge[u] = tcnt++;          // 更新u的第一条边，并递增计数器
}

// ==================== 查询存储函数 ====================
// 在节点u处添加一个查询(u,v)，查询编号为i
void addQuery(int u, int v, int i) {
    queryNext[qcnt] = headQuery[u];  // 新查询的下一个查询指向原来的第一个查询
    queryTo[qcnt] = v;               // 查询的另一个节点是v
    queryIndex[qcnt] = i;            // 查询的编号是i
    headQuery[u] = qcnt++;           // 更新u的第一个查询，并递增计数器
}

// ==================== 并查集查找函数 ====================
// 找到节点i的根节点（带路径压缩）
int find(int i) {
    if (i != father[i]) {
        father[i] = find(father[i]);  // 路径压缩：直接指向根节点
    }
    return father[i];
}

// ==================== Tarjan离线LCA算法 ====================
// u: 当前节点, f: 父节点
void tarjan(int u, int f) {
    visited[u] = true;  // 标记当前节点已访问

    // 遍历当前节点的所有邻接节点
    for (int e = headEdge[u]; e != 0; e = edgeNext[e]) {
        int v = edgeTo[e];
        if (v != f) {  // 不回到父节点
            tarjan(v, u);    // 递归处理子树
            father[v] = u;   // 处理完子树后，将子树的根指向当前节点
        }
    }

    // 处理所有与当前节点相关的查询
    for (int e = headQuery[u]; e != 0; e = queryNext[e]) {
        int v = queryTo[e];
        // 如果查询的另一个节点已经被访问过
        if (visited[v]) {
            // 那么它们的LCA就是v的当前根节点
            ans[queryIndex[e]] = find(v);
        }
    }
}

// ==================== 主函数 ====================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, root;
    cin >> n >> m >> root;  // 读入节点数、查询数、根节点

    build(n);  // 初始化

    // 读入n-1条边，构建树
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        addEdge(u, v);  // 添加双向边
        addEdge(v, u);
    }

    // 读入m个查询
    for (int i = 1; i <= m; i++) {
        int u, v;
        cin >> u >> v;
        // 每个查询在两个节点处都要添加
        addQuery(u, v, i);
        addQuery(v, u, i);
    }

    // 从根节点开始执行Tarjan算法
    tarjan(root, 0);

    // 输出所有查询的答案
    for (int i = 1; i <= m; i++) {
        cout << ans[i] << "\n";
    }

    return 0;
}
