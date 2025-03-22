// 后序遍历（递归）
void posOrderRecur(Node* head) {
    // 如果当前节点为空，直接返回
    if (head == nullptr) {
        return;
    }
    // 递归遍历左子树
    posOrderRecur(head->left);
    // 递归遍历右子树
    posOrderRecur(head->right);
    // 输出当前节点的值
    std::cout << head->value << " ";
}
