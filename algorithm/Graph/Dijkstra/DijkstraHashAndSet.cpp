
// 获取距离最小且未被访问的节点
Node* getMinDistanceAndUnselectedNode(const std::unordered_map<Node*, int>& distanceMap,
                                       const std::unordered_set<Node*>& touchedNodes) {
    Node* minNode = nullptr; // 初始化最小节点为nullptr
    int minDistance = std::numeric_limits<int>::max(); // 初始化最小距离为最大整数
    for (const auto& entry : distanceMap) { // 遍历所有节点及其距离
        Node* node = entry.first; // 获取节点
        int distance = entry.second; // 获取节点到起点的距离
        if (touchedNodes.find(node) == touchedNodes.end() && distance < minDistance) { // 如果节点未被访问且距离更小
            minNode = node; // 更新最小节点
            minDistance = distance; // 更新最小距离
        }
    }
    return minNode; // 返回最小节点
}

// Dijkstra算法实现1，使用哈希表和集合
std::unordered_map<Node*, int> dijkstra1(Node* head) {
    // 存储节点到起点的距离，初始时起点节点的距离为0
    std::unordered_map<Node*, int> distanceMap;
    distanceMap[head] = 0;

    // 存储已选中的节点，即已经计算出最短路径的节点
    std::unordered_set<Node*> selectedNodes;

    // 获取距离最小且未被选中的节点
    Node* minNode = getMinDistanceAndUnselectedNode(distanceMap, selectedNodes);

    // 当存在未被选中的节点时，继续处理
    while (minNode != nullptr) {
        // 获取当前节点到起点的距离
        int distance = distanceMap[minNode];

        // 遍历当前节点的所有边
        for (const Edge& edge : minNode->edges) {
            Node* toNode = edge.to; // 边指向的目标节点
            int newDistance = distance + edge.weight; // 通过当前节点到达目标节点的距离

            // 如果目标节点不在distanceMap中，说明是第一次访问该节点，设置其距离
            if (distanceMap.find(toNode) == distanceMap.end()) {
                distanceMap[toNode] = newDistance;
            } else {
                // 如果目标节点已经在distanceMap中，更新其距离为更小的那个
                distanceMap[toNode] = std::min(distanceMap[toNode], newDistance);
            }
        }

        // 将当前节点标记为已选中
        selectedNodes.insert(minNode);

        // 获取下一个距离最小且未被选中的节点
        minNode = getMinDistanceAndUnselectedNode(distanceMap, selectedNodes);
    }

    // 返回从起点到所有节点的最短路径距离
    return distanceMap;
}

