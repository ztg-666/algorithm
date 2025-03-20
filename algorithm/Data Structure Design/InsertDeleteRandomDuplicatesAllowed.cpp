#include <bits/stdc++.h>
using namespace std;

// 定义一个支持重复元素的随机集合类 RandomizedCollection
class RandomizedCollection {
private:
    std::unordered_map<int, std::unordered_set<int>> map; // 用于存储每个值对应的索引集合
    std::vector<int> arr; // 用于存储所有插入的值，便于随机访问

public:
    // 构造函数
    RandomizedCollection() {}

    // 插入一个值到集合中
    bool insert(int val) {
        // 将值添加到数组末尾
        arr.push_back(val);
        // 将该值的索引加入 map 中对应的集合
        map[val].insert(arr.size() - 1);
        // 如果插入后该值的索引集合大小为 1，说明这是第一次插入该值，返回 true
        return map[val].size() == 1;
    }

    // 从集合中移除一个值
    bool remove(int val) {
        // 如果集合中不存在该值，直接返回 false
        if (map.find(val) == map.end()) {
            return false;
        }

        // 获取该值的任意一个索引
        int valAnyIndex = *map[val].begin();
        // 获取数组末尾的值
        int endValue = arr.back();

        // 如果要删除的值正好是数组末尾的值
        if (val == endValue) {
            // 从 map 中移除该值对应的一个索引
            map[val].erase(arr.size() - 1);
        } else {
            // 否则，将数组末尾的值移动到要删除值的位置
            map[endValue].insert(valAnyIndex); // 更新末尾值的索引
            arr[valAnyIndex] = endValue; // 修改数组中的值
            // 移除末尾值原来的索引
            map[endValue].erase(arr.size() - 1);
            // 移除要删除值的一个索引
            map[val].erase(valAnyIndex);
        }

        // 弹出数组末尾的值
        arr.pop_back();

        // 如果该值的所有索引都被移除，则从 map 中删除该值
        if (map[val].empty()) {
            map.erase(val);
        }

        // 返回 true 表示删除成功
        return true;
    }

    // 随机返回集合中的一个值
    int getRandom() {
        // 使用 rand() 函数生成一个随机索引，返回数组中对应位置的值
        return arr[rand() % arr.size()];
    }
};


int main() {
    RandomizedCollection rc;

    // 插入元素
    std::cout << "Insert 1: " << (rc.insert(1) ? "true" : "false") << std::endl;
    std::cout << "Insert 1: " << (rc.insert(1) ? "true" : "false") << std::endl;
    std::cout << "Insert 2: " << (rc.insert(2) ? "true" : "false") << std::endl;

    // 随机获取元素
    std::cout << "Random: " << rc.getRandom() << std::endl;
    std::cout << "Random: " << rc.getRandom() << std::endl;
    std::cout << "Random: " << rc.getRandom() << std::endl;

    // 删除元素
    std::cout << "Remove 1: " << (rc.remove(1) ? "true" : "false") << std::endl;
    std::cout << "Remove 2: " << (rc.remove(2) ? "true" : "false") << std::endl;
    std::cout << "Remove 2: " << (rc.remove(2) ? "true" : "false") << std::endl;

    // 再次随机获取元素
    std::cout << "Random: " << rc.getRandom() << std::endl;

    return 0;
}