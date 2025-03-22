#include <bits/stdc++.h>
using namespace std;
bool isCBT(Node* head) {
    // 如果根节点为空，认为是完全二叉树
    if (head == nullptr) {
        return true;
    }

    // 使用队列进行层次遍历（广度优先搜索）
    std::queue<Node*> queue;
    // 标记是否遇到了第一个不双全的节点
    bool leaf = false;
    Node* l = nullptr;
    Node* r = nullptr;

    // 将根节点加入队列
    queue.push(head);

    // 开始层次遍历
    while (!queue.empty()) {
        // 取出队列前端的节点
        head = queue.front();
        queue.pop();

        // 获取当前节点的左右子节点
        l = head->left;
        r = head->right;

        // 检查当前节点是否违反完全二叉树的条件
        // 1. 如果已经遇到过不双全的节点（leaf 为 true），而当前节点还有子节点，则返回 false
        // 2. 如果当前节点左子节点为空但右子节点不为空，则返回 false
        if ((leaf && (l != nullptr || r != nullptr)) || (l == nullptr && r != nullptr)) {
            return false;
        }

        // 如果左子节点不为空，将其加入队列
        if (l != nullptr) {
            queue.push(l);
        }

        // 如果右子节点不为空，将其加入队列
        if (r != nullptr) {
            queue.push(r);
        } else {
            // 如果右子节点为空，标记后续所有节点都必须是叶子节点
            leaf = true;
        }
    }

    // 如果遍历完整棵树没有违反上述条件，则返回 true
    return true;
}
