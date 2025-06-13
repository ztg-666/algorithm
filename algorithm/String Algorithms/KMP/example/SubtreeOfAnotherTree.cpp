#include <vector>
#include <string>
#include <functional>

// 另一棵树的子树
// 给你两棵二叉树root和subRoot
// 检验root中是否包含和subRoot具有相同结构和节点值的子树
// 如果存在，返回true
// 否则，返回false
// 测试链接 : https://leetcode.cn/problems/subtree-of-another-tree/

using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x = 0) : val(x), left(nullptr), right(nullptr) {}
};

// 方法1：暴力递归解法 O(n*m)
// 通过深度优先搜索遍历每个节点，与目标子树进行全匹配
class Solution1 {
public:
    // 判断subRoot是否是root的子树
    // 参数：root - 主树根节点，subRoot - 目标子树根节点
    // 返回值：存在子树则返回true，否则false
    bool isSubtree(TreeNode* root, TreeNode* subRoot) {
        if (!subRoot) return true;  // 空树视为任意树的子树
        if (!root) return false;    // 主树为空但目标树非空
        return isSame(root, subRoot) ||   // 当前节点匹配
               isSubtree(root->left, subRoot) ||  // 左子树递归检查
               isSubtree(root->right, subRoot);   // 右子树递归检查
    }

private:
    // 递归判断两棵树结构完全一致
    // 参数：a - 树A节点，b - 树B节点
    // 返回值：结构值相同返回true，否则false
    bool isSame(TreeNode* a, TreeNode* b) {
        if (!a && !b) return true;  // 两节点都为空视为相等
        if (!a || !b) return false; // 单一节点为空视为不等
        return a->val == b->val &&  // 当前节点值相等
               isSame(a->left, b->left) &&  // 递归比较左子树
               isSame(a->right, b->right);  // 递归比较右子树
    }
};

// 方法2：序列化+KMP解法 O(n+m)
// 通过二叉树序列化为字符串模式，使用KMP算法进行模式匹配
class Solution2 {
public:
    bool isSubtree(TreeNode* root, TreeNode* subRoot) {
        if (!subRoot) return true;  // 处理空目标树特殊情况
        if (!root) return false;     // 主树为空但目标树非空

        vector<string> s1, s2;
        serialize(root, s1);    // 序列化主树为字符串数组
        serialize(subRoot, s2); // 序列化目标树为字符串数组

        // 使用KMP算法进行模式匹配
        return kmpMatch(s1, s2) != -1;
    }

private:
    // 先序序列化二叉树（带空节点标记）
    // 参数：node - 当前节点，path - 序列化结果存储数组
    void serialize(TreeNode* node, vector<string>& path) {
        if (!node) {
            path.push_back("null");  // 空节点特殊标记
            return;
        }
        path.push_back(to_string(node->val));  // 当前节点值转为字符串
        serialize(node->left, path);  // 递归序列化左子树
        serialize(node->right, path); // 递归序列化右子树
    }

    // KMP模式匹配算法实现
    // 参数：s1 - 主串，s2 - 模式串
    // 返回值：匹配起始索引，未找到返回-1
    int kmpMatch(const vector<string>& s1, const vector<string>& s2) {
        int n = s1.size(), m = s2.size();
        if (m == 0) return 0;  // 空模式串特殊处理

        vector<int> next = buildNext(s2);  // 构建next数组
        int i = 0, j = 0;  // i-主串指针，j-模式串指针

        while (i < n && j < m) {
            if (s1[i] == s2[j]) {  // 当前字符匹配成功
                ++i;
                ++j;
            } else if (j == 0) {   // 模式串首字符不匹配
                ++i;               // 主串指针后移
            } else {               // 根据next数组回溯模式串指针
                j = next[j];       // 关键优化步骤
            }
        }
        return j == m ? i - j : -1;  // 完全匹配返回起始位置
    }

    // 构建KMP算法的next数组（部分匹配表）
    // 参数：pattern - 模式串
    // 返回值：next数组
    vector<int> buildNext(const vector<string>& pattern) {
        int m = pattern.size();
        vector<int> next(m, 0);
        if (m > 0) next[0] = -1;  // 首字符无前缀
        if (m > 1) next[1] = 0;   // 单字符无公共前后缀

        int i = 2, cn = 0;  // i-当前计算位置，cn-当前最长公共前后缀长度
        while (i < m) {
            if (pattern[i-1] == pattern[cn]) {  // 当前字符与前缀下一个字符匹配
                next[i++] = ++cn;  // 设置当前位置的匹配值并后移指针
            } else if (cn > 0) {   // 存在更短的前缀可尝试
                cn = next[cn];     // 回溯到前一个前缀位置
            } else {               // 无公共前缀可用
                next[i++] = 0;     // 匹配值设为0并后移指针
            }
        }
        return next;
    }
};
