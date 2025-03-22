#include <bits/stdc++.h>
using namespace std;

// 定义 TreeNode 结构体
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

// 查找最低公共祖先的函数
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    while (root->val != p->val && root->val != q->val) {
        if (std::min(p->val, q->val) < root->val && root->val < std::max(p->val, q->val)) {
            break;
        }
        root = root->val < std::min(p->val, q->val) ? root->right : root->left;
    }
    return root;
}

// 辅助函数：插入节点到二叉搜索树
TreeNode* insertIntoBST(TreeNode* root, int val) {
    if (!root) return new TreeNode(val);
    if (val < root->val) root->left = insertIntoBST(root->left, val);
    else root->right = insertIntoBST(root->right, val);
    return root;
}

// 辅助函数：查找指定值的节点
TreeNode* findNode(TreeNode* root, int val) {
    if (!root || root->val == val) return root;
    if (val < root->val) return findNode(root->left, val);
    return findNode(root->right, val);
}
void freeTree(TreeNode* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

int main() {
    // 创建一个二叉搜索树
    TreeNode* root = nullptr;
    vector<int> values = {6, 2, 8, 0, 4, 7, 9, 3, 5};
    for (int val : values) {
        root = insertIntoBST(root, val);
    }

    // 查找节点 p 和 q
    TreeNode* p = findNode(root, 2);
    TreeNode* q = findNode(root, 8);

    // 查找最低公共祖先
    TreeNode* lca = lowestCommonAncestor(root, p, q);
    if (lca) {
        cout << "Lowest Common Ancestor of " << p->val << " and " << q->val << " is " << lca->val << endl;
    } else {
        cout << "No common ancestor found." << endl;
    }
    freeTree(root);

    return 0;
}
