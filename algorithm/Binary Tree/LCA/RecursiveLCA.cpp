//适用于需要快速实现且不需要频繁查询最低公共祖先的场景。空间复杂度较低，但每次查询的时间复杂度较高
Node* lowestAncestor(Node* node, Node* o1, Node* o2) {
    // 如果当前节点为空，或者当前节点等于o1或o2，直接返回当前节点
    if (node == nullptr || node == o1 || node == o2) {
        return node;
    }

    // 递归查找左子树中o1和o2的最低公共祖先
    Node* left = lowestAncestor(node->left, o1, o2);

    // 递归查找右子树中o1和o2的最低公共祖先
    Node* right = lowestAncestor(node->right, o1, o2);

    // 如果左子树和右子树都找到了o1和o2，说明当前节点是最低公共祖先
    if (left != nullptr && right != nullptr) {
        return node;
    }

    // 如果左子树找到了o1或o2，返回左子树的结果
    // 如果右子树找到了o1或o2，返回右子树的结果
    // 如果都没有找到，返回nullptr
    return left != nullptr ? left : right;
}
