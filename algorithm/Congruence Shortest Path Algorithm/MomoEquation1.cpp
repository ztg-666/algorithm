#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <cstring>


// 墨墨的等式(dijkstra算法)
// 一共有n种正数，每种数可以选择任意个，个数不能是负数
// 那么一定有某些数值可以由这些数字累加得到
// 请问在[l...r]范围上，有多少个数能被累加得到
// 0 <= n <= 12
// 0 <= 数值范围 <= 5 * 10^5
// 1 <= l <= r <= 10^12
// 测试链接 : https://www.luogu.com.cn/problem/P2371

using namespace std;

const int MAXN = 500001;  // 余数的最大可能范围（用于状态数）
const int MAXM = 5000001; // 边的最大可能数量

int n, x;       // n: 输入的数的种类数, x: 最小的非零数(同时也是模数)
long long l, r; // 查询的区间 [l, r]

// 图的存储（链式前向星）
vector<int> head(MAXN, 0);   // 每个节点的链表头
vector<int> nxt(MAXM);       // 下一条边的索引
vector<int> to(MAXM);        // 当前边指向的节点
vector<long long> weight(MAXM); // 当前边的权重（也就是选择的数值）
int cnt = 1;                 // 边的计数器，从1开始（因为0表示无边）

// Dijkstra 所需数据结构
priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> heap; // 小根堆
vector<long long> dist(MAXN, LLONG_MAX); // dist[i] 表示从源点(0)到节点 i 的最短距离
vector<bool> visited(MAXN, false);      // 标记节点是否已经处理过

// 初始化函数：重置图、堆、dist、visited 等
void prepare() {
    cnt = 1;
    heap = priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>>();
    fill(head.begin(), head.begin() + x, 0);       // 清空链表头
    fill(dist.begin(), dist.begin() + x, LLONG_MAX); // 初始化距离为无穷大
    fill(visited.begin(), visited.begin() + x, false); // 初始化 visited 数组
}

// 添加边：从 u 到 v，权重为 w
void addEdge(int u, int v, long long w) {
    nxt[cnt] = head[u];
    to[cnt] = v;
    weight[cnt] = w;
    head[u] = cnt++;
}

// Dijkstra 算法：计算从 0 到每个余数状态的最短距离
void dijkstra() {
    heap.push({0, 0});  // 从节点 0 开始，距离为 0
    dist[0] = 0;
    while (!heap.empty()) {
        auto cur = heap.top();
        heap.pop();
        int u = cur.second;     // 当前节点
        long long w = cur.first; // 当前距离
        if (visited[u]) continue; // 如果已经访问过，跳过
        visited[u] = true;

        // 遍历当前节点的所有邻边
        for (int ei = head[u], v; ei > 0; ei = nxt[ei]) {
            v = to[ei];
            if (!visited[v] && dist[v] > w + weight[ei]) {
                dist[v] = w + weight[ei];
                heap.push({dist[v], v});
            }
        }
    }
}

// 计算答案：统计 [l+1, r] 中能被表示的数的个数
long long compute() {
    dijkstra();
    long long ans = 0;
    // 遍历所有模 x 的余数
    for (int i = 0; i < x; i++) {
        if (r >= dist[i]) {
            // 如果 r ≥ 当前余数最短距离，则有 (r - dist[i]) / x + 1 个数满足条件
            ans += (r - dist[i]) / x + 1;
        }
        if (l >= dist[i]) {
            // 从 [0, l] 部分减去同样计算结果
            ans -= (l - dist[i]) / x + 1;
        }
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    cin >> n;
    cin >> l >> r;
    l--; // 按照题意，计算 [l+1, r]，所以先减去 1
    x = 0;

    // 读取 n 个数
    for (int i = 1, vi; i <= n; i++) {
        cin >> vi;
        if (vi != 0) { // 只处理非零的数
            if (x == 0) {
                // 初始化模数 x
                x = vi;
                prepare();
            } else {
                // 为每个状态添加一条边
                for (int j = 0; j < x; j++) {
                    addEdge(j, (j + vi) % x, vi);
                }
            }
        }
    }
    
    cout << compute() << "\n";
    return 0;
}
