struct Node {
public:
    int value;
    Node* next;

    Node(int data) : value(data), next(nullptr) {}
};

// 获取链表的入环节点（如果有环）
Node* getLoopNode(Node* head) {
    if (head == nullptr || head->next == nullptr || head->next->next == nullptr) {
        return nullptr; // 如果链表为空或长度小于3，不可能有环
    }
    Node* n1 = head->next; // n1 -> slow
    Node* n2 = head->next->next; // n2 -> fast
    while (n1 != n2) {
        if (n2->next == nullptr || n2->next->next == nullptr) {
            return nullptr; // 如果快指针到达链表末尾，说明没有环
        }
        n2 = n2->next->next; // 快指针每次移动两步
        n1 = n1->next; // 慢指针每次移动一步
    }
    n2 = head; // 快指针重新从头开始
    while (n1 != n2) {
        n1 = n1->next; // 慢指针和快指针每次都移动一步
        n2 = n2->next;
    }
    return n1; // 慢指针和快指针相遇的节点即为入环节点
}

// 找到两个无环链表的第一个相交节点
Node* noLoop(Node* head1, Node* head2) {
    if (head1 == nullptr || head2 == nullptr) {
        return nullptr; // 如果任一链表为空，没有相交节点
    }
    Node* cur1 = head1;
    Node* cur2 = head2;
    int n = 0;
    while (cur1->next != nullptr) {
        n++;
        cur1 = cur1->next; // 计算链表1的长度
    }
    while (cur2->next != nullptr) {
        n--;
        cur2 = cur2->next; // 计算链表2的长度，并计算长度差
    }
    if (cur1 != cur2) {
        return nullptr; // 如果两个链表的尾节点不同，没有相交节点
    }
    cur1 = n > 0 ? head1 : head2; // 让较长的链表先走n步
    cur2 = cur1 == head1 ? head2 : head1;
    n = std::abs(n);
    while (n != 0) {
        n--;
        cur1 = cur1->next; // 较长的链表先走n步，使得两个链表剩余长度相同
    }
    while (cur1 != cur2) {
        cur1 = cur1->next; // 同时遍历两个链表，第一个相同的节点即为相交节点
        cur2 = cur2->next;
    }
    return cur1; // 返回相交节点
}

// 找到两个有环链表的第一个相交节点
Node* bothLoop(Node* head1, Node* loop1, Node* head2, Node* loop2) {
    Node* cur1 = nullptr;
    Node* cur2 = nullptr;
    if (loop1 == loop2) {
        // 如果两个链表的入环节点相同，问题转化为两个无环链表的相交节点问题
        cur1 = head1;
        cur2 = head2;
        int n = 0;
        while (cur1 != loop1) {
            n++;
            cur1 = cur1->next; // 计算链表1到入环节点的长度
        }
        while (cur2 != loop2) {
            n--;
            cur2 = cur2->next; // 计算链表2到入环节点的长度，并计算长度差
        }
        cur1 = n > 0 ? head1 : head2; // 让较长的链表先走n步
        cur2 = cur1 == head1 ? head2 : head1;
        n = std::abs(n);
        while (n != 0) {
            n--;
            cur1 = cur1->next; // 较长的链表先走n步，使得两个链表剩余长度相同
        }
        while (cur1 != cur2) {
            cur1 = cur1->next; // 同时遍历两个链表，第一个相同的节点即为相交节点
            cur2 = cur2->next;
        }
        return cur1; // 返回相交节点
    } else {
        // 如果两个链表的入环节点不同，检查其中一个环是否包含另一个环的入环节点
        cur1 = loop1->next;
        while (cur1 != loop1) {
            if (cur1 == loop2) {
                return loop1; // 如果找到相交节点，返回入环节点
            }
            cur1 = cur1->next;
        }
        return nullptr; // 如果没有找到相交节点，返回nullptr
    }
}

// 找到两个链表的第一个相交节点
Node* getIntersectNode(Node* head1, Node* head2) {
    if (head1 == nullptr || head2 == nullptr) {
        return nullptr; // 如果任一链表为空，没有相交节点
    }
    Node* loop1 = getLoopNode(head1); // 获取链表1的入环节点
    Node* loop2 = getLoopNode(head2); // 获取链表2的入环节点
    if (loop1 == nullptr && loop2 == nullptr) {
        return noLoop(head1, head2); // 如果两个链表都没有环，调用noLoop函数
    }
    if (loop1 != nullptr && loop2 != nullptr) {
        return bothLoop(head1, loop1, head2, loop2); // 如果两个链表都有环，调用bothLoop函数
    }
    return nullptr; // 如果一个链表有环，另一个没有环，没有相交节点
}
