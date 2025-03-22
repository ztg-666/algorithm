#include <bits/stdc++.h>
using namespace std;

// 定义二叉树节点结构
struct TreeNode {
    int val;             // 节点的值
    TreeNode* left;      // 左子节点指针
    TreeNode* right;     // 右子节点指针
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}  // 构造函数，初始化节点值及左右子节点为空
};

class DepthOfBinaryTree {
public:
    // 计算二叉树的最大深度
    int maxDepth(TreeNode* root) {
        // 如果当前节点为空，则返回深度为0
        return root == nullptr ? 0 :
            // 否则递归计算左子树和右子树的最大深度，并取较大值加1（当前层）
            std::max(maxDepth(root->left), maxDepth(root->right)) + 1;
    }

    // 计算二叉树的最小深度
    int minDepth(TreeNode* root) {
        if (root == nullptr) {
            // 当前的树是空树，返回深度为0
            return 0;
        }
        if (root->left == nullptr && root->right == nullptr) {
            // 当前root是叶节点（没有左右子节点），返回深度为1
            return 1;
        }
        int ldeep = INT_MAX;  // 初始化左子树最小深度为最大整数值
        int rdeep = INT_MAX;  // 初始化右子树最小深度为最大整数值
        if (root->left != nullptr) {
            // 如果左子树存在，递归计算其最小深度
            ldeep = minDepth(root->left);
        }
        if (root->right != nullptr) {
            // 如果右子树存在，递归计算其最小深度
            rdeep = minDepth(root->right);
        }
        // 返回左右子树中较小的深度值，并加1（当前层）
        return std::min(ldeep, rdeep) + 1;
    }
};


int main() {
    // 创建一个简单的二叉树
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);

    DepthOfBinaryTree treeDepth;

    // 计算并输出最大深度
    int max_depth = treeDepth.maxDepth(root);
    std::cout << "1: " << max_depth << std::endl;

    // 计算并输出最小深度
    int min_depth = treeDepth.minDepth(root);
    std::cout << "2: " << min_depth << std::endl;

    // 释放内存
    delete root->left->left;
    delete root->left->right;
    delete root->left;
    delete root->right;
    delete root;

    return 0;
}