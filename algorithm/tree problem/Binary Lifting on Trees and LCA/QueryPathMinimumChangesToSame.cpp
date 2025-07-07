#include <vector>
#include <cstring>
#include <algorithm>

// 边权相等的最小修改次数
// 一棵树有n个节点，编号0 ~ n-1，每条边(u,v,w)表示从u到v有一条权重为w的边
// 一共有m条查询，每条查询(a,b)表示，a到b的最短路径中把所有边变成一种值需要修改几条边
// 返回每条查询的查询结果
// 1 <= n <= 10^4
// 1 <= m <= 2 * 10^4
// 0 <= u、v、a、b < n
// 1 <= w <= 26
// 测试链接 : https://leetcode.cn/problems/minimum-edge-weight-equilibrium-queries-in-a-tree/


using namespace std;

class Solution {
public:
    static const int MAXN = 10001;  // 最大节点数
    static const int MAXM = 20001;  // 最大查询数
    static const int MAXW = 26;     // 最大权值（边权范围1-26）

    // 链式前向星建图 - 存储树的结构
    int headEdge[MAXN];      // 每个节点的第一条边的索引
    int edgeNext[MAXN << 1]; // 每条边的下一条边索引
    int edgeTo[MAXN << 1];   // 每条边的目标节点
    int edgeValue[MAXN << 1]; // 每条边的权值
    int tcnt;                // 边的计数器

    // weightCnt[i][w] : 从根节点到节点i的路径中，权值为w的边有多少条
    int weightCnt[MAXN][MAXW + 1];

    // 以下结构用于Tarjan算法的离线LCA查询
    int headQuery[MAXN];      // 每个节点的第一个查询的索引
    int queryNext[MAXM << 1]; // 每个查询的下一个查询索引
    int queryTo[MAXM << 1];   // 每个查询的目标节点
    int queryIndex[MAXM << 1]; // 每个查询的原始索引
    int qcnt;                 // 查询计数器

    bool visited[MAXN];       // 节点是否被访问过（用于Tarjan算法）
    int father[MAXN];         // 并查集的父节点
    int lca[MAXM];           // 存储每个查询的最低公共祖先

    /**
     * 主函数：计算树上路径的最小操作次数
     * @param n 节点数
     * @param edges 边的列表，每条边包含[u, v, w]
     * @param queries 查询列表，每个查询包含[u, v]
     * @return 每个查询的最小操作次数
     */
    vector<int> minOperationsQueries(int n, vector<vector<int>>& edges, vector<vector<int>>& queries) {
        // 初始化数据结构
        build(n);

        // 构建无向树
        for (auto& edge : edges) {
            addEdge(edge[0], edge[1], edge[2]);
            addEdge(edge[1], edge[0], edge[2]);
        }

        // 从根节点(0)开始DFS，统计每个节点到根的路径中各权值的边数
        dfs(0, 0, -1);

        // 添加所有查询到查询图中（每个查询添加两次，因为是双向的）
        int m = queries.size();
        for (int i = 0; i < m; i++) {
            addQuery(queries[i][0], queries[i][1], i);
            addQuery(queries[i][1], queries[i][0], i);
        }

        // 使用Tarjan算法批量计算所有查询的LCA
        tarjan(0, -1);

        // 计算每个查询的答案
        vector<int> ans(m);
        for (int i = 0, a, b, c; i < m; i++) {
            a = queries[i][0];  // 查询的起点
            b = queries[i][1];  // 查询的终点
            c = lca[i];         // 起点和终点的最低公共祖先

            int allCnt = 0;  // 路径上所有边的总数
            int maxCnt = 0;  // 路径上出现次数最多的权值的出现次数

            // 遍历所有可能的权值
            for (int w = 1, wcnt; w <= MAXW; w++) {
                // 利用LCA性质计算路径a到b上权值为w的边数
                // 路径a到b = 路径a到根 + 路径b到根 - 2*路径lca到根
                wcnt = weightCnt[a][w] + weightCnt[b][w] - 2 * weightCnt[c][w];
                maxCnt = max(maxCnt, wcnt);
                allCnt += wcnt;
            }

            // 最小操作次数 = 总边数 - 最多的权值边数
            // （保留最多的权值不变，其他都需要修改）
            ans[i] = allCnt - maxCnt;
        }
        return ans;
    }

    /**
     * 初始化所有数据结构
     * @param n 节点数
     */
    void build(int n) {
        tcnt = qcnt = 1;  // 边和查询的计数器从1开始（0表示空）
        memset(headEdge, 0, n * sizeof(int));
        memset(headQuery, 0, n * sizeof(int));
        memset(visited, false, n * sizeof(bool));

        // 初始化并查集
        for (int i = 0; i < n; i++) {
            father[i] = i;
        }
    }

    /**
     * 添加一条边到图中
     * @param u 起点
     * @param v 终点
     * @param w 权值
     */
    void addEdge(int u, int v, int w) {
        edgeNext[tcnt] = headEdge[u];  // 新边的下一条边指向原来的第一条边
        edgeTo[tcnt] = v;              // 边的目标节点
        edgeValue[tcnt] = w;           // 边的权值
        headEdge[u] = tcnt++;          // 更新节点u的第一条边
    }

    /**
     * DFS遍历树，统计从根节点到每个节点的路径中各权值的边数
     * @param u 当前节点
     * @param w 从父节点到当前节点的边权值
     * @param f 父节点
     */
    void dfs(int u, int w, int f) {
        if (u == 0) {
            // 根节点初始化
            memset(weightCnt[u], 0, (MAXW + 1) * sizeof(int));
        } else {
            // 继承父节点的权值计数
            for (int i = 1; i <= MAXW; i++) {
                weightCnt[u][i] = weightCnt[f][i];
            }
            // 加上从父节点到当前节点的边权值
            weightCnt[u][w]++;
        }

        // 遍历所有子节点
        for (int e = headEdge[u]; e != 0; e = edgeNext[e]) {
            if (edgeTo[e] != f) {  // 避免回到父节点
                dfs(edgeTo[e], edgeValue[e], u);
            }
        }
    }

    /**
     * 添加一个查询到查询图中
     * @param u 查询的一个节点
     * @param v 查询的另一个节点
     * @param i 查询的索引
     */
    void addQuery(int u, int v, int i) {
        queryNext[qcnt] = headQuery[u];
        queryTo[qcnt] = v;
        queryIndex[qcnt] = i;
        headQuery[u] = qcnt++;
    }

    /**
     * Tarjan算法批量查询两点的最低公共祖先
     * @param u 当前节点
     * @param f 父节点
     */
    void tarjan(int u, int f) {
        visited[u] = true;  // 标记当前节点已访问

        // 先递归处理所有子节点
        for (int e = headEdge[u]; e != 0; e = edgeNext[e]) {
            if (edgeTo[e] != f) {
                tarjan(edgeTo[e], u);
            }
        }

        // 处理与当前节点相关的所有查询
        for (int e = headQuery[u], v; e != 0; e = queryNext[e]) {
            v = queryTo[e];
            if (visited[v]) {  // 如果查询的另一个节点已经访问过
                lca[queryIndex[e]] = find(v);  // 找到LCA
            }
        }

        // 将当前节点合并到父节点的集合中
        father[u] = f;
    }

    /**
     * 并查集的查找操作（带路径压缩）
     * @param i 要查找的节点
     * @return 节点i所在集合的根节点
     */
    int find(int i) {
        if (i != father[i]) {
            father[i] = find(father[i]);  // 路径压缩
        }
        return father[i];
    }
};
