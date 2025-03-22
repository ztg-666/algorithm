// 中序遍历（非递归）
void inOrderUnRecur(Node* head) {
    std::cout << "in-order: ";
    // 如果根节点为空，直接返回
    if (head != nullptr) {
        // 创建一个栈，用于存储待访问的节点
        std::stack<Node*> stack;
        // 当前节点初始化为根节点
        Node* current = head;
        // 当栈不为空或当前节点不为空时，继续遍历
        while (!stack.empty() || current != nullptr) {
            // 如果当前节点不为空，将其压入栈中，并移动到左子节点
            if (current != nullptr) {
                stack.push(current);
                current = current->left;
            } else {
                // 如果当前节点为空，从栈顶弹出一个节点
                current = stack.top();
                stack.pop();
                // 输出当前节点的值
                std::cout << current->value << " ";
                // 移动到右子节点
                current = current->right;
            }
        }
    }
    std::cout << std::endl;
}

