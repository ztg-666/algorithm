#include <iostream>
using namespace std;

// Morris遍历判断搜索二叉树
// 测试链接 : https://leetcode.cn/problems/validate-binary-search-tree/

// 不提交这个类
struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;

    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};

class Solution {
public:
    // 提交如下的方法
    // 使用Morris遍历算法验证二叉搜索树
    // 时间复杂度：O(n)，空间复杂度：O(1)
    bool isValidBST(TreeNode* head) {
        TreeNode* cur = head;           // 当前遍历的节点
        TreeNode* mostRight = nullptr;  // 当前节点左子树的最右节点
        TreeNode* pre = nullptr;        // 前一个遍历的节点（中序遍历的前驱节点）
        bool ans = true;                // 记录是否为有效的二叉搜索树

        // Morris遍历的核心循环
        while (cur != nullptr) {
            mostRight = cur->left;      // 找到当前节点的左子树

            // 如果当前节点有左子树
            if (mostRight != nullptr) {
                // 找到左子树的最右节点（中序遍历的前驱节点）
                while (mostRight->right != nullptr && mostRight->right != cur) {
                    mostRight = mostRight->right;
                }

                // 如果最右节点的右指针为空，建立线索
                if (mostRight->right == nullptr) {
                    mostRight->right = cur;  // 建立线索，指向当前节点
                    cur = cur->left;         // 转向左子树
                    continue;                // 继续遍历左子树
                } else {
                    // 如果最右节点的右指针指向当前节点，说明左子树已经遍历完毕
                    mostRight->right = nullptr;  // 恢复树的原始结构
                }
            }

            // 检查二叉搜索树的性质：中序遍历应该是递增的
            // 如果前一个节点的值大于等于当前节点的值，则不是有效的BST
            if (pre != nullptr && pre->val >= cur->val) {
                ans = false;
            }

            pre = cur;          // 更新前驱节点
            cur = cur->right;   // 转向右子树
        }

        return ans;
    }
};

