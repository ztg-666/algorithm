#include <bits/stdc++.h>
using namespace std;
bool isCBT(Node* head) {
    // 如果根节点为空，认为是完全二叉树
    if (head == nullptr) {
        return true;
    }

    // 使用队列进行层次遍历（广度优先搜索）
    std::queue<Node*> queue;
    // 标记是否遇到了第一个不双全的节点
    bool leaf = false;
    Node* l = nullptr;
    Node* r = nullptr;

    // 将根节点加入队列
    queue.push(head);

    // 开始层次遍历
    while (!queue.empty()) {
        // 取出队列前端的节点
        head = queue.front();
        queue.pop();

        // 获取当前节点的左右子节点
        l = head->left;
        r = head->right;

        // 检查当前节点是否违反完全二叉树的条件
        // 1. 如果已经遇到过不双全的节点（leaf 为 true），而当前节点还有子节点，则返回 false
        // 2. 如果当前节点左子节点为空但右子节点不为空，则返回 false
        if ((leaf && (l != nullptr || r != nullptr)) || (l == nullptr && r != nullptr)) {
            return false;
        }

        // 如果左子节点不为空，将其加入队列
        if (l != nullptr) {
            queue.push(l);
        }

        // 如果右子节点不为空，将其加入队列
        if (r != nullptr) {
            queue.push(r);
        } else {
            // 如果右子节点为空，标记后续所有节点都必须是叶子节点
            leaf = true;
        }
    }

    // 如果遍历完整棵树没有违反上述条件，则返回 true
    return true;
}



// 定义二叉树节点类
class TreeNode {
public:
    int val;          // 节点的值
    TreeNode* left;   // 指向左子节点的指针
    TreeNode* right;  // 指向右子节点的指针

    // 构造函数，初始化节点的值，并将左右子节点设为nullptr
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// 定义判断二叉树是否为完全二叉树的类
class CompletenessOfBinaryTree {
public:
    // 如果测试数据量变大了就修改这个值
    static const int MAXN = 101;

    // 判断给定的二叉树是否为完全二叉树
    static bool isCompleteTree(TreeNode* h) {
        // 如果根节点为空，则树为空，空树被认为是完全二叉树
        if (h == nullptr) {
            return true;
        }

        // 使用队列进行层次遍历
        std::queue<TreeNode*> queue;
        queue.push(h);

        // 标记是否遇到过左右子节点不双全的节点
        bool leaf = false;

        // 开始层次遍历
        while (!queue.empty()) {
            // 取出队列中的第一个节点
            h = queue.front();
            queue.pop();

            // 如果当前节点左子节点为空但右子节点不为空，则不是完全二叉树
            // 或者如果已经遇到过左右子节点不双全的节点，但当前节点还有子节点，则不是完全二叉树
            if ((h->left == nullptr && h->right != nullptr) || (leaf && (h->left != nullptr || h->right != nullptr))) {
                return false;
            }

            // 如果左子节点不为空，将其加入队列
            if (h->left != nullptr) {
                queue.push(h->left);
            }

            // 如果右子节点不为空，将其加入队列
            if (h->right != nullptr) {
                queue.push(h->right);
            }

            // 如果当前节点左子节点或右子节点为空，则标记leaf为true
            if (h->left == nullptr || h->right == nullptr) {
                leaf = true;
            }
        }

        // 如果遍历完整棵树没有违反完全二叉树的条件，则返回true
        return true;
    }
};

void freeTree(TreeNode* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

int main() {
    // 创建一个完全二叉树
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    root->right->left = new TreeNode(6);

    // 检查是否为完全二叉树
    cout << (CompletenessOfBinaryTree::isCompleteTree(root) ? "Complete" : "Not complete") << endl;

    freeTree(root);

    return 0;
}
