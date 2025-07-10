#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>

// 边差分实战
// 使图不连通的方法数
// 有n个节点，给定n-1条老边使其连接成一棵树，再给定m条新边额外加在树上
// 你可以切断两条边让这个图不连通，切断的两条边必须是一条老边和一条新边
// 返回方法数
// 测试链接 : http://poj.org/problem?id=3417

using namespace std;

const int MAXN = 100001;  // 最大节点数
const int LIMIT = 17;     // 倍增跳跃的最大层数，log2(100000) < 17

int power;                // 实际需要的倍增层数
int n, m;                 // n个节点，m条新边
int num[MAXN];            // 边差分数组，记录每个节点的差分值
int head[MAXN];           // 邻接表头指针
int next_edge[MAXN << 1]; // 邻接表的next指针，双向边所以要*2
int to[MAXN << 1];        // 邻接表的目标节点
int cnt;                  // 边的计数器
int deep[MAXN];           // 每个节点的深度
int stjump[MAXN][LIMIT];  // 倍增跳跃表，stjump[i][j]表示从节点i向上跳2^j步到达的节点
int ans;                  // 最终答案

// 计算log2(n)，用于确定倍增的层数
int log2(int n) {
    int ans = 0;
    while ((1 << ans) <= (n >> 1)) {
        ans++;
    }
    return ans;
}

// 初始化函数
void build() {
    power = log2(n);                        // 计算需要的倍增层数
    fill(num + 1, num + n + 1, 0);         // 初始化差分数组
    cnt = 1;                                // 边计数器从1开始
    fill(head + 1, head + n + 1, 0);       // 初始化邻接表头指针
    ans = 0;                                // 初始化答案
}

// 添加边到邻接表
void addEdge(int u, int v) {
    next_edge[cnt] = head[u];  // 新边的next指向当前头边
    to[cnt] = v;               // 新边指向节点v
    head[u] = cnt++;           // 更新头指针并递增边计数器
}

// 第一次DFS：构建倍增跳跃表，计算每个节点的深度
void dfs1(int u, int f) {
    deep[u] = deep[f] + 1;      // 当前节点深度 = 父节点深度 + 1
    stjump[u][0] = f;           // 向上跳1步(2^0)到达父节点

    // 构建倍增跳跃表
    for (int p = 1; p <= power; p++) {
        // 向上跳2^p步 = 先跳2^(p-1)步，再跳2^(p-1)步
        stjump[u][p] = stjump[stjump[u][p - 1]][p - 1];
    }

    // 遍历所有子节点
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        if (to[e] != f) {  // 不回到父节点
            dfs1(to[e], u);
        }
    }
}

// 使用倍增算法求最近公共祖先(LCA)
int lca(int a, int b) {
    // 确保a的深度大于等于b的深度
    if (deep[a] < deep[b]) {
        swap(a, b);
    }

    // 将a向上跳到与b相同的深度
    for (int p = power; p >= 0; p--) {
        if (deep[stjump[a][p]] >= deep[b]) {
            a = stjump[a][p];
        }
    }

    // 如果a和b是同一个节点，则LCA就是a
    if (a == b) {
        return a;
    }

    // 同时向上跳，直到找到LCA
    for (int p = power; p >= 0; p--) {
        if (stjump[a][p] != stjump[b][p]) {  // 如果跳到的节点不同
            a = stjump[a][p];                // 继续向上跳
            b = stjump[b][p];
        }
    }

    return stjump[a][0];  // 返回LCA
}

// 第二次DFS：处理边差分，统计答案
void dfs2(int u, int f) {
    // 先递归处理所有子节点
    for (int e = head[u], v; e != 0; e = next_edge[e]) {
        v = to[e];
        if (v != f) {
            dfs2(v, u);
        }
    }

    // 处理每条从u到子节点的边
    for (int e = head[u], v, w; e != 0; e = next_edge[e]) {
        v = to[e];
        if (v != f) {
            w = 0 + num[v];  // 子节点的差分值

            // 根据差分值判断当前边的情况
            if (w == 0) {
                // 如果差分值为0，说明没有新边经过这条老边
                // 可以切断这条老边和任意一条新边
                ans += m;
            } else if (w == 1) {
                // 如果差分值为1，说明有且仅有一条新边经过这条老边
                // 只能切断这条老边和那条特定的新边
                ans += 1;
            } else {
                // 如果差分值>1，说明有多条新边经过这条老边
                // 切断这条老边不能让图不连通
                ans += 0;
            }

            // 将子节点的差分值累加到当前节点
            num[u] += num[v];
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);  // 加速输入输出
    cin.tie(NULL);

    cin >> n >> m;  // 读入节点数和新边数
    build();        // 初始化

    // 读入n-1条老边，构建树
    for (int i = 1, u, v; i < n; i++) {
        cin >> u >> v;
        addEdge(u, v);  // 添加双向边
        addEdge(v, u);
    }

    dfs1(1, 0);  // 第一次DFS，构建倍增跳跃表

    // 读入m条新边，进行边差分
    for (int i = 1, u, v, lca_val; i <= m; i++) {
        cin >> u >> v;
        lca_val = lca(u, v);  // 找到u和v的最近公共祖先

        // 边差分：在路径上+1，在LCA处-2
        num[u]++;
        num[v]++;
        num[lca_val] -= 2;
    }

    dfs2(1, 0);          // 第二次DFS，统计答案
    cout << ans << endl; // 输出答案

    return 0;
}
