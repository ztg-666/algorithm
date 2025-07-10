#include <vector>
#include <algorithm>
#include <climits>
#include <cmath>

// 最小化旅行的价格总和(倍增方法求lca)
// 有n个节点形成一棵树，每个节点上有点权，再给定很多路径
// 每条路径有开始点和结束点，路径代价就是从开始点到结束点的点权和
// 所有路径的代价总和就是旅行的价格总和
// 你可以选择把某些点的点权减少一半，来降低旅行的价格总和
// 但是要求选择的点不能相邻
// 返回旅行的价格总和最少能是多少
// 测试链接 : https://leetcode.cn/problems/minimize-the-total-price-of-the-trips/

using namespace std;

class Solution {
private:
    static const int MAXN = 51;        // 最大节点数
    static const int LIMIT = 6;        // 倍增表的最大层数

    int power;                         // log2(n)，用于倍增
    int price[MAXN];                   // 每个节点的价格
    int num[MAXN];                     // 每个节点被经过的次数
    int head[MAXN];                    // 邻接表头指针
    int next[MAXN << 1];               // 邻接表next指针
    int to[MAXN << 1];                 // 邻接表to指针
    int cnt;                           // 边的计数器
    int deep[MAXN];                    // 每个节点的深度
    int stjump[MAXN][LIMIT];           // 倍增表，stjump[i][j]表示节点i向上跳2^j步到达的节点
    int no, yes;                       // 树形DP的状态：no表示当前节点不减半，yes表示当前节点减半

    // 计算log2(n)
    int log2(int n) {
        int ans = 0;
        while ((1 << ans) <= (n >> 1)) {
            ans++;
        }
        return ans;
    }

    // 初始化数据结构
    void build(int n) {
        power = log2(n);                           // 计算倍增表的层数
        fill(num + 1, num + n + 1, 0);           // 初始化经过次数数组
        cnt = 1;                                  // 边计数器从1开始
        fill(head + 1, head + n + 1, 0);         // 初始化邻接表头指针
    }

    // 添加边到邻接表
    void addEdge(int u, int v) {
        next[cnt] = head[u];                      // 新边的next指向原来的第一条边
        to[cnt] = v;                              // 新边指向节点v
        head[u] = cnt++;                          // 更新头指针并递增计数器
    }

    // 第一次DFS：构建倍增表，计算每个节点的深度和祖先信息
    void dfs1(int u, int f) {
        deep[u] = deep[f] + 1;                    // 当前节点深度 = 父节点深度 + 1
        stjump[u][0] = f;                         // 节点u向上跳1步到达父节点f

        // 构建倍增表：stjump[u][p] = stjump[stjump[u][p-1]][p-1]
        for (int p = 1; p <= power; p++) {
            stjump[u][p] = stjump[stjump[u][p - 1]][p - 1];
        }

        // 遍历所有子节点
        for (int e = head[u]; e != 0; e = next[e]) {
            if (to[e] != f) {                     // 不回到父节点
                dfs1(to[e], u);
            }
        }
    }

    // 使用倍增法求最近公共祖先(LCA)
    int lca(int a, int b) {
        // 确保a的深度 >= b的深度
        if (deep[a] < deep[b]) {
            swap(a, b);
        }

        // 将a上移到与b相同的深度
        for (int p = power; p >= 0; p--) {
            if (deep[stjump[a][p]] >= deep[b]) {
                a = stjump[a][p];
            }
        }

        // 如果a和b相同，直接返回
        if (a == b) {
            return a;
        }

        // 同时上移a和b，直到它们的父节点相同
        for (int p = power; p >= 0; p--) {
            if (stjump[a][p] != stjump[b][p]) {
                a = stjump[a][p];
                b = stjump[b][p];
            }
        }

        return stjump[a][0];                      // 返回公共祖先
    }

    // 第二次DFS：计算每个节点被经过的次数
    void dfs2(int u, int f) {
        // 先递归处理所有子节点
        for (int e = head[u]; e != 0; e = next[e]) {
            int v = to[e];
            if (v != f) {                         // 不回到父节点
                dfs2(v, u);
            }
        }

        // 将所有子节点的经过次数累加到当前节点
        for (int e = head[u]; e != 0; e = next[e]) {
            int v = to[e];
            if (v != f) {
                num[u] += num[v];
            }
        }
    }

    // 树形DP：计算最小价格
    void dp(int u, int f) {
        // n: 当前节点不减半的价格
        // y: 当前节点减半的价格
        int n = price[u] * num[u];                // 不减半：原价格 * 经过次数
        int y = (price[u] / 2) * num[u];          // 减半：(原价格/2) * 经过次数

        // 遍历所有子节点
        for (int e = head[u]; e != 0; e = next[e]) {
            int v = to[e];
            if (v != f) {                         // 不回到父节点
                dp(v, u);                         // 递归处理子节点
                n += min(no, yes);                // 当前节点不减半，子节点可以选择减半或不减半
                y += no;                          // 当前节点减半，子节点不能减半（相邻节点不能同时减半）
            }
        }

        // 更新全局状态
        no = n;                                   // 当前节点不减半的最小价格
        yes = y;                                  // 当前节点减半的最小价格
    }

public:
    int minimumTotalPrice(int n, vector<vector<int>>& edges, vector<int>& prices, vector<vector<int>>& trips) {
        build(n);                                 // 初始化数据结构

        // 将价格数组从0索引转换为1索引
        for (int i = 0, j = 1; i < n; i++, j++) {
            price[j] = prices[i];
        }

        // 构建邻接表（将节点编号从0开始转换为从1开始）
        for (auto& edge : edges) {
            addEdge(edge[0] + 1, edge[1] + 1);
            addEdge(edge[1] + 1, edge[0] + 1);
        }

        dfs1(1, 0);                              // 构建倍增表

        // 处理每个旅行路径，计算每个节点被经过的次数
        int u, v, lca_node, lcafather;
        for (auto& trip : trips) {
            u = trip[0] + 1;                     // 起点（转换为1索引）
            v = trip[1] + 1;                     // 终点（转换为1索引）
            lca_node = lca(u, v);                // 求最近公共祖先
            lcafather = stjump[lca_node][0];     // LCA的父节点

            // 使用差分数组的思想：
            // 路径u到v会经过u到LCA的路径和v到LCA的路径
            // 但LCA会被重复计算，LCA的父节点需要减去多计算的部分
            num[u]++;                            // 起点+1
            num[v]++;                            // 终点+1
            num[lca_node]--;                     // LCA-1（因为被重复计算）
            num[lcafather]--;                    // LCA父节点-1（因为不在路径上）
        }

        dfs2(1, 0);                              // 计算每个节点的实际经过次数
        dp(1, 0);                                // 树形DP计算最小价格

        return min(no, yes);                     // 返回根节点选择减半或不减半的最小价格
    }
};
