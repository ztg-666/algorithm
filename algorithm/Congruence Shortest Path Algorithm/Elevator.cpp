#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <cstring>

// 跳楼机
// 一座大楼一共有h层，楼层编号1~h，有如下四种移动方式
// 1, 向上移动x层
// 2, 向上移动y层
// 3, 向上移动z层
// 4, 回到1层
// 假设你正在第1层，请问大楼里有多少楼层你可以到达
// 1 <= h <= 2^63 - 1
// 1 <= x、y、z <= 10^5
// 测试链接 : https://www.luogu.com.cn/problem/P3403

using namespace std;

const int MAXN = 100001;  // 最大节点数量
const int MAXM = 200001;  // 最大边数量

long long h; // 题目中的参数 h
int x, y, z; // 模数 x 和两个跳跃长度 y, z

// 图的邻接表存储方式
vector<int> head(MAXN, 0);     // 每个节点的邻接表起始位置
vector<int> nxt(MAXM);         // 下一条边的索引
vector<int> to(MAXM);          // 边的终点
vector<long long> weight(MAXM);// 边的权值
int cnt = 1;                   // 边的计数器，初始为 1

// Dijkstra 算法需要的变量
vector<long long> dis(MAXN, LLONG_MAX); // 从起点到各节点的最短距离
vector<bool> visited(MAXN, false);      // 标记节点是否已访问

// 添加边函数
void addEdge(int u, int v, long long w) {
    nxt[cnt] = head[u];  // 指向上一个边的索引
    to[cnt] = v;         // 边的终点
    weight[cnt] = w;     // 边的权值
    head[u] = cnt++;     // 更新起点的边索引
}

// Dijkstra 最短路算法
void dijkstra() {
    // 小根堆，存储 (当前距离, 节点编号)
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> heap;
    heap.push({0, 0});   // 从节点 0 开始，距离为 0
    dis[0] = 0;

    while (!heap.empty()) {
        long long w = heap.top().first;
        int u = heap.top().second;
        heap.pop();

        // 如果已经访问过则跳过
        if (visited[u]) continue;
        visited[u] = true;

        // 遍历 u 的所有出边
        for (int ei = head[u]; ei > 0; ei = nxt[ei]) {
            int v = to[ei];
            // 如果可以更新更短路径，则更新并入堆
            if (!visited[v] && dis[v] > w + weight[ei]) {
                dis[v] = w + weight[ei];
                heap.push({dis[v], v});
            }
        }
    }
}

// 计算最终答案
long long compute() {
    dijkstra(); // 先跑一次最短路
    long long ans = 0;
    for (int i = 0; i < x; i++) {
        // 如果到节点 i 的最短距离不超过 h
        if (dis[i] <= h) {
            // 计算可以到达的数量
            ans += (h - dis[i]) / x + 1;
        }
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    cin >> h >> x >> y >> z;
    h--;  // 下标从 0 开始，调整 h

    // 初始化数组，仅对前 x 个节点清零
    fill(head.begin(), head.begin() + x, 0);
    fill(dis.begin(), dis.begin() + x, LLONG_MAX);
    fill(visited.begin(), visited.begin() + x, false);

    // 构建图
    for (int i = 0; i < x; i++) {
        addEdge(i, (i + y) % x, y); // 从 i 跳到 (i+y)%x，代价为 y
        addEdge(i, (i + z) % x, z); // 从 i 跳到 (i+z)%x，代价为 z
    }

    // 输出最终结果
    cout << compute() << "\n";

    return 0;
}
