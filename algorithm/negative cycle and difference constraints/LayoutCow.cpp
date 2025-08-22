#include <bits/stdc++.h>

// 布局奶牛
// 编号1到编号n的奶牛从左往右站成一排，你可以决定任意相邻奶牛之间的距离
// 有m1条好友信息，有m2条情敌信息，好友间希望距离更近，情敌间希望距离更远
// 每条好友信息为 : u v w，表示希望u和v之间的距离 <= w，输入保证u < v
// 每条情敌信息为 : u v w，表示希望u和v之间的距离 >= w，输入保证u < v
// 你需要安排奶牛的布局，满足所有的好友信息和情敌信息
// 如果不存在合法方案，返回-1
// 如果存在合法方案，返回1号奶牛和n号奶牛之间的最大距离
// 如果存在合法方案，并且1号奶牛和n号奶牛之间的距离可以无穷远，返回-2
// 测试链接 : https://www.luogu.com.cn/problem/P4878

using namespace std;

// ========================== 常量定义 ==========================
// 最大点数
const int MAXN = 1001;
// 最大边数（好友 + 情敌 + 超级源点 + 连续性约束）
const int MAXM = 20001;
// 队列最大长度（保证足够大）
const int MAXQ = 1000001;

// ========================== 图存储（链式前向星） ==========================
int head[MAXN];     // head[u] 存储 u 的第一条边下标
int nxt[MAXM];      // nxt[e] 表示边 e 的下一条边
int to[MAXM];       // to[e] 表示边 e 的目标点
int weight[MAXM];   // weight[e] 表示边 e 的权值
int cnt;            // 边的计数器

// ========================== SPFA 需要的数据结构 ==========================
int dist[MAXN];         // dist[i] = 源点到 i 的最短路估计值
int updateCnt[MAXN];    // updateCnt[i] = i 被更新的次数（用于检测负环）
int q[MAXQ];            // 手动维护的队列（避免 STL 超时）
bool inQueue[MAXN];     // 是否在队列里

// ========================== 输入数据 ==========================
int n, m1, m2;

// ========================== 函数部分 ==========================

// 初始化
void prepare() {
    cnt = 1; // 注意：边从 1 开始存储（方便判断 head[u]==0 表示无边）
    fill(head, head + n + 1, 0);
}

// 添加一条有向边 u -> v，权值为 w
void addEdge(int u, int v, int w) {
    nxt[cnt] = head[u];
    to[cnt] = v;
    weight[cnt] = w;
    head[u] = cnt++;
}

// SPFA 算法，求差分约束系统解
// 返回值：
//   -1 表示存在负环，无解
//   -2 表示 dist[n] == INF，即 1 到 n 不连通，答案为无穷大
//   其它值：表示 dist[n] 的最短路径长度
int spfa(int s) {
    int h = 0, t = 0; // 手动队列 [h, t)

    // 初始化
    fill(dist, dist + n + 1, INT_MAX);
    fill(updateCnt, updateCnt + n + 1, 0);
    fill(inQueue, inQueue + n + 1, false);

    // 源点入队
    dist[s] = 0;
    updateCnt[s] = 1;
    q[t++] = s;
    inQueue[s] = true;

    // 开始 SPFA
    while (h < t) {
        int u = q[h++];
        inQueue[u] = false;

        // 遍历 u 的所有出边
        for (int ei = head[u]; ei; ei = nxt[ei]) {
            int v = to[ei], w = weight[ei];

            // 松弛操作
            if (dist[u] != INT_MAX && dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                if (!inQueue[v]) {
                    // 检测负环：某个点更新次数超过 n 次，必然有负环
                    if (++updateCnt[v] > n) {
                        return -1; // 无解
                    }
                    q[t++] = v;
                    inQueue[v] = true;
                }
            }
        }
    }

    // 判断结果
    if (dist[n] == INT_MAX) {
        return -2; // 无穷大
    }
    return dist[n]; // 最短路径
}

// ========================== 主函数 ==========================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m1 >> m2;
    prepare();

    // 0 号点是超级源点，保证图连通性
    // 0 -> i 边权为 0
    for (int i = 1; i <= n; i++) {
        addEdge(0, i, 0);
    }

    // 好友关系 (u, v, w) => u 到 v 的距离 <= w
    // 转换成差分约束：dist[v] - dist[u] <= w
    // 即 u -> v 权值 w
    for (int i = 0; i < m1; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        addEdge(u, v, w);
    }

    // 情敌关系 (u, v, w) => v 到 u 的距离 <= -w
    // 即 dist[u] - dist[v] >= w
    // 转换成 dist[v] - dist[u] <= -w
    // 即 v -> u 权值 -w
    for (int i = 0; i < m2; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        addEdge(v, u, -w);
    }

    // 邻接关系：保证 i+1 和 i 的相对顺序
    // 即 dist[i] - dist[i+1] <= 0
    // 转换成 i+1 -> i 权值 0
    for (int i = 1; i < n; i++) {
        addEdge(i + 1, i, 0);
    }

    // 第一次跑 SPFA(0) 检查是否有负环
    int ans = spfa(0);
    if (ans == -1) {
        cout << -1 << "\n"; // 无解
    } else {
        // 第二次从 1 出发，求解 1 到 n 的最短路
        ans = spfa(1);
        cout << ans << "\n";
    }

    return 0;
}
