#include <bits/stdc++.h>
using namespace std;
class AbstractSelfBalancingBinarySearchTree : public AbstractBinarySearchTree {
protected:

    Node* rotateLeft(Node* node) {
        // temp 指向 node 的右子节点
        Node* temp = node->right;
        // 将 temp 的父节点设置为 node 的父节点
        temp->parent = node->parent;

        // 将 node 的右子节点设置为 temp 的左子节点
        node->right = temp->left;
        // 如果 temp 的左子节点不为空，则将其父节点设置为 node
        if (node->right != nullptr) {
            node->right->parent = node;
        }

        // 将 temp 的左子节点设置为 node
        temp->left = node;
        // 将 node 的父节点设置为 temp
        node->parent = temp;

        // temp 现在占据了 node 的位置，因此需要更新 temp 的父节点的子节点指针
        // 如果 temp 有父节点
        if (temp->parent != nullptr) {
            // 如果 node 是 temp 父节点的左子节点
            if (node == temp->parent->left) {
                // 将 temp 父节点的左子节点设置为 temp
                temp->parent->left = temp;
            } else {
                // 否则，将 temp 父节点的右子节点设置为 temp
                temp->parent->right = temp;
            }
        } else {
            // 如果 temp 没有父节点，说明 node 原本是根节点
            // 将根节点设置为 temp
            root = temp;
        }

        // 返回新的根节点 temp
        return temp;
    }


    Node* rotateRight(Node* node) {
        Node* temp = node->left;
        temp->parent = node->parent;

        node->left = temp->right;
        if (node->left != nullptr) {
            node->left->parent = node;
        }

        temp->right = node;
        node->parent = temp;

        // temp took over node's place so now its parent should point to temp
        if (temp->parent != nullptr) {
            if (node == temp->parent->left) {
                temp->parent->left = temp;
            } else {
                temp->parent->right = temp;
            }
        } else {
            root = temp;
        }

        return temp;
    }
};

}

