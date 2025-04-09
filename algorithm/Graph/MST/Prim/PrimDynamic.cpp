#include <cstdio>
#include <vector>
#include <queue>
#include <functional>
using namespace std;
// Prim算法模版（洛谷）
// 动态空间实现
// 测试链接 : https://www.luogu.com.cn/problem/P3366
int main() {
    int n, m;
    scanf("%d%d", &n, &m); // 输入顶点数n和边数m

    // 构建邻接表 graph[0]弃用，节点从1开始
    vector<vector<pair<int, int>>> graph(n + 1); // pair<相邻节点, 边权重>
    for (int i = 0, u, v, w; i < m; i++) {
        scanf("%d%d%d", &u, &v, &w); // 输入每条边的起点u、终点v和权重w
        graph[u].emplace_back(v, w); // 无向图添加双向边
        graph[v].emplace_back(u, w);
    }

    // 优先队列（最小堆）存储pair<权重, 节点>
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> heap;
    vector<bool> visited(n + 1, false); // 访问标记数组，初始化为false

    // 初始化：从节点1开始扩展
    for (auto& edge : graph[1]) {
        heap.emplace(edge.second, edge.first); // 注意权重在前，节点在后
    }
    visited[1] = true; // 标记节点1为已访问
    int nodeCnt = 1, ans = 0; // nodeCnt记录已访问节点数，ans记录最小生成树的总权重

    while (!heap.empty()) {
        auto [cost, u] = heap.top(); // C++17结构化绑定，获取堆顶元素
        heap.pop(); // 弹出堆顶元素

        if (!visited[u]) { // 如果节点u未被访问过
            visited[u] = true; // 标记节点u为已访问
            nodeCnt++; // 已访问节点数加1
            ans += cost; // 累加边的权重到总权重ans

            // 将新节点的所有邻接边加入堆
            for (auto& [v, w] : graph[u]) { // C++17结构化绑定
                if (!visited[v]) { // 如果邻接节点v未被访问过
                    heap.emplace(w, v); // 将边加入堆
                }
            }
        }
    }

    // 如果已访问节点数等于n，输出最小生成树的总权重；否则输出"orz"
    printf(nodeCnt == n ? "%d" : "orz", ans);
    return 0;
}
