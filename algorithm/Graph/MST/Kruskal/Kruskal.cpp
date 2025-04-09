#include <cstdio>
#include <algorithm>
using namespace std;

// 定义常量：最大顶点数和最大边数
const int MAXN = 5001;  // 最大顶点数
const int MAXM = 200001; // 最大边数

// 定义边的结构体，包含起点u、终点v和权重w
struct Edge {
    int u; // 边的起点
    int v; // 边的终点
    int w; // 边的权重
} edges[MAXM]; // 存储所有边的数组

int father[MAXN]{}; // 并查集数组，用于维护连通分量
int n = 0, m = 0;   // n为顶点数，m为边数

// 初始化并查集，每个节点的父节点初始化为自己
void build() {
    for (int i = 1; i <= n; i++) {
        father[i] = i; // 每个节点初始时都是自己的父节点
    }
}

// 查找操作，带路径压缩优化
int find(int x) {
    if (x != father[x]) { // 如果x不是自己的父节点
        father[x] = find(father[x]); // 路径压缩，直接将x连接到根节点
    }
    return father[x]; // 返回x所属集合的根节点
}

// 合并操作，尝试将x和y所在的集合合并
bool unionSet(int x, int y) {
    int fx = find(x); // 找到x的根节点
    int fy = find(y); // 找到y的根节点
    if (fx != fy) {   // 如果x和y不在同一个集合
        father[fx] = fy; // 将fx的父节点设为fy，合并两个集合
        return true;     // 合并成功
    }
    return false; // 如果已经在同一个集合，返回false
}

// 边的比较函数，按权重从小到大排序
bool cmp(const Edge& a, const Edge& b) {
    return a.w < b.w; // 按边的权重升序排列
}
// Kruskal算法模版（洛谷）
// 静态空间实现
// 测试链接 : https://www.luogu.com.cn/problem/P3366
int main() {
    scanf("%d%d", &n, &m); // 输入顶点数n和边数m
    build();               // 初始化并查集

    // 输入所有边的信息
    for (int i = 0; i < m; i++) {
        scanf("%d%d%d", &edges[i].u, &edges[i].v, &edges[i].w); // 输入每条边的起点、终点和权重
    }

    sort(edges, edges + m, cmp); // 对所有边按权重从小到大排序

    int ans = 0, cnt = 0; // ans记录最小生成树的总权重，cnt记录已选边的数量
    for (int i = 0; i < m; i++) { // 遍历所有边
        if (unionSet(edges[i].u, edges[i].v)) { // 如果当前边的两个顶点不在同一个集合
            ans += edges[i].w; // 将该边加入最小生成树，累加权重
            cnt++;             // 已选边数量加1
            if (cnt == n - 1) break; // 如果已选边数达到n-1，说明最小生成树已经构建完成
        }
    }

    // 如果已选边数等于n-1，输出最小生成树的总权重；否则输出"orz"
    if (cnt == n - 1) printf("%d", ans); // 输出最小生成树的总权重
    else printf("orz");                  // 如果无法构成最小生成树，输出"orz"

    return 0;
}
