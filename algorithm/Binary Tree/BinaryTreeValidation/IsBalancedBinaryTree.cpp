#include<bits/stdc++.h>

// 定义二叉树节点结构体
struct TreeNode {
    int val;          // 节点的值
    TreeNode* left;   // 指向左子节点的指针
    TreeNode* right;  // 指向右子节点的指针
    // 构造函数，初始化节点值和左右子节点指针
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// 全局变量，用于标记树是否平衡
bool balance;

// 计算树的高度，并检查树是否平衡
// 一旦发现不平衡，返回什么高度已经不重要了
int height(TreeNode* cur) {
    // 如果树已经不平衡或者当前节点为空，返回高度0
    if (!balance || cur == nullptr) {
        return 0;
    }
    // 递归计算左子树的高度
    int lh = height(cur->left);
    // 递归计算右子树的高度
    int rh = height(cur->right);
    // 如果左右子树高度差大于1，标记树不平衡
    if (abs(lh - rh) > 1) {
        balance = false;
    }
    // 返回当前节点的高度
    return std::max(lh, rh) + 1;
}

// 判断二叉树是否平衡
bool isBalanced(TreeNode* root) {
    // 每次判断开始时，设置balance为true
    balance = true;
    // 调用height函数计算树的高度并检查平衡性
    height(root);
    // 返回balance的值，表示树是否平衡
    return balance;
}
void freeTree(TreeNode* root) {
    if (!root) return; // 如果当前节点为空，直接返回
    freeTree(root->left); // 递归释放左子树
    freeTree(root->right); // 递归释放右子树
    delete root; // 删除当前节点
}
// 主函数，用于测试isBalanced函数
int main() {
    // 创建一个示例二叉树
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    root->left->left->left = new TreeNode(6);

    // 检查二叉树是否平衡
    if (isBalanced(root)) {
        std::cout << "The tree is balanced." << std::endl;
    } else {
        std::cout << "The tree is not balanced." << std::endl;
    }
    freeTree(root);
    return 0;
}
