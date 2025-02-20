
// 定义边比较器，用于优先队列，按权重从小到大排序
struct EdgeComparator {
    bool operator()(const Edge* e1, const Edge* e2) {
        return e1->weight > e2->weight;
    }
};
// 使用Prim算法计算最小生成树，返回边的集合
set<Edge*> primMST(Graph graph) {
    // 优先队列用于存储边，按权重从小到大排序
    priority_queue<Edge*, vector<Edge*>, EdgeComparator> priorityQueue;
    // 集合用于记录已访问的节点
    set<int> visited;
    // 集合用于存储最小生成树的边
    set<Edge*> result;

    // 遍历图中的每个节点
    for (Node& node : graph.nodes) {
        // 如果节点未被访问过
        if (visited.find(node.id) == visited.end()) {
            // 标记节点为已访问
            visited.insert(node.id);
            // 将该节点的所有边加入优先队列
            for (Edge* edge : node.edges) {
                priorityQueue.push(edge);
            }
            // 当优先队列不为空时，继续处理
            while (!priorityQueue.empty()) {
                // 取出权重最小的边
                Edge* edge = priorityQueue.top();
                priorityQueue.pop();
                int toNode = edge->to;
                // 如果边的终点节点未被访问过
                if (visited.find(toNode) == visited.end()) {
                    // 标记终点节点为已访问
                    visited.insert(toNode);
                    // 将该边加入最小生成树结果集合
                    result.insert(edge);
                    // 将终点节点的所有边加入优先队列
                    for (Edge* nextEdge : graph.nodes[toNode].edges) {
                        priorityQueue.push(nextEdge);
                    }
                }
            }
        }
    }
    // 返回最小生成树的边集合
    return result;
}


