#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>

// 树上倍增解法迭代版
// 测试链接 : https://www.luogu.com.cn/problem/P3379

using namespace std;

const int MAXN = 500001;    // 最大节点数
const int LIMIT = 20;       // 倍增的最大跳跃层数，2^20 > 500000

int power;                  // 倍增的最大有效层数
int cnt;                    // 边的计数器
int head[MAXN];             // 邻接表的头指针
int next_edge[MAXN << 1];   // 邻接表的下一条边
int to[MAXN << 1];          // 邻接表的目标节点
int stjump[MAXN][LIMIT];    // 倍增数组，stjump[i][j]表示从节点i向上跳2^j步到达的祖先
int deep[MAXN];             // 每个节点的深度

// 计算log2(n)，用于确定倍增的最大层数
int log2(int n) {
    int ans = 0;
    while ((1 << ans) <= (n >> 1)) {
        ans++;
    }
    return ans;
}

// 初始化图结构
void build(int n) {
    power = log2(n);        // 计算倍增的最大层数
    cnt = 1;                // 边的编号从1开始
    memset(head, 0, sizeof(head));  // 初始化邻接表头指针
}

// 添加边（无向图需要添加两条边）
void addEdge(int u, int v) {
    next_edge[cnt] = head[u];   // 新边的下一条边指向原来的第一条边
    to[cnt] = v;                // 新边的目标节点
    head[u] = cnt++;            // 更新头指针并递增边计数器
}

// DFS迭代版实现需要的栈结构
int ufe[MAXN][3];          // 栈，存储[当前节点u，父节点f，当前处理的边e]
int stackSize, u, f, e;    // 栈大小和当前处理的变量

// 入栈操作
void push(int u, int f, int e) {
    ufe[stackSize][0] = u;  // 当前节点
    ufe[stackSize][1] = f;  // 父节点
    ufe[stackSize][2] = e;  // 当前处理的边
    stackSize++;
}

// 出栈操作
void pop() {
    --stackSize;
    u = ufe[stackSize][0];  // 取出当前节点
    f = ufe[stackSize][1];  // 取出父节点
    e = ufe[stackSize][2];  // 取出当前处理的边
}

// DFS遍历，建立倍增数组和深度数组
void dfs(int root) {
    stackSize = 0;
    // 栈里存放三个信息：
    // u : 当前处理的节点
    // f : 当前节点u的父节点
    // e : 处理到第几条边了
    // 如果e==-1，表示之前没有处理过u的任何边（第一次访问该节点）
    // 如果e==0，表示u的边都已经处理完了
    push(root, 0, -1);

    while (stackSize > 0) {
        pop();

        if (e == -1) {
            // 第一次访问该节点，初始化深度和倍增数组
            deep[u] = deep[f] + 1;          // 深度 = 父节点深度 + 1
            stjump[u][0] = f;               // 向上跳2^0=1步到达的祖先就是父节点

            // 建立倍增数组：stjump[u][p] = stjump[stjump[u][p-1]][p-1]
            // 即：从u向上跳2^p步 = 先跳2^(p-1)步，再从到达的节点跳2^(p-1)步
            for (int p = 1; p <= power; p++) {
                stjump[u][p] = stjump[stjump[u][p - 1]][p - 1];
            }
            e = head[u];  // 开始处理该节点的第一条边
        } else {
            e = next_edge[e];  // 处理该节点的下一条边
        }

        if (e != 0) {
            // 还有边需要处理
            push(u, f, e);              // 将当前状态压栈（继续处理下一条边）
            if (to[e] != f) {           // 如果目标节点不是父节点（避免回到父节点）
                push(to[e], u, -1);     // 将子节点压栈进行DFS
            }
        }
    }
}

// LCA查询：找到节点a和b的最近公共祖先
int lca(int a, int b) {
    // 保证a的深度 >= b的深度
    if (deep[a] < deep[b]) {
        swap(a, b);
    }

    // 第一步：将a提升到与b相同的深度
    for (int p = power; p >= 0; p--) {
        if (deep[stjump[a][p]] >= deep[b]) {
            a = stjump[a][p];  // 从a向上跳2^p步
        }
    }

    // 如果a和b已经相同，说明b就是a的祖先
    if (a == b) {
        return a;
    }

    // 第二步：同时提升a和b，直到它们的父节点相同
    for (int p = power; p >= 0; p--) {
        if (stjump[a][p] != stjump[b][p]) {
            // 如果a和b向上跳2^p步后到达的节点不同，则可以安全地跳跃
            a = stjump[a][p];
            b = stjump[b][p];
        }
    }

    // 此时a和b的父节点相同，返回它们的父节点
    return stjump[a][0];
}

int main() {
    ios::sync_with_stdio(false);  // 加速输入输出
    cin.tie(nullptr);

    int n, m, root;
    cin >> n >> m >> root;  // 读入节点数、查询数、根节点

    build(n);  // 初始化图结构

    // 读入n-1条边构建树
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        addEdge(u, v);  // 添加双向边
        addEdge(v, u);
    }

    dfs(root);  // 从根节点开始DFS，建立倍增数组

    // 处理m个LCA查询
    for (int i = 1; i <= m; i++) {
        int a, b;
        cin >> a >> b;
        cout << lca(a, b) << "\n";  // 输出LCA结果
    }

    return 0;
}
