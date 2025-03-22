void morrisPre(Node* head) {
    if (head == nullptr) {
        return; // 如果头节点为空，直接返回
    }
    Node* cur = head; // 当前节点指针
    Node* mostRight = nullptr; // 辅助节点指针，用于找到当前节点左子树的最右节点
    while (cur != nullptr) {
        mostRight = cur->left; // 初始化mostRight为当前节点的左子节点
        if (mostRight != nullptr) {
            // 找到cur左子树的最右节点
            while (mostRight->right != nullptr && mostRight->right != cur) {
                mostRight = mostRight->right;
            }
            // 如果最右节点的右指针为空，建立连接并访问当前节点
            if (mostRight->right == nullptr) {
                mostRight->right = cur;
                cout << cur->value << " "; // 访问当前节点
                cur = cur->left; // 移动到左子树
                continue;
            } else { // 如果最右节点的右指针指向cur，断开连接
                mostRight->right = nullptr;
            }
        } else {
            // 如果没有左子树，直接访问当前节点
            cout << cur->value << " ";
        }
        // 移动到右子树
        cur = cur->right;
    }
    cout << endl; // 打印换行符
}

