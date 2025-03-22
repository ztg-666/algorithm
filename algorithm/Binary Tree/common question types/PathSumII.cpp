#include<bits/stdc++.h>
using namespace std;

// 定义树节点结构，每个节点包含一个整数值以及左右子节点指针
struct TreeNode {
    int val; // 节点的值
    TreeNode* left; // 左子节点
    TreeNode* right; // 右子节点
    TreeNode(int x) : val(x), left(NULL), right(NULL) {} // 构造函数初始化节点值和子节点指针
};

// PathSumII 类用于解决路径和问题
class PathSumII {
public:
    // 主函数：找到所有从根节点到叶节点的路径，使得路径上的节点值之和等于目标值aim
    vector<vector<int>> pathSum(TreeNode* root, int aim) {
        vector<vector<int>> ans; // 存储最终结果的二维向量
        if (root != nullptr) { // 如果根节点不为空
            vector<int> path; // 用于存储当前路径上的节点值
            f(root, aim, 0, path, ans); // 调用递归辅助函数进行深度优先搜索
        }
        return ans; // 返回所有满足条件的路径
    }

private:
    // 辅助递归函数：用于遍历树并寻找符合条件的路径
    void f(TreeNode* cur, int aim, int sum, vector<int>& path, vector<vector<int>>& ans) {
        if (cur->left == nullptr && cur->right == nullptr) { // 如果当前节点是叶节点
            // 检查当前路径的节点值之和是否等于目标值aim
            if (cur->val + sum == aim) {
                path.push_back(cur->val); // 将当前节点值加入路径
                ans.push_back(path); // 将当前路径加入结果集
                path.pop_back(); // 回溯，移除最后一个节点值
            }
        } else { // 如果当前节点不是叶节点
            path.push_back(cur->val); // 将当前节点值加入路径
            if (cur->left != nullptr) { // 如果存在左子节点
                f(cur->left, aim, sum + cur->val, path, ans); // 递归处理左子树
            }
            if (cur->right != nullptr) { // 如果存在右子节点
                f(cur->right, aim, sum + cur->val, path, ans); // 递归处理右子树
            }
            path.pop_back(); // 回溯，移除最后一个节点值
        }
    }
};

void freeTree(TreeNode* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}
int main() {
    // 创建一个示例二叉树
    TreeNode* root = new TreeNode(5);
    root->left = new TreeNode(4);
    root->right = new TreeNode(8);
    root->left->left = new TreeNode(11);
    root->left->left->left = new TreeNode(7);
    root->left->left->right = new TreeNode(2);
    root->right->left = new TreeNode(13);
    root->right->right = new TreeNode(4);
    root->right->right->left = new TreeNode(5);
    root->right->right->right = new TreeNode(1);

    // 创建 PathSumII 对象
    PathSumII pathSumII;

    // 定义目标值
    int aim = 22;

    // 调用 pathSum 方法获取所有符合条件的路径
    vector<vector<int>> paths = pathSumII.pathSum(root, aim);

    // 输出结果
    std::cout << "Paths that sum to " << aim << " are:" << std::endl;
    for (const auto& path : paths) {
        std::cout << "[";
        for (size_t i = 0; i < path.size(); ++i) {
            std::cout << path[i];
            if (i < path.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "]" << std::endl;
    }

    // 释放内存
    freeTree(root);

    return 0;
}