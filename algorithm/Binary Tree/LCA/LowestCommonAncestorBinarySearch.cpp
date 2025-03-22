#include <bits/stdc++.h>
using namespace std;

// 定义二叉树节点结构体 TreeNode
struct TreeNode {
    int val; // 节点值
    TreeNode* left; // 左子节点指针
    TreeNode* right; // 右子节点指针
    TreeNode(int x) : val(x), left(NULL), right(NULL) {} // 构造函数初始化节点值和左右指针
};

// 查找最低公共祖先（LCA）的函数
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    while (root->val != p->val && root->val != q->val) { // 当前节点不是 p 或 q
        if (std::min(p->val, q->val) < root->val && root->val < std::max(p->val, q->val)) {
            break; // 如果 p 和 q 分别位于当前节点的两侧，则当前节点就是 LCA
        }
        root = root->val < std::min(p->val, q->val) ? root->right : root->left; // 根据 BST 性质移动到左子树或右子树
    }
    return root; // 返回找到的最低公共祖先节点
}

// 辅助函数：将一个值插入到二叉搜索树（BST）中
TreeNode* insertIntoBST(TreeNode* root, int val) {
    if (!root) return new TreeNode(val); // 如果当前节点为空，创建新节点
    if (val < root->val) root->left = insertIntoBST(root->left, val); // 插入到左子树
    else root->right = insertIntoBST(root->right, val); // 插入到右子树
    return root; // 返回根节点
}

// 辅助函数：在二叉搜索树中查找指定值的节点
TreeNode* findNode(TreeNode* root, int val) {
    if (!root || root->val == val) return root; // 如果当前节点为空或值匹配，返回该节点
    if (val < root->val) return findNode(root->left, val); // 在左子树中查找
    return findNode(root->right, val); // 在右子树中查找
}

// 释放二叉树内存的辅助函数
void freeTree(TreeNode* root) {
    if (!root) return; // 如果当前节点为空，直接返回
    freeTree(root->left); // 递归释放左子树
    freeTree(root->right); // 递归释放右子树
    delete root; // 删除当前节点
}

int main() {
    // 创建一个二叉搜索树
    TreeNode* root = nullptr; // 初始化根节点为空
    vector<int> values = {6, 2, 8, 0, 4, 7, 9, 3, 5}; // 待插入的节点值列表
    for (int val : values) {
        root = insertIntoBST(root, val); // 将每个值插入到 BST 中
    }

    // 查找节点 p 和 q
    TreeNode* p = findNode(root, 2); // 查找值为 2 的节点
    TreeNode* q = findNode(root, 8); // 查找值为 8 的节点

    // 查找最低公共祖先
    TreeNode* lca = lowestCommonAncestor(root, p, q); // 调用 LCA 函数
    if (lca) { // 如果找到 LCA
        cout << "Lowest Common Ancestor of " << p->val << " and " << q->val << " is " << lca->val << endl;
    } else { // 如果未找到 LCA
        cout << "No common ancestor found." << endl;
    }

    // 释放二叉树占用的内存
    freeTree(root);

    return 0; // 程序结束
}
