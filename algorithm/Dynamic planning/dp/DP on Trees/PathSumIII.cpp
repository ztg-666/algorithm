#include <unordered_map> // For std::unordered_map

// 路径总和 III
// 给定一个二叉树的根节点 root ，和一个整数 targetSum
// 求该二叉树里节点值之和等于 targetSum 的 路径 的数目
// 路径 不需要从根节点开始，也不需要在叶子节点结束
// 但是路径方向必须是向下的（只能从父节点到子节点）
// 测试链接 : https://leetcode.cn/problems/path-sum-iii/

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

    int count_paths; // Renamed from ans for clarity

    // x: 当前节点
    // target_sum_val: 目标路径和
    // current_path_sum: 从根节点到当前节点 x 的路径和 (不包括 x 之前的任何分支)
    // prefix_sums: 哈希表，存储从根到当前递归路径上，所有前缀和及其出现的次数
    void find_paths_recursive(TreeNode* x, int target_sum_val, long long current_path_sum, std::unordered_map<long long, int>& prefix_sums) {
        if (x == nullptr) {
            return;
        }

        // 1. 更新从根到当前节点 x 的路径和
        current_path_sum += x->val;

        // 2. 检查是否存在一个前缀和 `prev_sum` 使得 `current_path_sum - prev_sum == target_sum_val`
        //    这等同于查找 `prev_sum = current_path_sum - target_sum_val`
        //    如果 `prefix_sums` 包含 `current_path_sum - target_sum_val`，
        //    那么以 x 结尾的、和为 target_sum_val 的路径数量就是 `prefix_sums[current_path_sum - target_sum_val]`
        if (prefix_sums.count(current_path_sum - target_sum_val)) {
            count_paths += prefix_sums[current_path_sum - target_sum_val];
        }

        // 3. 将当前路径和加入到哈希表中，表示这个前缀和又出现了一次
        prefix_sums[current_path_sum]++;

        // 4. 递归处理左右子树
        find_paths_recursive(x->left, target_sum_val, current_path_sum, prefix_sums);
        find_paths_recursive(x->right, target_sum_val, current_path_sum, prefix_sums);

        // 5. 回溯：当离开当前节点 x 的递归调用时，
        //    需要将当前路径和 `current_path_sum` 在哈希表中的计数减一。
        //    这是因为 `current_path_sum` (以 x 的父节点到 x 结尾的路径和)
        //    对于 x 的兄弟节点或 x 父节点的其他分支来说，不是一个有效的前缀和。
        prefix_sums[current_path_sum]--;
        // 可选: 如果计数变为0，可以从map中移除，但对于此问题，仅递减即可。
        // if (prefix_sums[current_path_sum] == 0) {
        //     prefix_sums.erase(current_path_sum);
        // }
    }

    // 主函数，计算路径和等于 targetSum 的路径数量

    int pathSum(TreeNode* root, int targetSum) {
        // 哈希表，键是前缀和，值是该前缀和出现的次数

        std::unordered_map<long long, int> prefix_sums;

        // 初始化：前缀和为 0 出现过 1 次（代表空路径，或者说从 "根之前" 开始的路径和为0）
        // 这对于处理从根节点开始的路径是必要的。
        // 例如，如果根节点的值就是 targetSum, 那么 current_path_sum = root->val.
        // 我们需要找到 current_path_sum - targetSum = 0 在 prefix_sums 中的计数。
        prefix_sums[0] = 1; // 0LL 表示 long long 类型的 0

        count_paths = 0; // 初始化路径计数器

        // 从根节点开始递归，初始的 "上方累加和" 为 0
        find_paths_recursive(root, targetSum, 0LL, prefix_sums);

        return count_paths;
    }
};

// === 用于测试的辅助代码 (可选) ===

#include <iostream>
#include <vector>
#include <queue>

// 函数：根据层序遍历数组创建二叉树 (LeetCode 常用方式)
TreeNode* createTree(const std::vector<int>& values) {
    if (values.empty() || values[0] == -9999) { // Using -9999 as a placeholder for null
        return nullptr;
    }
    TreeNode* root = new TreeNode(values[0]);
    std::queue<TreeNode*> q;
    q.push(root);
    int i = 1;
    while (i < values.size() && !q.empty()) {
        TreeNode* current = q.front();
        q.pop();
        if (values[i] != -9999) {
            current->left = new TreeNode(values[i]);
            q.push(current->left);
        }
        i++;
        if (i < values.size() && values[i] != -9999) {
            current->right = new TreeNode(values[i]);
            q.push(current->right);
        }
        i++;
    }
    return root;
}

// 函数：删除二叉树 (释放内存)
void deleteTree(TreeNode* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    Solution sol;

    // 示例 1: root = [10,5,-3,3,2,null,11,3,-2,null,1], targetSum = 8
    // LeetCode format uses null. Using -9999 as a placeholder for null in vector.
    std::vector<int> values1 = {10, 5, -3, 3, 2, -9999, 11, 3, -2, -9999, 1};
    TreeNode* root1 = createTree(values1);
    int target1 = 8;
    std::cout << "Example 1 Output: " << sol.pathSum(root1, target1) << std::endl; // Expected: 3
    deleteTree(root1);

    // 示例 2: root = [5,4,8,11,null,13,4,7,2,null,null,5,1], targetSum = 22
    std::vector<int> values2 = {5, 4, 8, 11, -9999, 13, 4, 7, 2, -9999, -9999, 5, 1};
    TreeNode* root2 = createTree(values2);
    int target2 = 22;
    std::cout << "Example 2 Output: " << sol.pathSum(root2, target2) << std::endl; // Expected: 3
    deleteTree(root2);

    // 示例 3: root = [1], targetSum = 0
    std::vector<int> values3 = {1};
    TreeNode* root3 = createTree(values3);
    int target3 = 0;
    std::cout << "Example 3 Output: " << sol.pathSum(root3, target3) << std::endl; // Expected: 0
    deleteTree(root3);

    // 示例 4: root = [1], targetSum = 1
    std::vector<int> values4 = {1};
    TreeNode* root4 = createTree(values4);
    int target4 = 1;
    std::cout << "Example 4 Output: " << sol.pathSum(root4, target4) << std::endl; // Expected: 1
    deleteTree(root4);

    TreeNode* r5 = new TreeNode(0);
    r5->left = new TreeNode(0);
    r5->right = new TreeNode(0);


    deleteTree(r5);


    return 0;
}

