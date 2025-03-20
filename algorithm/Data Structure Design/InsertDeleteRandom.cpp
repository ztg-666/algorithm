#include<bits/stdc++.h>
using namespace std;
class RandomizedSet {
public:
    std::unordered_map<int, int> map; // 用于存储值到索引的映射
    std::vector<int> arr;             // 用于存储值的数组

    RandomizedSet() {
        // 构造函数，初始化 map 和 arr
    }

    bool insert(int val) {
        // 如果值已经存在于 map 中，返回 false
        if (map.find(val) != map.end()) {
            return false;
        }
        // 将值插入到数组的末尾，并在 map 中记录该值的索引
        map[val] = arr.size();
        arr.push_back(val);
        return true;
    }

    bool remove(int val) {
        // 如果值不存在于 map 中，返回 false
        if (map.find(val) == map.end()) {
            return false;
        }
        // 获取要删除值的索引
        int valIndex = map[val];
        // 获取数组最后一个元素的值
        int endValue = arr.back();
        // 将最后一个元素的值移动到要删除值的位置
        map[endValue] = valIndex;
        arr[valIndex] = endValue;
        // 从 map 中删除该值
        map.erase(val);
        // 从数组中移除最后一个元素
        arr.pop_back();
        return true;
    }

    int getRandom() {
        // 返回数组中的一个随机元素
        return arr[rand() % arr.size()];
    }
};



int main() {
    RandomizedSet rs;

    // 测试插入操作
    std::cout << "Insert 1: " << (rs.insert(1) ? "true" : "false") << std::endl; // 应该输出 true
    std::cout << "Insert 2: " << (rs.insert(2) ? "true" : "false") << std::endl; // 应该输出 true
    std::cout << "Insert 3: " << (rs.insert(3) ? "true" : "false") << std::endl; // 应该输出 true
    std::cout << "Insert 2 again: " << (rs.insert(2) ? "true" : "false") << std::endl; // 应该输出 false

    // 测试获取随机元素操作
    std::cout << "Random element: " << rs.getRandom() << std::endl; // 输出可能是 1, 2, 或 3

    // 测试删除操作
    std::cout << "Remove 2: " << (rs.remove(2) ? "true" : "false") << std::endl; // 应该输出 true
    std::cout << "Remove 4: " << (rs.remove(4) ? "true" : "false") << std::endl; // 应该输出 false

    // 再次测试获取随机元素操作
    std::cout << "Random element after removal: " << rs.getRandom() << std::endl; // 输出可能是 1 或 3

    return 0;
}