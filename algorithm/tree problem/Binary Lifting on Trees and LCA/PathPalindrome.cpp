#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>

// 检查树上两节点间的路径是否是回文
// 一颗树上有n个节点，编号1~n
// 给定长度为n的数组parent, parent[i]表示节点i的父节点编号
// 给定长度为n的数组s, s[i]表示节点i上是什么字符
// 从节点a到节点b经过节点最少的路，叫做a和b的路径
// 一共有m条查询，每条查询(a,b)，a和b的路径字符串是否是回文
// 是回文打印"YES"，不是回文打印"NO"
// 1 <= n <= 10^5
// 1 <= m <= 10^5
// parent[1] = 0，即整棵树的头节点一定是1号节点
// 每个节点上的字符一定是小写字母a~z
// 测试链接 : https://ac.nowcoder.com/acm/contest/78807/G

using namespace std;

const int MAXN = 100001;        // 最大节点数
const int LIMIT = 17;           // 二进制跳跃的最大层数 (log2(100000) ≈ 17)
const long long K = 499;        // 哈希函数的底数

int power;                      // 实际使用的跳跃层数
int s[MAXN];                   // 存储每个节点的字符值
int head[MAXN];                // 邻接表头节点
int to[MAXN << 1];             // 边的终点
int next_edge[MAXN << 1];      // 下一条边的索引
int cnt;                       // 边的计数器
int deep[MAXN];                // 每个节点的深度
int jump[MAXN][LIMIT];         // 二进制跳跃表，jump[i][j]表示从节点i向上跳2^j步到达的节点
long long kpow[MAXN];          // K的幂次数组，kpow[i] = K^i
long long stup[MAXN][LIMIT];   // 向上跳跃时的哈希值
long long stdown[MAXN][LIMIT]; // 向下跳跃时的哈希值

/**
 * 初始化函数
 * @param n 节点数量
 */
void build(int n) {
    power = 0;
    // 计算需要的跳跃层数
    while ((1 << power) <= (n >> 1)) {
        power++;
    }
    cnt = 1;  // 边的索引从1开始
    fill(head + 1, head + n + 1, 0);  // 初始化邻接表

    // 预计算K的幂次
    kpow[0] = 1;
    for (int i = 1; i <= n; i++) {
        kpow[i] = kpow[i - 1] * K;
    }
}

/**
 * 添加边到邻接表
 * @param u 起点
 * @param v 终点
 */
void addEdge(int u, int v) {
    next_edge[cnt] = head[u];
    to[cnt] = v;
    head[u] = cnt++;
}

/**
 * 深度优先搜索，构建二进制跳跃表和哈希值
 * @param u 当前节点
 * @param f 父节点
 */
void dfs(int u, int f) {
    deep[u] = deep[f] + 1;        // 设置深度
    jump[u][0] = f;               // 父节点
    stup[u][0] = stdown[u][0] = s[f];  // 初始化哈希值为父节点的字符值

    // 构建二进制跳跃表
    for (int p = 1; p <= power; p++) {
        int v = jump[u][p - 1];
        jump[u][p] = jump[v][p - 1];  // 向上跳2^p步 = 向上跳2^(p-1)步两次

        // 计算向上跳跃的哈希值（正向）
        stup[u][p] = stup[u][p - 1] * kpow[1 << (p - 1)] + stup[v][p - 1];

        // 计算向下跳跃的哈希值（反向）
        stdown[u][p] = stdown[v][p - 1] * kpow[1 << (p - 1)] + stdown[u][p - 1];
    }

    // 递归处理子节点
    for (int e = head[u]; e != 0; e = next_edge[e]) {
        int v = to[e];
        if (v != f) {
            dfs(v, u);
        }
    }
}

/**
 * 求两个节点的最近公共祖先（LCA）
 * @param a 节点a
 * @param b 节点b
 * @return 最近公共祖先
 */
int lca(int a, int b) {
    // 确保a的深度不小于b的深度
    if (deep[a] < deep[b]) {
        swap(a, b);
    }

    // 将a上移到与b相同的深度
    for (int p = power; p >= 0; p--) {
        if (deep[jump[a][p]] >= deep[b]) {
            a = jump[a][p];
        }
    }

    // 如果a和b相同，则a就是LCA
    if (a == b) {
        return a;
    }

    // 同时上移a和b，直到它们的父节点相同
    for (int p = power; p >= 0; p--) {
        if (jump[a][p] != jump[b][p]) {
            a = jump[a][p];
            b = jump[b][p];
        }
    }

    return jump[a][0];  // 返回公共父节点
}

/**
 * 计算从from到to经过lca_node的路径哈希值
 * @param from 起点
 * @param lca_node 最近公共祖先
 * @param to 终点
 * @return 路径的哈希值
 */
long long hash_path(int from, int lca_node, int to) {
    // 计算从from到lca_node的上坡哈希值（正向）
    long long up = s[from];
    for (int p = power; p >= 0; p--) {
        if (deep[jump[from][p]] >= deep[lca_node]) {
            up = up * kpow[1 << p] + stup[from][p];
            from = jump[from][p];
        }
    }

    // 如果终点就是LCA，直接返回上坡哈希值
    if (to == lca_node) {
        return up;
    }

    // 计算从lca_node到to的下坡哈希值（反向）
    long long down = s[to];
    int height = 1;  // 当前下坡路径的长度
    for (int p = power; p >= 0; p--) {
        if (deep[jump[to][p]] > deep[lca_node]) {
            down = stdown[to][p] * kpow[height] + down;
            height += 1 << p;
            to = jump[to][p];
        }
    }

    // 合并上坡和下坡的哈希值
    return up * kpow[height] + down;
}

/**
 * 判断从节点a到节点b的路径是否为回文
 * @param a 起点
 * @param b 终点
 * @return 是否为回文
 */
bool isPalindrome(int a, int b) {
    int lca_node = lca(a, b);
    // 计算a到b的路径哈希值
    long long hash1 = hash_path(a, lca_node, b);
    // 计算b到a的路径哈希值（即a到b的反向）
    long long hash2 = hash_path(b, lca_node, a);
    // 如果正向和反向哈希值相同，则为回文
    return hash1 == hash2;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    build(n);

    // 读取字符串并转换为数字
    string str;
    cin >> str;
    for (int i = 0; i < n; i++) {
        s[i + 1] = str[i] - 'a' + 1;  // 'a'对应1，'b'对应2，等等
    }

    // 构建树的邻接表
    for (int u = 1; u <= n; u++) {
        int v;
        cin >> v;
        addEdge(u, v);
        addEdge(v, u);
    }

    // 从节点1开始DFS构建跳跃表
    dfs(1, 0);

    // 处理查询
    int m;
    cin >> m;
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        cout << (isPalindrome(a, b) ? "YES" : "NO") << "\n";
    }

    return 0;
}
