#include <bits/stdc++.h>
using namespace std;
// 定义跳表节点结构体
template<typename K, typename V>
struct SkipListNode {
    K key; // 节点键值
    V val; // 节点存储的值
    std::vector<SkipListNode<K, V>*> nextNodes; // 指向不同层级的下一个节点指针数组
    bool isHead; // 标记是否为头节点

    // 构造函数，初始化节点
    SkipListNode(const K& k, const V& v, bool isHeadNode = false)
        : key(k), val(v), isHead(isHeadNode) {}

    // 判断当前节点的键是否小于给定的键
    bool isKeyLess(const K& otherKey) const {
        return isHead || key < otherKey;
    }

    // 判断当前节点的键是否等于给定的键
    bool isKeyEqual(const K& otherKey) const {
        return isHead ||  key == otherKey;
    }
};

// 定义跳表类
template<typename K, typename V>
class SkipListMap {
private:
    static const double PROBABILITY ; // 新节点提升到更高层级的概率
    SkipListNode<K, V>* head; // 跳表头节点
    int size_; // 跳表中节点的数量
    int maxLevel; // 跳表当前的最大层级

    // 找到小于给定键的最大节点
    SkipListNode<K, V>* mostRightLessNodeInTree(const K& key) {
        // 如果跳表中没有节点（即头节点的下一个节点为空），则返回头节点
        if (head->nextNodes[0] == nullptr) {
            return head;
        }

        // 从头节点开始查找
        SkipListNode<K, V>* cur = head;
        // 从最高层级开始查找
        int level = maxLevel;

        // 逐层向下查找，直到最低层级
        while (level >= 0) {
            // 在当前层级中找到小于给定键的最大节点
            cur = mostRightLessNodeInLevel(key, cur, level--);
        }

        // 返回找到的节点
        return cur;
    }


    // 在指定层级中找到小于给定键的最大节点
    SkipListNode<K, V>* mostRightLessNodeInLevel(const K& key, SkipListNode<K, V>* cur, int level) {
        // 如果当前节点在指定层级的下一个节点为空，或者下一个节点的键小于给定键，则返回当前节点
        if (cur->nextNodes[level] == nullptr || cur->nextNodes[level]->isKeyLess(key)) {
            return cur;
        }

        // 初始化 next 为当前节点在指定层级的下一个节点
        SkipListNode<K, V>* next = cur->nextNodes[level];

        // 遍历当前层级，找到小于给定键的最大节点
        while (next != nullptr && next->isKeyLess(key)) {
            cur = next; // 将 cur 更新为 next
            next = cur->nextNodes[level]; // 更新 next 为 cur 在指定层级的下一个节点
        }

        // 返回找到的节点
        return cur;
    }


public:
    // 构造函数，初始化跳表
    SkipListMap() : size_(0), maxLevel(0) {
        head = new SkipListNode<K, V>(K(), V(), true);
        head->nextNodes.push_back(nullptr);
    }

    // 析构函数，释放跳表占用的内存
    ~SkipListMap() {
        std::vector<SkipListNode<K, V>*> nodesToDelete;
        SkipListNode<K, V>* current = head->nextNodes[0];
        while (current != nullptr) {
            nodesToDelete.push_back(current);
            current = current->nextNodes[0];
        }
        for (auto node : nodesToDelete) {
            delete node;
        }
        delete head;
    }

    // 判断跳表中是否包含指定键
    bool containsKey(const K& key) {
        // 如果跳表中没有节点（即头节点的下一个节点为空），则直接返回 false
        if (head->nextNodes[0] == nullptr) {
            return false;
        }

        // 找到小于给定键的最大节点
        SkipListNode<K, V>* less = mostRightLessNodeInTree(key);

        // 获取小于给定键的最大节点的下一个节点
        SkipListNode<K, V>* next = less->nextNodes[0];

        // 检查下一个节点是否存在且其键是否等于给定键
        return next != nullptr && next->isKeyEqual(key);
    }


    // 向跳表中插入键值对
    void put(const K& key, const V& value) {
        // 找到小于给定键的最大节点
        SkipListNode<K, V>* less = mostRightLessNodeInTree(key);

        // 获取小于给定键的最大节点的下一个节点
        SkipListNode<K, V>* find = less->nextNodes[0];

        // 如果键已存在，则更新值
        if (find != nullptr && find->isKeyEqual(key)) {
            find->val = value;
        } else {
            // 增加节点数量
            size_++;

            // 以概率决定新节点的层级
            int newNodeLevel = 0;
            while (rand() / static_cast<double>(RAND_MAX) < PROBABILITY) {
                newNodeLevel++;
            }

            // 如果新节点的层级大于当前最大层级，则增加头节点的层级指针
            while (newNodeLevel > maxLevel) {
                head->nextNodes.push_back(nullptr);
                maxLevel++;
            }

            // 创建新节点
            auto* newNode = new SkipListNode<K, V>(key, value);

            // 初始化新节点的 nextNodes 向量
            for (int i = 0; i <= newNodeLevel; i++) {
                newNode->nextNodes.push_back(nullptr);
            }

            // 从最高层级开始，将新节点插入到合适的位置
            int level = maxLevel;
            SkipListNode<K, V>* pre = head;
            while (level >= 0) {
                pre = mostRightLessNodeInLevel(key, pre, level);

                // 如果当前层级小于或等于新节点的层级，则插入新节点
                if (level <= newNodeLevel) {
                    newNode->nextNodes[level] = pre->nextNodes[level];
                    pre->nextNodes[level] = newNode;
                }

                level--;
            }
        }
    }


    // 根据键获取对应的值
    V get(const K& key) {
        // 检查跳表中是否包含指定键
        if (!containsKey(key)) {
            return V();
        }

        // 找到小于给定键的最大节点
        SkipListNode<K, V>* less = mostRightLessNodeInTree(key);

        // 获取小于给定键的最大节点的下一个节点
        SkipListNode<K, V>* next = less->nextNodes[0];

        // 检查下一个节点是否存在且其键是否等于给定键
        if (next != nullptr && next->isKeyEqual(key)) {
            return next->val; // 如果键存在，返回对应的值
        }

        return V();
    }


    // 从跳表中删除指定键的节点
    void remove(const K& key) {
        // 如果键不存在，直接返回
        if (!containsKey(key)) {
            return;
        }

        // 减少节点数量
        size_--;

        // 从最高层级开始删除指定键的节点
        int level = maxLevel;
        SkipListNode<K, V>* pre = head;
        SkipListNode<K, V>* toDelete = nullptr;

        while (level >= 0) {
            // 在当前层级中找到小于给定键的最大节点
            pre = mostRightLessNodeInLevel(key, pre, level);

            // 获取小于给定键的最大节点的下一个节点
            SkipListNode<K, V>* next = pre->nextNodes[level];

            // 如果下一个节点存在且其键等于给定键
            if (next != nullptr && next->isKeyEqual(key)) {
                if (toDelete == nullptr) {
                    toDelete = next; // 记录要删除的节点
                }
                // 更新 pre 的 nextNodes 指针，跳过要删除的节点
                pre->nextNodes[level] = next->nextNodes[level];
            }

            // 如果当前层级没有节点，则减少最大层级
            if (level != 0 && pre == head && pre->nextNodes[level] == nullptr) {
                head->nextNodes.pop_back(); // 移除头节点的层级指针
                maxLevel--; // 减少最大层级
            }

            level--; // 继续查找下一层级
        }

        // 释放被删除节点的内存
        if (toDelete != nullptr) {
            delete toDelete;
        }
    }


    // 获取跳表中最小的键
    K firstKey() {
        if (head->nextNodes[0] != nullptr) {
            return head->nextNodes[0]->key; // 如果跳表不为空，返回最小键
        }
        return K(); // 如果跳表为空，返回 std::nullopt
    }

    // 获取跳表中最大的键
    K lastKey() {
        int level = maxLevel;
        SkipListNode<K, V>* cur = head;
        // 从最高层级开始，找到最右侧的节点
        while (level >= 0) {
            SkipListNode<K, V>* next = cur->nextNodes[level];
            while (next != nullptr) {
                cur = next;
                next = cur->nextNodes[level];
            }
            level--;
        }
        return cur == head ? K() : cur->key; // 如果跳表为空，返回默认键值
    }

    // 获取大于或等于给定键的最小键
     K ceilingKey(const K& key) {
        SkipListNode<K, V>* less = mostRightLessNodeInTree(key);
        SkipListNode<K, V>* next = less->nextNodes[0];
        return next != nullptr ? next->key : K(); // 如果没有找到，返回默认键值
    }

    // 获取小于或等于给定键的最大键
    K floorKey(const K& key) {
        SkipListNode<K, V>* less = mostRightLessNodeInTree(key);
        SkipListNode<K, V>* next = less->nextNodes[0];
        if (next != nullptr && next->isKeyEqual(key)) {
            return next->key;
        } else {
            return less->isHead ? K() : less->key; // 如果没有找到，返回默认键值
        }
    }

    // 获取跳表中节点的数量
    int size() const {
        return size_;
    }


};
// 定义新节点提升到更高层级的概率
template<typename K, typename V>
const double SkipListMap<K, V>::PROBABILITY = 0.5;

