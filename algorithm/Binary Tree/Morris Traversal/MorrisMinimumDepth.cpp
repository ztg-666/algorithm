#include <algorithm>
#include <climits>

// Morris遍历求二叉树最小高度
// 测试链接 : https://leetcode.cn/problems/minimum-depth-of-binary-tree/

// 二叉树节点定义
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution {
public:
    int minDepth(TreeNode* head) {
        // 空树的最小深度为0
        if (head == nullptr) {
            return 0;
        }

        TreeNode* cur = head;           // 当前遍历到的节点
        TreeNode* mostRight = nullptr;  // 当前节点左子树的最右节点
        int preLevel = 0;               // morris遍历中，上一个节点所在的层数
        int rightLen;                   // 树的右边界长度
        int ans = INT_MAX;              // 记录最小深度

        // Morris遍历主循环
        while (cur != nullptr) {
            mostRight = cur->left;

            // 如果当前节点有左子树
            if (mostRight != nullptr) {
                rightLen = 1;

                // 找到当前节点左子树的最右节点，同时计算右边界长度
                while (mostRight->right != nullptr && mostRight->right != cur) {
                    rightLen++;
                    mostRight = mostRight->right;
                }

                // 第一次到达当前节点，建立线索
                if (mostRight->right == nullptr) {
                    preLevel++;                 // 深度加1
                    mostRight->right = cur;     // 建立线索，指向当前节点
                    cur = cur->left;            // 转向左子树
                    continue;
                } else {
                    // 第二次到达当前节点，说明左子树已经遍历完毕
                    // 检查左子树的最右节点是否为叶节点
                    if (mostRight->left == nullptr) {
                        ans = std::min(ans, preLevel);  // 更新最小深度
                    }
                    preLevel -= rightLen;       // 回退到当前节点的层数
                    mostRight->right = nullptr; // 删除线索
                }
            } else {
                // 当前节点没有左子树，直接深度加1
                preLevel++;
            }

            // 转向右子树
            cur = cur->right;
        }

        // 特殊处理：检查整棵树的最右节点
        // 因为Morris遍历可能不会经过最右边的叶节点
        rightLen = 1;
        cur = head;

        // 找到整棵树的最右节点，同时计算从根到最右节点的路径长度
        while (cur->right != nullptr) {
            rightLen++;
            cur = cur->right;
        }

        // 如果整棵树的最右节点是叶节点（没有左子树），则纳入统计
        if (cur->left == nullptr) {
            ans = std::min(ans, rightLen);
        }

        return ans;
    }
};
