#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>

// 松鼠的新家(递归版)
// 有n个节点形成一棵树
// 给定一个由点编号组成的数组，表示松鼠依次要去往的地点
// 松鼠每走到一个节点都必须拿一个糖果，否则松鼠就停止前进
// 松鼠来到最后一个地点时不需要吃糖果
// 打印每个节点上至少准备多少糖果才能让松鼠依次走完数组里的节点
// 测试链接 : https://www.luogu.com.cn/problem/P3258

using namespace std;

const int MAXN = 300001;

// ==================== 数据结构定义 ====================

// 依次去往节点的顺序（旅行路线）
int travel[MAXN];

// 每个节点需要分配多少糖果（最终答案）
int num[MAXN];

// ==================== 链式前向星建图 ====================
int headEdge[MAXN];     // 每个节点的第一条边的索引
int edgeNext[MAXN << 1]; // 每条边的下一条边索引
int edgeTo[MAXN << 1];   // 每条边指向的节点
int tcnt;                // 边的计数器

// ==================== Tarjan算法相关结构 ====================
int headQuery[MAXN];     // 每个节点的第一个查询的索引
int queryNext[MAXN << 1]; // 每个查询的下一个查询索引
int queryTo[MAXN << 1];   // 每个查询的目标节点
int queryIndex[MAXN << 1]; // 每个查询的编号
int qcnt;                 // 查询计数器

bool visited[MAXN];       // 标记节点是否被访问过

// unionfind数组是tarjan算法专用的并查集结构
int unionfind[MAXN];

// father数组不用作并查集，就是记录每个节点的父亲节点
int father[MAXN];

// ans数组是tarjan算法的输出结果，记录每次旅行两端点的最低公共祖先
int ans[MAXN];

// ==================== 初始化函数 ====================
void build(int n) {
    // 初始化糖果数组
    fill(num + 1, num + n + 1, 0);

    // 初始化边和查询计数器
    tcnt = qcnt = 1;

    // 初始化邻接表头节点
    fill(headEdge + 1, headEdge + n + 1, 0);
    fill(headQuery + 1, headQuery + n + 1, 0);

    // 初始化访问标记
    fill(visited + 1, visited + n + 1, false);

    // 初始化并查集，每个节点的父节点是自己
    for (int i = 1; i <= n; i++) {
        unionfind[i] = i;
    }
}

// ==================== 建图函数 ====================
// 添加无向边（u-v）
void addEdge(int u, int v) {
    edgeNext[tcnt] = headEdge[u];  // 新边的下一条边指向原来的第一条边
    edgeTo[tcnt] = v;              // 新边指向节点v
    headEdge[u] = tcnt++;          // 更新节点u的第一条边为新边
}

// 添加查询：询问节点u和节点v的最低公共祖先（第i个查询）
void addQuery(int u, int v, int i) {
    queryNext[qcnt] = headQuery[u]; // 新查询的下一个查询指向原来的第一个查询
    queryTo[qcnt] = v;              // 新查询的目标节点是v
    queryIndex[qcnt] = i;           // 新查询的编号是i
    headQuery[u] = qcnt++;          // 更新节点u的第一个查询为新查询
}

// ==================== 并查集操作 ====================
// 并查集的查找操作（带路径压缩）
int find(int i) {
    if (i != unionfind[i]) {
        unionfind[i] = find(unionfind[i]); // 路径压缩
    }
    return unionfind[i];
}

// ==================== Tarjan算法求LCA ====================
// 离线求最低公共祖先的Tarjan算法
void tarjan(int u, int f) {
    visited[u] = true; // 标记当前节点已访问

    // 遍历当前节点的所有子节点
    for (int e = headEdge[u], v; e != 0; e = edgeNext[e]) {
        v = edgeTo[e];
        if (v != f) { // 如果不是父节点，就是子节点
            tarjan(v, u); // 递归访问子节点
        }
    }

    // 处理与当前节点相关的所有查询
    for (int e = headQuery[u], v; e != 0; e = queryNext[e]) {
        v = queryTo[e];
        if (visited[v]) { // 如果查询的目标节点已经访问过
            ans[queryIndex[e]] = find(v); // 找到LCA
        }
    }

    // 将当前节点合并到父节点的集合中
    unionfind[u] = f;
    father[u] = f; // 记录父节点
}

// ==================== 树上DFS统计 ====================
// 深度优先搜索，统计每个节点的子树中需要的糖果总数
void dfs(int u, int f) {
    // 先递归处理所有子节点
    for (int e = headEdge[u], v; e != 0; e = edgeNext[e]) {
        v = edgeTo[e];
        if (v != f) { // 如果不是父节点，就是子节点
            dfs(v, u); // 递归处理子节点
        }
    }

    // 将所有子节点的糖果数累加到当前节点
    for (int e = headEdge[u], v; e != 0; e = edgeNext[e]) {
        v = edgeTo[e];
        if (v != f) { // 如果不是父节点，就是子节点
            num[u] += num[v]; // 累加子节点的糖果数
        }
    }
}

// ==================== 主要计算函数 ====================
void compute(int n) {
    // 第一步：使用Tarjan算法求出所有相邻旅行节点的LCA
    tarjan(1, 0);

    // 第二步：对每条旅行路径，使用差分数组的思想统计路径上的节点
    for (int i = 1, u, v, lca, lcafather; i < n; i++) {
        u = travel[i];       // 当前旅行的起点
        v = travel[i + 1];   // 当前旅行的终点
        lca = ans[i];        // 起点和终点的最低公共祖先
        lcafather = father[lca]; // LCA的父节点

        // 使用差分数组的方法：
        // 路径u到v经过lca，相当于路径u到lca + 路径v到lca - 路径lca到lca
        num[u]++;           // 起点+1
        num[v]++;           // 终点+1
        num[lca]--;         // LCA-1（因为被重复计算了一次）
        num[lcafather]--;   // LCA的父节点-1（差分数组边界处理）
    }

    // 第三步：通过DFS将差分数组转换为实际的路径经过次数
    dfs(1, 0);

    // 第四步：每个节点需要减去1，因为旅行者本身会消耗一个糖果
    for (int i = 2; i <= n; i++) {
        num[travel[i]]--; // 除了起点外，每个旅行节点都要减1
    }
}

// ==================== 主函数 ====================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    build(n); // 初始化数据结构

    // 读入旅行路线
    for (int i = 1; i <= n; i++) {
        cin >> travel[i];
    }

    // 读入树的边并建图
    for (int i = 1, u, v; i < n; i++) {
        cin >> u >> v;
        addEdge(u, v); // 添加边u-v
        addEdge(v, u); // 添加边v-u（无向图）
    }

    // 添加LCA查询：每相邻两个旅行节点之间的LCA
    for (int i = 1; i < n; i++) {
        addQuery(travel[i], travel[i + 1], i);     // 正向查询
        addQuery(travel[i + 1], travel[i], i);     // 反向查询（对称）
    }

    // 执行主要计算
    compute(n);

    // 输出结果：每个节点需要的糖果数
    for (int i = 1; i <= n; i++) {
        cout << num[i] << "\n";
    }

    return 0;
}
