#include <bits/stdc++.h>

// 牛场围栏
// 给定一个长度为n的数组arr, arr[i]代表第i种木棍的长度，每种木棍有无穷多个
// 给定一个正数m，表示你可以把任何一根木棍消去最多m的长度，同一种木棍可以消去不同的长度
// 你可以随意拼接木棍形成一个长度，返回不能拼出来的长度中，最大值是多少
// 如果你可以拼出所有的长度，返回-1
// 如果不能拼出来的长度有无穷多，返回-1
// 1 <= n <= 100
// 1 <= arr[i] <= 3000
// 1 <= m <= 3000
// 测试链接 : https://www.luogu.com.cn/problem/P2662

using namespace std;

// 常量定义
const int MAXN = 101;    // 木棍种类数最大值
const int MAXV = 3001;   // 节点数量上限 (最大余数范围)
const int MAXM = 30001;  // 边数上限
const int INF = INT_MAX; // 无穷大，用于初始化距离

// 输入变量
int n, m, x;
int arr[MAXN];
bool used[MAXV]; // 记录某个长度是否已经加入过图中，避免重复加边

// 链式前向星存储图
int head[MAXV], nxt[MAXM], to_[MAXM], weight[MAXM];
int cnt; // 当前边的编号

// Dijkstra 所需数据
int dist_[MAXV];      // 源点到每个点的最短距离
bool visited[MAXV];   // 标记节点是否已确定最短距离
priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> heap;
// 小根堆 (distance, node)

// 初始化图结构及 Dijkstra 辅助数据
void prepare() {
    cnt = 1; // 边编号从 1 开始
    while(!heap.empty()) heap.pop(); // 清空优先队列
    fill(used, used + MAXV, false);  // 标记数组重置
    fill(head, head + x, 0);         // 每个节点的链表头置 0
    fill(dist_, dist_ + x, INF);     // 距离初始化为无穷大
    fill(visited, visited + x, false); // 访问标记清零
}

// 链式前向星添加有向边 u -> v，权重为 w
void addEdge(int u, int v, int w) {
    nxt[cnt] = head[u];
    to_[cnt] = v;
    weight[cnt] = w;
    head[u] = cnt++;
}

// Dijkstra 算法求最短路径
void dijkstra() {
    heap.push({0, 0}); // 从节点 0 出发，距离为 0
    dist_[0] = 0;
    while (!heap.empty()) {
        auto [w, u] = heap.top();
        heap.pop();
        if (visited[u]) continue; // 如果已经确定最短路径，跳过
        visited[u] = true;
        // 遍历邻接边
        for (int ei = head[u]; ei > 0; ei = nxt[ei]) {
            int v = to_[ei];
            if (!visited[v] && dist_[v] > w + weight[ei]) {
                dist_[v] = w + weight[ei];
                heap.push({dist_[v], v});
            }
        }
    }
}

// 计算答案
int compute() {
    int ans = 0;
    // 如果最小可能的步长为 1，则所有长度都可以拼接出来
    if (x == 1) return -1;

    // 建图：将可用的长度添加为图中的边
    for (int i = 1; i <= n; i++) {
        // 每种木棍允许削减长度，范围 [arr[i] - m, arr[i]]
        for (int j = max(1, arr[i] - m); j <= arr[i]; j++) {
            if (!used[j]) { // 避免重复处理同一长度
                used[j] = true;
                // 建立从每个余数状态 k 到 (k + j) % x 的边
                for (int k = 0; k < x; k++) {
                    addEdge(k, (k + j) % x, j);
                }
            }
        }
    }

    // 最短路径计算
    dijkstra();

    // 遍历所有余数类，检查哪些长度不可达
    for (int i = 1; i < x; i++) {
        if (dist_[i] == INF) return -1; // 存在不可达，意味着无穷多长度无法拼接
        ans = max(ans, dist_[i] - x);   // 记录最大无法拼出的长度
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 输入 n 和 m
    cin >> n >> m;
    x = INF; // 初始化为极大值
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
        // x 取所有 (arr[i] - m) 中的最小值，且至少为 1
        x = min(x, max(1, arr[i] - m));
    }

    // 初始化数据结构
    prepare();
    // 输出结果
    cout << compute() << "\n";
    return 0;
}
