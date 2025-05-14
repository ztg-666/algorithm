#include <cmath>
#include <algorithm>

// 在二叉树中分配硬币
// 给你一个有 n 个结点的二叉树的根结点 root
// 其中树中每个结点 node 都对应有 node.val 枚硬币
// 整棵树上一共有 n 枚硬币
// 在一次移动中，我们可以选择两个相邻的结点，然后将一枚硬币从其中一个结点移动到另一个结点
// 移动可以是从父结点到子结点，或者从子结点移动到父结点
// 返回使每个结点上 只有 一枚硬币所需的 最少 移动次数
// 测试链接 : https://leetcode.cn/problems/distribute-coins-in-binary-tree/

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
    // 辅助结构体，用于在递归中传递多个信息

    struct Info {
        int cnt;  // 子树中的节点数量
        int sum;  // 子树中金币的总数
        int move; // 使子树平衡所需的最小移动次数

        Info(int c, int s, int m) : cnt(c), sum(s), move(m) {}
    };

    // 递归函数，计算以节点 x 为根的子树的信息

    Info f(TreeNode* x) {
        if (x == nullptr) {
            return Info(0, 0, 0); // 空节点，节点数为0，金币数为0，移动次数为0
        }

        Info infol = f(x->left);   // 递归处理左子树
        Info infor = f(x->right);  // 递归处理右子树

        // 计算当前子树的节点总数
        int cnts = infol.cnt + infor.cnt + 1;
        // 计算当前子树的金币总数
        int sums = infol.sum + infor.sum + x->val;

        // 计算总的移动次数：
        // infol.move: 左子树内部平衡所需的移动次数
        // infor.move: 右子树内部平衡所需的移动次数
        // std::abs(infol.cnt - infol.sum): 左子树需要向父节点(x)上交或从父节点获取的金币数，即跨越 x 与 x->left 边的移动次数
        // std::abs(infor.cnt - infor.sum): 右子树需要向父节点(x)上交或从父节点获取的金币数，即跨越 x 与 x->right 边的移动次数
        int moves = infol.move + infor.move + std::abs(infol.cnt - infol.sum) + std::abs(infor.cnt - infor.sum);

        return Info(cnts, sums, moves);
    }

    int distributeCoins(TreeNode* root) {
        return f(root).move;
    }
};
