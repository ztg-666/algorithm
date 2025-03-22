#include <iostream>
#include <cmath>

// 定义树节点结构
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};
// 计算从cur节点开始，不断向左走能到达的最大层数
int mostLeft(TreeNode* cur, int level) {
    while (cur != nullptr) {
        level++;
        cur = cur->left;
    }
    return level - 1;
}
// 计算以cur为根节点的子树的节点个数
int f(TreeNode* cur, int level, int h) {
    if (level == h) {
        return 1;
    }
    if (mostLeft(cur->right, level + 1) == h) {
        // cur右子树上的最左节点，扎到了最深层
        return (1 << (h - level)) + f(cur->right, level + 1, h);
    } else {
        // cur右子树上的最左节点，没扎到最深层
        return (1 << (h - level - 1)) + f(cur->left, level + 1, h);
    }
}

// 计算完全二叉树的节点个数
int countNodes(TreeNode* head) {
    if (head == nullptr) {
        return 0;
    }
    return f(head, 1, mostLeft(head, 1));
}

void freeTree(TreeNode* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}
int main() {
    // 示例用法
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    root->right->left = new TreeNode(6);

    std::cout << "Number of nodes in the complete binary tree: " << countNodes(root) << std::endl;

    // 释放内存
    freeTree(root);
    return 0;
}
