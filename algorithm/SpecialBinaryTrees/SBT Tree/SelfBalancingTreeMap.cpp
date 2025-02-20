
template <typename K, typename V>
class SBTNode {
public:
    K key; // 节点键值
    V value; // 节点值
    SBTNode<K, V>* l; // 左子节点
    SBTNode<K, V>* r; // 右子节点
    int size; // 以当前节点为根的子树节点总数

    // 构造函数，初始化节点键值和值，子树大小为1
    SBTNode(K key, V value) : key(key), value(value), l(nullptr), r(nullptr), size(1) {}
};

template <typename K, typename V>
class SizeBalancedTreeMap {
private:
    // 右旋操作，调整树结构以维持平衡
    // 对以 cur 为根节点的子树进行右旋操作
    SBTNode<K, V>* rightRotate(SBTNode<K, V>* cur) {
        // 将 cur 的左子节点存储在 leftNode 中
        SBTNode<K, V>* leftNode = cur->l;
        // 将 leftNode 的右子节点变为 cur 的左子节点
        cur->l = leftNode->r;
        // 将 cur 变为 leftNode 的右子节点
        leftNode->r = cur;
        // 更新 leftNode 的 size，使其包含 cur 的所有节点
        leftNode->size = cur->size;
        // 更新 cur 的 size，减去 leftNode 的节点数
        cur->size = (cur->l != nullptr ? cur->l->size : 0) + (cur->r != nullptr ? cur->r->size : 0) + 1;
        // 返回新的根节点 leftNode
        return leftNode;
    }


    // 左旋操作，调整树结构以维持平衡
    SBTNode<K, V>* leftRotate(SBTNode<K, V>* cur) {
        SBTNode<K, V>* rightNode = cur->r;
        cur->r = rightNode->l;
        rightNode->l = cur;
        rightNode->size = cur->size;
        cur->size = (cur->l != nullptr ? cur->l->size : 0) + (cur->r != nullptr ? cur->r->size : 0) + 1;
        return rightNode;
    }

    // 维护树的平衡性
    SBTNode<K, V>* matain(SBTNode<K, V>* cur) {
        // 如果当前节点为空，直接返回
        if (cur == nullptr) {
            return nullptr;
        }

        // 根据子树大小关系进行相应的旋转操作以维持平衡
        // 情况1: 左左子树的大小大于右子树的大小，进行右旋
        if (cur->l != nullptr && cur->l->l != nullptr && cur->r != nullptr && cur->l->l->size > cur->r->size) {
            cur = rightRotate(cur);
            cur->r = matain(cur->r);
            cur = matain(cur);
        }
        // 情况2: 左右子树的大小大于右子树的大小，先对左子树进行左旋，再对当前节点进行右旋
        else if (cur->l != nullptr && cur->l->r != nullptr && cur->r != nullptr && cur->l->r->size > cur->r->size) {
            cur->l = leftRotate(cur->l);
            cur = rightRotate(cur);
            cur->l = matain(cur->l);
            cur->r = matain(cur->r);
            cur = matain(cur);
        }
        // 情况3: 右右子树的大小大于左子树的大小，进行左旋
        else if (cur->r != nullptr && cur->r->r != nullptr && cur->l != nullptr && cur->r->r->size > cur->l->size) {
            cur = leftRotate(cur);
            cur->l = matain(cur->l);
            cur = matain(cur);
        }
        // 情况4: 右左子树的大小大于左子树的大小，先对右子树进行右旋，再对当前节点进行左旋
        else if (cur->r != nullptr && cur->r->l != nullptr && cur->l != nullptr && cur->r->l->size > cur->l->size) {
            cur->r = rightRotate(cur->r);
            cur = leftRotate(cur);
            cur->l = matain(cur->l);
            cur->r = matain(cur->r);
            cur = matain(cur);
        }

        // 返回维护平衡后的当前节点
        return cur;
    }


    // 查找最后一个等于key的节点
    SBTNode<K, V>* findLastIndex(K key) {
        SBTNode<K, V>* pre = root;
        SBTNode<K, V>* cur = root;
        while (cur != nullptr) {
            pre = cur;
            if (key < cur->key) {
                cur = cur->l;
            } else if (key > cur->key) {
                cur = cur->r;
            } else {
                break;
            }
        }
        return pre;
    }

    // 查找最后一个不小于key的节点
    SBTNode<K, V>* findLastNoSmallIndex(K key) {
        SBTNode<K, V>* ans = nullptr;
        SBTNode<K, V>* cur = root;
        while (cur != nullptr) {
            if (key <= cur->key) {
                ans = cur;
                cur = cur->l;
            } else {
                cur = cur->r;
            }
        }
        return ans;
    }

    // 查找最后一个小于等于key的节点
    SBTNode<K, V>* findLastNoBigIndex(K key) {
        SBTNode<K, V>* ans = nullptr;
        SBTNode<K, V>* cur = root;
        while (cur != nullptr) {
            if (key >= cur->key) {
                ans = cur;
                cur = cur->r;
            } else {
                cur = cur->l;
            }
        }
        return ans;
    }

    // 向树中添加节点
    SBTNode<K, V>* add(SBTNode<K, V>* cur, K key, V value) {
        if (cur == nullptr) {
            return new SBTNode<K, V>(key, value);
        } else {
            cur->size++;
            if (key < cur->key) {
                cur->l = add(cur->l, key, value);
            } else {
                cur->r = add(cur->r, key, value);
            }
            return matain(cur);
        }
    }

    // 删除节点并维护树的平衡性
    SBTNode<K, V>* deleteNode(SBTNode<K, V>* cur, K key) {
        // 如果当前节点为空，直接返回
        if (cur == nullptr) {
            return nullptr;
        }

        // 减少当前节点的大小
        cur->size--;

        // 如果要删除的键大于当前节点的键，递归删除右子树中的节点
        if (key > cur->key) {
            cur->r = deleteNode(cur->r, key);
        }
        // 如果要删除的键小于当前节点的键，递归删除左子树中的节点
        else if (key < cur->key) {
            cur->l = deleteNode(cur->l, key);
        }
        // 找到要删除的节点
        else {
            // 情况1: 当前节点是叶子节点（没有子节点）
            if (cur->l == nullptr && cur->r == nullptr) {
                delete cur;
                cur = nullptr;
            }
            // 情况2: 当前节点只有右子节点
            else if (cur->l == nullptr && cur->r != nullptr) {
                SBTNode<K, V>* temp = cur;
                cur = cur->r;
                delete temp;
            }
            // 情况3: 当前节点只有左子节点
            else if (cur->l != nullptr && cur->r == nullptr) {
                SBTNode<K, V>* temp = cur;
                cur = cur->l;
                delete temp;
            }
            // 情况4: 当前节点有两个子节点
            else {
                SBTNode<K, V>* pre = nullptr;
                SBTNode<K, V>* des = cur->r;
                des->size--; // 减少右子节点的大小
                // 找到右子树中的最小节点（中序后继）
                while (des->l != nullptr) {
                    pre = des;
                    des = des->l;
                    des->size--; // 减少路径上每个节点的大小
                }
                // 如果中序后继不是右子节点的直接子节点
                if (pre != nullptr) {
                    pre->l = des->r; // 将中序后继的右子节点连接到前驱节点的左子节点
                    des->r = cur->r; // 将中序后继的右子节点指向当前节点的右子节点
                }
                // 将中序后继的左子节点指向当前节点的左子节点
                des->l = cur->l;
                // 更新中序后继的大小
                des->size = des->l->size + des->r->size + 1;
                // 删除当前节点
                delete cur;
                // 将中序后继作为新的根节点
                cur = des;
            }
        }

        // 返回删除节点后的当前节点
        return cur;
    }


    // 获取第k小的节点
    SBTNode<K, V>* getIndex(SBTNode<K, V>* cur, int kth) {
        if (kth == (cur->l != nullptr ? cur->l->size : 0) + 1) {
            return cur;
        } else if (kth <= (cur->l != nullptr ? cur->l->size : 0)) {
            return getIndex(cur->l, kth);
        } else {
            return getIndex(cur->r, kth - (cur->l != nullptr ? cur->l->size : 0) - 1);
        }
    }

public:
    SBTNode<K, V>* root; // 树的根节点
    // 获取树中节点总数
    int size() {
        return root == nullptr ? 0 : root->size;
    }

    // 判断树中是否包含指定键值
    bool containsKey(K key) {
        if (key == K()) {
            throw std::runtime_error("invalid parameter.");
        }
        SBTNode<K, V>* lastNode = findLastIndex(key);
        return lastNode != nullptr && key == lastNode->key;
    }

    // 向树中添加或更新键值对
    void put(K key, V value) {
        if (key == K()) {
            throw std::runtime_error("invalid parameter.");
        }
        SBTNode<K, V>* lastNode = findLastIndex(key);
        if (lastNode != nullptr && key == lastNode->key) {
            lastNode->value = value;
        } else {
            root = add(root, key, value);
        }
    }

    // 从树中删除指定键值对
    void remove(K key) {
        if (key == K()) {
            throw std::runtime_error("invalid parameter.");
        }
        if (containsKey(key)) {
            root = deleteNode(root, key);
        }
    }

    // 获取第index小的键值
    K getIndexKey(int index) {
        if (index < 0 || index >= this->size()) {
            throw std::runtime_error("invalid parameter.");
        }
        return getIndex(root, index + 1)->key;
    }

    // 获取第index小的值
    V getIndexValue(int index) {
        if (index < 0 || index >= this->size()) {
            throw std::runtime_error("invalid parameter.");
        }
        return getIndex(root, index + 1)->value;
    }

    // 获取指定键值对应的值
    V get(K key) {
        if (key == K()) {
            throw std::runtime_error("invalid parameter.");
        }
        SBTNode<K, V>* lastNode = findLastIndex(key);
        if (lastNode != nullptr && key == lastNode->key) {
            return lastNode->value;
        } else {
            return V();
        }
    }

    // 获取树中最小的键值
    K firstKey() {
        if (root == nullptr) {
            return K();
        }
        SBTNode<K, V>* cur = root;
        while (cur->l != nullptr) {
            cur = cur->l;
        }
        return cur->key;
    }

    // 获取树中最大的键值
    K lastKey() {
        if (root == nullptr) {
            return K();
        }
        SBTNode<K, V>* cur = root;
        while (cur->r != nullptr) {
            cur = cur->r;
        }
        return cur->key;
    }

    // 获取小于等于指定键值的最大键值
    K floorKey(K key) {
        if (key == K()) {
            throw std::runtime_error("invalid parameter.");
        }
        SBTNode<K, V>* lastNoBigNode = findLastNoBigIndex(key);
        return lastNoBigNode == nullptr ? K() : lastNoBigNode->key;
    }

    // 获取大于等于指定键值的最小键值
    K ceilingKey(K key) {
        if (key == K()) {
            throw std::runtime_error("invalid parameter.");
        }
        SBTNode<K, V>* lastNoSmallNode = findLastNoSmallIndex(key);
        return lastNoSmallNode == nullptr ? K() : lastNoSmallNode->key;
    }


    // 构造函数
    SizeBalancedTreeMap() : root(nullptr) {}

    // 析构函数
    ~SizeBalancedTreeMap() {
        clear(root);
    }

    // 清空树
    void clear(SBTNode<K, V>* node) {
        if (node == nullptr) {
            return;
        }
        clear(node->l);
        clear(node->r);
        delete node;
    }
};

