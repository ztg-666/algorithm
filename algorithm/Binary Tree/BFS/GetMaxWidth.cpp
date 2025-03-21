#include <bits/stdc++.h>
using namespace std;
int getMaxWidth(Node* head) {
    // 如果根节点为空，返回宽度为0
    if (head == nullptr) {
        return 0;
    }

    // 初始化最大宽度为0
    int maxWidth = 0;

    // 当前层的节点数量
    int curWidth = 0;

    // 当前遍历到的层级
    int curLevel = 0;

    // 使用哈希表记录每个节点所在的层级
    std::unordered_map<Node*, int> levelMap;

    // 根节点的层级为1
    levelMap[head] = 1;

    // 使用队列进行广度优先遍历
    std::queue<Node*> queue;
    queue.push(head);

    // 当前处理的节点
    Node* node = nullptr;

    // 左右子节点
    Node* left = nullptr;
    Node* right = nullptr;

    // 开始广度优先遍历
    while (!queue.empty()) {
        // 取出队首节点
        node = queue.front();
        queue.pop();

        // 获取左右子节点
        left = node->left;
        right = node->right;

        // 如果左子节点不为空，记录其层级并加入队列
        if (left != nullptr) {
            levelMap[left] = levelMap[node] + 1;
            queue.push(left);
        }

        // 如果右子节点不为空，记录其层级并加入队列
        if (right != nullptr) {
            levelMap[right] = levelMap[node] + 1;
            queue.push(right);
        }

        // 如果当前节点的层级大于当前遍历到的层级，说明进入下一层
        if (levelMap[node] > curLevel) {
            curWidth = 0;  // 重置当前层的节点数量
            curLevel = levelMap[node];  // 更新当前遍历到的层级
        } else {
            curWidth++;  // 同一层级的节点数量加1
        }

        // 更新最大宽度
        maxWidth = std::max(maxWidth, curWidth);
    }

    // 返回最大宽度
    return maxWidth;
}
