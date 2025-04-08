#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

const int MAXN = 11;  // 最大节点数（支持从1到n编号）
const int MAXM = 21;  // 最大边数（无向图需要×2，因为每条边存储两次）

// 邻接矩阵：graph1[i][j] 表示从节点i到节点j的边权值，0表示无边
int graph1[MAXN][MAXN];

// 邻接表：graph2[i] 存储节点i的所有邻居及其边权值，使用pair<int, int>表示(目标节点, 权重)
vector<vector<pair<int, int>>> graph2;

// 链式前向星存储结构：
int head[MAXN];   // head[i] 表示以节点i为起点的第一条边在链式前向星中的索引
int next_[MAXM];  // next_[i] 表示与第i条边同起点的下一条边的索引
int to[MAXM];     // to[i] 表示第i条边指向的目标节点
int weight[MAXM]; // weight[i] 表示第i条边的权重
int cnt;          // cnt 记录当前边的计数器，用于分配边的唯一索引

// 初始化图结构
void build(int n) {
    // 初始化邻接矩阵：将所有元素置为0，表示初始时没有边
    memset(graph1, 0, sizeof(graph1));

    // 初始化邻接表：清空并调整大小为n+1（支持节点编号从1到n）
    graph2.clear();
    graph2.resize(n + 1);

    // 初始化链式前向星：
    // cnt从1开始，head数组置为0，表示每个节点目前没有边
    cnt = 1;
    memset(head, 0, sizeof(head));
}

// 向链式前向星中添加一条边
void addEdge(int u, int v, int w) {
    // 将新边插入到链式前向星中：
    // next_[cnt] 指向前一条边（即head[u]），形成链表结构
    next_[cnt] = head[u];
    to[cnt] = v;         // 设置目标节点
    weight[cnt] = w;     // 设置边权重
    head[u] = cnt++;     // 更新head[u]为当前边的索引，并递增计数器
}

// 构建有向图
void directGraph(vector<vector<int>>& edges) {
    // 使用邻接矩阵建图：
    // graph1[edge[0]][edge[1]] = edge[2] 表示从edge[0]到edge[1]有一条权重为edge[2]的边
    for (auto& edge : edges) {
        graph1[edge[0]][edge[1]] = edge[2];
    }

    // 使用邻接表建图：
    // graph2[edge[0]].emplace_back(edge[1], edge[2]) 表示从edge[0]到edge[1]有一条权重为edge[2]的边
    for (auto& edge : edges) {
        graph2[edge[0]].emplace_back(edge[1], edge[2]);
    }

    // 使用链式前向星建图：
    // 调用addEdge函数依次添加每条边
    for (auto& edge : edges) {
        addEdge(edge[0], edge[1], edge[2]);
    }
}

// 构建无向图
void undirectGraph(vector<vector<int>>& edges) {
    // 使用邻接矩阵建图：
    // 无向图需要双向存储，因此同时设置graph1[edge[0]][edge[1]]和graph1[edge[1]][edge[0]]
    for (auto& edge : edges) {
        graph1[edge[0]][edge[1]] = edge[2];
        graph1[edge[1]][edge[0]] = edge[2];
    }

    // 使用邻接表建图：
    // 无向图需要双向存储，因此同时添加edge[0]->edge[1]和edge[1]->edge[0]
    for (auto& edge : edges) {
        graph2[edge[0]].emplace_back(edge[1], edge[2]);
        graph2[edge[1]].emplace_back(edge[0], edge[2]);
    }

    // 使用链式前向星建图：
    // 无向图需要双向存储，因此调用两次addEdge函数
    for (auto& edge : edges) {
        addEdge(edge[0], edge[1], edge[2]);
        addEdge(edge[1], edge[0], edge[2]);
    }
}

// 遍历并输出三种图的存储结构
void traversal(int n) {
    // 输出邻接矩阵：
    cout << "Adjacency Matrix Traversal:" << endl;
    for (int i = 1; i <= n; ++i) {  // 遍历所有节点
        for (int j = 1; j <= n; ++j) {  // 遍历每个节点的邻居
            cout << graph1[i][j] << " ";  // 输出边权值
        }
        cout << endl;
    }

    // 输出邻接表：
    cout << "Adjacency List Traversal::" << endl;
    for (int i = 1; i <= n; ++i) {  // 遍历所有节点
        cout << i << "(Neighbor, Weight): ";  // 当前节点编号
        for (auto& [v, w] : graph2[i]) {  // 遍历当前节点的所有邻居
            cout << "(" << v << "," << w << ") ";  // 输出邻居节点及边权值
        }
        cout << endl;
    }

    // 输出链式前向星：
    cout << "Chain Forward Star:" << endl;
    for (int i = 1; i <= n; ++i) {  // 遍历所有节点
        cout << i << "(Neighbor, Weight): ";  // 当前节点编号
        for (int ei = head[i]; ei != 0; ei = next_[ei]) {  // 遍历链式前向星中的边
            cout << "(" << to[ei] << "," << weight[ei] << ") ";  // 输出目标节点及边权值
        }
        cout << endl;
    }
}

int main() {
    // 测试有向图
    int n1 = 4;  // 节点数
    vector<vector<int>> edges1 = {{1,3,6}, {4,3,4}, {2,4,2}, {1,2,7}, {2,3,5}, {3,1,1}};  // 边列表
    build(n1);  // 初始化图结构
    directGraph(edges1);  // 构建有向图
    traversal(n1);  // 遍历并输出图结构

    cout << "==============================" << endl;

    // 测试无向图
    int n2 = 5;  // 节点数
    vector<vector<int>> edges2 = {{3,5,4}, {4,1,1}, {3,4,2}, {5,2,4}, {2,3,7}, {1,5,5}, {4,2,6}};  // 边列表
    build(n2);  // 初始化图结构
    undirectGraph(edges2);  // 构建无向图
    traversal(n2);  // 遍历并输出图结构

    return 0;
}
