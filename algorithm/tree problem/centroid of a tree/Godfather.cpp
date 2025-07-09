#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>


// 教父
// 一共有n个节点，编号1~n，有n-1条边形成一棵树
// 返回所有重心点
// 树的重心第二种求解方式
// 以某个节点为根时，每颗子树的节点数不超过总节点数的一半，那么这个节点是重心
// 测试链接 : http://poj.org/problem?id=3107

using namespace std;

const int MAXN = 50001;

// 全局变量定义
int n;                    // 节点数
int head[MAXN];          // 邻接表的头节点数组
int next_edge[MAXN << 1]; // 邻接表中每条边的下一条边
int to[MAXN << 1];       // 邻接表中每条边的终点
int cnt;                 // 边的计数器
int size_node[MAXN];     // 每个节点为根的子树大小
int maxsub[MAXN];        // 每个节点的最大子树大小

// 初始化邻接表
void build() {
    cnt = 1;  // 从1开始计数（0表示空边）
    memset(head, 0, sizeof(head));  // 初始化头节点数组
}

// 添加无向边：在邻接表中添加从u到v的边
void addEdge(int u, int v) {
    next_edge[cnt] = head[u];  // 新边的下一条边指向原来的第一条边
    to[cnt] = v;               // 新边的终点是v
    head[u] = cnt++;           // 更新u的第一条边为新边，并递增计数器
}

// 深度优先搜索，计算子树大小和最大子树大小
void dfs(int u, int f) {
    size_node[u] = 1;  // 当前节点本身算作1个节点
    maxsub[u] = 0;     // 初始化最大子树大小为0

    // 遍历当前节点u的所有邻接边
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        int v = to[e];  // 获取边的终点
        if (v != f) {   // 如果不是父节点（避免重复访问）
            dfs(v, u);  // 递归处理子节点
            size_node[u] += size_node[v];  // 累加子树大小
            maxsub[u] = max(maxsub[u], size_node[v]);  // 更新最大子树大小
        }
    }
    // 考虑"向上"的部分（除了当前子树外的其他部分）
    maxsub[u] = max(maxsub[u], n - size_node[u]);
}

int main() {
    cin >> n;  // 读入节点数
    build();   // 初始化邻接表

    // 读入n-1条边，构建树
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        addEdge(u, v);  // 添加u->v的边
        addEdge(v, u);  // 添加v->u的边（无向图）
    }

    // 从节点1开始DFS，计算所有节点的子树信息
    dfs(1, 0);

    // 寻找重心：最大子树大小不超过n/2的节点
    int m = 0;        // 重心的数量
    int centers[2];   // 存储重心（最多2个）
    for (int i = 1; i <= n; i++) {
        if (maxsub[i] <= n / 2) {  // 如果最大子树大小不超过总节点数的一半
            centers[m++] = i;       // 该节点是重心
        }
    }

    // 输出结果
    if (m == 1) {
        cout << centers[0] << endl;  // 只有一个重心
    } else { // m == 2
        cout << centers[0] << " " << centers[1] << endl;  // 有两个重心
    }

    return 0;
}
