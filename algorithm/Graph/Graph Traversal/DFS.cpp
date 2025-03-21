
// 定义节点结构体，包含一个整数值和一个指向其他节点的指针向量
struct Node {
    int value;          // 节点的值
    std::vector<Node*> nexts; // 指向相邻节点的指针向量
};

// 深度优先搜索函数，参数为指向起始节点的指针
void dfs(Node* node) {
    // 如果起始节点为空，直接返回
    if (node == nullptr) {
        return;
    }

    // 使用栈来辅助进行深度优先搜索
    std::stack<Node*> stack;
    // 使用无序集合来记录已经访问过的节点，避免重复访问
    std::unordered_set<Node*> set;

    // 将起始节点压入栈，并标记为已访问
    stack.push(node);
    set.insert(node);

    // 输出起始节点的值
    std::cout << node->value << std::endl;

    // 当栈不为空时，继续进行搜索
    while (!stack.empty()) {
        // 获取栈顶节点并弹出栈
        Node* cur = stack.top();
        stack.pop();

        // 遍历当前节点的所有相邻节点
        for (Node* next : cur->nexts) {
            // 如果相邻节点未被访问过
            if (set.find(next) == set.end()) {
                // 将当前节点重新压入栈（以便回溯）
                stack.push(cur);
                // 将相邻节点压入栈
                stack.push(next);
                // 标记相邻节点为已访问
                set.insert(next);
                // 输出相邻节点的值
                std::cout << next->value << std::endl;
                // 找到一个未访问的相邻节点后，跳出循环，继续处理该节点
                break;
            }
        }
    }
}

unordered_set<Node*> set2;
void dfs(Node* node) {
    if (node == nullptr) {
        return;
    }
    cout << node->value;
    set2.insert(node);
    for (auto next : node->nexts) {
        if (set2.find(next) == set2.end()) {
            dfs(next);

        }
    }
}
