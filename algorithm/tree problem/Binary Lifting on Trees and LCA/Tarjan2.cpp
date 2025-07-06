#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>

// tarjan算法解法迭代版
// 测试链接 : https://www.luogu.com.cn/problem/P3379

using namespace std;

const int MAXN = 500001;

// 邻接表存储树的边
int headEdge[MAXN];      // 每个节点的第一条边的索引
int edgeNext[MAXN << 1]; // 每条边的下一条边的索引
int edgeTo[MAXN << 1];   // 每条边指向的节点
int tcnt;                // 边的计数器

// 邻接表存储查询
int headQuery[MAXN];      // 每个节点的第一个查询的索引
int queryNext[MAXN << 1]; // 每个查询的下一个查询的索引
int queryTo[MAXN << 1];   // 每个查询的另一个节点
int queryIndex[MAXN << 1]; // 查询的原始编号
int qcnt;                 // 查询计数器

bool visited[MAXN];  // 标记节点是否已被访问
int father[MAXN];    // 并查集的父节点数组
int ans[MAXN];       // 存储每个查询的答案

// 为了实现迭代版Tarjan算法而准备的栈
int stack_arr[MAXN]; // 用于并查集路径压缩的临时栈

// 为了实现迭代版Tarjan算法而准备的栈
int ufe[MAXN][3];    // 主栈，存储三元组(u, f, e)
int stackSize, u, f, e; // 栈大小和当前处理的三元组

/**
 * 初始化所有数据结构
 * @param n 节点数量
 */
void build(int n) {
    tcnt = qcnt = 1; // 从1开始编号，0表示空
    fill(headEdge + 1, headEdge + n + 1, 0);
    fill(headQuery + 1, headQuery + n + 1, 0);
    fill(visited + 1, visited + n + 1, false);
    // 初始化并查集，每个节点的父节点都是自己
    for (int i = 1; i <= n; i++) {
        father[i] = i;
    }
}

/**
 * 在邻接表中添加一条边
 * @param u 起始节点
 * @param v 目标节点
 */
void addEdge(int u, int v) {
    edgeNext[tcnt] = headEdge[u]; // 新边的下一条边是当前头边
    edgeTo[tcnt] = v;             // 新边指向v
    headEdge[u] = tcnt++;         // 更新头边为新边
}

/**
 * 在邻接表中添加一个查询
 * @param u 查询的一个节点
 * @param v 查询的另一个节点
 * @param i 查询的原始编号
 */
void addQuery(int u, int v, int i) {
    queryNext[qcnt] = headQuery[u]; // 新查询的下一个查询是当前头查询
    queryTo[qcnt] = v;              // 新查询指向v
    queryIndex[qcnt] = i;           // 记录原始查询编号
    headQuery[u] = qcnt++;          // 更新头查询为新查询
}

/**
 * 并查集查找根节点（带路径压缩的迭代版本）
 * @param i 要查找的节点
 * @return 根节点
 */
int find(int i) {
    int size = 0;
    // 向上查找直到根节点，同时记录路径
    while (i != father[i]) {
        stack_arr[size++] = i;
        i = father[i];
    }
    // 路径压缩：将路径上的所有节点直接连接到根节点
    while (size > 0) {
        father[stack_arr[--size]] = i;
    }
    return i;
}

/**
 * 将三元组(u, f, e)压入栈
 * @param u 当前处理的节点
 * @param f 当前节点的父节点
 * @param e 当前处理到的边的编号
 */
void push(int u, int f, int e) {
    ufe[stackSize][0] = u;
    ufe[stackSize][1] = f;
    ufe[stackSize][2] = e;
    stackSize++;
}

/**
 * 从栈中弹出一个三元组
 */
void pop() {
    --stackSize;
    u = ufe[stackSize][0];
    f = ufe[stackSize][1];
    e = ufe[stackSize][2];
}

/**
 * Tarjan算法求LCA（迭代版本）
 * 算法原理：
 * 1. 深度优先遍历树
 * 2. 对于每个节点，处理完所有子树后，处理与该节点相关的查询
 * 3. 使用并查集维护已访问节点的祖先信息
 *
 * @param root 树的根节点
 */
void tarjan(int root) {
    stackSize = 0;
    // 栈里存放三个信息：
    // u : 当前处理的点
    // f : 当前点u的父节点
    // e : 处理到几号边了
    // 如果e==-1，表示之前没有处理过u的任何边（初始状态）
    // 如果e==0，表示u的边都已经处理完了
    push(root, 0, -1);

    while (stackSize > 0) {
        pop();

        if (e == -1) {
            // 第一次访问节点u
            visited[u] = true;
            e = headEdge[u]; // 开始处理u的第一条边
        } else {
            // 处理u的下一条边
            e = edgeNext[e];
        }

        if (e != 0) {
            // 还有边需要处理
            push(u, f, e); // 将当前状态压栈，稍后继续处理
            if (edgeTo[e] != f) { // 避免回到父节点
                push(edgeTo[e], u, -1); // 递归处理子节点
            }
        } else {
            // e == 0代表u后续已经没有边需要处理了
            // 处理与节点u相关的所有查询
            for (int q = headQuery[u], v; q != 0; q = queryNext[q]) {
                v = queryTo[q];
                if (visited[v]) {
                    // 如果查询的另一个节点v已经被访问过
                    // 那么u和v的LCA就是v在并查集中的根节点
                    ans[queryIndex[q]] = find(v);
                }
            }
            // 将u合并到其父节点的集合中
            // 这样后续访问的节点查询u时，会找到u的祖先
            father[u] = f;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, root;
    cin >> n >> m >> root; // n个节点，m个查询，root为根节点

    build(n); // 初始化数据结构

    // 读入n-1条边构建树
    for (int i = 1; i < n; i++) {
        int u, v;
        cin >> u >> v;
        addEdge(u, v); // 无向边，需要添加两个方向
        addEdge(v, u);
    }

    // 读入m个查询
    for (int i = 1; i <= m; i++) {
        int u, v;
        cin >> u >> v;
        addQuery(u, v, i); // 每个查询也需要添加两个方向
        addQuery(v, u, i); // 这样无论从哪个节点开始都能找到对应的查询
    }

    tarjan(root); // 运行Tarjan算法

    // 输出所有查询的答案
    for (int i = 1; i <= m; i++) {
        cout << ans[i] << "\n";
    }

    return 0;
}
