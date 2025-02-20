// 前序遍历（非递归）
void preOrderUnRecur(Node* head) {
    std::cout << "pre-order: ";
    // 如果根节点为空，直接返回
    if (head != nullptr) {
        // 创建一个栈，用于存储待访问的节点
        std::stack<Node*> stack;
        // 将根节点压入栈中
        stack.push(head);
        // 当栈不为空时，继续遍历
        while (!stack.empty()) {
            // 从栈顶弹出一个节点
            Node* current = stack.top();
            stack.pop();
            // 输出当前节点的值
            std::cout << current->value << " ";
            // 如果当前节点有右子节点，将其压入栈中
            if (current->right != nullptr) {
                stack.push(current->right);
            }
            // 如果当前节点有左子节点，将其压入栈中
            if (current->left != nullptr) {
                stack.push(current->left);
            }
        }
    }
    std::cout << std::endl;
}

