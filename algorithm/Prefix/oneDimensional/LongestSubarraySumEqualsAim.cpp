#include <iostream>
#include <unordered_map>
#include <vector>

// 定义数组的最大长度
const int MAXN = 100001;

// 全局变量声明
int arr[MAXN]; // 存储输入的数组
int n, aim;    // n: 数组长度，aim: 目标和

// 哈希表用于存储前缀和及其最早出现的位置
// key: 某个前缀和
// value: 这个前缀和最早出现的位置
std::unordered_map<int, int> map;


// 返回无序数组中累加和为给定值的最长子数组长度
// 给定一个无序数组arr, 其中元素可正、可负、可0
// 给定一个整数aim
// 求arr所有子数组中累加和为aim的最长子数组长度
// 计算满足条件的最长子数组长度
int compute() {
    map.clear(); // 清空哈希表
    // 初始化：前缀和为0时，位置为-1（表示一个数字也没有）
    map[0] = -1;

    int ans = 0; // 存储最终结果
    // 遍历数组，计算前缀和并更新哈希表
    for (int i = 0, sum = 0; i < n; i++) {
        sum += arr[i]; // 更新当前前缀和

        // 如果存在前缀和等于 (sum - aim)，说明找到了一个满足条件的子数组
        if (map.find(sum - aim) != map.end()) {
            // 更新最长子数组的长度
            ans = std::max(ans, i - map[sum - aim]);
        }

        // 如果当前前缀和尚未出现在哈希表中，则记录其位置
        if (map.find(sum) == map.end()) {
            map[sum] = i;
        }
    }
    return ans; // 返回最长子数组的长度
}

int main() {
    // 循环读取输入，直到输入结束
    while (std::cin >> n >> aim) {
        // 读取数组元素
        for (int i = 0; i < n; i++) {
            std::cin >> arr[i];
        }
        // 输出结果
        std::cout << compute() << std::endl;
    }
    return 0; // 程序正常结束
}
