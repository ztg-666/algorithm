void morris(Node* head) {
    if (head == nullptr) {
        return; // 如果头节点为空，直接返回
    }
    Node* cur = head; // 当前节点指针
    Node* mostRight = nullptr; // 辅助节点指针，用于找到当前节点左子树的最右节点
    while (cur != nullptr) {
        mostRight = cur->left; // 初始化mostRight为当前节点的左子节点
        if (mostRight != nullptr) {
            // 找到cur1左子树的最右节点
            while (mostRight->right != nullptr && mostRight->right != cur) {
                mostRight = mostRight->right;
            }
            // 如果最右节点的右指针为空，建立连接并移动到左子树
            if (mostRight->right == nullptr) {
                mostRight->right = cur;
                cur = cur->left;
                continue;
            } else { // 如果最右节点的右指针指向cur1，断开连接
                mostRight->right = nullptr;
            }
        }
        // 移动到右子树
        cur = cur->right;
    }
}
