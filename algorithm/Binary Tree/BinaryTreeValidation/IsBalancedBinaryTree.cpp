#include <bits/stdc++.h>
using namespace std;
class ReturnType {
public:
    bool isBalanced;    // 子树是否平衡
    int height;         // 子树的高度

    // 构造函数，初始化子树的平衡状态和高度
    ReturnType(bool isB, int hei) : isBalanced(isB), height(hei) {}
};

// 递归函数，检查以节点 x 为根的子树是否平衡，并返回子树的状态信息
ReturnType process(Node* x) {
    // 如果节点 x 为空，返回一个表示平衡且高度为 0 的 ReturnType 对象
    if (x == nullptr) {
        return ReturnType(true, 0);
    }

    // 递归地获取左子树的状态信息
    ReturnType leftData = process(x->left);
    // 递归地获取右子树的状态信息
    ReturnType rightData = process(x->right);

    // 计算当前节点的高度为左右子树高度的最大值加 1
    int height = std::max(leftData.height, rightData.height) + 1;

    // 判断当前节点是否平衡：左子树和右子树都平衡且左右子树高度差小于 2
    bool isBalanced = leftData.isBalanced && rightData.isBalanced
            && std::abs(leftData.height - rightData.height) < 2;

    // 返回当前节点的状态信息
    return ReturnType(isBalanced, height);
}

// 检查整棵树是否平衡
bool isBalanced(Node* head) {
    // 调用 process 函数检查以 head 为根的树是否平衡，并返回结果
    return process(head).isBalanced;
}
