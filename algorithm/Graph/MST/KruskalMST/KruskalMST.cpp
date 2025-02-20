
#include <unordered_map>
#include <vector>
#include <set>
#include <queue>

// 定义节点类，每个节点有一个整数值作为标识
class Node {
public:
    int value; // 节点的值
    Node(int v) : value(v) {} // 构造函数，初始化节点的值
};

// 定义边类，每条边连接两个节点，并有一个权重
class Edge {
public:
    Node* from; // 边的起始节点
    Node* to;   // 边的结束节点
    int weight; // 边的权重
    Edge(Node* f, Node* t, int w) : from(f), to(t), weight(w) {} // 构造函数，初始化边的起始节点、结束节点和权重

    // 声明友元函数
    friend bool compareEdges(const Edge* o1, const Edge* o2);
};

// 实现友元函数，用于优先队列，按边的权重升序排列
bool compareEdges(const Edge* o1, const Edge* o2) {
    return o1->weight > o2->weight;
}

// 定义并查集类，用于管理节点的集合
class UnionFind {
private:
    std::unordered_map<Node*, Node*> fatherMap; // 记录每个节点的父节点
    std::unordered_map<Node*, int> rankMap;     // 记录每个节点的秩（树的高度）

public:
    UnionFind() {} // 构造函数

    // 查找节点的根节点，并进行路径压缩
    Node* findFather(Node* n) {
        Node* father = fatherMap[n];
        if (father != n) {
            father = findFather(father);
        }
        fatherMap[n] = father;
        return father;
    }

    // 初始化并查集，将每个节点的父节点设为自己，秩设为1
    void makeSets(const std::vector<Node*>& nodes) {
        fatherMap.clear();
        rankMap.clear();
        for (Node* node : nodes) {
            fatherMap[node] = node;
            rankMap[node] = 1;
        }
    }

    // 判断两个节点是否属于同一个集合
    bool isSameSet(Node* a, Node* b) {
        return findFather(a) == findFather(b);
    }

    // 合并两个节点所在的集合
    void unionNodes(Node* a, Node* b) {
        if (a == nullptr || b == nullptr) {
            return;
        }
        Node* aFather = findFather(a);
        Node* bFather = findFather(b);
        if (aFather != bFather) {
            int aFrank = rankMap[aFather];
            int bFrank = rankMap[bFather];
            if (aFrank <= bFrank) {
                fatherMap[aFather] = bFather;
                rankMap[bFather] = aFrank + bFrank;
            } else {
                fatherMap[bFather] = aFather;
                rankMap[aFather] = aFrank + bFrank;
            }
        }
    }
};

// 定义图类，包含节点和边的集合
class Graph {
public:
    std::unordered_map<int, Node*> nodes; // 节点集合，键为节点的值，值为节点指针
    std::vector<Edge*> edges;             // 边集合
};

// 使用Kruskal算法计算图的最小生成树
std::set<Edge*> kruskalMST(Graph graph) {
    UnionFind unionFind; // 创建并查集对象
    std::vector<Node*> nodeVec;
    for (auto& pair : graph.nodes) {
        nodeVec.push_back(pair.second); // 将图中的所有节点放入向量中
    }
    unionFind.makeSets(nodeVec); // 初始化并查集

    // 使用友元函数作为比较函数
    std::priority_queue<Edge*, std::vector<Edge*>, bool(*)(const Edge*, const Edge*)> priorityQueue(compareEdges);

    // 将图中的所有边放入优先队列中
    for (Edge* edge : graph.edges) {
        priorityQueue.push(edge);
    }

    std::set<Edge*> result; // 用于存储最小生成树的边
    while (!priorityQueue.empty()) {
        Edge* edge = priorityQueue.top(); // 取出权重最小的边
        priorityQueue.pop();
        // 如果这条边的两个节点不在同一个集合中，则将其加入最小生成树，并合并这两个节点所在的集合
        if (!unionFind.isSameSet(edge->from, edge->to)) {
            result.insert(edge);
            unionFind.unionNodes(edge->from, edge->to);
        }
    }
    return result; // 返回最小生成树的边集合
}

