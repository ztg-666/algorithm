#include<bits/stdc++.h>
using namespace std;

// AllOne 类实现了一个数据结构，支持以下操作：
// - inc(key): 增加 key 的计数
// - dec(key): 减少 key 的计数（如果计数变为 0，则移除 key）
// - getMaxKey(): 返回当前计数最大的 key
// - getMinKey(): 返回当前计数最小的 key
class AllOne {
public:
    // 定义 Bucket 结构体，表示一个计数桶
    struct Bucket {
        std::unordered_set<std::string> set; // 存储具有相同计数的 key
        int cnt;                             // 当前桶的计数值
        Bucket* last;                        // 指向前一个桶的指针
        Bucket* next;                        // 指向后一个桶的指针

        // 构造函数：初始化一个桶，包含一个 key 和对应的计数值
        Bucket(std::string s, int c) : set({s}), cnt(c), last(nullptr), next(nullptr) {}
    };

private:
    // 在链表中插入一个新的桶到指定位置
    void insert(Bucket* cur, Bucket* pos) {
        cur->next->last = pos;
        pos->next = cur->next;
        cur->next = pos;
        pos->last = cur;
    }

    // 从链表中移除一个桶
    void remove(Bucket* cur) {
        cur->last->next = cur->next;
        cur->next->last = cur->last;
    }

    Bucket* head; // 链表头节点，cnt = 0
    Bucket* tail; // 链表尾节点，cnt = INT_MAX
    std::unordered_map<std::string, Bucket*> map; // key 到其所在桶的映射

public:
    // 构造函数：初始化链表头尾节点
    AllOne() {
        head = new Bucket("", 0); // 虚拟头节点
        tail = new Bucket("", INT_MAX); // 虚拟尾节点
        head->next = tail; // 初始化头尾连接
        tail->last = head;
    }

    // 增加 key 的计数
    void inc(std::string key) {
        if (map.find(key) == map.end()) { // 如果 key 不存在
            if (head->next->cnt == 1) { // 如果下一个桶的计数为 1
                map[key] = head->next; // 将 key 加入该桶
                head->next->set.insert(key);
            } else { // 否则创建一个新的桶，计数为 1
                Bucket* newBucket = new Bucket(key, 1);
                map[key] = newBucket; // 更新映射
                insert(head, newBucket); // 插入新桶
            }
        } else { // 如果 key 已存在
            Bucket* bucket = map[key]; // 获取 key 所在的桶
            if (bucket->next->cnt == bucket->cnt + 1) { // 如果下一个桶的计数等于当前计数 + 1
                map[key] = bucket->next; // 将 key 移动到下一个桶
                bucket->next->set.insert(key);
            } else { // 否则创建一个新的桶，计数为当前计数 + 1
                Bucket* newBucket = new Bucket(key, bucket->cnt + 1);
                map[key] = newBucket; // 更新映射
                insert(bucket, newBucket); // 插入新桶
            }
            bucket->set.erase(key); // 从原桶中移除 key
            if (bucket->set.empty()) { // 如果原桶为空，则移除该桶
                remove(bucket);
            }
        }
    }

    // 减少 key 的计数
    void dec(std::string key) {
        if (map.find(key) != map.end()) { // 如果 key 存在
            Bucket* bucket = map[key]; // 获取 key 所在的桶
            if (bucket->cnt == 1) { // 如果计数为 1，则直接移除 key
                map.erase(key);
            } else { // 否则减少计数
                if (bucket->last->cnt == bucket->cnt - 1) { // 如果前一个桶的计数等于当前计数 - 1
                    map[key] = bucket->last; // 将 key 移动到前一个桶
                    bucket->last->set.insert(key);
                } else { // 否则创建一个新的桶，计数为当前计数 - 1
                    Bucket* newBucket = new Bucket(key, bucket->cnt - 1);
                    map[key] = newBucket; // 更新映射
                    insert(bucket->last, newBucket); // 插入新桶
                }
            }
            bucket->set.erase(key); // 从原桶中移除 key
            if (bucket->set.empty()) { // 如果原桶为空，则移除该桶
                remove(bucket);
            }
        }
    }

    // 返回当前计数最大的 key
    std::string getMaxKey() {
        return tail->last->set.empty() ? "" : *tail->last->set.begin(); // 如果最大桶为空，返回空字符串
    }

    // 返回当前计数最小的 key
    std::string getMinKey() {
        return head->next->set.empty() ? "" : *head->next->set.begin(); // 如果最小桶为空，返回空字符串
    }
};


int main() {
    AllOne allOne;

    // 测试 inc 方法
    allOne.inc("a");
    allOne.inc("b");
    allOne.inc("b");
    allOne.inc("c");
    allOne.inc("c");
    allOne.inc("c");
    allOne.inc("c");
    // 测试 getMaxKey 方法
    std::cout << "Max Key: " << allOne.getMaxKey() << std::endl; // 应该输出 "c"

    // 测试 getMinKey 方法
    std::cout << "Min Key: " << allOne.getMinKey() << std::endl; // 应该输出 "a"

    // 测试 dec 方法
    allOne.dec("b");
    allOne.dec("b");
    allOne.dec("c");

    // 再次测试 getMaxKey 和 getMinKey 方法
    std::cout << "Max Key after dec: " << allOne.getMaxKey() << std::endl; // 应该输出 "c"
    std::cout << "Min Key after dec: " << allOne.getMinKey() << std::endl; // 应该输出 "a"

    allOne.dec("c");
    allOne.dec("c");
    allOne.dec("c");

    // 测试 key 被移除的情况
    std::cout << "Max Key after all dec: " << allOne.getMaxKey() << std::endl; // 应该输出 "a"
    std::cout << "Min Key after all dec: " << allOne.getMinKey() << std::endl; // 应该输出 "a"

    allOne.dec("a");

    // 测试所有 key 被移除的情况
    std::cout << "Max Key after all keys dec: " << allOne.getMaxKey() << std::endl; // 应该输出 ""
    std::cout << "Min Key after all keys dec: " << allOne.getMinKey() << std::endl; // 应该输出 ""

    return 0;
}