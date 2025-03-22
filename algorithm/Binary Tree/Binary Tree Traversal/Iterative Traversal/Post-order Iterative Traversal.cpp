// 后序遍历（非递归，使用两个栈）
void posOrderUnRecur1(Node* head) {
    std::cout << "pos-order: ";
    // 如果根节点为空，直接返回
    if (head != nullptr) {
        // 创建两个栈，s1用于辅助遍历，s2用于存储节点以便最终按后序输出
        std::stack<Node*> s1;
        std::stack<Node*> s2;
        // 将根节点压入s1
        s1.push(head);
        // 当s1不为空时，继续遍历
        while (!s1.empty()) {
            // 从s1的栈顶弹出一个节点
            Node* current = s1.top();
            s1.pop();
            // 将该节点压入s2
            s2.push(current);
            // 如果当前节点有左子节点，将其压入s1
            if (current->left != nullptr) {
                s1.push(current->left);
            }
            // 如果当前节点有右子节点，将其压入s1
            if (current->right != nullptr) {
                s1.push(current->right);
            }
        }
        // 当s2不为空时，继续输出节点的值
        while (!s2.empty()) {
            // 从s2的栈顶弹出一个节点并输出其值
            std::cout << s2.top()->value << " ";
            s2.pop();
        }
    }
    std::cout << std::endl;
}

// 后序遍历（非递归，使用一个栈）
void posOrderUnRecur2(Node* head) {
    std::cout << "pos-order: ";
    // 如果根节点为空，直接返回
    if (head != nullptr) {
        // 创建一个栈，用于辅助遍历
        std::stack<Node*> stack;
        // 将根节点压入栈中
        stack.push(head);
        // 当前节点初始化为nullptr
        Node* current = nullptr;
        // 记录最后访问的节点
        Node* lastVisited = nullptr;
        // 当栈不为空时，继续遍历
        while (!stack.empty()) {
            // 从栈顶获取一个节点但不弹出
            current = stack.top();
            // 如果当前节点有左子节点且左子节点和右子节点都不是最后访问的节点
            if (current->left != nullptr && lastVisited != current->left && lastVisited != current->right) {
                // 将左子节点压入栈中
                stack.push(current->left);
            } else if (current->right != nullptr && lastVisited != current->right) {
                // 如果当前节点有右子节点且右子节点不是最后访问的节点
                // 将右子节点压入栈中
                stack.push(current->right);
            } else {
                // 输出当前节点的值
                std::cout << current->value << " ";
                // 从栈中弹出当前节点
                stack.pop();
                // 记录当前节点为最后访问的节点
                lastVisited = current;
            }
        }
    }
    std::cout << std::endl;
}
