#include <bits/stdc++.h>
using namespace std;
/**
 * AVL树实现。
 *
 * 在计算机科学中，AVL树是一种自平衡二叉搜索树，是最早发明的自平衡数据结构之一。
 * 在AVL树中，任何节点的两个子树的高度差最多为1。查找、插入和删除操作在平均和最坏情况下的时间复杂度都是O(log n)。
 * 插入和删除操作可能需要通过一个或多个树旋转来重新平衡树。
 *
 *
 */
class AVLTree : public AbstractSelfBalancingBinarySearchTree {
public:
    /**
     * AVL树的插入方法，插入后会平衡树。
     * 使用节点的高度参数来跟踪子树的高度差异，如果差异超过1，则进行AVL树旋转以恢复平衡。
     */
    Node* insert(int element) override {
        // 使用基类的插入方法插入元素
        Node* newNode = AbstractSelfBalancingBinarySearchTree::insert(element);
        // 插入后重新平衡树
        rebalance(static_cast<AVLNode*>(newNode));
        return newNode;
    }

    /**
     * AVL树的删除方法，删除后会平衡树。
     */
    Node* deleteNode(int element) override {
        // 查找要删除的节点
        Node* deleteNode = search(element);
        if (deleteNode != nullptr) {
            // 使用基类的删除方法删除节点
            Node* successorNode = AbstractSelfBalancingBinarySearchTree::deleteNode(deleteNode);
            if (successorNode != nullptr) {
                // 如果节点被其后继节点替换，则更新高度并重新平衡
                AVLNode* minimum = successorNode->right != nullptr ? static_cast<AVLNode*>(getMinimum(successorNode->right)) : static_cast<AVLNode*>(successorNode);
                recomputeHeight(minimum);
                rebalance(minimum);
            } else {
                // 如果节点被删除且没有后继节点，则更新其父节点的高度并重新平衡
                recomputeHeight(static_cast<AVLNode*>(deleteNode->parent));
                rebalance(static_cast<AVLNode*>(deleteNode->parent));
            }
            return successorNode;
        }
        return nullptr;
    }

protected:
    /**
     * 创建一个新的AVLNode。
     */
    Node* createNode(int value, Node* parent, Node* left, Node* right) override {
        return new AVLNode(value, parent, left, right);
    }

private:
    /**
     * AVL树的节点类，包含高度属性。
     * 如果平衡因子等于2（或-2），则需要重新平衡该节点。
     * 高度是子树的高度，平衡因子是左右子树高度的差值。
     *
     * @author Ignas Lelys
     * @created Jun 30,2011
     *
     */
    class AVLNode : public Node {
    public:
        int height;

        AVLNode(int value, Node* parent, Node* left, Node* right) : Node(value, parent, left, right), height(0) {}
    };

    /**
     * 从插入的节点开始向上遍历，更新高度和平衡因子。
     * 如果某个节点的平衡因子达到2或-2，则需要重新平衡子树。
     *
     * @param node 插入的节点。
     */
    void rebalance(AVLNode* node) {
        while (node != nullptr) {
            Node* parent = node->parent;

            // 计算左子树和右子树的高度
            int leftHeight = (node->left == nullptr) ? -1 : static_cast<AVLNode*>(node->left)->height;
            int rightHeight = (node->right == nullptr) ? -1 : static_cast<AVLNode*>(node->right)->height;
            int nodeBalance = rightHeight - leftHeight;

            // 根据平衡因子执行相应的旋转操作以保持AVL树的平衡
            if (nodeBalance == 2) {
                // 右子树较高
                if (node->right->right != nullptr) {
                    // 单次左旋
                    node = static_cast<AVLNode*>(avlRotateLeft(node));
                    break;
                } else {
                    // 双旋（右-左）
                    node = static_cast<AVLNode*>(doubleRotateRightLeft(node));
                    break;
                }
            } else if (nodeBalance == -2) {
                // 左子树较高
                if (node->left->left != nullptr) {
                    // 单次右旋
                    node = static_cast<AVLNode*>(avlRotateRight(node));
                    break;
                } else {
                    // 双旋（左-右）
                    node = static_cast<AVLNode*>(doubleRotateLeftRight(node));
                    break;
                }
            } else {
                // 更新当前节点的高度
                updateHeight(node);
            }

            // 向上移动到父节点
            node = static_cast<AVLNode*>(parent);
        }
    }

    /**
     * 执行左旋操作。
     */
    Node* avlRotateLeft(Node* node) {
        // 使用基类的左旋方法执行左旋
        Node* temp = rotateLeft(node);

        // 更新旋转后节点的高度
        updateHeight(static_cast<AVLNode*>(temp->left));
        updateHeight(static_cast<AVLNode*>(temp));
        return temp;
    }

    /**
     * 执行右旋操作。
     */
    Node* avlRotateRight(Node* node) {
        // 使用基类的右旋方法执行右旋
        Node* temp = rotateRight(node);

        // 更新旋转后节点的高度
        updateHeight(static_cast<AVLNode*>(temp->right));
        updateHeight(static_cast<AVLNode*>(temp));
        return temp;
    }

    /**
     * 先对右子节点执行右旋，再对当前节点执行左旋。
     */
    Node* doubleRotateRightLeft(Node* node) {
        // 先对右子节点执行右旋
        node->right = avlRotateRight(node->right);
        // 再对当前节点执行左旋
        return avlRotateLeft(node);
    }

    /**
     * 先对左子节点执行左旋，再对当前节点执行右旋。
     */
    Node* doubleRotateLeftRight(Node* node) {
        // 先对左子节点执行左旋
        node->left = avlRotateLeft(node->left);
        // 再对当前节点执行右旋
        return avlRotateRight(node);
    }

    /**
     * 从给定节点开始向上遍历，重新计算每个节点的高度。
     * 删除节点后需要调用此方法。
     */
    void recomputeHeight(AVLNode* node) {
        while (node != nullptr) {
            // 更新当前节点的高度
            node->height = maxHeight(static_cast<AVLNode*>(node->left), static_cast<AVLNode*>(node->right)) + 1;
            // 向上移动到父节点
            node = static_cast<AVLNode*>(node->parent);
        }
    }

    /**
     * 返回两个节点中较高的高度。
     */
    int maxHeight(AVLNode* node1, AVLNode* node2) {
        if (node1 != nullptr && node2 != nullptr) {
            return std::max(node1->height, node2->height);
        } else if (node1 == nullptr) {
            return (node2 != nullptr) ? node2->height : -1;
        } else if (node2 == nullptr) {
            return (node1 != nullptr) ? node1->height : -1;
        }
        return -1;
    }

    /**
     * 更新给定节点的高度。
     *
     * @param node 需要更新高度的节点。
     */
    static void updateHeight(AVLNode* node) {
        // 计算左子树和右子树的高度
        int leftHeight = (node->left == nullptr) ? -1 : static_cast<AVLNode*>(node->left)->height;
        int rightHeight = (node->right == nullptr) ? -1 : static_cast<AVLNode*>(node->right)->height;
        // 更新当前节点的高度
        node->height = 1 + std::max(leftHeight, rightHeight);
    }
};

