#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>


// 染色的最大收益
// 给定一棵n个点的树，初始时所有节点全是白点
// 第一次操作，你可以选择任意点染黑
// 以后每次操作，必须选择已经染黑的点的相邻点继续染黑，一直到所有的点都被染完
// 每次都获得，当前被染色点的白色连通块大小，作为收益
// 返回可获得的最大收益和
// 测试链接 : https://www.luogu.com.cn/problem/CF1187E

using namespace std;

const int MAXN = 200001;  // 最大节点数

// 全局变量
int n;                    // 树的节点数
int head[MAXN];          // 邻接表头节点数组
int next_edge[MAXN << 1]; // 邻接表的next指针数组
int to[MAXN << 1];       // 邻接表的目标节点数组
int cnt;                 // 边的计数器
int size_arr[MAXN];      // 每个节点的子树大小
long long dp[MAXN];      // 动态规划数组

/**
 * 初始化数据结构
 */
void build() {
    cnt = 1;  // 边计数器从1开始，0表示空
    // 清空邻接表头节点数组
    memset(head + 1, 0, n * sizeof(int));
    // 清空子树大小数组
    memset(size_arr + 1, 0, n * sizeof(int));
    // 清空dp数组
    memset(dp + 1, 0, n * sizeof(long long));
}

/**
 * 添加无向边 u-v
 * @param u 起始节点
 * @param v 目标节点
 */
void addEdge(int u, int v) {
    next_edge[cnt] = head[u];  // 新边的next指向当前头节点
    to[cnt] = v;               // 新边指向节点v
    head[u] = cnt++;           // 更新头节点为新边，计数器递增
}

/**
 * 第一次DFS：计算每个节点作为子树根节点时的最大收益
 * dp[i] = 节点i作为自己这棵子树最先染的点，染完子树后的收益
 * @param u 当前节点
 * @param f 父节点（避免回溯）
 */
void dfs1(int u, int f) {
    size_arr[u] = 1;  // 当前节点的子树大小至少为1（自己）

    // 遍历当前节点的所有邻接节点
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        int v = to[e];
        if (v != f) {  // 不是父节点，避免回溯
            dfs1(v, u);                    // 递归处理子节点
            size_arr[u] += size_arr[v];    // 累加子树大小
            dp[u] += dp[v];                // 累加子树的收益
        }
    }

    // 当前节点被染色时，获得的收益是整个子树的大小
    dp[u] += size_arr[u];
}

/**
 * 第二次DFS：换根DP，计算每个节点作为整棵树根节点时的最大收益
 * dp[i] = 节点i作为整棵树最先染的点，染完整棵树后的收益
 * @param u 当前节点
 * @param f 父节点（避免回溯）
 */
void dfs2(int u, int f) {
    // 遍历当前节点的所有邻接节点
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        int v = to[e];
        if (v != f) {  // 不是父节点，避免回溯
            // 换根DP的核心公式：
            // dp[v] = dp[u] + (n - size[v]) - size[v]
            // 解释：当v作为根时，原本u的子树变成了v的"上方"部分
            // 增加收益：n - size[v]（v上方的节点数）
            // 减少收益：size[v]（v原本的子树在计算中被重复计算了）
            dp[v] = dp[u] + n - size_arr[v] - size_arr[v];
            dfs2(v, u);  // 递归处理子节点
        }
    }
}

int main() {
    // 优化输入输出速度
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;    // 读入节点数
    build();     // 初始化数据结构

    // 读入n-1条边，构建树
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        addEdge(u, v);  // 添加边u->v
        addEdge(v, u);  // 添加边v->u（无向图）
    }

    // 第一次DFS：以节点1为根，计算每个子树的收益
    dfs1(1, 0);

    // 第二次DFS：换根DP，计算每个节点作为根时的收益
    dfs2(1, 0);

    // 找出所有节点作为起始点时的最大收益
    long long ans = 0;
    for (int i = 1; i <= n; i++) {
        ans = max(ans, dp[i]);
    }

    cout << ans << endl;  // 输出答案

    return 0;
}
