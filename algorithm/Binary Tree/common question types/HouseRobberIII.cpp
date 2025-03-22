#include<bits/stdc++.h>
// 二叉树打家劫舍问题

// 定义二叉树节点结构
struct TreeNode {
    int val;          // 节点的值
    TreeNode* left;   // 左子节点指针
    TreeNode* right;  // 右子节点指针
    // 构造函数，初始化节点值和左右子节点指针
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

class HouseRobberIIII {
public:
    // 主函数，计算从根节点开始的最大收益
    int rob(TreeNode* root) {
        f(root);  // 调用辅助函数计算收益
        return std::max(yes, no);  // 返回偷或不偷根节点的最大收益
    }

private:
    // 成员变量，记录在偷当前节点的情况下，子树的最大收益
    int yes;
    // 成员变量，记录在不偷当前节点的情况下，子树的最大收益
    int no;

    // 辅助函数，递归计算每个节点的收益
    void f(TreeNode* root) {
        if (root == nullptr) {
            // 如果当前节点为空，收益为0
            yes = 0;
            no = 0;
        } else {
            // 当前节点的值
            int y = root->val;
            // 如果不偷当前节点，收益为0
            int n = 0;
            // 递归计算左子树的收益
            f(root->left);
            // 如果偷当前节点，则不能偷左子节点，收益为左子树不偷的情况加上当前节点的值
            y += no;
            // 如果不偷当前节点，则收益为左子树偷或不偷的最大值
            n += std::max(yes, no);
            // 递归计算右子树的收益
            f(root->right);
            // 如果偷当前节点，则不能偷右子节点，收益为右子树不偷的情况加上之前计算的y
            y += no;
            // 如果不偷当前节点，则收益为右子树偷或不偷的最大值加上之前计算的n
            n += std::max(yes, no);
            // 更新当前节点的yes和no
            yes = y;
            no = n;
        }
    }
};


int main() {
    // 创建一个示例二叉树
    TreeNode* root = new TreeNode(3);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->right = new TreeNode(3);
    root->right->right = new TreeNode(1);

    // 创建 HouseRobberIIII 类的实例
    HouseRobberIIII solution;

    // 计算最大收益
    int maxRob = solution.rob(root);

    // 输出结果
    std::cout << "The maximum amount of money the thief can rob is: " << maxRob << std::endl;

    // 释放内存
    delete root->left->right;
    delete root->right->right;
    delete root->left;
    delete root->right;
    delete root;

    return 0;
}