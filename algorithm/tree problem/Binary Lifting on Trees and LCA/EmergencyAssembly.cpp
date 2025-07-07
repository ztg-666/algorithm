#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstdio>

// 紧急集合
// 一共有n个节点，编号1 ~ n，一定有n-1条边连接形成一颗树
// 从一个点到另一个点的路径上有几条边，就需要耗费几个金币
// 每条查询(a, b, c)表示有三个人分别站在a、b、c点上
// 他们想集合在树上的某个点，并且想花费的金币总数最少
// 一共有m条查询，打印m个答案
// 1 <= n <= 5 * 10^5
// 1 <= m <= 5 * 10^5
// 测试链接 : https://www.luogu.com.cn/problem/P4281

using namespace std;

const int MAXN = 500001;  // 最大节点数
const int LIMIT = 19;     // 倍增的最大层数 (log2(500000) ≈ 19)

int power;                // 实际需要的倍增层数
int head[MAXN];          // 邻接表头指针
int edgeNext[MAXN << 1]; // 邻接表中边的下一条边
int edgeTo[MAXN << 1];   // 邻接表中边指向的节点
int cnt;                 // 边的计数器
int deep[MAXN];          // 每个节点的深度
int stjump[MAXN][LIMIT]; // 倍增跳跃表：stjump[i][j]表示从节点i向上跳2^j步到达的节点
int togather;            // 三人集合的最优位置
long long cost;          // 三人集合的最小总花费

// 计算log2(n)的值
int log2(int n) {
    int ans = 0;
    while ((1 << ans) <= (n >> 1)) {
        ans++;
    }
    return ans;
}

// 初始化图的结构
void build(int n) {
    power = log2(n);  // 计算倍增需要的层数
    cnt = 1;          // 边的编号从1开始
    fill(head, head + n + 1, 0);  // 初始化邻接表头指针
}

// 添加无向边
void addEdge(int u, int v) {
    edgeNext[cnt] = head[u];  // 新边的下一条边指向当前头边
    edgeTo[cnt] = v;          // 新边指向节点v
    head[u] = cnt++;          // 更新头指针为新边
}

// 深度优先搜索，构建倍增跳跃表
void dfs(int u, int f) {
    deep[u] = deep[f] + 1;    // 当前节点深度 = 父节点深度 + 1
    stjump[u][0] = f;         // 向上跳1步(2^0)到达父节点
    
    // 构建倍增跳跃表
    for (int p = 1; p <= power; p++) {
        // 从u向上跳2^p步 = 先跳2^(p-1)步，再跳2^(p-1)步
        stjump[u][p] = stjump[stjump[u][p - 1]][p - 1];
    }
    
    // 遍历所有相邻节点
    for (int e = head[u]; e != 0; e = edgeNext[e]) {
        if (edgeTo[e] != f) {  // 避免回到父节点
            dfs(edgeTo[e], u);
        }
    }
}

// 求两个节点的最近公共祖先(LCA)
int lca(int a, int b) {
    // 确保a的深度不小于b的深度
    if (deep[a] < deep[b]) swap(a, b);
    
    // 将a调整到与b相同的深度
    for (int p = power; p >= 0; p--) {
        if (deep[stjump[a][p]] >= deep[b]) {
            a = stjump[a][p];
        }
    }
    
    // 如果调整后a和b相等，则b就是LCA
    if (a == b) return a;
    
    // 同时向上跳跃，直到找到LCA
    for (int p = power; p >= 0; p--) {
        if (stjump[a][p] != stjump[b][p]) {
            a = stjump[a][p];
            b = stjump[b][p];
        }
    }
    
    return stjump[a][0];  // 返回LCA
}

// 计算三个人集合的最优位置和最小花费
void compute(int a, int b, int c) {
    // 求三对节点的LCA
    int h1 = lca(a, b);   // a和b的LCA
    int h2 = lca(a, c);   // a和c的LCA
    int h3 = lca(b, c);   // b和c的LCA
    
    // 找到三个LCA中深度最小的(最高的)和深度最大的(最低的)
    int high = h1 != h2 ? (deep[h1] < deep[h2] ? h1 : h2) : h1;
    int low = h1 != h2 ? (deep[h1] > deep[h2] ? h1 : h2) : h3;
    
    // 最优集合位置就是三个LCA中深度最大的那个
    togather = low;
    
    // 计算最小花费：三个人到集合点的距离之和
    // 距离 = 深度差，总花费 = (deep[a] + deep[b] + deep[c]) - 2*deep[high] - deep[low]
    cost = (long long)deep[a] + deep[b] + deep[c] - deep[high] * 2 - deep[low];
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    build(n);
    
    // 读入n-1条边，构建树
    for (int i = 1, u, v; i < n; i++) {
        scanf("%d %d", &u, &v);
        addEdge(u, v);
        addEdge(v, u);
    }
    
    // 从节点1开始DFS，构建倍增跳跃表
    dfs(1, 0);
    
    // 处理m个查询
    for (int i = 1, a, b, c; i <= m; i++) {
        scanf("%d %d %d", &a, &b, &c);
        compute(a, b, c);
        printf("%d %lld\n", togather, cost);
    }
    
    return 0;
}
