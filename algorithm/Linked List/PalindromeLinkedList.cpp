#include <bits/stdc++.h>
using namespace std;

// 判断链表是否是回文结构

// 不要提交这个结构体
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

// 判断链表是否为回文链表
bool isPalindrome(ListNode* head) {
    // 如果链表为空或者只有一个节点，直接返回true
    if (head == nullptr || head->next == nullptr) {
        return true;
    }

    ListNode* slow = head;  // 慢指针，用于找到链表中点
    ListNode* fast = head;  // 快指针，用于找到链表中点

    // 找到链表的中点（slow最终会指向中点）
    while (fast->next != nullptr && fast->next->next != nullptr) {
        slow = slow->next;  // 慢指针每次移动一步
        fast = fast->next->next;  // 快指针每次移动两步
    }

    // 从慢指针位置开始，将后半部分链表逆序
    ListNode* pre = slow;  // pre记录前一个节点
    ListNode* cur = pre->next;  // cur记录当前节点
    ListNode* next = nullptr;  // next记录下一个节点

    // 断开前后两部分链表
    pre->next = nullptr;

    // 逆序后半部分链表
    while (cur != nullptr) {
        next = cur->next;  // 保存当前节点的下一个节点
        cur->next = pre;  // 将当前节点指向前一个节点
        pre = cur;  // 更新pre为当前节点
        cur = next;  // 移动到下一个节点
    }

    // 此时链表被分为两部分：head -> ... -> slow 和 pre <- ... <- slow
    bool ans = true;  // 初始化结果为true
    ListNode* left = head;  // 左侧指针从头节点开始
    ListNode* right = pre;  // 右侧指针从逆序后的头节点开始

    // 比较左右两侧节点值是否相同
    while (left != nullptr && right != nullptr) {
        if (left->val != right->val) {  // 如果发现不同，则设置结果为false并退出循环
            ans = false;
            break;
        }
        left = left->next;  // 左侧指针向右移动
        right = right->next;  // 右侧指针向左移动
    }

    // 将链表恢复原状（将后半部分重新逆序）
    cur = pre->next;  // cur从逆序后的第二个节点开始
    pre->next = nullptr;  // 断开逆序后的链表

    // 恢复链表顺序
    while (cur != nullptr) {
        next = cur->next;  // 保存当前节点的下一个节点
        cur->next = pre;  // 将当前节点指向前一个节点
        pre = cur;  // 更新pre为当前节点
        cur = next;  // 移动到下一个节点
    }

    // 返回判断结果
    return ans;
}


int main() {
    // 创建一个回文链表 1 -> 2 -> 2 -> 1
    auto* head = new ListNode(1);
    head->next = new ListNode(2);
    head->next->next = new ListNode(2);
    head->next->next->next = new ListNode(1);

    // 测试isPalindrome函数
    if (isPalindrome(head)) {
        std::cout << "1" << std::endl;
    } else {
        std::cout << "0" << std::endl;
    }

    // 释放链表内存
    ListNode* current = head;
    while (current != nullptr) {
        ListNode* next = current->next;
        delete current;
        current = next;
    }

    return 0;
}