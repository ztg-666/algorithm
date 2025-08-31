#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

// 聚会后送每个人回家最短用时
// 给定一棵n个点的树，边权代表走过边需要花费的时间
// 给定k个人分别在树上的哪些节点
// 这k个人选择了一个聚会点，所有的人都去往聚会点进行聚会
// 聚会结束后，所有人都会上一辆车，车会把每个人送回家
// 送完最后一个乘客，车不需要回到聚会点
// 如果聚会点在i，请问从聚会地点出发直到送最后一个人回家，最短用时多少
// i = 1 ~ n，打印所有的答案
// 测试链接 : https://www.luogu.com.cn/problem/P6419

using namespace std;

const int MAXN = 500001;

int n, k;  // n: 节点数, k: 乘客数

// 邻接表存储树的结构
int head[MAXN];           // head[i]: 节点i的第一条边的编号
int next_edge[MAXN << 1]; // next_edge[i]: 编号为i的边的下一条边编号
int to[MAXN << 1];        // to[i]: 编号为i的边指向的节点
int weight[MAXN << 1];    // weight[i]: 编号为i的边的权重
int cnt;                  // 边的编号计数器

// 核心数据结构
int people[MAXN];         // people[i]: 节点i子树中有多少乘客需要送回家
long long incost[MAXN];   // incost[i]: 从节点i出发送完i子树内所有乘客并回到i的最少代价
long long inner1[MAXN];   // inner1[i]: 从节点i出发送i子树内乘客的最长链（不需要回来）
long long inner2[MAXN];   // inner2[i]: 从节点i出发送i子树内乘客的次长链（不需要回来）

// 注意：inner1[i]和inner2[i]所代表的链，一定要来自i的不同儿子

int choose[MAXN];         // choose[i]: 送乘客的最长链来自i的哪个儿子
long long outcost[MAXN];  // outcost[i]: 从节点i出发送完i子树外所有乘客并回到i的最少代价
long long outer[MAXN];    // outer[i]: 从节点i出发送i子树外乘客的最长链（不需要回来）

// 初始化所有数组
void build() {
    cnt = 1;  // 边编号从1开始（0表示没有边）
    memset(head, 0, sizeof(head));
    memset(people, 0, sizeof(people));
    memset(incost, 0, sizeof(incost));
    memset(inner1, 0, sizeof(inner1));
    memset(inner2, 0, sizeof(inner2));
    memset(choose, 0, sizeof(choose));
    memset(outcost, 0, sizeof(outcost));
    memset(outer, 0, sizeof(outer));
}

// 添加无向边：u <-> v，权重为w
void addEdge(int u, int v, int w) {
    next_edge[cnt] = head[u];  // 新边的下一条边是原来的第一条边
    to[cnt] = v;               // 新边指向节点v
    weight[cnt] = w;           // 新边权重为w
    head[u] = cnt++;           // 更新节点u的第一条边为新边，边编号递增
}

// 第一次DFS：计算每个节点子树内的信息
void dfs1(int u, int f) {  // u: 当前节点, f: 父节点
    // 遍历当前节点u的所有邻边
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        int v = to[e];      // 邻接节点
        int w = weight[e];  // 边权重

        if (v != f) {  // 如果不是父节点，则是子节点
            dfs1(v, u);  // 递归处理子节点

            // 累加子树中的乘客数量
            people[u] += people[v];

            // 如果子节点v的子树中有乘客
            if (people[v] > 0) {
                // 累加子树内送乘客的代价
                // 需要往返，所以是 w * 2
                incost[u] += incost[v] + (long long)w * 2;

                // 更新最长链和次长链
                if (inner1[u] < inner1[v] + w) {
                    // 发现更长的链，更新最长链和次长链
                    choose[u] = v;                    // 记录最长链来自哪个儿子
                    inner2[u] = inner1[u];           // 原最长链变为次长链
                    inner1[u] = inner1[v] + w;       // 更新最长链
                } else if (inner2[u] < inner1[v] + w) {
                    // 更新次长链
                    inner2[u] = inner1[v] + w;
                }
            }
        }
    }
}

// 第二次DFS：计算每个节点子树外的信息
void dfs2(int u, int f) {  // u: 当前节点, f: 父节点
    // 遍历当前节点u的所有邻边
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        int v = to[e];      // 邻接节点
        int w = weight[e];  // 边权重

        if (v != f) {  // 如果不是父节点，则是子节点
            // 如果v的子树外有乘客需要送
            if (k - people[v] > 0) {
                // 计算从v出发送完子树外所有乘客并回到v的代价
                if (people[v] == 0) {
                    // 如果v的子树内没有乘客，直接累加
                    outcost[v] = outcost[u] + incost[u] + (long long)w * 2;
                } else {
                    // 如果v的子树内有乘客，要减去重复计算的部分
                    outcost[v] = outcost[u] + incost[u] - incost[v];
                }

                // 计算从v出发送子树外乘客的最长链
                if (v != choose[u]) {
                    // 如果v不是u的最长链所在的儿子，可以使用u的最长链
                    outer[v] = max(outer[u], inner1[u]) + w;
                } else {
                    // 如果v是u的最长链所在的儿子，只能使用u的次长链
                    outer[v] = max(outer[u], inner2[u]) + w;
                }
            }

            dfs2(v, u);  // 递归处理子节点
        }
    }
}

int main() {
    ios::sync_with_stdio(false);  // 关闭C++与C的同步，提高输入输出速度
    cin.tie(nullptr);             // 取消cin与cout的绑定，提高性能

    cin >> n;  // 读入节点数
    build();   // 初始化
    cin >> k;  // 读入乘客数

    // 读入n-1条边，构建树
    for (int i = 1; i < n; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        addEdge(u, v, w);  // 添加边 u -> v
        addEdge(v, u, w);  // 添加边 v -> u（无向边）
    }

    // 读入k个乘客的初始位置
    for (int i = 1; i <= k; i++) {
        int u;
        cin >> u;
        people[u]++;  // 节点u的乘客数量加1
    }

    // 执行两次DFS
    dfs1(1, 0);  // 第一次DFS：计算子树内信息
    dfs2(1, 0);  // 第二次DFS：计算子树外信息

    // 输出每个节点作为聚会点时的最短送客时间
    for (int i = 1; i <= n; i++) {
        // 总代价 = 子树内代价 + 子树外代价 - 最长的一条链（最后一次不需要回来）
        cout << incost[i] + outcost[i] - max(inner1[i], outer[i]) << "\n";
    }

    return 0;
}
