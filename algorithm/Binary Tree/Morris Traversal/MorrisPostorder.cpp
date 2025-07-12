#include <vector>
#include <algorithm>
using namespace std;

// Morris遍历实现后序遍历
// 测试链接 : https://leetcode.cn/problems/binary-tree-postorder-traversal/

// 不提交这个结构体（LeetCode已定义）
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
    // 提交如下的方法
    vector<int> postorderTraversal(TreeNode* head) {
        vector<int> ans;
        morrisPostorder(head, ans);
        return ans;
    }

private:
    void morrisPostorder(TreeNode* head, vector<int>& ans) {
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
                    collect(cur->left, ans);
                }
            }
            cur = cur->right;
        }
        collect(head, ans);
    }

    // 以head为头的子树，树的右边界逆序收集
    void collect(TreeNode* head, vector<int>& ans) {
        TreeNode* tail = reverse(head);
        TreeNode* cur = tail;
        while (cur != nullptr) {
            ans.push_back(cur->val);
            cur = cur->right;
        }
        reverse(tail);
    }

    // 从from出发，类似单链表翻转，去翻转right指针的方向
    TreeNode* reverse(TreeNode* from) {
        TreeNode* pre = nullptr;
        TreeNode* next = nullptr;
        while (from != nullptr) {
            next = from->right;
            from->right = pre;
            pre = from;
            from = next;
        }
        return pre;
    }
};
