#include<bits/stdc++.h>
using namespace std;

// 定义二叉树节点结构
struct TreeNode {
    int val; // 节点的值
    TreeNode* left; // 左子节点指针
    TreeNode* right; // 右子节点指针
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {} // 构造函数，初始化节点值和左右子节点为空
};

class LevelOrderTraversal {
public:
    // 普通BFS方法，不推荐
    std::vector<std::vector<int>> levelOrder1(TreeNode* root) {
        std::vector<std::vector<int>> ans; // 存储最终结果的二维向量
        if (root != nullptr) { // 如果根节点不为空
            std::queue<TreeNode*> queue; // 队列用于存储待处理的节点
            std::unordered_map<TreeNode*, int> levels; // 哈希表用于记录每个节点所在的层数
            queue.push(root); // 将根节点加入队列
            levels[root] = 0; // 根节点所在层为0
            while (!queue.empty()) { // 当队列不为空时
                TreeNode* cur = queue.front(); // 获取队列头部节点
                queue.pop(); // 弹出队列头部节点
                int level = levels[cur]; // 获取当前节点所在的层数
                if (ans.size() == level) { // 如果当前层数的结果数组还未创建
                    ans.push_back(std::vector<int>()); // 创建新的结果数组
                }
                ans[level].push_back(cur->val); // 将当前节点值加入对应层的结果数组中
                if (cur->left != nullptr) { // 如果左子节点存在
                    queue.push(cur->left); // 将左子节点加入队列
                    levels[cur->left] = level + 1; // 记录左子节点所在的层数
                }
                if (cur->right != nullptr) { // 如果右子节点存在
                    queue.push(cur->right); // 将右子节点加入队列
                    levels[cur->right] = level + 1; // 记录右子节点所在的层数
                }
            }
        }
        return ans; // 返回结果
    }

    // 每次处理一层的优化BFS方法，推荐
    std::vector<std::vector<int>> levelOrder2(TreeNode* root) {
        std::vector<std::vector<int>> ans; // 存储最终结果的二维向量
        if (root != nullptr) { // 如果根节点不为空
            TreeNode* queue[2001]; // 使用数组模拟队列，存储待处理的节点
            int l = 0, r = 0; // 队列的左右指针，l表示队列头，r表示队列尾
            queue[r++] = root; // 将根节点加入队列
            while (l < r) { // 当队列不为空时
                int size = r - l; // 当前层的节点数
                std::vector<int> list; // 存储当前层节点值的向量
                for (int i = 0; i < size; i++) { // 遍历当前层的所有节点
                    TreeNode* cur = queue[l++]; // 获取队列头部节点并弹出
                    list.push_back(cur->val); // 将当前节点值加入当前层的结果数组中
                    if (cur->left != nullptr) { // 如果左子节点存在
                        queue[r++] = cur->left; // 将左子节点加入队列
                    }
                    if (cur->right != nullptr) { // 如果右子节点存在
                        queue[r++] = cur->right; // 将右子节点加入队列
                    }
                }
                ans.push_back(list); // 将当前层的结果数组加入最终结果中
            }
        }
        return ans; // 返回结果
    }
};

int main() {
    // 创建一个示例二叉树
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    root->left->left = new TreeNode(4);
    root->left->right = new TreeNode(5);
    root->right->left = new TreeNode(6);
    root->right->right = new TreeNode(7);

    LevelOrderTraversal solution;

    // 使用普通BFS方法进行层次遍历
    std::vector<std::vector<int>> result1 = solution.levelOrder1(root);
    std::cout << "Level Order Traversal using levelOrder1:" << std::endl;
    for (const auto& level : result1) {
        for (int val : level) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    // 使用优化的BFS方法进行层次遍历
    std::vector<std::vector<int>> result2 = solution.levelOrder2(root);
    std::cout << "Level Order Traversal using levelOrder2:" << std::endl;
    for (const auto& level : result2) {
        for (int val : level) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    // 释放内存
    delete root->right->right;
    delete root->right->left;
    delete root->right;
    delete root->left->right;
    delete root->left->left;
    delete root->left;
    delete root;

    return 0;
}