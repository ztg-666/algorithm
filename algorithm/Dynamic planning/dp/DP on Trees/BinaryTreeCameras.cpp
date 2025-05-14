#include <algorithm> // Potentially for std::min, std::max if used, though not in this specific logic

// 监控二叉树
// 给定一个二叉树，我们在树的节点上安装摄像头
// 节点上的每个摄影头都可以监视其父对象、自身及其直接子对象
// 计算监控树的所有节点所需的最小摄像头数量
// 测试链接 : https://leetcode.cn/problems/binary-tree-cameras/

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
    // 用于累计摄像头数量的成员变量，对应 Java 代码中的静态变量 ans
    int ans;

    // 递归辅助函数，与 Java 中的 f 方法对应
    // 返回值含义:
    // 0: 节点 x 本身未被覆盖，但其子树节点均已被覆盖。
    // 1: 节点 x 本身已被覆盖，但 x 上没有放置摄像头，其子树节点均已被覆盖。
    // 2: 节点 x 本身已被覆盖，且 x 上放置了摄像头，其子树节点均已被覆盖。
    int f(TreeNode* x) {
        if (x == nullptr) {
            // 空节点可以被视为“已覆盖且其上无摄像头”。
            // 它不需要被监控，也不会主动去监控其他节点。
            // 返回 1 使得父节点不需要为了这个空孩子而放置摄像头。
            return 1;
        }

        int left_status = f(x->left);   // 获取左子节点的状态
        int right_status = f(x->right); // 获取右子节点的状态

        // 情况 1: 如果任一子节点未被覆盖 (状态为 0)
        // 那么当前节点 x 必须放置一个摄像头。
        // 这个摄像头将覆盖 x, x->left, x->right (如果存在) 以及 x 的父节点。
        if (left_status == 0 || right_status == 0) {
            ans++; // 放置一个摄像头
            return 2; // x 被覆盖且其上有摄像头
        }

        // 情况 2: 如果两个子节点都已被覆盖，且它们自身都没有摄像头 (状态都为 1)
        // 这意味着 x 目前没有被其子节点覆盖。
        // x 自身变为未覆盖状态，需要其父节点来覆盖它。
        if (left_status == 1 && right_status == 1) {
            return 0; // x 未被覆盖
        }

        // 情况 3: 两个子节点都已被覆盖，并且至少一个子节点上有摄像头
        // (即 left_status 为 2 或 right_status 为 2, 或者两者都为 2)
        // (另一种方式理解是：排除了 left_status==0, right_status==0 的情况，
        //  也排除了 left_status==1 && right_status==1 的情况)
        // 那么当前节点 x 已经被其子节点上的摄像头覆盖了。
        // x 不需要再放置摄像头。
        return 1; // x 已被覆盖，但其上无摄像头
    }

    // 主函数，计算所需的最小摄像头数量
    // 对应 Java 中的 minCameraCover 方法
    int minCameraCover(TreeNode* root) {
        ans = 0; // 初始化摄像头数量

        // 调用递归函数处理整棵树。
        // 如果根节点在处理后仍未被覆盖 (状态为 0)，
        // 则必须在根节点放置一个摄像头。
        if (f(root) == 0) {
            ans++;
        }
        return ans;
    }
};

// === 用于测试的辅助代码 (可选) ===

#include <iostream>
#include <vector>
#include <queue>

// 函数：根据层序遍历数组创建二叉树 (LeetCode 常用方式)
TreeNode* createTree(const std::vector<int>& values) {
    if (values.empty() || values[0] == -1) { // -1 表示 null
        return nullptr;
    }
    TreeNode* root = new TreeNode(values[0]);
    std::queue<TreeNode*> q;
    q.push(root);
    int i = 1;
    while (i < values.size() && !q.empty()) {
        TreeNode* current = q.front();
        q.pop();
        if (values[i] != -1) {
            current->left = new TreeNode(values[i]);
            q.push(current->left);
        }
        i++;
        if (i < values.size() && values[i] != -1) {
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

    // 示例 1: root = [0,0,null,0,0] -> LeetCode format uses null for empty nodes
    // Custom createTree uses -1 for null for simplicity in vector
    std::vector<int> values1 = {0, 0, -1, 0, 0};
    TreeNode* root1 = createTree(values1);
    std::cout << "Example 1 Output: " << sol.minCameraCover(root1) << std::endl; // Expected: 1
    deleteTree(root1);
    sol.ans = 0; // Reset ans for next test case if Solution object is reused

    // 示例 2: root = [0,null,0,null,0,null,0]
    std::vector<int> values2 = {0, -1, 0, -1, 0, -1, 0};
    TreeNode* root2 = createTree(values2);
    std::cout << "Example 2 Output: " << sol.minCameraCover(root2) << std::endl; // Expected: 2
    deleteTree(root2);
    sol.ans = 0;

    // 示例 3: root = [0]
    std::vector<int> values3 = {0};
    TreeNode* root3 = createTree(values3);
    std::cout << "Example 3 Output: " << sol.minCameraCover(root3) << std::endl; // Expected: 1
    deleteTree(root3);
    sol.ans = 0;

    // 示例 4: root = [0,0,0] (all children of root)
    std::vector<int> values4 = {0,0,0};
    TreeNode* root4 = createTree(values4);
    std::cout << "Example 4 Output: " << sol.minCameraCover(root4) << std::endl; // Expected: 1
    deleteTree(root4);


    return 0;
}

