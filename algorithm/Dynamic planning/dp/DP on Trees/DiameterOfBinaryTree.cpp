#include <algorithm> // 用于 std::max

// 二叉树的直径
// 给你一棵二叉树的根节点，返回该树的直径
// 二叉树的 直径 是指树中任意两个节点之间最长路径的长度
// 这条路径可能经过也可能不经过根节点 root
// 两节点之间路径的 长度 由它们之间边数表示
// 测试链接 : https://leetcode.cn/problems/diameter-of-binary-tree/

// 二叉树节点的定义。

struct TreeNode {
    int val; // 节点值
    TreeNode *left; // 左子节点指针
    TreeNode *right; // 右子节点指针
    // 构造函数
    TreeNode(int x = 0, TreeNode *l = nullptr, TreeNode *r = nullptr) : val(x), left(l), right(r) {}
};

// 这个结构体将保存递归辅助函数返回的信息。

struct Info {
    int diameter; // 在子树中找到的最大直径
    int height;   // 子树的高度 (向下最长路径上的节点数)

    // 构造函数
    Info(int d, int h) : diameter(d), height(h) {}
};



class Solution {
public:

    Info f(TreeNode* x) {
        if (x == nullptr) {
            // 空节点的基准情况：
            // 直径为 0。
            // 高度 (从这里向下最长路径上的节点数) 为 0。
            return Info(0, 0);
        }

        // 从左右子节点递归获取信息
        Info leftInfo = f(x->left);
        Info rightInfo = f(x->right);

        // 计算以 x 为根的当前子树的高度。
        // 高度是 1 (对于节点 x) + 其子节点高度的最大值。
        int current_height = std::max(leftInfo.height, rightInfo.height) + 1;

        // 计算到目前为止找到的最大直径。
        // 可能性 1: 直径完全在左子树内。
        // 可能性 2: 直径完全在右子树内。
        int max_diameter_from_children = std::max(leftInfo.diameter, rightInfo.diameter);

        // 可能性 3: 直径穿过当前节点 x。
        // 这样一条路径的长度是从 x 向下延伸的“臂长”之和。
        // leftInfo.height 是从 x->left 向下最长路径上的节点数。
        // 这也等于从 x 到经由 x->left 的最远叶子节点的路径上的边数。
        // rightInfo.height 同理。
        int diameter_through_x = leftInfo.height + rightInfo.height;

        // x 处子树的整体最大直径是这些可能性的最大值。
        int current_diameter = std::max(max_diameter_from_children, diameter_through_x);

        return Info(current_diameter, current_height);
    }


    int diameterOfBinaryTree(TreeNode* root) {
        if (root == nullptr) {
            return 0;
        }
        return f(root).diameter;
    }
};

