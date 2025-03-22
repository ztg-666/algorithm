// 前序遍历（递归）
void preOrderRecur(Node* head) {
    // 如果当前节点为空，直接返回
    if (head == nullptr) {
        return;
    }
    // 输出当前节点的值
    std::cout << head->value << " ";
    // 递归遍历左子树
    preOrderRecur(head->left);
    // 递归遍历右子树
    preOrderRecur(head->right);
}
