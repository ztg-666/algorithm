#include <bits/stdc++.h>

// 幸运数字(递归版)
// 一共有n个点，编号1~n，由n-1条边连成一棵树，每个点上有数字
// 一共有q条查询，每次返回a到b的路径上，可以随意选择数字，能得到的最大异或和
// 1 <= n <= 2 * 10^4
// 1 <= q <= 2 * 10^5
// 0 <= 点上的数字 <= 2^60
// 测试链接 : https://www.luogu.com.cn/problem/P3292

using namespace std;

const int MAXN = 20002;   // 最大节点数（树节点个数 <= 20000）
const int LIMIT = 16;     // 倍增的最大层数（因为 2^16 > 20000）
const int BIT = 60;       // 节点权值最大二进制位数（假设值 <= 2^60）

// 节点权值
long long arr[MAXN];

// 链式前向星存储树的邻接表
int head[MAXN];                  // 邻接表的头指针
int nxt_[MAXN << 1];              // 下一条边的索引
int to_[MAXN << 1];               // 目标节点
int ecnt;                         // 边计数器（从 1 开始）

// 倍增 LCA（最近公共祖先）数组
int deep_[MAXN];                  // 节点深度
int stjump[MAXN][LIMIT + 1];      // stjump[u][p] = u 的 2^p 级祖先

// 记录 log2(n)（用于倍增层数限制）
int powerLog;

// bases[u][i] 表示从根到 u 路径上维护的线性基的第 i 位
// levels[u][i] 表示该线性基中该位是由哪个层级（深度）的节点提供的
long long bases[MAXN][BIT + 1];
int levels[MAXN][BIT + 1];

// 临时线性基（查询用）
long long tempBasis[BIT + 1];

// ---------------- 工具函数 ----------------

// 计算 log2(n)（向下取整）
inline int mylog2(int n) {
    int ans = 0;
    while ((1 << ans) <= (n >> 1)) ans++;
    return ans;
}

// 初始化（清空邻接表等）
inline void prepare(int n) {
    ecnt = 1;
    fill(head, head + n + 1, 0); // 邻接表初始化
    powerLog = mylog2(n);
}

// 添加一条无向边（链式前向星）
inline void addEdge(int u, int v) {
    nxt_[ecnt] = head[u];
    to_[ecnt] = v;
    head[u] = ecnt++;
}

/**
 * 向线性基中插入一个数，如果冲突则替换（按深度优先）
 * curv:   要插入的值
 * curl:   该值所在节点的深度
 * basis[]:当前线性基数组
 * level[]:每个位对应的节点深度
 */
inline bool insertReplace(long long curv, int curl,
                          long long basis[], int level[]) {
    for (int i = BIT; i >= 0; --i) {
        if (((curv >> i) & 1ULL) == 0) continue;  // 如果当前位是 0，跳过
        if (basis[i] == 0) {                      // 如果该位还没有基向量，直接放入
            basis[i] = curv;
            level[i] = curl;
            return true;
        }
        if (curl > level[i]) {                    // 深度优先：如果当前值所在节点更深，则替换基向量
            long long tmp1 = curv;
            curv = basis[i];
            basis[i] = tmp1;
            int tmp2 = level[i];
            level[i] = curl;
            curl = tmp2;
        }
        curv ^= basis[i];                         // 消去最高位
    }
    return false;
}

// ---------------- DFS 建树 ----------------
void dfs(int u, int f) {
    deep_[u] = deep_[f] + 1;              // 记录深度
    stjump[u][0] = f;                     // 记录父节点（倍增第一层）
    for (int p = 1; p <= powerLog; ++p) { // 倍增预处理
        stjump[u][p] = stjump[ stjump[u][p - 1] ][p - 1];
    }

    // 继承父节点的线性基
    for (int i = 0; i <= BIT; ++i) {
        bases[u][i] = bases[f][i];
        levels[u][i] = levels[f][i];
    }

    // 把当前节点的权值插入线性基
    insertReplace(arr[u], deep_[u], bases[u], levels[u]);

    // 遍历子节点
    for (int e = head[u]; e != 0; e = nxt_[e]) {
        int v = to_[e];
        if (v == f) continue; // 避免回到父节点
        dfs(v, u);
    }
}

// ---------------- 倍增法 LCA ----------------
int lca(int x, int y) {
    if (deep_[x] < deep_[y]) swap(x, y);
    // 先让 x 和 y 到同一深度
    for (int p = powerLog; p >= 0; --p) {
        if (deep_[ stjump[x][p] ] >= deep_[y]) x = stjump[x][p];
    }
    if (x == y) return x;
    // 同时向上跳
    for (int p = powerLog; p >= 0; --p) {
        if (stjump[x][p] != stjump[y][p]) {
            x = stjump[x][p];
            y = stjump[y][p];
        }
    }
    return stjump[x][0];
}

// ---------------- 查询路径最大异或值 ----------------
long long query(int x, int y) {
    int g = lca(x, y); // 最近公共祖先

    // 获取两端点的线性基

    // 清空临时线性基
    for (int i = 0; i <= BIT; ++i) tempBasis[i] = 0LL;

    // 从 x 向上到 LCA 的部分基加入 tempBasis
    for (int i = BIT; i >= 0; --i) {
        long long num = bases[x][i];
        if (num != 0 && levels[x][i] >= deep_[g]) {
            tempBasis[i] = num;
        }
    }

    // 合并 y 侧的基
    for (int i = BIT; i >= 0; --i) {
        long long num = bases[y][i];
        if (num != 0 && levels[y][i] >= deep_[g]) {
            for (int j = i; j >= 0; --j) {
                if (((num >> j) & 1LL) == 0) continue;
                if (tempBasis[j] == 0) {
                    tempBasis[j] = num;
                    break;
                }
                num ^= tempBasis[j];
            }
        }
    }

    // 在线性基中求最大异或值
    long long ans = 0LL;
    for (int i = BIT; i >= 0; --i) {
        ans = max(ans, ans ^ tempBasis[i]);
    }
    return ans;
}

// ---------------- 主函数 ----------------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    if (!(cin >> n >> q)) return 0;
    prepare(n);

    // 输入每个节点的权值
    for (int i = 1; i <= n; ++i) {
        cin >> arr[i];
    }

    // 输入树的边
    for (int i = 1, u, v; i < n; ++i) {
        cin >> u >> v;
        addEdge(u, v);
        addEdge(v, u);
    }

    // 从 1 号节点开始 DFS
    dfs(1, 0);

    // 处理 q 次查询
    for (int i = 1, x, y; i <= q; ++i) {
        cin >> x >> y;
        cout << query(x, y) << '\n';
    }
    return 0;
}
