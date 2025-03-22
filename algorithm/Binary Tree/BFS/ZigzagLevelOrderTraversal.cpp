#include<bits/stdc++.h>
using namespace std;

// 定义二叉树的节点结构
struct TreeNode {
    int val; // 节点的值
    TreeNode* left; // 左子节点指针
    TreeNode* right; // 右子节点指针
    TreeNode(int x) : val(x), left(NULL), right(NULL) {} // 构造函数初始化节点值和左右子节点指针
};

// 实现二叉树的锯齿形层次遍历的类
class ZigzagLevelOrderTraversal {
public:
    // 主函数：实现锯齿形层次遍历
    vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
        vector<vector<int>> ans; // 用于存储最终结果的二维向量
        if (root == nullptr) { // 如果根节点为空，直接返回空的结果
            return ans;
        }

        queue<TreeNode*> q; // 创建队列用于层次遍历
        q.push(root); // 将根节点压入队列
        bool reverse = false; // 标志位，用于控制是否反转当前层的遍历顺序

        while (!q.empty()) { // 当队列不为空时，继续遍历
            int size = q.size(); // 获取当前层的节点数量
            vector<int> level(size); // 用于存储当前层节点值的向量

            for (int i = 0; i < size; ++i) { // 遍历当前层的所有节点
                TreeNode* cur = q.front(); // 获取队列头部的节点
                q.pop(); // 弹出该节点

                // 根据reverse标志决定插入的位置
                // 如果需要反转，则从后往前插入；否则从前往后插入
                int index = reverse ? (size - 1 - i) : i;
                level[index] = cur->val; // 将当前节点的值存入对应位置

                // 将当前节点的左右子节点压入队列（如果存在）
                if (cur->left) {
                    q.push(cur->left);
                }
                if (cur->right) {
                    q.push(cur->right);
                }
            }

            ans.push_back(level); // 将当前层的结果加入最终结果
            reverse = !reverse; // 切换反转标志
        }

        return ans; // 返回最终结果
    }
};


int main() {
    // 创建一个示例二叉树
    TreeNode* root = new TreeNode(3);
    root->left = new TreeNode(9);
    root->right = new TreeNode(20);
    root->right->left = new TreeNode(15);
    root->right->right = new TreeNode(7);

    // 创建 ZigzagLevelOrderTraversal 类的对象
    ZigzagLevelOrderTraversal traversal;

    // 调用 zigzagLevelOrder 函数并获取结果
    vector<vector<int>> result = traversal.zigzagLevelOrder(root);

    // 打印结果
    for (const auto& level : result) {
        for (int val : level) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    // 释放内存
    delete root->right->right;
    delete root->right->left;
    delete root->right;
    delete root->left;
    delete root;

    return 0;
}
