#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

// 定义常量：最大交叉路口数和最大道路数
const int MAXN = 301; // 最大交叉路口数量
const int MAXM = 8001; // 最大道路数量

// 定义结构体表示一条边（道路）
struct Edge {
    int u, v, w; // u和v是两个交叉路口，w是道路的分值
    // 重载小于运算符，用于按分值从小到大排序
    bool operator<(const Edge& other) const {
        return w < other.w;
    }
} edges[MAXM]; // 存储所有道路的数组

int father[MAXN]; // 并查集数组，用于维护连通性

// 初始化并查集，每个点初始时是自己的父节点
void build(int n) {
    for (int i = 1; i <= n; ++i) {
        father[i] = i; // 每个交叉路口单独作为一个集合
    }
}

// 查找某个点所在集合的根节点，并进行路径压缩
int find(int x) {
    if (x != father[x]) { // 如果当前节点不是根节点
        father[x] = find(father[x]); // 路径压缩，直接连接到根节点
    }
    return father[x]; // 返回根节点
}

// 合并两个集合，返回是否成功合并
bool unionSet(int x, int y) {
    int fx = find(x); // 找到x所在集合的根节点
    int fy = find(y); // 找到y所在集合的根节点
    if (fx != fy) { // 如果两个集合不同
        father[fx] = fy; // 将fx集合合并到fy集合
        return true; // 合并成功
    }
    return false; // 合并失败（已经在同一个集合中）
}
// 繁忙的都市
// 一个非常繁忙的大都市，城市中的道路十分的拥挤，于是市长决定对其中的道路进行改造
// 城市的道路是这样分布的：城市中有n个交叉路口，有些交叉路口之间有道路相连
// 两个交叉路口之间最多有一条道路相连接，这些道路是双向的
// 且把所有的交叉路口直接或间接的连接起来了
// 每条道路都有一个分值，分值越小表示这个道路越繁忙，越需要进行改造
// 但是市政府的资金有限，市长希望进行改造的道路越少越好，于是他提出下面的要求：
// 1. 改造的那些道路能够把所有的交叉路口直接或间接的连通起来
// 2. 在满足要求1的情况下，改造的道路尽量少
// 3. 在满足要求1、2的情况下，改造的那些道路中分值最大的道路分值尽量小
// 作为市规划局的你，应当作出最佳的决策，选择哪些道路应当被修建
// 返回选出了几条道路 以及 分值最大的那条道路的分值是多少
// 测试链接 : https://www.luogu.com.cn/problem/P2330

int main() {
    int n, m; // n: 交叉路口数，m: 道路数
    while (cin >> n >> m) { // 输入交叉路口数和道路数
        build(n); // 初始化并查集

        // 输入每条道路的信息
        for (int i = 0; i < m; ++i) {
            cin >> edges[i].u >> edges[i].v >> edges[i].w; // 输入起点、终点和分值
        }

        // 按照道路分值从小到大排序
        sort(edges, edges + m);

        int maxWeight = 0; // 记录改造的道路中分值最大的那条道路的分值
        int edgeCount = 0; // 记录已经选择的道路数量
        for (int i = 0; i < m; ++i) { // 遍历所有道路
            if (unionSet(edges[i].u, edges[i].v)) { // 如果这条道路连接了两个不同的集合
                maxWeight = max(maxWeight, edges[i].w); // 更新最大分值
                if (++edgeCount == n - 1) break; // 如果已经选择了n-1条道路，提前退出
            }
        }

        // 输出结果：选中的道路数量和分值最大的那条道路的分值
        cout << n - 1 << " " << maxWeight << endl;
    }
    return 0;
}
