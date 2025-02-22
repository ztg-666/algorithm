#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MAXN = 11; // 点的最大数量
const int MAXM = 21; // 边的最大数量

int graph1[MAXN][MAXN]; // 邻接矩阵方式建图
vector<vector<pair<int, int>>> graph2; // 邻接表方式建图
int head[MAXN]; // 链式前向星方式建图
int nexts[MAXM];
int to[MAXM];
int weight[MAXM];
int cnt;

void build(int n) {
    // 邻接矩阵清空
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            graph1[i][j] = 0;
        }
    }
    // 邻接表清空和准备
    graph2.clear();
    graph2.resize(n + 1); // 下标需要支持1~n，所以加入n+1个列表，0下标准备但不用
    // 链式前向星清空
    cnt = 1;
    fill(head + 1, head + n + 1, 0);
}

// 链式前向星加边
void addEdge(int u, int v, int w) {
    // u -> v , 边权重是w
    nexts[cnt] = head[u];
    to[cnt] = v;
    weight[cnt] = w;
    head[u] = cnt++;
}

// 三种方式建立有向图带权图
void directGraph(const vector<vector<int>>& edges) {
    // 邻接矩阵建图
    for (const auto& edge : edges) {
        graph1[edge[0]][edge[1]] = edge[2];
    }
    // 邻接表建图
    for (const auto& edge : edges) {
        graph2[edge[0]].emplace_back(edge[1], edge[2]);
    }
    // 链式前向星建图
    for (const auto& edge : edges) {
        addEdge(edge[0], edge[1], edge[2]);
    }
}

// 三种方式建立无向图带权图
void undirectGraph(const vector<vector<int>>& edges) {
    // 邻接矩阵建图
    for (const auto& edge : edges) {
        graph1[edge[0]][edge[1]] = edge[2];
        graph1[edge[1]][edge[0]] = edge[2];
    }
    // 邻接表建图
    for (const auto& edge : edges) {
        graph2[edge[0]].emplace_back(edge[1], edge[2]);
        graph2[edge[1]].emplace_back(edge[0], edge[2]);
    }
    // 链式前向星建图
    for (const auto& edge : edges) {
        addEdge(edge[0], edge[1], edge[2]);
        addEdge(edge[1], edge[0], edge[2]);
    }
}

void traversal(int n) {
    cout << "邻接矩阵遍历 :" << endl;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            cout << graph1[i][j] << " ";
        }
        cout << endl;
    }
    cout << "邻接表遍历 :" << endl;
    for (int i = 1; i <= n; i++) {
        cout << i << "(邻居、边权) : ";
        for (const auto& edge : graph2[i]) {
            cout << "(" << edge.first << "," << edge.second << ") ";
        }
        cout << endl;
    }
    cout << "链式前向星 :" << endl;
    for (int i = 1; i <= n; i++) {
        cout << i << "(邻居、边权) : ";
        // 注意这个for循环，链式前向星的方式遍历
        for (int ei = head[i]; ei > 0; ei = nexts[ei]) {
            cout << "(" << to[ei] << "," << weight[ei] << ") ";
        }
        cout << endl;
    }
}

int main() {
    // 理解了带权图的建立过程，也就理解了不带权图
    // 点的编号为1...n
    // 例子1自己画一下图，有向带权图，然后打印结果
    int n1 = 4;
    vector<vector<int>> edges1 = { { 1, 3, 6 }, { 4, 3, 4 }, { 2, 4, 2 }, { 1, 2, 7 }, { 2, 3, 5 }, { 3, 1, 1 } };
    build(n1);
    directGraph(edges1);
    traversal(n1);
    cout << "==============================" << endl;
    // 例子2自己画一下图，无向带权图，然后打印结果
    int n2 = 5;
    vector<vector<int>> edges2 = { { 3, 5, 4 }, { 4, 1, 1 }, { 3, 4, 2 }, { 5, 2, 4 }, { 2, 3, 7 }, { 1, 5, 5 }, { 4, 2, 6 } };
    build(n2);
    undirectGraph(edges2);
    traversal(n2);
    return 0;
}
