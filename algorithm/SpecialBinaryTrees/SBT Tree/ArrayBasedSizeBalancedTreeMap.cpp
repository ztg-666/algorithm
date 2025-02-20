
template <typename K, typename V>
class SizeBalancedTreeMap {
private:
    // 定义树的节点结构
    struct Node {
        K key;       // 节点键值
        V value;     // 节点值
        int left;    // 左子节点的索引
        int right;   // 右子节点的索引
        int size;    // 以当前节点为根的子树节点总数
    };

    std::vector<Node> nodes; // 存储所有节点的数组
    int root;                // 根节点的索引
    int len;                 // 当前树中节点的数量

    // 右旋操作，调整树结构以维持平衡
    int rightRotate(int index) {
        int iLeft = nodes[index].left; // 获取当前节点的左子节点索引
        nodes[index].left = nodes[iLeft].right; // 将左子节点的右子节点变为当前节点的左子节点
        nodes[iLeft].right = index; // 将当前节点变为左子节点的右子节点
        nodes[iLeft].size = nodes[index].size; // 更新左子节点的大小
        nodes[index].size = nodes[nodes[index].left].size + nodes[nodes[index].right].size + 1; // 更新当前节点的大小
        return iLeft; // 返回新的根节点索引
    }

    // 左旋操作，调整树结构以维持平衡
    int leftRotate(int index) {
        int iRight = nodes[index].right; // 获取当前节点的右子节点索引
        nodes[index].right = nodes[iRight].left; // 将右子节点的左子节点变为当前节点的右子节点
        nodes[iRight].left = index; // 将当前节点变为右子节点的左子节点
        nodes[iRight].size = nodes[index].size; // 更新右子节点的大小
        nodes[index].size = nodes[nodes[index].left].size + nodes[nodes[index].right].size + 1; // 更新当前节点的大小
        return iRight; // 返回新的根节点索引
    }

    // 维护树的平衡性
    int matain(int index) {
        // 情况1: 左左子树的大小大于右子树的大小，进行右旋
        if (nodes[nodes[nodes[index].left].left].size > nodes[nodes[index].right].size) {
            index = rightRotate(index);
            nodes[index].right = matain(nodes[index].right);
            index = matain(index);
        }
        // 情况2: 左右子树的大小大于右子树的大小，先对左子树进行左旋，再对当前节点进行右旋
        else if (nodes[nodes[nodes[index].left].right].size > nodes[nodes[index].right].size) {
            nodes[index].left = leftRotate(nodes[index].left);
            index = rightRotate(index);
            nodes[index].left = matain(nodes[index].left);
            nodes[index].right = matain(nodes[index].right);
            index = matain(index);
        }
        // 情况3: 右右子树的大小大于左子树的大小，进行左旋
        else if (nodes[nodes[nodes[index].right].right].size > nodes[nodes[index].left].size) {
            index = leftRotate(index);
            nodes[index].left = matain(nodes[index].left);
            index = matain(index);
        }
        // 情况4: 右左子树的大小大于左子树的大小，先对右子树进行右旋，再对当前节点进行左旋
        else if (nodes[nodes[nodes[index].right].left].size > nodes[nodes[index].left].size) {
            nodes[index].right = rightRotate(nodes[index].right);
            index = leftRotate(index);
            nodes[index].left = matain(nodes[index].left);
            nodes[index].right = matain(nodes[index].right);
            index = matain(index);
        }
        return index; // 返回维护平衡后的当前节点索引
    }

    // 查找最后一个等于key的节点
    int findLastIndex(const K& key) {
        int pre = root; // 记录前一个节点的索引
        int cur = root; // 当前节点的索引
        while (cur != 0) {
            pre = cur; // 更新前一个节点的索引
            if (key == nodes[cur].key) {
                break; // 找到目标节点，退出循环
            } else if (key < nodes[cur].key) {
                cur = nodes[cur].left; // 向左子树查找
            } else {
                cur = nodes[cur].right; // 向右子树查找
            }
        }
        return pre; // 返回最后一个等于或最接近key的节点索引
    }

    // 查找最后一个不小于key的节点
    int findLastNoSmallIndex(const K& key) {
        int ans = 0; // 记录结果节点的索引
        int cur = root; // 当前节点的索引
        while (cur != 0) {
            if (key == nodes[cur].key) {
                ans = cur; // 找到目标节点，记录索引
                break; // 退出循环
            } else if (key < nodes[cur].key) {
                ans = cur; // 记录当前节点索引
                cur = nodes[cur].left; // 向左子树查找
            } else {
                cur = nodes[cur].right; // 向右子树查找
            }
        }
        return ans; // 返回最后一个不小于key的节点索引
    }

    // 查找最后一个小于等于key的节点
    int findLastNoBigIndex(const K& key) {
        int ans = 0; // 记录结果节点的索引
        int cur = root; // 当前节点的索引
        while (cur != 0) {
            if (key == nodes[cur].key) {
                ans = cur; // 找到目标节点，记录索引
                break; // 退出循环
            } else if (key < nodes[cur].key) {
                cur = nodes[cur].left; // 向左子树查找
            } else {
                ans = cur; // 记录当前节点索引
                cur = nodes[cur].right; // 向右子树查找
            }
        }
        return ans; // 返回最后一个小于等于key的节点索引
    }

    // 向树中添加节点
    int add(int index, const K& key, const V& value) {
        if (index == 0) {
            index = ++len; // 增加节点数量
            nodes.push_back({key, value, 0, 0, 1}); // 添加新节点
            return index; // 返回新节点的索引
        } else {
            nodes[index].size++; // 增加当前节点的子树大小
            if (key < nodes[index].key) {
                nodes[index].left = add(nodes[index].left, key, value); // 向左子树添加节点
            } else {
                nodes[index].right = add(nodes[index].right, key, value); // 向右子树添加节点
            }
            return matain(index); // 维护树的平衡性
        }
    }

    // 获取第k小的节点
    int getIndex(int index, int kth) {
        if (kth == nodes[nodes[index].left].size + 1) {
            return index; // 找到第k小的节点，返回其索引
        } else if (kth <= nodes[nodes[index].left].size) {
            return getIndex(nodes[index].left, kth); // 在左子树中查找
        } else {
            return getIndex(nodes[index].right, kth - nodes[nodes[index].left].size - 1); // 在右子树中查找
        }
    }

public:
    // 构造函数，初始化树
    SizeBalancedTreeMap(int init) : root(0), len(0) {
        nodes.reserve(init + 1); // 预留空间
        nodes.push_back({"", V(), 0, 0, 0}); // 添加一个虚拟节点
    }

    // 获取树中节点总数
    int size() {
        return len;
    }

    // 判断树中是否包含指定键值
    bool containsKey(const K& key) {
        if (key == "") {
            throw std::runtime_error("invalid parameter."); // 抛出异常，无效参数
        }
        int lastIndex = findLastIndex(key); // 查找最后一个等于或最接近key的节点
        return lastIndex != 0 && key == nodes[lastIndex].key; // 判断是否找到目标节点
    }

    // 向树中添加或更新键值对
    void put(const K& key, const V& value) {
        if (key == "") {
            throw std::runtime_error("invalid parameter."); // 抛出异常，无效参数
        }
        if (len == nodes.capacity() - 1) {
            throw std::runtime_error("size balanced tree is full."); // 抛出异常，树已满
        }
        int lastIndex = findLastIndex(key); // 查找最后一个等于或最接近key的节点
        if (lastIndex != 0 && key == nodes[lastIndex].key) {
            nodes[lastIndex].value = value; // 更新节点值
        } else {
            root = add(root, key, value); // 添加新节点
        }
    }

    // 获取第index小的键值
    K getIndexKey(int index) {
        if (index < 0 || index >= len) {
            throw std::runtime_error("invalid parameter."); // 抛出异常，无效参数
        }
        return nodes[getIndex(root, index + 1)].key; // 获取第index小的节点的键值
    }

    // 获取第index小的值
    V getIndexValue(int index) {
        if (index < 0 || index >= len) {
            throw std::runtime_error("invalid parameter."); // 抛出异常，无效参数
        }
        return nodes[getIndex(root, index + 1)].value; // 获取第index小的节点的值
    }

    // 获取指定键值对应的值
    V get(const K& key) {
        if (key == "") {
            throw std::runtime_error("invalid parameter."); // 抛出异常，无效参数
        }
        int lastIndex = findLastIndex(key); // 查找最后一个等于或最接近key的节点
        if (lastIndex != 0 && key == nodes[lastIndex].key) {
            return nodes[lastIndex].value; // 返回节点值
        } else {
            return V(); // 返回默认值
        }
    }

    // 获取树中最小的键值
    K firstKey() {
        int cur = root; // 当前节点的索引
        while (nodes[cur].left != 0) {
            cur = nodes[cur].left; // 向左子树查找
        }
        return cur == 0 ? "" : nodes[cur].key; // 返回最小键值
    }

    // 获取树中最大的键值
    K lastKey() {
        int cur = root; // 当前节点的索引
        while (nodes[cur].right != 0) {
            cur = nodes[cur].right; // 向右子树查找
        }
        return cur == 0 ? "" : nodes[cur].key; // 返回最大键值
    }

    // 获取小于等于指定键值的最大键值
    K floorKey(const K& key) {
        if (key == "") {
            throw std::runtime_error("invalid parameter."); // 抛出异常，无效参数
        }
        int lastNoBigIndex = findLastNoBigIndex(key); // 查找最后一个小于等于key的节点
        return lastNoBigIndex == 0 ? "" : nodes[lastNoBigIndex].key; // 返回键值
    }

    // 获取大于等于指定键值的最小键值
    K ceilingKey(const K& key) {
        if (key == "") {
            throw std::runtime_error("invalid parameter."); // 抛出异常，无效参数
        }
        int lastNoSmallIndex = findLastNoSmallIndex(key); // 查找最后一个不小于key的节点
        return lastNoSmallIndex == 0 ? "" : nodes[lastNoSmallIndex].key; // 返回键值
    }
};

