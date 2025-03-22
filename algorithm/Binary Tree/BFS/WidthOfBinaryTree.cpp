#include<bits/stdc++.h>
using namespace std;

// 定义二叉树节点结构
struct TreeNode {
    int val;          // 节点值
    TreeNode* left;   // 左子节点指针
    TreeNode* right;  // 右子节点指针
    // 构造函数，初始化节点值和子节点指针
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// 使用类型别名定义无符号长整型
using ULL = unsigned long long;

class Solution {
public:
    // 定义最大节点数常量
    static const int MAXN = 3001;
    // 定义节点队列，用于层次遍历
    TreeNode* nq[MAXN];
    // 定义索引队列，用于记录节点在二叉树中的索引
    ULL iq[MAXN];
    // 定义队列的左右指针
    int l, r;

    // 计算二叉树的最大宽度
    int widthOfBinaryTree(TreeNode* root) {
        int ans = 1;  // 初始化最大宽度为1
        l = r = 0;    // 初始化队列指针
        nq[r] = root; // 将根节点入队
        iq[r++] = 1ULL; // 根节点索引为1

        // 层次遍历二叉树
        while (l < r) {
            int size = r - l; // 当前层的节点数
            // 更新最大宽度，当前层的宽度为队尾索引减去队首索引加1
            ans = std::max(ans, static_cast<int>(iq[r - 1] - iq[l] + 1ULL));

            // 遍历当前层的所有节点
            for (int i = 0; i < size; i++) {
                TreeNode* node = nq[l]; // 取出队首节点
                ULL id = iq[l++];       // 取出节点索引并移动队首指针

                // 如果左子节点存在，将其入队，并设置索引为当前节点索引的两倍
                if (node->left != nullptr) {
                    nq[r] = node->left;
                    iq[r++] = id * 2ULL;
                }

                // 如果右子节点存在，将其入队，并设置索引为当前节点索引的两倍加一
                if (node->right != nullptr) {
                    nq[r] = node->right;
                    iq[r++] = id * 2ULL + 1ULL;
                }
            }
        }
        return ans; // 返回最大宽度
    }
};



int main() {
    // 创建一个示例二叉树
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(3);
    root->right = new TreeNode(2);
    root->left->left = new TreeNode(5);
    root->left->right = new TreeNode(3);
    root->right->right = new TreeNode(9);

    Solution solution;
    int maxWidth = solution.widthOfBinaryTree(root);

    std::cout << "The maximum width of the binary tree is: " << maxWidth << std::endl;

    // 释放内存
    delete root->left->left;
    delete root->left->right;
    delete root->right->right;
    delete root->left;
    delete root->right;
    delete root;

    return 0;
}