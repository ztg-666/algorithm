#include <iostream>
#include <vector>
#include <cstring>
#include <climits>
#include <queue>

// 小k的农场
// 一共有n个农场，编号1~n，给定m条关系，每条关系是如下三种形式中的一种
// 关系1 a b c : 表示农场a比农场b至少多种植了c个作物
// 关系2 a b c : 表示农场a比农场b至多多种植了c个作物
// 关系3 a b   : 表示农场a和农场b种植了一样多的作物
// 如果关系之间能推出矛盾，打印"No"，不存在矛盾，打印"Yes"
// 1 <= n、m <= 5 * 10^3
// 1 <= c <= 5 * 10^3
// 测试链接 : https://www.luogu.com.cn/problem/P1993

using namespace std;

const int MAXN = 5001;       // 最大节点数
const int MAXM = 20001;      // 最大边数
const int MAXQ = 20000001;   // 队列的最大大小

// 图的边的存储
int head[MAXN], nxt[MAXM], to[MAXM], weight[MAXM];
// dist[] 存储各个节点的最短路径
int dist[MAXN], update[MAXN], q[MAXQ];
// enter[] 判断节点是否已经在队列中
bool enter[MAXN];

// cnt 记录当前边的编号，h 和 t 分别是队列的头尾，n 为节点数，m 为边数
int cnt, h, t, n, m;

// 初始化各个变量
void prepare() {
    cnt = 1;
    h = t = 0;
    memset(head, 0, sizeof(head));  // head 数组清零
    memset(dist, 0x3f, sizeof(dist));  // dist 数组初始化为最大值（即无穷大）
    memset(update, 0, sizeof(update)); // update 数组清零
    memset(enter, false, sizeof(enter));  // enter 数组初始化为 false，表示没有节点在队列中
}

// 添加边 u -> v，权重为 w
void addEdge(int u, int v, int w) {
    nxt[cnt] = head[u];  // 当前边指向 u 之前的第一条边
    to[cnt] = v;         // 目标节点是 v
    weight[cnt] = w;     // 边的权重是 w
    head[u] = cnt++;     // 更新 u 的边列表，指向新边
}

// 使用 SPFA 算法判断是否存在负环
bool spfa(int s) {
    dist[s] = 0;  // 从源点 s 开始，源点的最短路径为 0
    update[s] = 1; // update 数组标记源点已经进入队列
    q[t++] = s;    // 将源点放入队列
    enter[s] = true;  // 标记源点已在队列中

    // SPFA 算法的主循环
    while (h < t) {
        int u = q[h++];   // 从队列中取出一个节点 u
        enter[u] = false; // 标记 u 已经出队
        // 遍历所有与 u 相连的边
        for (int ei = head[u]; ei > 0; ei = nxt[ei]) {
            int v = to[ei], w = weight[ei]; // 获取边的目标节点 v 和权重 w
            // 如果通过 u 到 v 的路径更短，更新 dist[v]
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                // 如果 v 不在队列中，加入队列
                if (!enter[v]) {
                    // 如果更新次数超过 n，说明图中有负环
                    if (++update[v] > n) {
                        return true;  // 发现负环
                    }
                    q[t++] = v;  // 将节点 v 加入队列
                    enter[v] = true;  // 标记 v 已经在队列中
                }
            }
        }
    }
    return false; // 如果没有负环，返回 false
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);  // 提高输入输出效率

    cin >> n >> m;  // 输入节点数 n 和边数 m
    prepare();      // 初始化图和相关数组

    // 0 号点作为超级源点，保证图的连通性
    for (int i = 1; i <= n; i++) {
        addEdge(0, i, 0);  // 给每个节点添加从超级源点 0 到其本身的边，权重为 0
    }

    // 输入所有的边
    for (int i = 1; i <= m; i++) {
        int type, u, v, w;
        cin >> type >> u >> v;  // 输入关系的类型和两个农场编号
        if (type == 1) {
            cin >> w;  // 关系1：农场a比农场b至少多种植了c个作物
            addEdge(u, v, -w);  // 转化为 u - v <= -w
        } else if (type == 2) {
            cin >> w;  // 关系2：农场a比农场b至多多种植了c个作物
            addEdge(v, u, w);  // 转化为 v - u <= w
        } else {
            addEdge(u, v, 0);  // 关系3：农场a和农场b种植了一样多的作物
            addEdge(v, u, 0);  // 双向边
        }
    }

    // 使用 SPFA 算法判断是否存在负环
    if (spfa(0)) {
        cout << "No\n";  // 如果存在负环，输出 No
    } else {
        cout << "Yes\n";  // 如果没有负环，输出 Yes
    }

    return 0;
}
