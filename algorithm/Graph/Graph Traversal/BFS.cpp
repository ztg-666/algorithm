
// 定义图中的节点结构体
struct Node {
    int value;                // 节点的值
    std::vector<Node*> nexts; // 指向相邻节点的指针向量
};

// 实现广度优先搜索（BFS）遍历图结构的函数
void bfs(Node* node) {
    // 如果传入的节点为空，则直接返回
    if (node == nullptr) {
        return;
    }

    // 使用队列来存储待访问的节点
    std::queue<Node*> queue;
    // 使用无序集合来记录已访问过的节点，防止重复访问
    std::unordered_set<Node*> map;

    // 将起始节点加入队列和已访问集合
    queue.push(node);
    map.insert(node);

    // 进入循环，直到队列为空
    while (!queue.empty()) {
        // 取出队列头部的节点
        Node* cur = queue.front();
        // 弹出队列头部的节点
        queue.pop();
        // 打印当前节点的值
        std::cout << cur->value << std::endl;

        // 遍历当前节点的所有邻接节点
        for (Node* next : cur->nexts) {
            // 如果该邻接节点未被访问过
            if (map.find(next) == map.end()) {
                // 将其加入已访问集合
                map.insert(next);
                // 将其加入队列
                queue.push(next);
            }
        }
    }
}
