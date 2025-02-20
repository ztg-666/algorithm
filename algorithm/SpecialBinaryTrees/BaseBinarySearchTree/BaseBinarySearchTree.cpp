#include<bits/stdc++.h>
using namespace std;
namespace class02 {
class AbstractBinarySearchTree {
public:
    // 定义树节点结构
    struct Node {
        int value;          // 节点值
        Node* parent;       // 父节点指针
        Node* left;         // 左子节点指针
        Node* right;        // 右子节点指针

        // 构造函数，初始化节点值、父节点、左子节点和右子节点
        Node(int val, Node* par = nullptr, Node* l = nullptr, Node* r = nullptr)
            : value(val), parent(par), left(l), right(r) {}

        // 判断节点是否为叶子节点
        bool isLeaf() const {
            return left == nullptr && right == nullptr;
        }
    };

    // 构造函数，初始化根节点为空，树大小为0
    AbstractBinarySearchTree() : root(nullptr), size(0) {}

    // 查找元素，返回找到的节点指针，未找到返回nullptr
    Node* search(int element) {
        Node* node = root;
        while (node != nullptr && node->value != element) {
            if (element < node->value) {
                node = node->left;
            } else {
                node = node->right;
            }
        }
        return node;
    }

    // 插入元素，返回新插入的节点指针
    virtual Node* insert(int element) {
        // 如果根节点为空，则创建一个新的节点作为根节点，并返回该节点
        if (root == nullptr) {
            root = createNode(element, nullptr, nullptr, nullptr);
            size++;  // 增加树的大小计数
            return root;
        }

        Node* insertParentNode = nullptr;  // 用于存储新节点的父节点
        Node* searchTempNode = root;       // 用于遍历树的临时节点

        // 遍历树以找到新节点应该插入的位置
        while (searchTempNode != nullptr) {
            insertParentNode = searchTempNode;  // 更新父节点为当前节点
            // 根据元素值决定向左子树还是右子树移动
            if (element < searchTempNode->value) {
                searchTempNode = searchTempNode->left;
            } else {
                searchTempNode = searchTempNode->right;
            }
        }

        // 创建新节点，其父节点为找到的父节点
        Node* newNode = createNode(element, insertParentNode, nullptr, nullptr);
        // 根据新节点的值与父节点的值比较，决定将其插入为左子节点还是右子节点
        if (insertParentNode->value > newNode->value) {
            insertParentNode->left = newNode;
        } else {
            insertParentNode->right = newNode;
        }

        size++;  // 增加树的大小计数
        return newNode;  // 返回新插入的节点
    }

    // 删除指定值的节点，返回删除后的新节点指针，未找到返回nullptr
    virtual Node* deleteNode(int element) {
        Node* deleteNode = search(element);
        if (deleteNode != nullptr) {
            return deleteNode(deleteNode);
        } else {
            return nullptr;
        }
    }

    // 删除指定节点，返回删除后的新节点指针
    Node* deleteNode(Node* deleteNode) {
        if (deleteNode != nullptr) {
            Node* nodeToReturn = nullptr;  // 用于存储删除操作后需要返回的节点

            // 情况1：删除节点没有左子节点
            if (deleteNode->left == nullptr) {
                nodeToReturn = transplant(deleteNode, deleteNode->right);
            }
            // 情况2：删除节点没有右子节点
            else if (deleteNode->right == nullptr) {
                nodeToReturn = transplant(deleteNode, deleteNode->left);
            }
            // 情况3：删除节点有两个子节点
            else {
                // 找到删除节点的后继节点（右子树中的最小节点）
                Node* successorNode = getMinimum(deleteNode->right);

                // 如果后继节点不是删除节点的直接右子节点
                if (successorNode->parent != deleteNode) {
                    // 将后继节点替换为其右子节点
                    transplant(successorNode, successorNode->right);
                    // 将删除节点的右子树连接到后继节点的右子树
                    successorNode->right = deleteNode->right;
                    successorNode->right->parent = successorNode;
                }

                // 将删除节点替换为后继节点
                transplant(deleteNode, successorNode);
                // 将删除节点的左子树连接到后继节点的左子树
                successorNode->left = deleteNode->left;
                successorNode->left->parent = successorNode;
                nodeToReturn = successorNode;
            }

            size--;  // 减少树的大小计数
            return nodeToReturn;  // 返回删除操作后的新节点
        }
        return nullptr;  // 如果删除节点为空，返回空指针
    }

    // 判断树中是否包含指定元素
    bool contains(int element) {
        return search(element) != nullptr;
    }

    // 获取树中最小值
    int getMinimum() {
        return getMinimum(root)->value;
    }

    // 获取树中最大值
    int getMaximum() {
        return getMaximum(root)->value;
    }

    // 获取指定元素的后继节点值
    int getSuccessor(int element) {
        return getSuccessor(search(element))->value;
    }

    // 获取树的大小
    int getSize() const {
        return size;
    }

    // 中序遍历打印树
    void printTreeInOrder() {
        printTreeInOrder(root);
    }

    // 前序遍历打印树
    void printTreePreOrder() {
        printTreePreOrder(root);
    }

    // 后序遍历打印树
    void printTreePostOrder() {
        printTreePostOrder(root);
    }

protected:
    Node* root;  // 树的根节点
    int size;    // 树的大小

    // 创建新节点
    virtual Node* createNode(int value, Node* parent, Node* left, Node* right) {
        return new Node(value, parent, left, right);
    }

    // 中序遍历打印树
    void printTreeInOrder(Node* entry) {
        if (entry != nullptr) {
            printTreeInOrder(entry->left);
            std::cout << entry->value << std::endl;
            printTreeInOrder(entry->right);
        }
    }

    // 前序遍历打印树
    void printTreePreOrder(Node* entry) {
        if (entry != nullptr) {
            std::cout << entry->value << std::endl;
            printTreePreOrder(entry->left);
            printTreePreOrder(entry->right);
        }
    }

    // 后序遍历打印树
    void printTreePostOrder(Node* entry) {
        if (entry != nullptr) {
            printTreePostOrder(entry->left);
            printTreePostOrder(entry->right);
            std::cout << entry->value << std::endl;
        }
    }

    // 获取子树中的最小节点
    Node* getMinimum(Node* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    // 获取子树中的最大节点
    Node* getMaximum(Node* node) {
        while (node->right != nullptr) {
            node = node->right;
        }
        return node;
    }

    // 获取节点的后继节点
    Node* getSuccessor(Node* node) {
        // 如果节点有右子树，则后继节点是右子树中的最小节点
        if (node->right != nullptr) {
            return getMinimum(node->right);
        } else {
            // 否则，我们需要向上查找，直到找到一个节点是其父节点的左子节点
            Node* currentNode = node;
            Node* parentNode = node->parent;
            // 向上遍历，直到找到一个节点是其父节点的左子节点或到达根节点
            while (parentNode != nullptr && currentNode == parentNode->right) {
                currentNode = parentNode;
                parentNode = parentNode->parent;
            }
            // 返回找到的父节点，如果没有找到则返回nullptr（表示当前节点是最大节点）
            return parentNode;
        }
    }

    // 替换节点
    // 将节点 nodeToReplace 替换为节点 newNode
    Node* transplant(Node* nodeToReplace, Node* newNode) {
        // 如果要替换的节点是根节点
        if (nodeToReplace->parent == nullptr) {
            this->root = newNode;  // 将根节点设置为 newNode
        }
        // 如果要替换的节点是其父节点的左子节点
        else if (nodeToReplace == nodeToReplace->parent->left) {
            nodeToReplace->parent->left = newNode;  // 将父节点的左子节点设置为 newNode
        }
        // 如果要替换的节点是其父节点的右子节点
        else {
            nodeToReplace->parent->right = newNode;  // 将父节点的右子节点设置为 newNode
        }

        // 如果 newNode 不为空，更新 newNode 的父节点为 nodeToReplace 的父节点
        if (newNode != nullptr) {
            newNode->parent = nodeToReplace->parent;
        }

        return newNode;  // 返回替换后的新节点
    }
};
}
