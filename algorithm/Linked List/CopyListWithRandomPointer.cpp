#include <bits/stdc++.h>
using namespace std;

// 定义链表节点结构
class Node {
public:
    int val;          // 节点的值
    Node* next;       // 指向下一个节点的指针
    Node* random;     // 指向随机节点的指针

    // 构造函数
    Node(int v) : val(v), next(nullptr), random(nullptr) {}
};

// 深拷贝带有随机指针的链表
static Node* copyRandomList(Node* head) {
    if (head == nullptr) {
        return nullptr; // 如果原链表为空，直接返回空
    }

    Node* cur = head;
    Node* next = nullptr;

    // 第一步：在每个节点后面插入一个复制节点
    // 例如：1 -> 2 -> 3 变成 1 -> 1' -> 2 -> 2' -> 3 -> 3'
    while (cur != nullptr) {
        next = cur->next; // 保存当前节点的下一个节点
        cur->next = new Node(cur->val); // 创建新节点并插入到当前节点之后
        cur->next->next = next; // 将新节点的next指向原链表的下一个节点
        cur = next; // 移动到原链表的下一个节点
    }

    cur = head;
    Node* copy = nullptr;

    // 第二步：设置复制节点的random指针
    // 利用原节点和复制节点的关系设置random指针
    while (cur != nullptr) {
        next = cur->next->next; // 保存原链表中下一个节点
        copy = cur->next; // 获取当前复制节点
        copy->random = cur->random != nullptr ? cur->random->next : nullptr; // 设置复制节点的random指针
        cur = next; // 移动到原链表的下一个节点
    }

    Node* ans = head->next; // 新链表的头节点
    cur = head;

    // 第三步：分离原链表和复制链表
    while (cur != nullptr) {
        next = cur->next->next; // 保存原链表中下一个节点
        copy = cur->next; // 获取当前复制节点
        cur->next = next; // 恢复原链表的next指针
        copy->next = next != nullptr ? next->next : nullptr; // 设置复制链表的next指针
        cur = next; // 移动到原链表的下一个节点
    }

    // 返回新链表的头节点
    return ans;
}

int main() {
    // 创建一个带有随机指针的链表
    Node* head = new Node(1);
    head->next = new Node(2);
    head->next->next = new Node(3);
    head->next->next->next = new Node(4);

    // 设置随机指针
    head->random = head->next->next; // 1 -> 3
    head->next->random = head;       // 2 -> 1
    head->next->next->random = head->next->next->next; // 3 -> 4
    head->next->next->next->random = head->next;       // 4 -> 2

    // 打印原始链表
    std::cout << "Original list:" << std::endl;
    Node* cur = head;
    while (cur != nullptr) {
        std::cout << "Node value: " << cur->val;
        if (cur->random != nullptr) {
            std::cout << ", Random value: " << cur->random->val;
        } else {
            std::cout << ", Random value: nullptr";
        }
        std::cout << std::endl;
        cur = cur->next;
    }

    // 复制链表
    Node* copiedHead = copyRandomList(head);

    // 打印复制后的链表
    std::cout << "Copied list:" << std::endl;
    cur = copiedHead;
    while (cur != nullptr) {
        std::cout << "Node value: " << cur->val;
        if (cur->random != nullptr) {
            std::cout << ", Random value: " << cur->random->val;
        } else {
            std::cout << ", Random value: nullptr";
        }
        std::cout << std::endl;
        cur = cur->next;
    }

    // 释放原始链表的内存
    cur = head;
    while (cur != nullptr) {
        Node* next = cur->next; // 保存下一个节点
        delete cur; // 删除当前节点
        cur = next; // 移动到下一个节点
    }

    // 释放复制链表的内存
    cur = copiedHead;
    while (cur != nullptr) {
        Node* next = cur->next; // 保存下一个节点
        delete cur; // 删除当前节点
        cur = next; // 移动到下一个节点
    }

    return 0;
}
