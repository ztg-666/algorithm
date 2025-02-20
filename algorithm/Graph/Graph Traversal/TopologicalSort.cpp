拓扑排序

// 定义图中的节点结构
struct Node {
    int in; // 节点的入度，即有多少条边指向该节点
    std::vector<Node*> nexts; // 该节点指向的邻接节点列表
};

// 定义图的结构
struct Graph {
    std::map<int, Node*> nodes; // 使用整数标识图中的节点，map的键为节点标识，值为节点指针
};

// 拓扑排序函数，返回一个节点指针的向量，表示拓扑排序的结果
std::vector<Node*> sortedTopology(Graph graph) {
    std::map<Node*, int> inMap; // 存储每个节点的入度
    std::queue<Node*> zeroInQueue; // 存储入度为0的节点队列

    // 遍历图中的所有节点，初始化入度映射表和入度为0的节点队列
    for (auto& pair : graph.nodes) {
        Node* node = pair.second;
        inMap[node] = node->in; // 将节点的入度存入映射表
        if (node->in == 0) {
            zeroInQueue.push(node); // 如果节点的入度为0，则将其加入队列
        }
    }

    std::vector<Node*> result; // 存储拓扑排序的结果

    // 当队列不为空时，执行拓扑排序
    while (!zeroInQueue.empty()) {
        Node* cur = zeroInQueue.front(); // 取出队列头部节点
        zeroInQueue.pop(); // 移除队列头部节点
        result.push_back(cur); // 将当前节点加入结果向量

        // 遍历当前节点的所有邻接节点，更新邻接节点的入度
        for (Node* next : cur->nexts) {
            inMap[next] = inMap[next] - 1; // 邻接节点的入度减1
            if (inMap[next] == 0) {
                zeroInQueue.push(next); // 如果邻接节点的入度变为0，则将其加入队列
            }
        }
    }

    return result; // 返回拓扑排序的结果
}

