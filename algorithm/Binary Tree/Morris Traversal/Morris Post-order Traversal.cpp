// 反转从from节点开始的链表
Node* reverseEdge(Node* from) {
    Node* pre = nullptr; // 前一个节点指针
    Node* next = nullptr; // 后一个节点指针
    while (from != nullptr) {
        next = from->right; // 保存当前节点的右子节点
        from->right = pre; // 将当前节点的右子节点指向前一个节点
        pre = from; // 更新前一个节点为当前节点
        from = next; // 移动到下一个节点
    }
    return pre; // 返回反转后的链表头节点
}

// 打印从head节点开始的链表
void printEdge(Node* head) {
    Node* tail = reverseEdge(head); // 反转链表并获取尾节点
    Node* cur = tail;
    while (cur != nullptr) {
        cout << cur->value << " "; // 打印当前节点的值
        cur = cur->right; // 移动到下一个节点
    }
    reverseEdge(tail); // 再次反转链表以恢复原结构
}

// Morris后序遍历算法
void morrisPos(Node* head) {
    if (head == nullptr) {
        return; // 如果头节点为空，直接返回
    }
    Node* cur1 = head; // 当前节点指针
    Node* cur2 = nullptr; // 辅助节点指针，用于找到当前节点左子树的最右节点
    while (cur1 != nullptr) {
        cur2 = cur1->left; // 初始化cur2为当前节点的左子节点
        if (cur2 != nullptr) {
            // 找到cur1左子树的最右节点
            while (cur2->right != nullptr && cur2->right != cur1) {
                cur2 = cur2->right;
            }
            // 如果最右节点的右指针为空，建立连接
            if (cur2->right == nullptr) {
                cur2->right = cur1;
                cur1 = cur1->left;
                continue;
            } else { // 如果最右节点的右指针指向cur1，断开连接并打印左子树的后序遍历
                cur2->right = nullptr;
                printEdge(cur1->left); // 打印左子树的后序遍历
            }
        }
        cur1 = cur1->right; // 移动到右子树
    }
    printEdge(head); // 打印整棵树的后序遍历
    cout << endl; // 打印换行符
}
