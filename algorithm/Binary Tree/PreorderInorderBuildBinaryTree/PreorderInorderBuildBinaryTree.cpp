#include<bits/stdc++.h>
using namespace std;

// 定义二叉树节点结构
struct TreeNode {
    int val;        // 节点的值
    TreeNode* left;   // 左子节点指针
    TreeNode* right;  // 右子节点指针

    // 构造函数，初始化节点值和子节点指针
    TreeNode(int v) : val(v), left(nullptr), right(nullptr) {}
};

class PreorderInorderBuildBinaryTree {
public:
    // 提交如下的方法
    static TreeNode* buildTree(const std::vector<int>& pre, const std::vector<int>& in) {
        // 检查输入的有效性：前序遍历和中序遍历不能为空且长度相同
        if (pre.empty() || in.empty() || pre.size() != in.size()) {
            return nullptr;
        }

        // 使用哈希表存储中序遍历中每个值的索引，以便快速查找
        std::unordered_map<int, int> map;
        for (int i = 0; i < in.size(); i++) {
            map[in[i]] = i;
        }

        // 调用辅助函数构建二叉树
        return f(pre, 0, pre.size() - 1, in, 0, in.size() - 1, map);
    }

private:
    // 辅助函数，递归地构建二叉树
    static TreeNode* f(const std::vector<int>& pre, int l1, int r1, const std::vector<int>& in, int l2, int r2, std::unordered_map<int, int>& map) {
        // 如果前序遍历的子数组为空，返回空指针
        if (l1 > r1) {
            return nullptr;
        }

        // 创建当前子树的根节点，根节点的值为前序遍历的第一个元素
        TreeNode* head = new TreeNode(pre[l1]);

        // 如果当前子数组只有一个元素，直接返回该节点
        if (l1 == r1) {
            return head;
        }

        // 在中序遍历中找到当前根节点的索引
        int k = map[pre[l1]];

        // 递归构建左子树
        // 前序遍历的左子树范围：[l1 + 1, l1 + k - l2]
        // 中序遍历的左子树范围：[l2, k - 1]
        head->left = f(pre, l1 + 1, l1 + k - l2, in, l2, k - 1, map);

        // 递归构建右子树
        // 前序遍历的右子树范围：[l1 + k - l2 + 1, r1]
        // 中序遍历的右子树范围：[k + 1, r2]
        head->right = f(pre, l1 + k - l2 + 1, r1, in, k + 1, r2, map);

        // 返回构建好的子树的根节点
        return head;
    }
};

// 前序遍历函数，用于验证构建的二叉树
void preorderTraversal(TreeNode* root) {
    if (root == nullptr) {
        return;
    }
    std::cout << root->val << " ";
    preorderTraversal(root->left);
    preorderTraversal(root->right);
}
void freeTree(TreeNode* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}
int main() {
    // 示例前序遍历和中序遍历数组
    std::vector<int> preorder = {3, 9, 20, 15, 7};
    std::vector<int> inorder = {9, 3, 15, 20, 7};

    // 构建二叉树
    TreeNode* root = PreorderInorderBuildBinaryTree::buildTree(preorder, inorder);

    // 前序遍历构建的二叉树，验证结果
    std::cout << "Preorder traversal of the constructed tree: ";
    preorderTraversal(root);
    std::cout << std::endl;

    freeTree(root);
    return 0;
}