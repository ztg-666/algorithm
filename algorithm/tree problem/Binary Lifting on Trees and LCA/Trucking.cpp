#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <climits>
using namespace std;

const int MAXN = 10001;   // 最大节点数
const int MAXM = 50001;   // 最大边数
const int LIMIT = 21;     // 倍增的最大幂次 (2^21 > 10^6)

int power;  // 实际使用的最大幂次

// 边的结构体
struct Edge {
    int u, v, w;  // 起点、终点、权重
    bool operator<(const Edge& other) const {
        return w > other.w;  // 按权重从大到小排序（最大生成树）
    }
};

vector<Edge> edges;  // 存储所有边

// 并查集相关
int father[MAXN];  // 并查集的父节点数组

// DFS遍历标记
bool visited[MAXN];  // 标记节点是否已访问

// 最大生成树的邻接表表示
int head[MAXN];      // 链式前向星的头节点
int nxt[MAXM << 1];  // 下一条边的索引
int to[MAXM << 1];   // 边的终点
int weight[MAXM << 1]; // 边的权重
int cnt;             // 边的计数器

// 倍增LCA相关数组
int deep[MAXN];      // 每个节点的深度

// stjump[u][p] : 从节点u向上跳2^p步到达的节点
int stjump[MAXN][LIMIT];

// stmin[u][p] : 从节点u向上跳2^p步的路径中的最小权值
int stmin[MAXN][LIMIT];

// 计算log2(n)，用于确定倍增的最大幂次
int log2(int n) {
    int ans = 0;
    while ((1 << ans) <= (n >> 1)) {
        ans++;
    }
    return ans;
}

// 初始化函数
void build(int n) {
    power = log2(n);  // 计算倍增所需的最大幂次
    cnt = 1;          // 边计数器从1开始

    // 初始化并查集
    for (int i = 1; i <= n; i++) {
        father[i] = i;
    }

    // 初始化访问标记和邻接表
    memset(visited, false, sizeof(visited));
    memset(head, 0, sizeof(head));
    edges.clear();
}

// 并查集的查找函数（带路径压缩）
int find(int i) {
    if (i != father[i]) {
        father[i] = find(father[i]);  // 路径压缩
    }
    return father[i];
}

// 向邻接表中添加边
void addEdge(int u, int v, int w) {
    nxt[cnt] = head[u];  // 新边指向原来的第一条边
    to[cnt] = v;         // 设置终点
    weight[cnt] = w;     // 设置权重
    head[u] = cnt++;     // 更新头节点并递增计数器
}

// Kruskal算法构建最大生成树
void kruskal(int n) {
    sort(edges.begin(), edges.end());  // 按权重从大到小排序

    // 遍历所有边
    for (const Edge& e : edges) {
        int fa = find(e.u);  // 找到u的根节点
        int fb = find(e.v);  // 找到v的根节点

        // 如果u和v不在同一个连通分量中
        if (fa != fb) {
            father[fa] = fb;  // 合并两个连通分量
            // 添加无向边到最大生成树
            addEdge(e.u, e.v, e.w);
            addEdge(e.v, e.u, e.w);
        }
    }
}

// DFS遍历，构建倍增LCA所需的数据结构
void dfs(int u, int w, int f) {
    visited[u] = true;  // 标记当前节点已访问

    // 如果是根节点
    if (f == 0) {
        deep[u] = 1;              // 深度为1
        stjump[u][0] = u;         // 向上跳0步还是自己
        stmin[u][0] = INT_MAX;    // 根节点没有父边，设为无穷大
    } else {
        // 非根节点
        deep[u] = deep[f] + 1;    // 深度 = 父节点深度 + 1
        stjump[u][0] = f;         // 向上跳1步到达父节点
        stmin[u][0] = w;          // 到父节点的边权
    }

    // 构建倍增数组
    for (int p = 1; p <= power; p++) {
        // 向上跳2^p步 = 先跳2^(p-1)步，再跳2^(p-1)步
        stjump[u][p] = stjump[stjump[u][p - 1]][p - 1];

        // 路径上的最小值 = min(前半段最小值, 后半段最小值)
        stmin[u][p] = min(stmin[u][p - 1], stmin[stjump[u][p - 1]][p - 1]);
    }

    // 遍历所有相邻节点
    for (int e = head[u]; e != 0; e = nxt[e]) {
        if (!visited[to[e]]) {
            dfs(to[e], weight[e], u);  // 递归访问未访问的邻节点
        }
    }
}

// 查询两点间路径上的最小边权
int lca(int a, int b) {
    // 如果a和b不在同一个连通分量中，返回-1
    if (find(a) != find(b)) {
        return -1;
    }

    // 确保a的深度 >= b的深度
    if (deep[a] < deep[b]) {
        swap(a, b);
    }

    int ans = INT_MAX;  // 路径上的最小边权

    // 将a向上调整到与b相同的深度
    for (int p = power; p >= 0; p--) {
        if (deep[stjump[a][p]] >= deep[b]) {
            ans = min(ans, stmin[a][p]);  // 更新最小边权
            a = stjump[a][p];             // 向上跳跃
        }
    }

    // 如果调整后a和b相同，说明b是a的祖先
    if (a == b) {
        return ans;
    }

    // 同时向上调整a和b，直到它们的父节点相同
    for (int p = power; p >= 0; p--) {
        if (stjump[a][p] != stjump[b][p]) {
            ans = min(ans, min(stmin[a][p], stmin[b][p]));  // 更新最小边权
            a = stjump[a][p];  // a向上跳跃
            b = stjump[b][p];  // b向上跳跃
        }
    }

    // 最后一步：a和b都向上跳一步到达LCA
    ans = min(ans, min(stmin[a][0], stmin[b][0]));
    return ans;
}

int main() {
    ios::sync_with_stdio(false);  // 关闭C++与C的同步，提高输入输出效率
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;  // 读入节点数和边数

    build(n);  // 初始化

    // 读入所有边
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        edges.push_back({u, v, w});
    }

    kruskal(n);  // 构建最大生成树

    // 对每个连通分量进行DFS
    for (int i = 1; i <= n; i++) {
        if (!visited[i]) {
            dfs(i, 0, 0);  // 从未访问的节点开始DFS
        }
    }

    int q;
    cin >> q;  // 读入查询数量

    // 处理每个查询
    for (int i = 0; i < q; i++) {
        int a, b;
        cin >> a >> b;
        cout << lca(a, b) << "\n";  // 输出a到b路径上的最小边权
    }
    
    return 0;
}
