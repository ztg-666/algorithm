#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

// 树的直径模版(两遍dfs)
// 给定一棵树，边权可能为负，求直径长度
// 测试链接 : https://www.luogu.com.cn/problem/U81904

using namespace std;

const int MAXN = 500001;

int n;                      // 树的节点数
int head[MAXN];            // 邻接表头指针，head[i]表示节点i的第一条边
int next_edge[MAXN << 1];  // 邻接表中指向下一条边的指针
int to[MAXN << 1];         // 边的终点
int weight[MAXN << 1];     // 边的权重
int cnt;                   // 边的计数器

// 直径的开始点
int start_node;

// 直径的结束点
int end_node;

// 直径长度
int diameter;

// dist[i] : 从规定的头节点出发，走到i的距离
int dist[MAXN];

// last[i] : 从规定的头节点出发，i节点的上一个节点（用于路径追踪）
int last[MAXN];

/**
 * 初始化邻接表
 */
void build() {
    cnt = 1;  // 边的编号从1开始
    fill(head + 1, head + n + 1, 0);  // 初始化头指针数组
}

/**
 * 添加一条从u到v权重为w的边
 * @param u 起点
 * @param v 终点
 * @param w 权重
 */
void addEdge(int u, int v, int w) {
    next_edge[cnt] = head[u];  // 新边指向u的原来的第一条边
    to[cnt] = v;               // 边的终点
    weight[cnt] = w;           // 边的权重
    head[u] = cnt++;           // 更新u的第一条边为新边，计数器递增
}

/**
 * 深度优先搜索，计算从指定起点到所有节点的距离
 * @param u 当前节点
 * @param f 父节点（避免回溯）
 * @param w 从父节点到当前节点的边权重
 */
void dfs(int u, int f, int w) {
    last[u] = f;                    // 记录父节点
    dist[u] = dist[f] + w;          // 累加距离

    // 遍历当前节点的所有邻接边
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        if (to[e] != f) {           // 避免回到父节点
            dfs(to[e], u, weight[e]); // 递归访问子节点
        }
    }
}

/**
 * 求树的直径
 * 使用两次DFS的经典算法：
 * 1. 从任意点出发，找到距离最远的点（直径的一个端点）
 * 2. 从该端点出发，找到距离最远的点（直径的另一个端点）
 */
void road() {
    // 第一次DFS：从节点1开始，找到距离最远的节点
    dfs(1, 0, 0);
    start_node = 1;
    for (int i = 2; i <= n; i++) {
        if (dist[i] > dist[start_node]) {
            start_node = i;         // 更新最远节点
        }
    }

    // 第二次DFS：从第一次找到的最远节点开始，找到真正的直径端点
    dfs(start_node, 0, 0);
    end_node = 1;
    for (int i = 2; i <= n; i++) {
        if (dist[i] > dist[end_node]) {
            end_node = i;           // 更新直径的另一个端点
        }
    }

    diameter = dist[end_node];      // 直径长度就是两个端点间的距离
}

int main() {
    ios::sync_with_stdio(false);    // 关闭C++与C的同步，提高输入输出速度
    cin.tie(nullptr);               // 解除cin与cout的绑定

    cin >> n;                       // 读入节点数
    build();                        // 初始化邻接表

    // 读入n-1条边（树有n-1条边）
    for (int i = 1, u, v, w; i < n; i++) {
        cin >> u >> v >> w;
        addEdge(u, v, w);           // 添加u->v的边
        addEdge(v, u, w);           // 添加v->u的边（无向图）
    }

    road();                         // 计算直径
    cout << diameter << endl;       // 输出直径长度

    return 0;
}
