#include <iostream>
#include <vector>
#include <climits>
#include <cstring>

// 最大深度和(递归版)
// 给定一棵n个点的树，找到一个节点，使得以这个节点为根时，到达所有节点的深度之和最大
// 如果有多个节点满足要求，返回节点编号最小的
// 测试链接 : https://www.luogu.com.cn/problem/P3478

using namespace std;

const int MAXN = 1000001;  // 最大节点数

// 全局变量
int n;                      // 节点数量
int head[MAXN];            // 邻接表头指针
int next_edge[MAXN << 1];  // 邻接表下一条边指针
int to[MAXN << 1];         // 邻接表边的终点
int cnt;                   // 边的计数器
int size_node[MAXN];       // 每个节点的子树大小
long long sum_node[MAXN];  // 每个节点的子树深度和
long long dp[MAXN];        // 每个节点作为根时的总深度和

// 初始化邻接表
void build() {
    cnt = 1;                           // 边计数器从1开始
    memset(head, 0, sizeof(head));     // 清空头指针数组
}

// 添加一条无向边
void addEdge(int u, int v) {
    next_edge[cnt] = head[u];  // 新边指向原来的第一条边
    to[cnt] = v;               // 新边的终点是v
    head[u] = cnt++;           // 更新u的第一条边为新边，计数器加1
}

// 第一次DFS：计算每个节点的子树大小和子树深度和
void dfs1(int u, int f) {
    // 先递归处理所有子节点
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        int v = to[e];
        if (v != f) {  // 避免回到父节点
            dfs1(v, u);
        }
    }

    // 初始化当前节点
    size_node[u] = 1;     // 当前节点本身算1个节点
    sum_node[u] = 0;      // 初始深度和为0

    // 累加所有子节点的贡献
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        int v = to[e];
        if (v != f) {
            size_node[u] += size_node[v];                    // 子树大小累加
            sum_node[u] += sum_node[v] + size_node[v];       // 深度和累加
            // sum_node[v] + size_node[v] 表示：
            // - sum_node[v]：子树v内部的深度和
            // - size_node[v]：子树v中每个节点深度都要+1（因为到u的距离增加1）
        }
    }
}

// 第二次DFS：计算每个节点作为根时的总深度和
void dfs2(int u, int f) {
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        int v = to[e];
        if (v != f) {
            // 换根DP的状态转移方程
            // dp[v] = dp[u] - size_node[v] + (n - size_node[v])
            // 解释：
            // - dp[u]：u作为根时的总深度和
            // - size_node[v]：子树v的大小，这些节点在v作为根时深度减1
            // - (n - size_node[v])：除了子树v外的其他节点，深度增加1
            dp[v] = dp[u] - size_node[v] + n - size_node[v];
            dfs2(v, u);  // 递归处理子节点
        }
    }
}

int main() {
    // 优化输入输出速度
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 读入节点数量
    cin >> n;
    build();

    // 读入n-1条边，构建树
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        addEdge(u, v);  // 添加边u->v
        addEdge(v, u);  // 添加边v->u（无向图）
    }

    // 第一次DFS：以节点1为根计算子树信息
    dfs1(1, 0);

    // 初始化dp[1]：节点1作为根时的总深度和
    dp[1] = sum_node[1];

    // 第二次DFS：换根DP计算所有节点作为根时的总深度和
    dfs2(1, 0);

    // 找到使总深度和最大的节点
    long long max_val = LLONG_MIN;
    int ans = 0;
    for (int i = 1; i <= n; i++) {
        if (dp[i] > max_val) {
            max_val = dp[i];
            ans = i;
        }
    }

    // 输出答案（如果有多个最优解，会自动选择编号最小的）
    cout << ans << endl;

    return 0;
}
