#include <bits/stdc++.h>
using namespace std;

// 定义链表节点结构
struct ListNode {
    int val;        // 节点存储的值
    ListNode* next; // 指向下一个节点的指针
    ListNode(int x) : val(x), next(nullptr) {} // 构造函数，初始化节点值和下一个节点指针
};

// 每k个节点一组翻转链表

// 当前组的开始节点是s，往下数k个找到当前组的结束节点返回
ListNode* teamEnd(ListNode* s, int k) {
    while (--k != 0 && s != nullptr) { // 循环k-1次，找到第k个节点
        s = s->next; // 移动到下一个节点
    }
    return s; // 返回第k个节点或nullptr（如果不足k个节点）
}

// s -> a -> b -> c -> e -> 下一组的开始节点
// 上面的链表通过如下的reverse方法调整成 : e -> c -> b -> a -> s -> 下一组的开始节点
void reverse(ListNode* s, ListNode* e) {
    e = e->next; // 记录下一组的开始节点
    ListNode* pre = nullptr; // 初始化前驱节点为nullptr
    ListNode* cur = s;       // 当前节点初始化为s
    ListNode* next = nullptr; // 用于存储下一个节点的临时指针
    while (cur != e) { // 循环直到当前节点等于下一组的开始节点
        next = cur->next; // 存储下一个节点
        cur->next = pre;  // 反转当前节点的指针
        pre = cur;        // 更新前驱节点为当前节点
        cur = next;       // 移动到下一个节点
    }
    s->next = e; // 将反转后的链表连接到下一组的开始节点
}

// 提交如下的方法
ListNode* reverseKGroup(ListNode* head, int k) {
    ListNode* start = head; // 初始化第一组的开始节点为head
    ListNode* end = teamEnd(start, k); // 找到第一组的结束节点
    if (end == nullptr) { // 如果不足k个节点，直接返回原链表
        return head;
    }
    // 第一组很特殊因为牵扯到换头的问题
    head = end; // 更新头节点为第一组的结束节点
    reverse(start, end); // 反转第一组
    // 翻转之后start变成了上一组的结尾节点
    ListNode* lastTeamEnd = start; // 记录上一组的结束节点
    while (lastTeamEnd->next != nullptr) { // 循环直到没有更多的组
        start = lastTeamEnd->next; // 更新当前组的开始节点
        end = teamEnd(start, k); // 找到当前组的结束节点
        if (end == nullptr) { // 如果不足k个节点，直接返回结果
            return head;
        }
        reverse(start, end); // 反转当前组
        lastTeamEnd->next = end; // 将上一组的结束节点连接到当前组的结束节点
        lastTeamEnd = start; // 更新上一组的结束节点为当前组的开始节点
    }
    return head; // 返回反转后的链表头节点
}

// 辅助函数：打印链表
void printList(ListNode* head) {
    while (head != nullptr) { // 循环直到链表结束
        std::cout << head->val << " "; // 打印当前节点的值
        head = head->next; // 移动到下一个节点
    }
    std::cout << std::endl; // 打印换行符
}

// 辅助函数：创建链表
ListNode* createList(const vector<int>& values) {
    if (values.empty()) return nullptr; // 如果输入为空，返回nullptr
    ListNode* head = new ListNode(values[0]); // 创建头节点
    ListNode* current = head; // 当前节点初始化为头节点
    for (int i = 1; i < values.size(); ++i) { // 循环插入剩余节点
        current->next = new ListNode(values[i]); // 创建新节点并连接到当前节点
        current = current->next; // 移动到下一个节点
    }
    current->next = nullptr; // 最后一个节点的next指针设为nullptr
    return head; // 返回链表头节点
}

// 辅助函数：释放链表内存
void deleteList(ListNode* head) {
    while (head != nullptr) { // 循环直到链表结束
        ListNode* temp = head; // 存储当前节点
        head = head->next; // 移动到下一个节点
        delete temp; // 释放当前节点的内存
    }
}

int main() {
    vector<int> values = {1, 2, 3, 4, 5}; // 定义链表的值
    ListNode* head = createList(values); // 创建链表
    int k = 2; // 定义每组的节点数

    std::cout << "Original list: ";
    printList(head); // 打印原始链表

    ListNode* newHead = reverseKGroup(head, k); // 反转链表

    std::cout << "Reversed list in groups of " << k << ": ";
    printList(newHead); // 打印反转后的链表

    // 释放链表内存
    deleteList(newHead);

    return 0;
}
