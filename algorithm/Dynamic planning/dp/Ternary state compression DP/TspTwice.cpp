#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cmath>


// 节点最多经过两次的tsp问题
// 给定有n个地点，用m条边无向边连接，每条边有权值
// 你可以任选一点出发，目标是经过所有的点，最终不必回到出发点
// 并且每个点最多可以到达两次
// 返回总路程最小是多少
// 1 <= n <= 10
// 1 <= m <= 100
// 测试链接 : https://acm.hdu.edu.cn/showproblem.php?pid=3001

using namespace std;

const int MAXN = 10;  // 最大节点数

int n, m;              // n: 节点数, m: 边数
int maxs;              // 3^n，表示所有可能的状态数
int graph[MAXN][MAXN]; // 邻接矩阵存储图
int dp[MAXN][59049];   // dp[i][s]: 从状态s到达节点i的最小代价，3^10 = 59049
vector<int> complete;  // 存储所有完整访问状态（每个点至少访问1次）

// 初始化函数
void build() {
    // 初始化邻接矩阵为无穷大
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            graph[i][j] = INT_MAX;
        }
    }

    maxs = (int)pow(3, n);  // 计算总状态数

    // 初始化dp数组为-1（表示未计算）
    for (int i = 0; i < n; i++) {
        for (int s = 0; s < maxs; s++) {
            dp[i][s] = -1;
        }
    }

    complete.clear();  // 清空完整状态数组
}

// 深度优先搜索生成所有完整访问状态
// i: 当前处理的节点索引
// bit: 当前节点在三进制状态中的位权（1, 3, 9, 27, ...）
// s: 当前构建的状态
void dfs(int i, int bit, int s) {
    if (i == n) {
        // 已经处理完所有节点，将当前状态加入完整状态列表
        complete.push_back(s);
    } else {
        // 对于每个节点，有两种选择：访问1次或访问2次
        dfs(i + 1, bit * 3, s + bit);      // 节点i访问1次
        dfs(i + 1, bit * 3, s + 2 * bit);  // 节点i访问2次
    }
}

// 记忆化搜索函数
// i: 当前所在的节点
// s: 当前剩余需要访问的状态（三进制表示）
// 返回：从状态s到达节点i的最小代价
int f(int i, int s) {
    if (s == 0) {
        // 所有节点都已访问完毕，代价为0
        return 0;
    }

    if (dp[i][s] != -1) {
        // 已经计算过，直接返回结果
        return dp[i][s];
    }

    int ans = INT_MAX;

    // 尝试从每个可能的前驱节点j转移到当前节点i
    for (int j = 0, bit = 1; j < n; j++, bit *= 3) {
        // 检查节点j在状态s中是否还需要访问
        if ((s / bit) % 3 > 0) {
            // 递归计算从节点j出发的最小代价
            int pre = f(j, s - bit);

            // 如果存在可行路径且j到i有边
            if (pre != INT_MAX && graph[j][i] != INT_MAX) {
                ans = min(ans, pre + graph[j][i]);
            }
        }
    }

    dp[i][s] = ans;  // 记忆化存储
    return ans;
}

// 计算最终答案
int compute() {
    int ans = INT_MAX;

    // 遍历所有完整访问状态
    for (int k = 0; k < complete.size(); k++) {
        // 尝试从每个节点作为终点
        for (int i = 0, bit = 1; i < n; i++, bit *= 3) {
            // complete[k] - bit 表示到达节点i之前的状态
            ans = min(ans, f(i, complete[k] - bit));
        }
    }

    return ans;
}

int main() {
    ios::sync_with_stdio(false);  // 加速输入输出
    cin.tie(nullptr);

    // 处理多组测试数据
    while (cin >> n >> m) {
        build();        // 初始化
        dfs(0, 1, 0);   // 生成所有完整状态

        // 读取边信息
        for (int i = 0; i < m; i++) {
            int u, v, w;
            cin >> u >> v >> w;
            u--; v--;  // 转换为0-based索引

            // 如果发现更短的边，更新邻接矩阵（处理重边）
            if (w < graph[u][v]) {
                graph[u][v] = graph[v][u] = w;  // 无向图
            }
        }

        // 计算并输出答案
        int ans = compute();
        cout << (ans == INT_MAX ? -1 : ans) << "\n";
    }
    
    return 0;
}
