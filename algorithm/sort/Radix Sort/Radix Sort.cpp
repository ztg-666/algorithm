#include <bits/stdc++.h>
using namespace std;
// 获取数字 x 的第 d 位数字
int getDigit(int x, int d) {
    // 通过将 x 除以 10^(d-1) 并取模 10 来获取第 d 位数字
    return x / static_cast<int>(std::pow(10, d - 1)) % 10;
}

// 找到数组中最大数字的位数
int maxbits(std::vector<int>& arr) {
    int Max = INT_MIN; // 初始化 Max 为最小可能的整数
    for (int i = 0; i < std::size(arr); i++) {
        Max = std::max(Max, arr[i]); // 找到数组中的最大元素
    }
    int res = 0; // 初始化结果以计数位数
    while (Max != 0) {
        res++; // 增加位数计数
        Max /= 10; // 去掉最后一位数字
    }
    return res; // 返回最大元素的位数
}

// 对数组的子部分进行基数排序
void radixSort(std::vector<int>& arr, int begin, int end, int digit) {
    const int radix = 10; // 定义基数（进制），对于十进制数为 10
    int i = 0, j = 0;

    std::vector<int> bucket(end - begin + 1); // 临时数组用于存放排序后的元素
    for (int d = 1; d <= digit; d++) { // 循环遍历每一位数字
        std::vector<int> count(radix, 0); // 计数数组用于存储每个数字的出现次数
        for (i = begin; i <= end; i++) {
            j = getDigit(arr[i], d); // 获取 arr[i] 的第 d 位数字
            count[j]++; // 增加该位数字的计数
        }
        for (i = 1; i < radix; i++) {
            count[i] = count[i] + count[i - 1]; // 将计数转换为累积计数
        }
        for (i = end; i >= begin; i--) {
            j = getDigit(arr[i], d); // 获取 arr[i] 的第 d 位数字
            bucket[count[j] - 1] = arr[i]; // 将 arr[i] 放在 bucket 的正确位置
            count[j]--; // 减少该位数字的计数
        }
        for (i = begin, j = 0; i <= end; i++, j++) {
            arr[i] = bucket[j]; // 将排序后的元素复制回原数组
        }
    }
}

// 对整个数组进行基数排序
void radixSort(std::vector<int>& arr) {
    if (arr.empty() || std::size(arr) < 2) {
        return; // 如果数组为空或长度小于 2，则不需要排序
    }
    radixSort(arr, 0, std::size(arr) - 1, maxbits(arr)); // 调用辅助函数对数组进行排序
}
