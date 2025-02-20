#include <bits/stdc++.h>
using namespace std;
// 中序遍历二叉树并将节点按顺序存储到 inOrderList 中
void process(Node* node, std::vector<Node*>& inOrderList) {
    // 如果当前节点为空，直接返回
    if (node == nullptr) {
        return;
    }
    // 递归遍历左子树
    process(node->left, inOrderList);
    // 将当前节点添加到 inOrderList 中
    inOrderList.push_back(node);
    // 递归遍历右子树
    process(node->right, inOrderList);
}

// 判断给定的二叉树是否为二叉搜索树
bool isBST(Node* head) {
    // 如果根节点为空，返回 true，因为空树也是二叉搜索树
    if (head == nullptr) {
        return true;
    }
    // 存储中序遍历结果的向量
    std::vector<Node*> inOrderList;
    // 调用 process 函数进行中序遍历
    process(head, inOrderList);
    // 初始化前一个节点的值为整数最小值
    int pre = std::numeric_limits<int>::min();
    // 遍历中序遍历结果
    for (Node* cur : inOrderList) {
        // 如果当前节点的值不大于前一个节点的值，返回 false
        if (pre >= cur->value) {
            return false;
        }
        // 更新前一个节点的值为当前节点的值
        pre = cur->value;
    }
    // 如果所有节点都满足条件，返回 true
    return true;
}
