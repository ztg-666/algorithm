#include<bits/stdc++.h>
using namespace std;

// 定义二叉树节点结构
struct TreeNode {
    int val;          // 节点的值
    TreeNode* left;   // 指向左子节点的指针
    TreeNode* right;  // 指向右子节点的指针
    // 构造函数，初始化节点值，左右子节点指针为空
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

// 函数声明：修剪二叉搜索树，使其所有节点的值在[low, high]范围内
// 参数：
//   cur: 当前处理的节点
//   low: 范围的下界
//   high: 范围的上界
// 返回值：修剪后的子树的根节点
TreeNode* trimBST(TreeNode* cur, int low, int high) {
    // 如果当前节点为空，直接返回空指针
    if (cur == nullptr) {
        return nullptr;
    }
    // 如果当前节点的值小于范围的下界，说明当前节点及其左子树都不在范围内
    // 因此，返回修剪后的右子树
    if (cur->val < low) {
        return trimBST(cur->right, low, high);
    }
    // 如果当前节点的值大于范围的上界，说明当前节点及其右子树都不在范围内
    // 因此，返回修剪后的左子树
    if (cur->val > high) {
        return trimBST(cur->left, low, high);
    }
    // 当前节点的值在范围内，递归修剪其左右子树
    cur->left = trimBST(cur->left, low, high);
    cur->right = trimBST(cur->right, low, high);
    // 返回当前节点作为修剪后子树的根节点
    return cur;
}

// 辅助函数：中序遍历打印二叉树
void inorderTraversal(TreeNode* root) {
    if (root == nullptr) {
        return;
    }
    inorderTraversal(root->left);
    cout << root->val << " ";
    inorderTraversal(root->right);
}
// 释放二叉树内存的辅助函数
void freeTree(TreeNode* root) {
    if (!root) return; // 如果当前节点为空，直接返回
    freeTree(root->left); // 递归释放左子树
    freeTree(root->right); // 递归释放右子树
    delete root; // 删除当前节点
}
int main() {
    // 创建一个示例二叉搜索树
    TreeNode* root = new TreeNode(10);
    root->left = new TreeNode(5);
    root->right = new TreeNode(15);
    root->left->left = new TreeNode(3);
    root->left->right = new TreeNode(7);
    root->right->right = new TreeNode(18);

    cout << "Original BST (Inorder): ";
    inorderTraversal(root);
    cout << endl;

    // 定义修剪范围
    int low = 3;
    int high = 10;

    // 修剪二叉搜索树
    TreeNode* trimmedRoot = trimBST(root, low, high);

    cout << "Trimmed BST (Inorder): ";
    inorderTraversal(trimmedRoot);
    cout << endl;

    // 释放内存（在实际应用中，建议使用智能指针来管理内存）
    // 这里为了简单起见，手动释放内存
    freeTree(root);

    return 0;
}