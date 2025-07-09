#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cstring>

// 消防(递归版)
// 一共n个节点，编号1~n，有n-1条边连接成一棵树，每条边上有非负权值
// 给定一个非负整数s，表示可以在树上选择一条长度不超过s的路径
// 然后在这条路径的点上建立消防站，每个居民可以去往这条路径上的任何消防站
// 目标：哪怕最远的居民走到消防站的距离也要尽量少
// 返回最远居民走到消防站的最短距离
// 测试链接 : https://www.luogu.com.cn/problem/P2491

using namespace std;

const int MAXN = 300001;

// 全局变量定义
int n, s;                    // n: 节点数, s: 限制条件
int head[MAXN];              // 邻接表头指针
int next_edge[MAXN << 1];    // 下一条边的指针
int to[MAXN << 1];           // 边的终点
int weight[MAXN << 1];       // 边的权重
int cnt;                     // 边的计数器

int start_node, end_node;    // 直径的起点和终点
int diameter;                // 直径长度
int dist[MAXN];              // 距离数组
int last[MAXN];              // 父节点记录
int pred[MAXN];              // 到父节点的边权重
bool diameterPath[MAXN];     // 标记是否在直径路径上
int maxDist[MAXN];           // 除直径外的最大距离

/**
 * 初始化图的数据结构
 */
void build() {
    cnt = 1;
    memset(head, 0, sizeof(head));
    memset(diameterPath, false, sizeof(diameterPath));
}

/**
 * 添加无向边
 * @param u 起点
 * @param v 终点
 * @param w 权重
 */
void addEdge(int u, int v, int w) {
    next_edge[cnt] = head[u];
    to[cnt] = v;
    weight[cnt] = w;
    head[u] = cnt++;
}

/**
 * 深度优先搜索，计算距离和路径信息
 * @param u 当前节点
 * @param f 父节点
 * @param w 到父节点的边权重
 */
void dfs(int u, int f, int w) {
    last[u] = f;              // 记录父节点
    dist[u] = dist[f] + w;    // 累计距离
    pred[u] = w;              // 到父节点的边权重

    // 遍历所有邻接节点
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        if (to[e] != f) {      // 不回到父节点
            dfs(to[e], u, weight[e]);
        }
    }
}

/**
 * 找到树的直径
 * 使用两次DFS：
 * 1. 从任意点找最远点作为直径一端
 * 2. 从该点找最远点作为直径另一端
 */
void road() {
    // 第一次DFS：从节点1开始找最远点
    dfs(1, 0, 0);
    start_node = 1;
    for (int i = 2; i <= n; i++) {
        if (dist[i] > dist[start_node]) {
            start_node = i;
        }
    }

    // 第二次DFS：从start_node开始找最远点
    dfs(start_node, 0, 0);
    end_node = 1;
    for (int i = 2; i <= n; i++) {
        if (dist[i] > dist[end_node]) {
            end_node = i;
        }
    }
    diameter = dist[end_node];  // 直径长度
}

/**
 * 计算从节点u开始，不经过直径路径的最大距离
 * @param u 当前节点
 * @param f 父节点
 * @param c 累计距离
 * @return 最大距离
 */
int maxDistanceExceptDiameter(int u, int f, int c) {
    int ans = c;
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        int v = to[e];
        // 只考虑不在直径路径上的节点
        if (!diameterPath[v] && v != f) {
            ans = max(ans, maxDistanceExceptDiameter(v, u, c + weight[e]));
        }
    }
    return ans;
}

/**
 * 标记直径路径上的节点，并计算每个节点的最大分支距离
 */
void distance() {
    // 标记直径路径上的所有节点
    for (int i = end_node; i != 0; i = last[i]) {
        diameterPath[i] = true;
    }

    // 计算直径路径上每个节点的最大分支距离
    for (int i = end_node; i != 0; i = last[i]) {
        maxDist[i] = maxDistanceExceptDiameter(i, 0, 0);
    }
}

int queue_mono[MAXN];  // 单调队列

/**
 * 计算最优解
 * 使用滑动窗口和单调队列优化
 * @return 最小的最大距离
 */
int compute() {
    int suml = 0, sumr = 0;    // 左端和右端的权重和
    int h = 0, t = 0;          // 单调队列的头尾指针
    int ans = INT_MAX;

    // 双指针遍历直径路径
    for (int l = end_node, r = end_node; l != 0; l = last[l]) {
        // 扩展右端点，直到权重和超过限制s
        while (r != 0 && sumr - suml + pred[r] <= s) {
            // 维护单调递减队列（最大值在队首）
            while (h < t && maxDist[queue_mono[t - 1]] <= maxDist[r]) {
                t--;
            }
            sumr += pred[r];
            queue_mono[t++] = r;
            r = last[r];
        }

        // 更新答案：考虑三个部分的最大值
        // 1. suml: 左端部分
        // 2. maxDist[queue_mono[h]]: 窗口内最大分支距离
        // 3. diameter - sumr: 右端部分
        ans = min(ans, max({suml, maxDist[queue_mono[h]], diameter - sumr}));

        // 如果队首元素是当前左端点，则出队
        if (queue_mono[h] == l) {
            h++;
        }
        suml += pred[l];
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> s;
    build();

    // 读入边信息，构建无向图
    for (int i = 1; i < n; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        addEdge(u, v, w);
        addEdge(v, u, w);
    }

    road();      // 找直径
    distance();  // 计算分支距离
    cout << compute() << endl;  // 输出最优解

    return 0;
}
