#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

// 所有直径的公共部分(递归版)
// 给定一棵树，边权都为正
// 打印直径长度、所有直径的公共部分有几条边
// 测试链接 : https://www.luogu.com.cn/problem/P3304

using namespace std;

const int MAXN = 200001;

// 全局变量定义
int n;                          // 节点数量
int head[MAXN];                 // 邻接表的头指针数组
int next_edge[MAXN << 1];       // 邻接表中下一条边的指针
int to[MAXN << 1];              // 边的终点
int weight[MAXN << 1];          // 边的权重
int cnt;                        // 边的计数器

int start_node;                 // 直径的起始节点
int end_node;                   // 直径的终止节点
long long dist[MAXN];           // 从当前根节点到各节点的距离
int last[MAXN];                 // 记录每个节点的父节点（用于路径重建）
long long diameter;             // 树的直径长度
bool diameterPath[MAXN];        // 标记节点是否在直径路径上
int commonEdges;                // 公共边的数量

// 初始化函数
void build() {
    cnt = 1;                    // 边计数器从1开始（0表示空）
    fill(head + 1, head + n + 1, 0);           // 清空邻接表头指针
    fill(diameterPath + 1, diameterPath + n + 1, false);  // 清空直径路径标记
}

// 添加无向边（u-v，权重为w）
void addEdge(int u, int v, int w) {
    next_edge[cnt] = head[u];   // 新边指向当前头节点
    to[cnt] = v;                // 设置边的终点
    weight[cnt] = w;            // 设置边的权重
    head[u] = cnt++;            // 更新头指针并递增计数器
}

// 深度优先搜索，计算从节点u到所有其他节点的距离
// u: 当前节点, f: 父节点, c: 当前距离
void dfs(int u, int f, long long c) {
    last[u] = f;                // 记录父节点
    dist[u] = c;                // 记录到根节点的距离

    // 遍历所有相邻节点
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        if (to[e] != f) {       // 避免回到父节点
            dfs(to[e], u, c + weight[e]);  // 递归访问子节点
        }
    }
}

// 使用两次DFS找到树的直径
void road() {
    // 第一次DFS：从节点1开始，找到距离最远的节点
    dfs(1, 0, 0);
    start_node = 1;
    for (int i = 2; i <= n; i++) {
        if (dist[i] > dist[start_node]) {
            start_node = i;     // 更新最远节点
        }
    }

    // 第二次DFS：从第一次找到的最远节点开始，找到真正的直径终点
    dfs(start_node, 0, 0);
    end_node = 1;
    for (int i = 2; i <= n; i++) {
        if (dist[i] > dist[end_node]) {
            end_node = i;       // 更新直径的另一端
        }
    }

    diameter = dist[end_node];  // 直径长度
}

// 计算从节点u出发，不经过直径路径上的节点，能到达的最大距离
// u: 当前节点, f: 父节点, c: 当前距离
long long maxDistanceExceptDiameter(int u, int f, long long c) {
    long long ans = c;          // 初始化为当前距离

    // 遍历所有相邻节点
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        int v = to[e];
        // 如果相邻节点不在直径路径上且不是父节点
        if (!diameterPath[v] && v != f) {
            ans = max(ans, maxDistanceExceptDiameter(v, u, c + weight[e]));
        }
    }
    return ans;
}

// 主要计算函数
void compute() {
    road();  // 找到直径

    // 标记直径路径上的所有节点
    for (int i = end_node; i != 0; i = last[i]) {
        diameterPath[i] = true;
    }

    int l = start_node;         // 左端点初始化
    int r = end_node;           // 右端点初始化
    long long maxDist;

    // 遍历直径路径上的节点（除了起始和终止节点）
    for (int i = last[end_node]; i != start_node; i = last[i]) {
        // 计算从当前节点出发，不经过直径路径能到达的最大距离
        maxDist = maxDistanceExceptDiameter(i, 0, 0);

        // 如果这个距离等于从当前节点到终点的距离，更新右端点
        if (maxDist == diameter - dist[i]) {
            r = i;
        }
        // 如果这个距离等于从起点到当前节点的距离，更新左端点
        if (maxDist == dist[i] && l == start_node) {
            l = i;
        }
    }

    // 计算公共边的数量
    if (l == r) {
        commonEdges = 0;        // 如果左右端点相同，没有公共边
    } else {
        commonEdges = 1;        // 初始化为1
        // 计算从l到r之间的边数
        for (int i = last[r]; i != l; i = last[i]) {
            commonEdges++;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;                   // 读入节点数
    build();                    // 初始化

    // 读入n-1条边
    for (int i = 1; i < n; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        addEdge(u, v, w);       // 添加边u->v
        addEdge(v, u, w);       // 添加边v->u（无向图）
    }

    compute();                  // 执行主要计算

    cout << diameter << "\n";   // 输出直径长度
    cout << commonEdges << "\n"; // 输出公共边数量

    return 0;
}
