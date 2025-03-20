#include <unordered_map>
#include <iostream>

  class LRUCache {
public:
    // 定义双向链表节点结构
    struct DoubleNode {
        int key;
        int val;
        DoubleNode* last; // 指向前一个节点的指针
        DoubleNode* next; // 指向后一个节点的指针

        // 构造函数，初始化节点的键和值
        DoubleNode(int k, int v) : key(k), val(v), last(nullptr), next(nullptr) {}
    };

    // 定义双向链表类
    class DoubleList {
    private:
        DoubleNode* head; // 链表头节点
        DoubleNode* tail; // 链表尾节点

    public:
        // 构造函数，初始化头尾节点为空
        DoubleList() : head(nullptr), tail(nullptr) {}

        // 在链表尾部添加新节点
        void addNode(DoubleNode* newNode) {
            if (!newNode) {
                return; // 如果新节点为空，直接返回
            }
            if (!head) {
                head = newNode; // 如果链表为空，新节点既是头节点也是尾节点
                tail = newNode;
            } else {
                tail->next = newNode; // 将新节点添加到尾部
                newNode->last = tail;
                tail = newNode;
            }
        }

        // 将指定节点移动到链表尾部
        void moveNodeToTail(DoubleNode* node) {
            if (tail == node) {
                return; // 如果节点已经是尾节点，直接返回
            }
            if (head == node) {
                head = node->next; // 如果节点是头节点，更新头节点
                if (head) {
                    head->last = nullptr;
                }
            } else {
                node->last->next = node->next; // 更新前一个节点的next指针
                if (node->next) {
                    node->next->last = node->last; // 更新后一个节点的last指针
                }
            }
            node->last = tail; // 将节点的last指针指向当前尾节点
            node->next = nullptr; // 将节点的next指针置为空
            if (tail) {
                tail->next = node; // 将当前尾节点的next指针指向新节点
            }
            tail = node; // 更新尾节点为新节点
        }

        // 移除并返回链表头部节点
        DoubleNode* removeHead() {
            if (!head) {
                return nullptr; // 如果链表为空，返回空指针
            }
            DoubleNode* ans = head; // 记录头节点
            if (head == tail) {
                head = nullptr; // 如果链表只有一个节点，更新头尾节点为空
                tail = nullptr;
            } else {
                head = ans->next; // 更新头节点为下一个节点
                if (head) {
                    head->last = nullptr; // 将新头节点的last指针置为空
                }
                ans->next = nullptr; // 将原头节点的next指针置为空
            }
            return ans; // 返回原头节点
        }
    };

private:
    std::unordered_map<int, DoubleNode*> keyNodeMap; // 键到节点的映射
    DoubleList nodeList; // 双向链表，用于维护节点顺序
    const int capacity; // 缓存容量

public:
    // 构造函数，初始化缓存容量
    LRUCache(int cap) : capacity(cap) {}

    // 获取指定键的值
    int get(int key) {
        if (keyNodeMap.find(key) != keyNodeMap.end()) {
            DoubleNode* ans = keyNodeMap[key]; // 找到节点
            nodeList.moveNodeToTail(ans); // 将节点移动到链表尾部
            return ans->val; // 返回节点的值
        }
        return -1; // 如果键不存在，返回-1
    }

    // 插入或更新指定键的值
    void put(int key, int value) {
        if (keyNodeMap.find(key) != keyNodeMap.end()) {
            DoubleNode* node = keyNodeMap[key]; // 找到节点
            node->val = value; // 更新节点的值
            nodeList.moveNodeToTail(node); // 将节点移动到链表尾部
        } else {
            if (keyNodeMap.size() == capacity) {
                DoubleNode* removedNode = nodeList.removeHead(); // 移除链表头部节点
                if (removedNode) {
                    keyNodeMap.erase(removedNode->key); // 从映射中移除键
                    delete removedNode; // 释放内存
                }
            }
            DoubleNode* newNode = new DoubleNode(key, value); // 创建新节点
            keyNodeMap[key] = newNode; // 将新节点添加到映射中
            nodeList.addNode(newNode); // 将新节点添加到链表尾部
        }
    }
};


int main() {
    LRUCache cache(2); // 创建一个容量为2的LRU缓存

    // 测试put方法
    cache.put(1, 1);
    cache.put(2, 2);

    // 测试get方法
    std::cout << "get(1): " << cache.get(1) << std::endl; // 返回 1

    // 测试put方法，触发淘汰机制
    cache.put(3, 3); // 淘汰键为2的元素
    std::cout << "get(2): " << cache.get(2) << std::endl; // 返回 -1 (未找到)

    // 测试put方法，更新现有键的值
    cache.put(4, 4); // 淘汰键为1的元素
    std::cout << "get(1): " << cache.get(1) << std::endl; // 返回 -1 (未找到)
    std::cout << "get(3): " << cache.get(3) << std::endl; // 返回 3
    std::cout << "get(4): " << cache.get(4) << std::endl; // 返回 4

    return 0;
}