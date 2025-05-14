#include <algorithm> // 用于 std::max 和 std::min
#include <limits>    // 用于 std::numeric_limits

// 二叉搜索子树的最大键值和
// 给你一棵以 root 为根的二叉树
// 请你返回 任意 二叉搜索子树的最大键值和
// 二叉搜索树的定义如下：
// 任意节点的左子树中的键值都 小于 此节点的键值
// 任意节点的右子树中的键值都 大于 此节点的键值
// 任意节点的左子树和右子树都是二叉搜索树
// 测试链接 : https://leetcode.cn/problems/maximum-sum-bst-in-binary-tree/

// 二叉树节点的定义。
// (通常由 LeetCode 提供)
struct TreeNode {
    int val; // 节点值
    TreeNode *left; // 左子节点指针
    TreeNode *right; // 右子节点指针
    // 构造函数
    TreeNode(int x = 0, TreeNode *l = nullptr, TreeNode *r = nullptr) : val(x), left(l), right(r) {}
};

// 这个结构体将保存递归辅助函数返回的信息。

struct Info {
    int max_val;      // 子树中的最大值
    int min_val;      // 子树中的最小值
    int sum;          // 当前子树中节点的和 (如果它是 BST，这就是它的和)
    bool isBst;       // 如果当前子树是 BST，则为 true
    int maxBstSum;    // 在此子树的层级结构中目前找到的任何 BST 的最大和

    // 构造函数
    Info(int mx_v, int mn_v, int s, bool is_bst_subtree, int current_max_bst_sum) :
        max_val(mx_v), min_val(mn_v), sum(s), isBst(is_bst_subtree), maxBstSum(current_max_bst_sum) {}
};


// 在 LeetCode 上，这通常命名为 "Solution"
class Solution {
public:

    Info f(TreeNode* x) {
        if (x == nullptr) {
            // 空节点的基准情况：
            // max_val 设置为 int 类型的最小值。
            // min_val 设置为 int 类型的最大值。
            // 这种设置确保了对于父节点，一个空子节点
            // 能正确满足 BST 条件 (parent_val > null_child_max_val, parent_val < null_child_min_val)。
            // 和为 0。
            // 它被认为是一个 BST。
            // 源自此处/此下的最大 BST 和为 0。
            return Info(std::numeric_limits<int>::min(), // max_val 使用 int 的最小值
                        std::numeric_limits<int>::max(), // min_val 使用 int 的最大值
                        0,                               // 空树的和
                        true,                            // 空树是 BST
                        0);                              // 最大 BST 和为 0
        }

        // 从左右子节点递归获取信息
        Info info_left = f(x->left);
        Info info_right = f(x->right);

        // 计算以 x 为根的子树中的整体最小值和最大值。
        // 如果 x 的子树构成 BST，x 的父节点将使用这些值进行 BST 验证。
        // 题目约束允许节点值为 int 类型。
        int current_node_val = x->val; // 当前节点值

        // 确定当前节点子树的整体最小和最大值（如果这是一个 BST，则其父节点使用）
        // 注意：这些 max/min 是通用的 max/min，不一定来自下面的 BST 结构。

        // int max = Math.max(x.val, Math.max(infol.max, infor.max));
		// int min = Math.min(x.val, Math.min(infol.min, infor.min));
        // 然而，对于 BST 检查，我们只关心来自 BST 的 infol.max_val 和 infor.min_val。
        // Info 结构体的 max_val 和 min_val 是供当前节点 x
        // 的父节点用来检查 x 的子树（如果它是 BST）是否合适的。
        // 因此，如果 x 的子树是 BST，其整体最小值是 min(x.val, info_left.min_val)
        // 整体最大值是 max(x.val, info_right.max_val)。
        // 如果 x.left 或 x.right 为空，它们的 min/max 哨兵值会起作用。

        int overall_max_for_parent = std::max({current_node_val, info_left.max_val, info_right.max_val});
        int overall_min_for_parent = std::min({current_node_val, info_left.min_val, info_right.min_val});


        // 计算以 x 为根的当前子树中所有节点的和
        int current_subtree_sum = info_left.sum + info_right.sum + current_node_val;

        bool current_subtree_is_bst = false; // 当前子树是否为 BST
        int new_max_bst_sum; // 我们可以从此节点向上传递的最大 BST 和

        // 首先考虑的最大 BST 和是在子节点中找到的最优值。
        // 这处理了当前节点 x 没有扩展/形成 BST，但一个有效的 BST 可能完全存在于其左或右子树中的情况。
        // 题目描述提到“如果不存在这样的[和 > 0 的 BST]子树，则返回 0”。
        // LeetCode 通常意味着“最大和，如果所有 BST 和都是负数或0，则可以是负数或0”。

        // 因此，如果所有实际的 BST 和都是负数，这将导致 0，这似乎是 LeetCode 上
        // “最大和”问题的常见解释，如果“空集”或“无有效元素”导致 0。
        new_max_bst_sum = std::max(info_left.maxBstSum, info_right.maxBstSum);


        // 检查以 x 为根的子树是否为 BST：
        // 1. 左子树必须是 BST。
        // 2. 右子树必须是 BST。
        // 3. 左子树中的最大值必须小于 x 的值。
        // 4. 右子树中的最小值必须大于 x 的值。
        if (info_left.isBst && info_right.isBst &&
            info_left.max_val < current_node_val &&
            current_node_val < info_right.min_val) {

            current_subtree_is_bst = true;
            // 如果当前子树是 BST，其和为 'current_subtree_sum'。
            // 如果此和更大，我们用它更新 new_max_bst_sum。
            new_max_bst_sum = std::max(new_max_bst_sum, current_subtree_sum);
        }
        // 如果 current_subtree_is_bst 为 false，new_max_bst_sum 保持为子节点中的最大值。

        // 返回为节点 x 收集的信息
        // 返回的 max/min 是以 x 为根的子树的真实 max/min，
        // 无论 x 是否形成 BST。这是 x 的父节点在检查
        // x 的父节点是否能与 x (作为子节点) 形成 BST 时所需要的。
        return Info(overall_max_for_parent, // 这是 x 下整个子树中的最大值，如果 x 的子树是较大 BST 的一部分，则 x 的父节点使用此值
                    overall_min_for_parent, // 这是 x 下整个子树中的最小值
                    current_subtree_sum,    // 这是 x 子树中所有节点的和，如果 x 是 BST 则使用此值
                    current_subtree_is_bst, // x 处的子树是否是 BST？
                    new_max_bst_sum);       // 到目前为止在 x 下找到的整体最大 BST 和
    }


    int maxSumBST(TreeNode* root) {
        // 问题要求最大和，和可以是负数。
        // 如果所有 BST 和都是负数，答案应该是这些负数和中的最大值。
        // 然而，如果约定（像 LeetCode 中经常出现的那样）是如果所有有效的和都 <=0 则返回 0，
        // 那么当前的逻辑（对于空节点将 maxBstSum 初始化为 0 并传播）是有效的。

        // 因为 `maxBstSum` 初始化为 0，且 `Math.max(0, negative_sum)` 为 0。
        // 如果允许负数和，则空情况下的 `maxBstSum` 应初始化为一个非常小的数。

        return f(root).maxBstSum;
    }
};

