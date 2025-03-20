#include <iostream>

// 定义链表节点结构
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(NULL) {}
};



// 检测链表中的环并返回环的起始节点
static ListNode* detectCycle(ListNode* head) {
    // 如果链表为空，或者链表只有一个节点，或者链表只有两个节点且没有循环，直接返回nullptr
    if (head == nullptr || head->next == nullptr || head->next->next == nullptr) {
        return nullptr;
    }

    // 初始化快慢指针，慢指针每次移动一步，快指针每次移动两步
    ListNode* slow = head->next;
    ListNode* fast = head->next->next;

    // 使用快慢指针检测是否存在循环
    while (slow != fast) {
        // 如果快指针到达链表末尾，说明没有循环，返回nullptr
        if (fast->next == nullptr || fast->next->next == nullptr) {
            return nullptr;
        }
        slow = slow->next;          // 慢指针移动一步
        fast = fast->next->next;    // 快指针移动两步
    }

    // 如果存在循环，将快指针重新指向链表头
    fast = head;

    // 移动快慢指针，直到它们相遇，相遇点即为循环的起始节点
    while (slow != fast) {
        slow = slow->next;
        fast = fast->next;
    }

    // 返回循环的起始节点
    return slow;
}


int main() {
    // 示例用法
    ListNode* head = new ListNode(3);
    head->next = new ListNode(2);
    head->next->next = new ListNode(0);
    head->next->next->next = new ListNode(-4);
    head->next->next->next->next = head->next; // 创建环

    ListNode* cycleStart = detectCycle(head);

    if (cycleStart != nullptr) {
        std::cout << "Cycle starts at node with value: " << cycleStart->val << std::endl;
    } else {
        std::cout << "No cycle detected." << std::endl;
    }


    return 0;
}
