#include <iostream>
#include <vector>
#include <algorithm>

// 离散化函数，返回离散化后的数组和去重排序后的数组
std::pair<std::vector<int>, std::vector<int>> discretize(const std::vector<int>& arr) {
    // 步骤 1：收集数据并去重排序
    std::vector<int> unique_arr = arr;
    std::sort(unique_arr.begin(), unique_arr.end());
    unique_arr.erase(std::unique(unique_arr.begin(), unique_arr.end()), unique_arr.end());

    // 初始化离散化后的数组
    std::vector<int> discrete_arr;
    discrete_arr.reserve(arr.size()); // 预留空间以提高性能

    // 步骤 2：映射
    for (int num : arr) {
        // 使用二分查找找到 num 在 unique_arr 中的位置
        auto it = std::lower_bound(unique_arr.begin(), unique_arr.end(), num);
        // 计算 num 在 unique_arr 中的索引位置
        int pos = std::distance(unique_arr.begin(), it);
        // 将索引位置添加到离散化后的数组中
        discrete_arr.push_back(pos);
    }
    // 返回离散化后的数组和去重排序后的数组
    return {discrete_arr, unique_arr};
}

int main() {
    // 定义一个测试数组
    std::vector<int> arr = {100, 200, 100, 300, 400};

    // 调用离散化函数对 arr 进行离散化处理
    auto [discrete_arr, unique_arr] = discretize(arr);

    // 输出原始数组

    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // 输出离散化后的数组

    for (int num : discrete_arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // 从离散化后的数组重新输出原来的数组
    for (int pos : discrete_arr) {
        std::cout << unique_arr[pos] << " ";
    }
    std::cout << std::endl;

    // 返回 0 表示程序成功结束
    return 0;
}
