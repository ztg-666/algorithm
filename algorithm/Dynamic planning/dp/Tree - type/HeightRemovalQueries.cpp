#include <vector>      // 包含 std::vector
#include <algorithm>   // 包含 std::max 和 std::fill
#include <map>         // 包含 std::map

// 移除子树后的二叉树高度
// 给你一棵 二叉树 的根节点 root ，树中有 n 个节点
// 每个节点都可以被分配一个从 1 到 n 且互不相同的值
// 另给你一个长度为 m 的数组 queries
// 你必须在树上执行 m 个 独立 的查询，其中第 i 个查询你需要执行以下操作：
// 从树中 移除 以 queries[i] 的值作为根节点的子树
// 题目所用测试用例保证 queries[i] 不等于根节点的值
// 返回一个长度为 m 的数组 answer
// 其中 answer[i] 是执行第 i 个查询后树的高度
// 注意：
// 查询之间是独立的，所以在每个查询执行后，树会回到其初始状态
// 树的高度是从根到树中某个节点的 最长简单路径中的边数
// 测试链接 : https://leetcode.cn/problems/height-of-binary-tree-after-subtree-removal-queries/

// 二叉树节点的定义
struct TreeNode {
    int val;          // 节点值
    TreeNode *left;   // 左子节点指针
    TreeNode *right;  // 右子节点指针
    TreeNode() : val(0), left(nullptr), right(nullptr) {} // 默认构造函数
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {} // 带值的构造函数
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {} // 带值和子节点的构造函数
};

class Solution {
public:
    static const int MAXN = 100010;

    // 存储每个节点值的 DFN (发现/结束时间) 序号
    // 键: 节点值, 值: DFN 序号
    std::map<int, int> dfn_map;

    // 基于 DFN 序号存储的信息
    // 索引: DFN 序号
    int deep[MAXN]; // DFN 序号对应节点的深度
    int size[MAXN]; // 以 DFN 序号对应节点为根的子树的大小

    // 预计算的最大深度
    // 索引: DFN 序号
    int maxl[MAXN]; // 从 DFN 1 到 i 的最大深度
    int maxr[MAXN]; // 从 DFN i 到 dfnCnt 的最大深度

    int dfnCnt; // DFN 计数器，记录当前分配到的 DFN 序号

    // DFS (深度优先搜索) 函数，用于填充 dfn_map, deep, 和 size 数组
    // x: 当前节点指针
    // k: 当前节点的深度
    void dfs(TreeNode* x, int k) {
        if (x == nullptr) { // 基线条件：如果节点为空，则返回
            return;
        }
        int current_dfn_order = ++dfnCnt;    // DFN 计数器增加，并分配给当前节点
        dfn_map[x->val] = current_dfn_order; // 记录节点值对应的 DFN 序号
        deep[current_dfn_order] = k;         // 记录当前 DFN 序号节点的深度
        size[current_dfn_order] = 1;         // 当前 DFN 序号节点的子树大小初始化为 1 (自身)

        if (x->left != nullptr) { // 如果存在左子节点
            dfs(x->left, k + 1);  // 递归处理左子节点，深度加 1
            // 更新当前节点子树的大小：加上左子树的大小
            size[current_dfn_order] += size[dfn_map[x->left->val]];
        }
        if (x->right != nullptr) { // 如果存在右子节点
            dfs(x->right, k + 1); // 递归处理右子节点，深度加 1
            // 更新当前节点子树的大小：加上右子树的大小
            size[current_dfn_order] += size[dfn_map[x->right->val]];
        }
    }

    std::vector<int> treeQueries(TreeNode* root, std::vector<int>& queries) {
        dfs(root, 0); // 从根节点开始进行 DFS，初始深度为 0

        // 预计算 maxl: 从 DFN 1 到 i 的最大深度
        // maxl[0] 隐式为 0 或一个非常小的数（如果需要），但循环从 1 开始。
        // 这里，maxl[i] 存储 max(maxl[i-1], deep[i])
        maxl[0] = 0; // 作为 maxl[1] 的基准
        for (int i = 1; i <= dfnCnt; i++) {
            maxl[i] = std::max(maxl[i - 1], deep[i]);
        }

        // 预计算 maxr: 从 DFN i 到 dfnCnt 的最大深度
        // maxr[dfnCnt + 1] 是基准。
        maxr[dfnCnt + 1] = 0;
        for (int i = dfnCnt; i >= 1; i--) {
            maxr[i] = std::max(maxr[i + 1], deep[i]);
        }

        int m = queries.size();            // 获取查询的数量
        std::vector<int> ans(m);           // 初始化答案数组
        for (int i = 0; i < m; i++) {
            int query_node_val = queries[i];                // 当前查询的节点值
            int dfn_of_query_node = dfn_map[query_node_val]; // 获取查询节点值的 DFN 序号

            // 要移除的子树的 DFN 范围是 [dfn_of_query_node, dfn_of_query_node + size[dfn_of_query_node] - 1]
            // 此范围左侧的节点结束于 dfn_of_query_node - 1
            // 此范围右侧的节点开始于 dfn_of_query_node + size[dfn_of_query_node]

            // 查询节点 DFN 序号左边的部分的最大深度
            int left_max_depth = (dfn_of_query_node == 1) ? 0 : maxl[dfn_of_query_node - 1];
            // 查询节点 DFN 序号 + 其子树大小 右边的部分的最大深度
            int right_max_depth_idx = dfn_of_query_node + size[dfn_of_query_node];
            int right_max_depth = (right_max_depth_idx > dfnCnt + 1) ? 0 : maxr[right_max_depth_idx];

            ans[i] = std::max(left_max_depth, right_max_depth); // 答案是这两部分最大深度的较大者
        }
        return ans;
    }
};
