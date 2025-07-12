#include <vector>
using namespace std;

// Morris遍历实现先序和中序遍历
// 测试链接 : https://leetcode.cn/problems/binary-tree-preorder-traversal/
// 测试链接 : https://leetcode.cn/problems/binary-tree-inorder-traversal/

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
    // Morris遍历
    void morris(TreeNode* head) {
        TreeNode* cur = head;
        TreeNode* mostRight = nullptr;
        while (cur != nullptr) {
            mostRight = cur->left;
            if (mostRight != nullptr) { // cur有左树
                // 找到左树最右节点
                // 注意左树最右节点的右指针可能指向空，也可能指向cur
                while (mostRight->right != nullptr && mostRight->right != cur) {
                    mostRight = mostRight->right;
                }
                // 判断左树最右节点的右指针状态
                if (mostRight->right == nullptr) { // 第一次到达
                    mostRight->right = cur;
                    cur = cur->left;
                    continue;
                } else { // 第二次到达
                    mostRight->right = nullptr;
                }
            }
            cur = cur->right;
        }
    }

    // Morris遍历实现先序遍历
    // 测试链接 : https://leetcode.cn/problems/binary-tree-preorder-traversal/
    vector<int> preorderTraversal(TreeNode* head) {
        vector<int> ans;
        morrisPreorder(head, ans);
        return ans;
    }

    void morrisPreorder(TreeNode* head, vector<int>& ans) {
        TreeNode* cur = head;
        TreeNode* mostRight = nullptr;
        while (cur != nullptr) {
            mostRight = cur->left;
            if (mostRight != nullptr) { // cur有左树
                // 找到左树最右节点
                // 注意左树最右节点的右指针可能指向空，也可能指向cur
                while (mostRight->right != nullptr && mostRight->right != cur) {
                    mostRight = mostRight->right;
                }
                // 判断左树最右节点的右指针状态
                if (mostRight->right == nullptr) { // 第一次到达
                    ans.push_back(cur->val);
                    mostRight->right = cur;
                    cur = cur->left;
                    continue;
                } else { // 第二次到达
                    mostRight->right = nullptr;
                }
            } else { // cur无左树
                ans.push_back(cur->val);
            }
            cur = cur->right;
        }
    }

    // Morris遍历实现中序遍历
    // 测试链接 : https://leetcode.cn/problems/binary-tree-inorder-traversal/
    vector<int> inorderTraversal(TreeNode* head) {
        vector<int> ans;
        morrisInorder(head, ans);
        return ans;
    }

    void morrisInorder(TreeNode* head, vector<int>& ans) {
        TreeNode* cur = head;
        TreeNode* mostRight = nullptr;
        while (cur != nullptr) {
            mostRight = cur->left;
            if (mostRight != nullptr) { // cur有左树
                // 找到左树最右节点
                // 注意左树最右节点的右指针可能指向空，也可能指向cur
                while (mostRight->right != nullptr && mostRight->right != cur) {
                    mostRight = mostRight->right;
                }
                // 判断左树最右节点的右指针状态
                if (mostRight->right == nullptr) { // 第一次到达
                    mostRight->right = cur;
                    cur = cur->left;
                    continue;
                } else { // 第二次到达
                    mostRight->right = nullptr;
                }
            }
            ans.push_back(cur->val);
            cur = cur->right;
        }
    }
};
