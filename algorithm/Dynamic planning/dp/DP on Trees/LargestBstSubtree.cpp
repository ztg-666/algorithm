#include <algorithm> // 用于 std::max 和 std::min
#include <limits>    // 用于 std::numeric_limits

// 最大BST子树
// 给定一个二叉树，找到其中最大的二叉搜索树（BST）子树，并返回该子树的大小
// 其中，最大指的是子树节点数最多的
// 二叉搜索树（BST）中的所有节点都具备以下属性：
// 左子树的值小于其父（根）节点的值
// 右子树的值大于其父（根）节点的值
// 注意：子树必须包含其所有后代
// 测试链接 : https://leetcode.cn/problems/largest-bst-subtree/

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
    long long max_val;    // 子树中的最大值
    long long min_val;    // 子树中的最小值
    bool isBst;           // 如果子树是 BST，则为 true
    int maxBstSize;       // 在此子树中找到的最大 BST 的大小

    // 构造函数
    Info(long long mx_v, long long mn_v, bool is_bst_subtree, int bst_size) :
        max_val(mx_v), min_val(mn_v), isBst(is_bst_subtree), maxBstSize(bst_size) {}
};


class Solution {
public:

    Info f(TreeNode* x) {
        if (x == nullptr) {
            // 空节点的基准情况：
            // 技术上讲，它是一个大小为 0 的 BST。
            // max_val 设置为 long long 类型的最小值。
            // min_val 设置为 long long 类型的最大值。
            // 这确保了对于父节点，一个空子节点将满足 BST 条件
            // (parent_val > null_child_max_val, parent_val < null_child_min_val)。
            return Info(std::numeric_limits<long long>::min(),  // max_val 使用 long long 的最小值
                        std::numeric_limits<long long>::max(),  // min_val 使用 long long 的最大值
                        true,                                   // 空树是 BST
                        0);                                     // 大小为 0
        }

        // 从左右子节点递归获取信息
        Info info_left = f(x->left);
        Info info_right = f(x->right);

        long long current_node_val = static_cast<long long>(x->val); // 当前节点值，转为 long long 以便比较

        // 确定以 x 为根的子树中的整体最小值和最大值。
        // 如果 x 本身形成 BST，其父节点将使用这些值来检查 BST 属性。
        // 注意: 使用初始化列表的 std::max 和 std::min 需要 C++11 或更高版本。
        // 如果是 C++11 之前的版本，请使用嵌套调用：std::max(current_node_val, std::max(info_left.max_val, info_right.max_val))
        long long overall_max_in_subtree = std::max({current_node_val, info_left.max_val, info_right.max_val});
        long long overall_min_in_subtree = std::min({current_node_val, info_left.min_val, info_right.min_val});

        bool current_subtree_is_bst = false; // 当前子树是否为BST
        int largest_bst_size_in_current_subtree; // 当前子树内最大BST的大小

        // 检查以 x 为根的子树是否为 BST：
        // 1. 左子树必须是 BST。
        // 2. 右子树必须是 BST。
        // 3. 左子树中的最大值必须小于 x 的值。
        // 4. 右子树中的最小值必须大于 x 的值。
        if (info_left.isBst && info_right.isBst &&
            info_left.max_val < current_node_val &&
            current_node_val < info_right.min_val) {

            current_subtree_is_bst = true;
            largest_bst_size_in_current_subtree = info_left.maxBstSize + info_right.maxBstSize + 1;
        } else {
            current_subtree_is_bst = false;
            // 如果以 x 为根的子树不是 BST，那么在 x 范围内最大的 BST
            // 要么是在其左子树中找到的最大 BST，要么是在其右子树中找到的最大 BST。
            largest_bst_size_in_current_subtree = std::max(info_left.maxBstSize, info_right.maxBstSize);
        }

        // 返回为节点 x 收集的信息
        return Info(overall_max_in_subtree,
                    overall_min_in_subtree,
                    current_subtree_is_bst,
                    largest_bst_size_in_current_subtree);
    }


    int largestBSTSubtree(TreeNode* root) {
        // f 函数能正确处理 root == nullptr 的情况，返回一个 Info 对象
        // 其中 maxBstSize 为 0。
        return f(root).maxBstSize;
    }
};

// 辅助函数，用于释放树的内存 (防止内存泄漏)
void deleteTree(TreeNode* node) {
    if (node == nullptr) {
        return;
    }
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}
#include<iostream>

int main() {
    Solution sol;

    // 测试用例 1: LeetCode 示例
    //     10
    //    /  \
    //   5    15
    //  / \    \
    // 1   8    7  (7 < 15, 所以以15为根的子树不是BST, 以10为根的也不是)
    // 最大的BST是:
    //   5
    //  / \
    // 1   8   (大小为3)
    TreeNode* root1 = new TreeNode(10);
    root1->left = new TreeNode(5);
    root1->right = new TreeNode(15);
    root1->left->left = new TreeNode(1);
    root1->left->right = new TreeNode(8);
    root1->right->right = new TreeNode(7); // 这使得以15为根的子树和以10为根的子树不是BST

    std::cout << " 1:" << std::endl;
    std::cout << ": [10, 5, 15, 1, 8, null, 7]" << std::endl;
    std::cout << ": " << sol.largestBSTSubtree(root1) << " (: 3)" << std::endl;
    std::cout << std::endl;
    deleteTree(root1);

    // 测试用例 2: 整个树是一个BST
    //      4
    //     / \
    //    2   5
    //   / \
    //  1   3
    TreeNode* root2 = new TreeNode(4);
    root2->left = new TreeNode(2);
    root2->right = new TreeNode(5);
    root2->left->left = new TreeNode(1);
    root2->left->right = new TreeNode(3);

    std::cout << " 2:" << std::endl;
    std::cout << ": [4, 2, 5, 1, 3]" << std::endl;
    std::cout << ": " << sol.largestBSTSubtree(root2) << " (: 5)" << std::endl;
    std::cout << std::endl;
    deleteTree(root2);

    // 测试用例 3: 空树
    TreeNode* root3 = nullptr;
    std::cout << " 3:" << std::endl;
    std::cout << ": [] ()" << std::endl;
    std::cout << ": " << sol.largestBSTSubtree(root3) << " (: 0)" << std::endl;
    std::cout << std::endl;
    // deleteTree(root3); // 不需要，因为是nullptr

    // 测试用例 4: 只有一个节点的树
    TreeNode* root4 = new TreeNode(100);
    std::cout << " 4:" << std::endl;
    std::cout << ": [100]" << std::endl;
    std::cout << ": " << sol.largestBSTSubtree(root4) << " (: 1)" << std::endl;
    std::cout << std::endl;
    deleteTree(root4);

    // 测试用例 5: 一个更复杂的例子
    //         50
    //        /  \
    //       30   60
    //      /  \  /  \
    //     5   20 45  70  (45 < 60, 以60为根的子树不是BST)
    //               /  \
    //              65  80
    // 最大的BST是:
    //         70
    //        /  \
    //       65  80 (大小为3) 或者 以30为根的子树 (大小为3)
    //   或者单独的节点5, 20, 45, 50, 60 (大小为1)
    //   仔细看，以30为根的 (5,20) 也是一个BST, 大小为3。
    //   以70为根的 (65,80) 也是一个BST, 大小为3。
    //   题目要求最大的。
    TreeNode* root5 = new TreeNode(50);
    root5->left = new TreeNode(30);
    root5->right = new TreeNode(60);
    root5->left->left = new TreeNode(5);
    root5->left->right = new TreeNode(20);
    root5->right->left = new TreeNode(45); // 破坏了以60为根的BST
    root5->right->right = new TreeNode(70);
    root5->right->right->left = new TreeNode(65);
    root5->right->right->right = new TreeNode(80);

    std::cout << " 5:" << std::endl;
    std::cout << ": [50, 30, 60, 5, 20, 45, 70, null, null, null, null, 65, 80]" << std::endl;
    std::cout << ": " << sol.largestBSTSubtree(root5) << " (: 3)" << std::endl;
    std::cout << std::endl;
    deleteTree(root5);

    // 测试用例 6: 左边有一个大的BST, 右边有一个小的BST
    //         20
    //        /  \
    //       10   30  <-- 这是一个大小为3的BST
    //      /  \    \
    //     5   15    25 <-- 破坏了以30为根的BST, 也破坏了以20为根的BST
    //    / \
    //   1   7
    // 最大的BST是：
    //      10
    //     /  \
    //    5   15
    //   / \
    //  1   7  (大小为5)
    TreeNode* root6 = new TreeNode(20);
    root6->left = new TreeNode(10);
    root6->right = new TreeNode(30);
    root6->left->left = new TreeNode(5);
    root6->left->right = new TreeNode(15);
    root6->left->left->left = new TreeNode(1);
    root6->left->left->right = new TreeNode(7);
    root6->right->right = new TreeNode(25); // 30的右孩子是25, 破坏了(30)和(20)的BST

    std::cout << " 6:" << std::endl;
    std::cout << ": " << sol.largestBSTSubtree(root6) << " (: 5)" << std::endl;
    std::cout << std::endl;
    deleteTree(root6);


    return 0;
}