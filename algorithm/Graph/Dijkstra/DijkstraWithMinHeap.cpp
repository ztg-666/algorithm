

// 定义节点记录结构，包含节点指针和从起点到该节点的距离
struct NodeRecord {
    Node* node;
    int distance;

    NodeRecord(Node* node, int distance) : node(node), distance(distance) {}
};

// 定义一个最小堆结构，用于Dijkstra算法中选择距离最小的节点
struct NodeHeap {
private:
    std::vector<Node*> nodes; // 存储节点的数组
    std::unordered_map<Node*, int> heapIndexMap; // 记录节点在堆中的索引
    std::unordered_map<Node*, int> distanceMap; // 记录节点到起点的距离
    int size; // 堆中当前节点的数量

public:
    // 构造函数，初始化堆的大小
    NodeHeap(int size) : size(0) {
        nodes.resize(size);
    }

    // 判断堆是否为空
    bool isEmpty() const {
        return size == 0;
    }

    // 添加、更新或忽略节点
    void addOrUpdateOrIgnore(Node* node, int distance) {
        if (inHeap(node)) { // 如果节点已经在堆中
            distanceMap[node] = std::min(distanceMap[node], distance); // 更新距离为更小的那个
            insertHeapify(node, heapIndexMap[node]); // 重新调整堆
        }
        if (!isEntered(node)) { // 如果节点没有进入过堆
            nodes[size] = node; // 将节点添加到堆中
            heapIndexMap[node] = size; // 记录节点在堆中的索引
            distanceMap[node] = distance; // 记录节点到起点的距离
            insertHeapify(node, size++); // 重新调整堆，并增加堆的大小
        }
    }

    // 弹出堆顶节点，即距离最小的节点
    NodeRecord pop() {
        NodeRecord nodeRecord(nodes[0], distanceMap[nodes[0]]); // 记录堆顶节点及其距离
        swap(0, size - 1); // 将堆顶节点与最后一个节点交换
        heapIndexMap[nodes[size - 1]] = -1; // 标记最后一个节点已移除
        distanceMap.erase(nodes[size - 1]); // 删除最后一个节点的距离记录
        nodes[size - 1] = nullptr; // 将最后一个节点置为空
        heapify(0, --size); // 重新调整堆
        return nodeRecord; // 返回堆顶节点及其距离
    }

private:
    // 向上调整堆
    void insertHeapify(Node* node, int index) {
        while (distanceMap[nodes[index]] < distanceMap[nodes[(index - 1) / 2]]) { // 如果当前节点的距离小于父节点的距离
            swap(index, (index - 1) / 2); // 交换当前节点和父节点
            index = (index - 1) / 2; // 更新当前节点的索引为父节点的索引
        }
    }

    // 向下调整堆
    void heapify(int index, int size) {
        int left = index * 2 + 1; // 计算左子节点的索引
        while (left < size) { // 如果左子节点存在
            int smallest = left + 1 < size && distanceMap[nodes[left + 1]] < distanceMap[nodes[left]]
                           ? left + 1 : left; // 找到左右子节点中距离最小的那个
            smallest = distanceMap[nodes[smallest]] < distanceMap[nodes[index]] ? smallest : index; // 如果子节点的距离小于当前节点的距离，则更新最小索引
            if (smallest == index) { // 如果最小索引没有变化，说明堆已经调整好
                break;
            }
            swap(smallest, index); // 交换当前节点和最小节点
            index = smallest; // 更新当前节点的索引为最小节点的索引
            left = index * 2 + 1; // 计算新的左子节点的索引
        }
    }

    // 判断节点是否进入过堆
    bool isEntered(Node* node) const {
        return heapIndexMap.find(node) != heapIndexMap.end();
    }

    // 判断节点是否在堆中
    bool inHeap(Node* node) const {
        if (!isEntered(node)) { // 如果节点没有进入过堆
            return false;
        }
        auto it = heapIndexMap.find(node);
        return it != heapIndexMap.end() && it->second != -1; // 如果节点在堆中且没有被移除
    }

    // 交换堆中的两个节点
    void swap(int index1, int index2) {
        heapIndexMap[nodes[index1]] = index2; // 更新节点1在堆中的索引
        heapIndexMap[nodes[index2]] = index1; // 更新节点2在堆中的索引
        std::swap(nodes[index1], nodes[index2]); // 交换节点1和节点2
    }
};


// 使用Dijkstra算法计算从起点到所有节点的最短距离
std::unordered_map<Node*, int> dijkstra2(Node* head, int size) {
    NodeHeap nodeHeap(size); // 创建一个最小堆
    nodeHeap.addOrUpdateOrIgnore(head, 0); // 将起点节点加入堆中，距离为0
    std::unordered_map<Node*, int> result; // 存储结果，即每个节点到起点的最短距离
    while (!nodeHeap.isEmpty()) { // 当堆不为空时
        NodeRecord record = nodeHeap.pop(); // 弹出堆顶节点，即距离最小的节点
        Node* cur = record.node; // 获取当前节点
        int distance = record.distance; // 获取当前节点到起点的距离
        for (const Edge& edge : cur->edges) { // 遍历当前节点的所有边
            nodeHeap.addOrUpdateOrIgnore(edge.to, edge.weight + distance); // 更新相邻节点的距离
        }
        result[cur] = distance; // 记录当前节点到起点的最短距离
    }
    return result; // 返回结果
}

