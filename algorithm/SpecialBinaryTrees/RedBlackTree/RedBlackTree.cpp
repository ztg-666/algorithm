
class RedBlackTree : public AbstractSelfBalancingBinarySearchTree {
protected:
    // 定义红黑树节点的颜色枚举
    enum class ColorEnum {
        RED,
        BLACK
    };

    // 定义红黑树节点结构体，继承自 Node 类，并增加颜色属性
    struct RedBlackNode : public Node {
        ColorEnum color;

        RedBlackNode(int value, Node* parent, Node* left, Node* right, ColorEnum color)
            : Node(value, parent, left, right), color(color) {}
    };

    // 定义 nil 节点，作为叶子节点的哨兵
    static RedBlackNode* nilNode;

    // 记录红黑树中节点的数量
    int size;

public:
    // 构造函数，初始化 size 和 nilNode
    RedBlackTree() : size(0) {
        nilNode = new RedBlackNode(0, nullptr, nullptr, nullptr, ColorEnum::BLACK);
    }

    // 插入新节点，并进行红黑树性质的修复
    Node* insert(int element) override {
        // 调用基类的插入方法，插入新节点并返回新节点指针
        Node* newNode = AbstractSelfBalancingBinarySearchTree::insert(element);

        // 将新节点的左子节点和右子节点设置为哨兵节点 nilNode
        newNode->left = nilNode;
        newNode->right = nilNode;

        // 确保根节点的父节点指向哨兵节点 nilNode
        root->parent = nilNode;

        // 将新节点的颜色设置为红色，并进行红黑树性质的修复
        insertRBFixup(static_cast<RedBlackNode*>(newNode));

        // 返回新插入的节点
        return newNode;
    }


    // 删除指定节点，并进行红黑树性质的修复
    Node* deleteNode(Node* deleteNode) override {
        Node* replaceNode = nullptr; // 记录替换被删除节点的节点

        // 检查要删除的节点是否为空或为哨兵节点 nilNode
        if (deleteNode != nullptr && deleteNode != nilNode) {
            // 如果被删除节点只有一个子节点，则 removedOrMovedNode 与 deleteNode 相同
            Node* removedOrMovedNode = deleteNode;
            // 记录被删除或移动节点的颜色
            ColorEnum removedOrMovedNodeColor = static_cast<RedBlackNode*>(removedOrMovedNode)->color;

            // 情况1：被删除节点没有左子节点
            if (deleteNode->left == nilNode) {
                // 用右子节点替换被删除节点
                replaceNode = deleteNode->right;
                rbTreeTransplant(deleteNode, deleteNode->right);
            }
            // 情况2：被删除节点没有右子节点
            else if (deleteNode->right == nilNode) {
                // 用左子节点替换被删除节点
                replaceNode = deleteNode->left;
                rbTreeTransplant(deleteNode, deleteNode->left);
            }
            // 情况3：被删除节点有两个子节点
            else {
                // 找到被删除节点的后继节点（右子树中的最小节点）
                removedOrMovedNode = getMinimum(deleteNode->right);
                // 记录后继节点的颜色
                removedOrMovedNodeColor = static_cast<RedBlackNode*>(removedOrMovedNode)->color;
                // 记录后继节点的右子节点
                replaceNode = removedOrMovedNode->right;

                // 如果后继节点是被删除节点的直接右子节点
                if (removedOrMovedNode->parent == deleteNode) {
                    replaceNode->parent = removedOrMovedNode;
                } else {
                    // 否则，将后继节点替换为其右子节点
                    rbTreeTransplant(removedOrMovedNode, removedOrMovedNode->right);
                    // 将被删除节点的右子树连接到后继节点的右子树
                    removedOrMovedNode->right = deleteNode->right;
                    removedOrMovedNode->right->parent = removedOrMovedNode;
                }

                // 将被删除节点替换为后继节点
                rbTreeTransplant(deleteNode, removedOrMovedNode);
                // 将被删除节点的左子树连接到后继节点的左子树
                removedOrMovedNode->left = deleteNode->left;
                removedOrMovedNode->left->parent = removedOrMovedNode;
                // 将后继节点的颜色设置为被删除节点的颜色
                static_cast<RedBlackNode*>(removedOrMovedNode)->color = static_cast<RedBlackNode*>(deleteNode)->color;
            }

            // 减少树的大小计数
            size--;

            // 如果被删除或移动的节点颜色为黑色，需要进行红黑树性质的修复
            if (removedOrMovedNodeColor == ColorEnum::BLACK) {
                deleteRBFixup(static_cast<RedBlackNode*>(replaceNode));
            }
        }

        // 返回替换被删除节点的节点
        return replaceNode;
    }


    // 创建一个新的红黑树节点
    Node* createNode(int value, Node* parent, Node* left, Node* right) override {
        return new RedBlackNode(value, parent, left, right, ColorEnum::RED);
    }

    // 获取以 node 为根节点的子树中的最小节点
    Node* getMinimum(Node* node) override {
        while (node->left != nilNode) {
            node = node->left;
        }
        return node;
    }

    // 获取以 node 为根节点的子树中的最大节点
    Node* getMaximum(Node* node) override {
        while (node->right != nilNode) {
            node = node->right;
        }
        return node;
    }

    // 左旋操作
    Node* rotateLeft(Node* node) override {
        Node* temp = node->right;
        temp->parent = node->parent;

        node->right = temp->left;
        if (node->right != nilNode) {
            node->right->parent = node;
        }

        temp->left = node;
        node->parent = temp;

        if (temp->parent != nilNode) {
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

    // 右旋操作
    Node* rotateRight(Node* node) override {
        Node* temp = node->left;
        temp->parent = node->parent;

        node->left = temp->right;
        if (node->left != nilNode) {
            node->left->parent = node;
        }

        temp->right = node;
        node->parent = temp;

        if (temp->parent != nilNode) {
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

private:
    // 根节点指针
    Node* root;

    // 用 newNode 替换 nodeToReplace
    Node* rbTreeTransplant(Node* nodeToReplace, Node* newNode) {
        if (nodeToReplace->parent == nilNode) {
            this->root = newNode;
        } else if (nodeToReplace == nodeToReplace->parent->left) {
            nodeToReplace->parent->left = newNode;
        } else {
            nodeToReplace->parent->right = newNode;
        }
        newNode->parent = nodeToReplace->parent;
        return newNode;
    }

    // 删除节点后的红黑树性质修复
    void deleteRBFixup(RedBlackNode* x) {
        // 循环直到 x 不是根节点且 x 的颜色不是黑色
        while (x != root && isBlack(x)) {
            // 情况1：x 是其父节点的左子节点
            if (x == x->parent->left) {
                // 获取 x 的兄弟节点（叔叔节点）
                RedBlackNode* w = static_cast<RedBlackNode*>(x->parent->right);

                // 情况1.1：叔叔节点是红色
                if (isRed(w)) {
                    // 将叔叔节点的颜色设置为黑色
                    w->color = ColorEnum::BLACK;
                    // 将父节点的颜色设置为红色
                    static_cast<RedBlackNode*>(x->parent)->color = ColorEnum::RED;
                    // 对父节点进行左旋操作
                    rotateLeft(x->parent);
                    // 更新叔叔节点为新的右子节点
                    w = static_cast<RedBlackNode*>(x->parent->right);
                    // 现在转换为情况2、3或4
                }

                // 情况1.2：叔叔节点是黑色且其两个子节点都是黑色
                if (isBlack(w->left) && isBlack(w->right)) {
                    // 将叔叔节点的颜色设置为红色
                    w->color = ColorEnum::RED;
                    // 将 x 更新为其父节点
                    x = static_cast<RedBlackNode*>(x->parent);
                } else if (w != nilNode) {
                    // 情况1.3：叔叔节点是黑色且其左子节点是红色，右子节点是黑色
                    if (isBlack(w->right)) {
                        // 将叔叔节点的左子节点的颜色设置为黑色
                        static_cast<RedBlackNode*>(w->left)->color = ColorEnum::BLACK;
                        // 将叔叔节点的颜色设置为红色
                        w->color = ColorEnum::RED;
                        // 对叔叔节点进行右旋操作
                        rotateRight(w);
                        // 更新叔叔节点为新的右子节点
                        w = static_cast<RedBlackNode*>(x->parent->right);
                    }
                    // 情况1.4：叔叔节点是黑色且其右子节点是红色
                    // 将叔叔节点的颜色设置为父节点的颜色
                    w->color = static_cast<RedBlackNode*>(x->parent)->color;
                    // 将父节点的颜色设置为黑色
                    static_cast<RedBlackNode*>(x->parent)->color = ColorEnum::BLACK;
                    // 将叔叔节点的右子节点的颜色设置为黑色
                    static_cast<RedBlackNode*>(w->right)->color = ColorEnum::BLACK;
                    // 对父节点进行左旋操作
                    rotateLeft(x->parent);
                    // 将 x 设置为根节点，结束循环
                    x = static_cast<RedBlackNode*>(root);
                } else {
                    // 将 x 的颜色设置为黑色
                    x->color = ColorEnum::BLACK;
                    // 将 x 更新为其父节点
                    x = static_cast<RedBlackNode*>(x->parent);
                }
            } else {
                // 情况2：x 是其父节点的右子节点
                // 获取 x 的兄弟节点（叔叔节点）
                RedBlackNode* w = static_cast<RedBlackNode*>(x->parent->left);

                // 情况2.1：叔叔节点是红色
                if (isRed(w)) {
                    // 将叔叔节点的颜色设置为黑色
                    w->color = ColorEnum::BLACK;
                    // 将父节点的颜色设置为红色
                    static_cast<RedBlackNode*>(x->parent)->color = ColorEnum::RED;
                    // 对父节点进行右旋操作
                    rotateRight(x->parent);
                    // 更新叔叔节点为新的左子节点
                    w = static_cast<RedBlackNode*>(x->parent->left);
                    // 现在转换为情况2、3或4
                }

                // 情况2.2：叔叔节点是黑色且其两个子节点都是黑色
                if (isBlack(w->left) && isBlack(w->right)) {
                    // 将叔叔节点的颜色设置为红色
                    w->color = ColorEnum::RED;
                    // 将 x 更新为其父节点
                    x = static_cast<RedBlackNode*>(x->parent);
                } else if (w != nilNode) {
                    // 情况2.3：叔叔节点是黑色且其右子节点是红色，左子节点是黑色
                    if (isBlack(w->left)) {
                        // 将叔叔节点的右子节点的颜色设置为黑色
                        static_cast<RedBlackNode*>(w->right)->color = ColorEnum::BLACK;
                        // 将叔叔节点的颜色设置为红色
                        w->color = ColorEnum::RED;
                        // 对叔叔节点进行左旋操作
                        rotateLeft(w);
                        // 更新叔叔节点为新的左子节点
                        w = static_cast<RedBlackNode*>(x->parent->left);
                    }
                    // 情况2.4：叔叔节点是黑色且其左子节点是红色
                    // 将叔叔节点的颜色设置为父节点的颜色
                    w->color = static_cast<RedBlackNode*>(x->parent)->color;
                    // 将父节点的颜色设置为黑色
                    static_cast<RedBlackNode*>(x->parent)->color = ColorEnum::BLACK;
                    // 将叔叔节点的左子节点的颜色设置为黑色
                    static_cast<RedBlackNode*>(w->left)->color = ColorEnum::BLACK;
                    // 对父节点进行右旋操作
                    rotateRight(x->parent);
                    // 将 x 设置为根节点，结束循环
                    x = static_cast<RedBlackNode*>(root);
                } else {
                    // 将 x 的颜色设置为黑色
                    x->color = ColorEnum::BLACK;
                    // 将 x 更新为其父节点
                    x = static_cast<RedBlackNode*>(x->parent);
                }
            }
        }
        // 确保根节点的颜色始终为黑色
        static_cast<RedBlackNode*>(root)->color = ColorEnum::BLACK;
    }


    // 判断节点是否为黑色
    bool isBlack(Node* node) {
        return node != nullptr ? static_cast<RedBlackNode*>(node)->color == ColorEnum::BLACK : false;
    }

    // 判断节点是否为红色
    bool isRed(Node* node) {
        return node != nullptr ? static_cast<RedBlackNode*>(node)->color == ColorEnum::RED : false;
    }

    // 插入节点后的红黑树性质修复
    void insertRBFixup(RedBlackNode* currentNode) {
    // 循环直到当前节点的父节点不再是根节点且父节点的颜色不是红色
    while (currentNode->parent != root && static_cast<RedBlackNode*>(currentNode->parent)->color == ColorEnum::RED) {
        // 获取当前节点的父节点
        RedBlackNode* parent = static_cast<RedBlackNode*>(currentNode->parent);
        // 获取当前节点的祖父节点
        RedBlackNode* grandParent = static_cast<RedBlackNode*>(parent->parent);

        // 情况1：父节点是祖父节点的左子节点
        if (parent == grandParent->left) {
            // 获取父节点的兄弟节点（叔叔节点）
            RedBlackNode* uncle = static_cast<RedBlackNode*>(grandParent->right);

            // 情况1.1：叔叔节点是红色
            if (static_cast<RedBlackNode*>(uncle)->color == ColorEnum::RED) {
                // 将父节点和叔叔节点的颜色设置为黑色
                parent->color = ColorEnum::BLACK;
                uncle->color = ColorEnum::BLACK;
                // 将祖父节点的颜色设置为红色
                grandParent->color = ColorEnum::RED;

                // 将当前节点设置为祖父节点，继续检查祖父节点的平衡性
                currentNode = grandParent;
            } else {
                // 情况1.2：叔叔节点是黑色
                // 情况1.2.1：当前节点是父节点的右子节点（LR情况）
                if (currentNode == parent->right) {
                    // 将当前节点设置为父节点
                    currentNode = parent;
                    // 对父节点进行左旋操作
                    rotateLeft(currentNode);
                }
                // 情况1.2.2：当前节点是父节点的左子节点（LL情况）
                // 将父节点的颜色设置为黑色
                parent->color = ColorEnum::BLACK;
                // 将祖父节点的颜色设置为红色
                grandParent->color = ColorEnum::RED;
                // 对祖父节点进行右旋操作
                rotateRight(grandParent);
            }
        } else if (parent == grandParent->right) {
            // 获取父节点的兄弟节点（叔叔节点）
            RedBlackNode* uncle = static_cast<RedBlackNode*>(grandParent->left);

            // 情况2.1：叔叔节点是红色
            if (static_cast<RedBlackNode*>(uncle)->color == ColorEnum::RED) {
                // 将父节点和叔叔节点的颜色设置为黑色
                parent->color = ColorEnum::BLACK;
                uncle->color = ColorEnum::BLACK;
                // 将祖父节点的颜色设置为红色
                grandParent->color = ColorEnum::RED;

                // 将当前节点设置为祖父节点，继续检查祖父节点的平衡性
                currentNode = grandParent;
            } else {
                // 情况2.2：叔叔节点是黑色
                // 情况2.2.1：当前节点是父节点的左子节点（RL情况）
                if (currentNode == parent->left) {
                    // 将当前节点设置为父节点
                    currentNode = parent;
                    // 对父节点进行右旋操作
                    rotateRight(currentNode);
                }
                // 情况2.2.2：当前节点是父节点的右子节点（RR情况）
                // 将父节点的颜色设置为黑色
                parent->color = ColorEnum::BLACK;
                // 将祖父节点的颜色设置为红色
                grandParent->color = ColorEnum::RED;
                // 对祖父节点进行左旋操作
                rotateLeft(grandParent);
            }
        }
    }
    // 确保根节点的颜色始终为黑色
    static_cast<RedBlackNode*>(root)->color = ColorEnum::BLACK;
    }

};

// 初始化 nilNode 为 nullptr
RedBlackTree::RedBlackNode* RedBlackTree::nilNode = nullptr;


