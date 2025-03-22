#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// 定义二叉树节点结构，用于构建二叉搜索树（BST）
// 不需要提交此部分
struct TreeNode {
    int val; // 节点值
    TreeNode* left; // 左子节点指针
    TreeNode* right; // 右子节点指针
    TreeNode(int x) : val(x), left(NULL), right(NULL) {} // 构造函数
};

// 提交以下的方法
class Solution {
public:
    // 中序遍历方法判断是否为合法的二叉搜索树
    // 提交时改名为isValidBST
    bool isValidBST1(TreeNode* head) {
        if (head == nullptr) { // 空树是合法的BST
            return true;
        }
        TreeNode* pre = nullptr; // 记录上一个访问的节点
        std::stack<TreeNode*> stack; // 辅助栈，用于中序遍历
        while (!stack.empty() || head != nullptr) { // 遍历条件
            if (head != nullptr) { // 一直向左走，直到没有左子树
                stack.push(head); // 当前节点入栈
                head = head->left; // 移动到左子节点
            } else { // 处理右子树或回溯
                head = stack.top(); // 获取栈顶节点
                stack.pop(); // 弹出栈顶节点
                if (pre != nullptr && pre->val >= head->val) { // 检查中序遍历顺序是否满足BST性质
                    return false; // 如果当前节点值小于等于前一个节点值，则不是BST
                }
                pre = head; // 更新前一个节点
                head = head->right; // 移动到右子节点
            }
        }
        return true; // 如果遍历完成且未发现异常，则是合法的BST
    }

    ll minVal, maxVal; // 用于记录子树的最小值和最大值

    // 递归方法判断是否为合法的二叉搜索树
    // 提交时改名为isValidBST
    bool isValidBST2(TreeNode* head) {
        if (head == nullptr) { // 空树是合法的BST
            minVal = LLONG_MAX; // 初始化子树最小值为正无穷
            maxVal = LLONG_MIN; // 初始化子树最大值为负无穷
            return true;
        }
        bool lok = isValidBST2(head->left); // 判断左子树是否为合法BST
        ll lmin = minVal; // 获取左子树的最小值
        ll lmax = maxVal; // 获取左子树的最大值
        bool rok = isValidBST2(head->right); // 判断右子树是否为合法BST
        ll rmin = minVal; // 获取右子树的最小值
        ll rmax = maxVal; // 获取右子树的最大值
        minVal = std::min({lmin, rmin, static_cast<ll>(head->val)}); // 更新当前子树的最小值
        maxVal = std::max({lmax, rmax, static_cast<ll>(head->val)}); // 更新当前子树的最大值
        // 判断当前节点是否满足BST性质：左子树的最大值小于当前节点值，且当前节点值小于右子树的最小值
        return lok && rok && lmax < head->val && head->val < rmin;
    }
};


int main() {
    // 创建一个示例二叉树
    TreeNode* root = new TreeNode(2);
    root->left = new TreeNode(1);
    root->right = new TreeNode(3);

    Solution solution;

    // 使用中序遍历方法验证是否为合法的二叉搜索树
    bool result1 = solution.isValidBST1(root);
    cout << "Is the tree a valid BST (inorder traversal method)? " << (result1 ? "Yes" : "No") << endl;

    // 使用递归方法验证是否为合法的二叉搜索树
    bool result2 = solution.isValidBST2(root);
    cout << "Is the tree a valid BST (recursive method)? " << (result2 ? "Yes" : "No") << endl;

    // 释放内存
    delete root->left;
    delete root->right;
    delete root;

    return 0;
}