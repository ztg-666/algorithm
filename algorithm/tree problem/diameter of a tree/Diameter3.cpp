#include <iostream>
#include <algorithm>
#include <climits>
using namespace std;

const int MAXN = 500001;

int n; // 树的节点数
int head[MAXN]; // 邻接表头指针
int next_edge[MAXN << 1]; // 下一条边的指针
int to[MAXN << 1]; // 边的终点
int weight[MAXN << 1]; // 边的权重
int cnt; // 边的计数器
int dist[MAXN]; // 从每个节点向下的最长距离
int diameter; // 树的直径

// 初始化函数
void build() {
    cnt = 1; // 边的编号从1开始
    // 初始化头指针数组
    for (int i = 1; i <= n; i++) {
        head[i] = 0;
    }
    // 初始化距离数组
    for (int i = 1; i <= n; i++) {
        dist[i] = 0;
    }
    diameter = INT_MIN; // 初始化直径为最小值
}

// 添加边的函数（邻接表实现）
void addEdge(int u, int v, int w) {
    next_edge[cnt] = head[u]; // 新边指向原来的第一条边
    to[cnt] = v; // 设置边的终点
    weight[cnt] = w; // 设置边的权重
    head[u] = cnt++; // 更新头指针，边计数器递增
}

// 动态规划求树的直径
void dp(int u, int f) {
    // 第一次遍历：递归处理所有子节点
    for (int e = head[u], v; e != 0; e = next_edge[e]) {
        v = to[e]; // 获取边的终点
        if (v != f) { // 如果不是父节点
            dp(v, u); // 递归处理子节点
        }
    }

    // 第二次遍历：更新直径和当前节点的最长距离
    for (int e = head[u], v; e != 0; e = next_edge[e]) {
        v = to[e]; // 获取边的终点
        if (v != f) { // 如果不是父节点
            // 更新直径：当前节点u的最长距离 + 子节点v的最长距离 + 边权重
            // 这表示经过节点u的最长路径
            diameter = max(diameter, dist[u] + dist[v] + weight[e]);

            // 更新当前节点u向下的最长距离
            dist[u] = max(dist[u], dist[v] + weight[e]);
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n; // 读入节点数
    build(); // 初始化

    // 读入n-1条边，构建树
    for (int i = 1; i < n; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        addEdge(u, v, w); // 添加边u->v
        addEdge(v, u, w); // 添加边v->u（无向图）
    }

    dp(1, 0); // 从节点1开始DFS，0表示没有父节点
    cout << diameter << endl; // 输出树的直径

    return 0;
}
