#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cstring>

// 平衡行为
// 一共有n个节点，编号1~n，有n-1条边形成一棵树
// 返回重心点，返回重心点最大子树的节点数
// 树的重心第一种求解方式
// 以某个节点为根时，最大子树的节点数最少，那么这个节点是重心
// 测试链接 : http://poj.org/problem?id=1655

using namespace std;

const int MAXN = 20001;  // 最大节点数

int n;                    // 当前树的节点数
int head[MAXN];          // 邻接表头指针数组
int next_edge[MAXN << 1]; // 邻接表中下一条边的指针
int to[MAXN << 1];       // 邻接表中边的终点
int cnt;                 // 边的计数器
int size_node[MAXN];     // 以每个节点为根的子树大小
int center;              // 重心节点
int best;                // 重心的最大子树大小

// 初始化函数
void build() {
    cnt = 1;  // 边的编号从1开始（0用作空指针）
    memset(head + 1, 0, sizeof(int) * n);  // 清空邻接表头指针
    best = INT_MAX;  // 初始化最优值为最大值
}

// 添加边的函数，构建无向图的邻接表
void addEdge(int u, int v) {
    next_edge[cnt] = head[u];  // 新边指向u的原有边链表
    to[cnt] = v;               // 新边的终点是v
    head[u] = cnt++;           // u的头指针指向新边，边计数器自增
}

// 深度优先搜索函数
// u: 当前访问的节点
// f: 父节点（用于避免回溯）
void dfs(int u, int f) {
    size_node[u] = 1;  // 初始化当前节点的子树大小为1（包含自己）

    // 以当前节点u为根时，其所有子树中最大的子树节点数
    int maxsub = 0;

    // 遍历u的所有邻接边
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        int v = to[e];  // 获取边的终点
        if (v != f) {   // 如果不是父节点（避免回溯）
            dfs(v, u);  // 递归访问子节点
            size_node[u] += size_node[v];  // 累加子树大小
            maxsub = max(maxsub, size_node[v]);  // 更新最大子树大小
        }
    }

    // 考虑"向上"的部分（除了以u为根的子树外，剩余的部分）
    // 这部分的大小是 n - size_node[u]
    maxsub = max(maxsub, n - size_node[u]);

    // 更新重心
    // 题目要求找到编号最小的重心，所以当最大子树大小相等时，选择编号更小的节点
    if (maxsub < best || (maxsub == best && u < center)) {
        best = maxsub;  // 更新最优的最大子树大小
        center = u;     // 更新重心节点
    }
}

int main() {
    ios::sync_with_stdio(false);  // 加速输入输出
    cin.tie(nullptr);

    int testCase;  // 测试用例数量
    cin >> testCase;

    // 处理每个测试用例
    for (int t = 1; t <= testCase; t++) {
        cin >> n;  // 读入节点数
        build();   // 初始化数据结构

        // 读入n-1条边，构建树
        for (int i = 1; i < n; i++) {
            int u, v;
            cin >> u >> v;
            addEdge(u, v);  // 添加边u->v
            addEdge(v, u);  // 添加边v->u（无向图）
        }

        dfs(1, 0);  // 从节点1开始深度优先搜索，父节点设为0
        cout << center << " " << best << "\n";  // 输出重心节点和其最大子树大小
    }

    return 0;
}
