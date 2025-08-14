#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>

// 路径最大异或和(递归版)
// 一共有n个点，编号1~n，由m条无向边连接
// 每条边有权值，输入保证图是连通的，可能有环
// 找到1到n的一条路径，路径可以重复经过某些点或边
// 当一条边在路径中出现了多次时，异或的时候也要算多次
// 希望找到一条从1到n的路径，所有边权异或和尽量大，返回这个最大异或和
// 1 <= n <= 50000
// 1 <= m <= 100000
// 0 <= 边权 <= 10^18
// 测试链接 : https://www.luogu.com.cn/problem/P4151

using namespace std;

const int MAXN = 50001;   // 最大节点数
const int MAXM = 200002;  // 最大边数（因为是无向图，每条边要存两次）
const int BIT = 60;       // 边权最大 10^18，大约需要 60 位二进制表示

// 链式前向星存图
vector<int> head(MAXN, 0), nxt(MAXM), to(MAXM);
// 边权
vector<long long> weight(MAXM);
// 异或基（basis[i] 表示最高位为 i 的基向量）
vector<long long> basis(BIT + 1, 0);
// path[u] 表示从 1 到 u 的路径异或和
vector<long long> path(MAXN, 0);
// visited[u] 表示 u 是否被访问过
vector<bool> visited(MAXN, false);

// 当前边编号（链式前向星用）
int cnt = 1;

// 初始化图和异或基
void prepare(int n) {
    cnt = 1;
    fill(head.begin(), head.begin() + n + 1, 0);
    fill(basis.begin(), basis.end(), 0);
}

// 添加一条边（u -> v, 权重 w）
void addEdge(int u, int v, long long w) {
    nxt[cnt] = head[u];  // 指向上一条边
    to[cnt] = v;         // 存储目标节点
    weight[cnt] = w;     // 存储边权
    head[u] = cnt++;     // 更新链表头
}

// 向线性基插入一个数
bool insert(long long num) {
    for (int i = BIT; i >= 0; i--) {
        if (num >> i & 1) {   // 找到最高位为 i 的数
            if (basis[i] == 0) {
                basis[i] = num;  // 如果该位还没有基向量，就插入
                return true;
            }
            num ^= basis[i];  // 否则消去该位
        }
    }
    return false; // num 可以被现有基向量表示
}

// DFS 建立 path 数组并找环
void dfs(int u, int f, long long p) {
    path[u] = p;       // 记录 1 到 u 的异或和
    visited[u] = true; // 标记访问
    for (int e = head[u]; e != 0; e = nxt[e]) { // 遍历所有出边
        int v = to[e];
        if (v != f) {  // 防止回到父节点
            long long xor_val = p ^ weight[e];
            if (visited[v]) {
                // 如果 v 已经访问过，说明找到一个环
                // 环的异或值 = 当前路径异或值 ^ v 的路径异或值
                insert(xor_val ^ path[v]);
            } else {
                dfs(v, u, xor_val);
            }
        }
    }
}

// 用线性基优化路径异或和，得到最大值
long long query(long long init) {
    for (int i = BIT; i >= 0; i--) {
        init = max(init, init ^ basis[i]); // 尝试用基向量提升异或值
    }
    return init;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    cin >> n >> m;

    prepare(n);

    // 读入 m 条无向边
    for (int i = 1; i <= m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        addEdge(u, v, w);
        addEdge(v, u, w); // 无向图需要双向加边
    }

    dfs(1, 0, 0); // 从 1 开始 DFS
    cout << query(path[n]) << '\n'; // 查询最大异或路径和

    return 0;
}
