#include <iostream>
using namespace std;

// Morris遍历求两个节点的最低公共祖先
// 测试链接 : https://leetcode.cn/problems/lowest-common-ancestor-of-a-binary-tree/

// 定义二叉树节点结构
struct TreeNode {
    int val;           // 节点值
    TreeNode *left;    // 左子节点指针
    TreeNode *right;   // 右子节点指针
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}  // 构造函数
};

class Solution {
public:
    // 主函数：使用Morris遍历求两个节点的最低公共祖先
    // head: 二叉树根节点
    // o1, o2: 要找LCA的两个节点
    TreeNode* lowestCommonAncestor(TreeNode* head, TreeNode* o1, TreeNode* o2) {
        // 特殊情况1：如果o1的左子树或右子树中包含o2，则o1就是LCA
        if (preOrder(o1->left, o1, o2) != nullptr || preOrder(o1->right, o1, o2) != nullptr) {
            return o1;
        }
        // 特殊情况2：如果o2的左子树或右子树中包含o1，则o2就是LCA
        if (preOrder(o2->left, o1, o2) != nullptr || preOrder(o2->right, o1, o2) != nullptr) {
            return o2;
        }

        // 先序遍历找到第一个遇到的目标节点（o1或o2）
        TreeNode* left = preOrder(head, o1, o2);
        TreeNode* cur = head;        // 当前遍历的节点
        TreeNode* mostRight = nullptr; // 左子树的最右节点
        TreeNode* lca = nullptr;     // 存储找到的最低公共祖先

        // Morris遍历的主循环
        while (cur != nullptr) {
            mostRight = cur->left;   // 获取当前节点的左子节点

            if (mostRight != nullptr) {
                // 找到左子树的最右节点
                while (mostRight->right != nullptr && mostRight->right != cur) {
                    mostRight = mostRight->right;
                }

                if (mostRight->right == nullptr) {
                    // 第一次到达cur，建立线索（mostRight指向cur）
                    mostRight->right = cur;
                    cur = cur->left;     // 继续遍历左子树
                    continue;
                } else {
                    // 第二次到达cur，说明左子树遍历完成
                    mostRight->right = nullptr;  // 恢复树的原始结构

                    if (lca == nullptr) {
                        // 检查left是否在cur左树的右边界上
                        if (rightCheck(cur->left, left)) {
                            // 如果left在右边界上，检查left的右子树是否包含另一个目标节点
                            if (preOrder(left->right, o1, o2) != nullptr) {
                                lca = left;  // 找到LCA
                            }
                            left = cur;      // 更新left为当前节点

                            // 为什么此时检查的是left而不是cur？
                            // 因为cur右树上的某些右指针可能没有恢复回来
                            // 需要等右指针恢复回来之后检查才不出错
                            // 所以此时检查的是left而不是cur
                        }
                    }
                }
            }
            cur = cur->right;  // 继续遍历右子树
        }

        // 如果Morris遍历结束了还没有收集到答案
        // 此时最后一个left还没有验证，它一定是答案
        return lca != nullptr ? lca : left;
    }

private:
    // 辅助函数：以head为头的树进行先序遍历，返回第一个找到的目标节点（o1或o2）
    // head: 子树的根节点
    // o1, o2: 要查找的两个目标节点
    // 返回值: 第一个遇到的目标节点，如果都没找到返回nullptr
    TreeNode* preOrder(TreeNode* head, TreeNode* o1, TreeNode* o2) {
        TreeNode* cur = head;          // 当前遍历节点
        TreeNode* mostRight = nullptr; // 左子树的最右节点
        TreeNode* ans = nullptr;       // 存储找到的目标节点

        // Morris先序遍历
        while (cur != nullptr) {
            mostRight = cur->left;

            if (mostRight != nullptr) {
                // 找到左子树的最右节点
                while (mostRight->right != nullptr && mostRight->right != cur) {
                    mostRight = mostRight->right;
                }

                if (mostRight->right == nullptr) {
                    // 第一次到达cur，这是先序遍历处理当前节点的时机
                    if (ans == nullptr && (cur == o1 || cur == o2)) {
                        ans = cur;  // 找到目标节点
                    }
                    mostRight->right = cur;  // 建立线索
                    cur = cur->left;         // 继续遍历左子树
                    continue;
                } else {
                    // 第二次到达cur，恢复树结构
                    mostRight->right = nullptr;
                }
            } else {
                // 没有左子树，直接处理当前节点
                if (ans == nullptr && (cur == o1 || cur == o2)) {
                    ans = cur;  // 找到目标节点
                }
            }
            cur = cur->right;  // 继续遍历右子树
        }
        return ans;
    }

    // 辅助函数：沿着右边界查找目标节点
    // head: 起始节点
    // target: 要查找的目标节点
    // 返回值: 如果在右边界上找到target返回true，否则返回false
    bool rightCheck(TreeNode* head, TreeNode* target) {
        while (head != nullptr) {
            if (head == target) {
                return true;   // 找到目标节点
            }
            head = head->right;  // 继续沿右边界向下
        }
        return false;  // 没有找到目标节点
    }
};


