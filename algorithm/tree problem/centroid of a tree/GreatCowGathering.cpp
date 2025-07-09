#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

// 牛群聚集(递归版)
// 一共有n个节点，编号1~n，每个点有牛的数量
// 一共有n-1条边把所有点连通起来形成一棵树，每条边有权值
// 想把所有的牛汇聚在一点，希望走过的总距离最小
// 返回总距离最小是多少
// 利用重心的性质：
// 树上的边权如果都>=0，不管边权怎么分布，所有节点都走向重心的总距离和最小
// 测试链接 : https://www.luogu.com.cn/problem/P2986

using namespace std;

const int MAXN = 100001;

// 全局变量声明
int n;                          // 节点数量
int cow[MAXN];                  // 每个节点的牛的数量
int cowSum;                     // 所有牛的总数
int head[MAXN];                 // 邻接表头指针
int next_edge[MAXN << 1];       // 边的下一个指针（邻接表实现）
int to[MAXN << 1];             // 边的目标节点
int weight[MAXN << 1];         // 边的权重
int cnt;                       // 边的计数器
int best, center;              // 最优值和树的重心
int size_arr[MAXN];            // 子树大小数组
int path[MAXN];                // 从重心到各节点的距离

// 初始化函数
void build() {
    cnt = 1;                   // 边计数器从1开始（0表示空）
    fill(head + 1, head + n + 1, 0);  // 初始化邻接表头指针
    cowSum = 0;                // 初始化牛的总数
    best = INT_MAX;            // 初始化最优值为最大整数
}

// 添加边的函数（无向图需要添加两条边）
void addEdge(int u, int v, int w) {
    next_edge[cnt] = head[u];   // 新边指向原来的第一条边
    to[cnt] = v;               // 边的目标节点
    weight[cnt] = w;           // 边的权重
    head[u] = cnt++;           // 更新头指针并递增边计数器
}

// 寻找树的重心的函数
void findCenter(int u, int f) {
    size_arr[u] = cow[u];      // 初始化当前节点的子树大小为其牛的数量

    // 遍历所有相邻节点
    for (int e = head[u], v; e != 0; e = next_edge[e]) {
        v = to[e];
        if (v != f) {          // 避免回到父节点
            findCenter(v, u);   // 递归处理子节点
        }
    }

    int maxsub = 0;            // 记录最大子树的大小
    // 再次遍历所有相邻节点，计算子树大小
    for (int e = head[u], v; e != 0; e = next_edge[e]) {
        v = to[e];
        if (v != f) {
            size_arr[u] += size_arr[v];    // 累加子树大小
            maxsub = max(maxsub, size_arr[v]);  // 更新最大子树大小
        }
    }

    // 考虑删除当前节点后，父节点所在的子树大小
    maxsub = max(maxsub, cowSum - size_arr[u]);

    // 如果当前节点作为重心更优，则更新重心
    if (maxsub < best) {
        best = maxsub;
        center = u;
    }
}

// 计算从重心到各节点距离的函数
void setPath(int u, int f) {
    // 遍历所有相邻节点
    for (int e = head[u], v; e != 0; e = next_edge[e]) {
        v = to[e];
        if (v != f) {          // 避免回到父节点
            path[v] = path[u] + weight[e];  // 计算到重心的距离
            setPath(v, u);     // 递归处理子节点
        }
    }
}

// 主计算函数
long long compute() {
    // 计算所有牛的总数
    for (int i = 1; i <= n; i++) {
        cowSum += cow[i];
    }

    // 寻找树的重心
    findCenter(1, 0);

    // 设置重心到自身的距离为0
    path[center] = 0;
    // 计算从重心到各节点的距离
    setPath(center, 0);

    // 计算总的加权距离
    long long ans = 0;
    for (int i = 1; i <= n; i++) {
        ans += (long long)cow[i] * path[i];
    }

    return ans;
}

int main() {
    ios::sync_with_stdio(false);  // 加速输入输出
    cin.tie(nullptr);

    cin >> n;                     // 读入节点数量
    build();                      // 初始化

    // 读入每个节点的牛的数量
    for (int i = 1; i <= n; i++) {
        cin >> cow[i];
    }

    // 读入边的信息并建图
    for (int i = 1; i < n; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        addEdge(u, v, w);         // 添加边u->v
        addEdge(v, u, w);         // 添加边v->u（无向图）
    }

    // 计算并输出结果
    cout << compute() << endl;

    return 0;
}
