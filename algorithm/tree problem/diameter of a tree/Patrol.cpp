#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

// 巡逻
// 一共n个节点，编号1~n，结构是一棵树，每条边都是双向的
// 警察局在1号点，警车每天从1号点出发，一定要走过树上所有的边，最后回到1号点
// 现在为了减少经过边的数量，你可以新建k条边，把树上任意两点直接相连
// 并且每天警车必须经过新建的道路正好一次
// 计算出最佳的新建道路的方案，返回巡逻走边数量的最小值
// 测试链接 : https://www.luogu.com.cn/problem/P3629

using namespace std;

const int MAXN = 100001;

// 全局变量定义
int n, k;                    // n: 节点数, k: 可以传送的次数
int head[MAXN];              // 邻接表头指针
int next_edge[MAXN << 1];    // 邻接表下一条边
int to[MAXN << 1];           // 边的终点
int cnt;                     // 边的计数器
int start_node, end_node;    // 直径的两个端点
int dist[MAXN];              // 距离数组
int last_node[MAXN];         // 记录父节点，用于路径回溯
int diameter1, diameter2;    // 第一长直径和第二长直径
bool diameterPath[MAXN];     // 标记节点是否在直径路径上

// 初始化函数
void build() {
    cnt = 1;
    fill(head + 1, head + n + 1, 0);           // 清空邻接表头指针
    diameter1 = 0;
    diameter2 = 0;
    fill(diameterPath + 1, diameterPath + n + 1, false);  // 清空直径路径标记
}

// 添加边到邻接表
void addEdge(int u, int v) {
    next_edge[cnt] = head[u];
    to[cnt] = v;
    head[u] = cnt++;
}

// DFS搜索，计算从起始节点到各节点的距离
void dfs(int u, int f, int w) {
    last_node[u] = f;        // 记录父节点
    dist[u] = dist[f] + w;   // 计算到根节点的距离

    // 遍历所有相邻节点
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        if (to[e] != f) {    // 避免回到父节点
            dfs(to[e], u, 1);
        }
    }
}

// 找到树的直径（最长路径）
void road() {
    // 第一次DFS：从节点1开始，找到距离最远的节点
    dfs(1, 0, 0);
    start_node = 1;
    for (int i = 2; i <= n; i++) {
        if (dist[i] > dist[start_node]) {
            start_node = i;
        }
    }

    // 第二次DFS：从刚才找到的最远节点开始，找到真正的直径
    dfs(start_node, 0, 0);
    end_node = 1;
    for (int i = 2; i <= n; i++) {
        if (dist[i] > dist[end_node]) {
            end_node = i;
        }
    }

    diameter1 = dist[end_node];  // 记录直径长度
}

// 动态规划计算第二长路径
void dp(int u, int f) {
    // 先递归处理所有子节点
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        int v = to[e];
        if (v != f) {
            dp(v, u);
        }
    }

    // 计算经过当前节点的最长路径
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        int v = to[e];
        if (v != f) {
            // 如果当前节点和子节点都在直径路径上，权重为-1（表示可以免费传送）
            // 否则权重为1（需要走路）
            int w = (diameterPath[u] && diameterPath[v]) ? -1 : 1;

            // 更新第二长直径
            diameter2 = max(diameter2, dist[u] + dist[v] + w);

            // 更新当前节点的最长距离
            dist[u] = max(dist[u], dist[v] + w);
        }
    }
}

// 计算最终结果
int compute() {
    road();  // 找到树的直径

    if (k == 1) {
        // 如果只能传送一次，最优策略是传送直径，然后遍历剩余部分
        return 2 * (n - 1) - diameter1 + 1;
    } else {
        // 如果能传送多次，标记直径路径上的所有节点
        for (int i = end_node; i != 0; i = last_node[i]) {
            diameterPath[i] = true;
        }

        // 重新初始化距离数组，计算第二长路径
        fill(dist + 1, dist + n + 1, 0);
        dp(1, 0);

        // 最优策略是传送两条最长路径
        return 2 * (n - 1) - diameter1 - diameter2 + 2;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> k;
    build();

    // 读入树的边
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        addEdge(u, v);
        addEdge(v, u);
    }

    cout << compute() << endl;

    return 0;
}
