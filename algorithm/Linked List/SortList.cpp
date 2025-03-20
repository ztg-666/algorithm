#include <bits/stdc++.h>
using namespace std;

// 定义链表节点结构
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

ListNode* start;
ListNode* endNode;

// 包括s在内，往下数k个节点返回
// 如果不够，返回最后一个数到的非空节点
ListNode* findendNode(ListNode* s, int k) {
    while (s->next != nullptr && --k != 0) {
        s = s->next;
    }
    return s;
}

// 合并两个有序链表 l1...r1 和 l2...r2，保证合并后的链表仍然有序
// 并且将全局变量 start 设置为合并后链表的头节点，endNode 设置为合并后链表的尾节点
void merge(ListNode* l1, ListNode* r1, ListNode* l2, ListNode* r2) {
    ListNode* pre; // 用于记录当前合并过程中最后一个节点的指针

    // 确定合并后链表的头节点
    if (l1->val <= l2->val) {
        start = l1; // 合并后链表的头节点为 l1
        pre = l1;
        l1 = l1->next; // 移动 l1 指针到下一个节点
    } else {
        start = l2; // 合并后链表的头节点为 l2
        pre = l2;
        l2 = l2->next; // 移动 l2 指针到下一个节点
    }

    // 遍历两个链表，依次比较节点值并进行合并
    while (l1 != nullptr && l2 != nullptr) {
        if (l1->val <= l2->val) {
            pre->next = l1; // 将较小的节点连接到合并链表中
            pre = l1;
            l1 = l1->next; // 移动 l1 指针到下一个节点
        } else {
            pre->next = l2; // 将较小的节点连接到合并链表中
            pre = l2;
            l2 = l2->next; // 移动 l2 指针到下一个节点
        }
    }

    // 如果其中一个链表已经遍历完，直接将另一个链表剩余部分连接到合并链表
    if (l1 != nullptr) {
        pre->next = l1; // 连接 l1 剩余部分
        endNode = r1; // 合并链表的尾节点为 r1
    } else {
        pre->next = l2; // 连接 l2 剩余部分
        endNode = r2; // 合并链表的尾节点为 r2
    }
}

// 对链表进行归并排序
ListNode* sortList(ListNode* head) {
    int n = 0; // 记录链表的长度
    ListNode* cur = head;

    // 计算链表的长度
    while (cur != nullptr) {
        n++;
        cur = cur->next;
    }

    // 定义分组相关变量
    // l1...r1: 每组的左部分
    // l2...r2: 每组的右部分
    // next: 下一组的开头
    // lastTeamendNode: 上一组的结尾
    ListNode *l1, *r1, *l2, *r2, *next, *lastTeamendNode;

    // 按照步长 step 进行分组和合并
    for (int step = 1; step < n; step <<= 1) { // 每次将步长翻倍
        // 第一组需要特殊处理，因为它决定了整个链表的头节点
        l1 = head;
        r1 = findendNode(l1, step); // 找到左部分的尾节点
        l2 = r1->next; // 右部分的头节点
        r2 = findendNode(l2, step); // 找到右部分的尾节点
        next = r2 != nullptr ? r2->next : nullptr; // 下一组的开头
        r1->next = nullptr; // 断开左部分与右部分的连接
        r2->next = nullptr; // 断开右部分与后续部分的连接
        merge(l1, r1, l2, r2); // 合并左部分和右部分
        head = start; // 更新链表的头节点
        lastTeamendNode = endNode; // 记录上一组的尾节点

        // 处理后续的分组
        while (next != nullptr) {
            l1 = next; // 当前组的左部分头节点
            r1 = findendNode(l1, step); // 当前组的左部分尾节点
            l2 = r1->next; // 当前组的右部分头节点

            // 如果没有右部分，则直接将左部分连接到上一组的尾部
            if (l2 == nullptr) {
                lastTeamendNode->next = l1;
                break;
            }

            r2 = findendNode(l2, step); // 当前组的右部分尾节点
            next = r2 != nullptr ? r2->next : nullptr; // 下一组的开头
            r1->next = nullptr; // 断开左部分与右部分的连接
            r2->next = nullptr; // 断开右部分与后续部分的连接
            merge(l1, r1, l2, r2); // 合并左部分和右部分
            lastTeamendNode->next = start; // 将当前组的头节点连接到上一组的尾部
            lastTeamendNode = endNode; // 更新上一组的尾节点
        }
    }

    return head; // 返回排序后的链表头节点
}

// 打印链表
void printList(ListNode* head) {
    while (head != nullptr) {
        std::cout << head->val << " ";
        head = head->next;
    }
    std::cout << std::endl;
}

int main() {
    // 创建一个链表: 4 -> 2 -> 1 -> 3
    ListNode* head = new ListNode(4);
    head->next = new ListNode(2);
    head->next->next = new ListNode(1);
    head->next->next->next = new ListNode(3);

    std::cout << "1: ";
    printList(head);

    // 对链表进行排序
    head = sortList(head);

    std::cout << "2: ";
    printList(head);

    // 释放链表内存
    while (head != nullptr) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }

    return 0;
}



