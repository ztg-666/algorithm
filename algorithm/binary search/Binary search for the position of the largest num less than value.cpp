#include <bits/stdc++.h>
using namespace std;
int nearestIndex(vector<int>& arr, int value) {
    // 初始化左边界和右边界
    int L = 0;
    int R = arr.size() - 1;
    // 初始化结果索引为-1，表示未找到合适值
    int index = -1;

    // 使用二分查找法在有序数组中寻找最接近且不小于value的元素索引
    while (L <= R) {
        // 计算中间索引
        int mid = L + ((R - L) >> 1);

        // 如果中间元素大于或等于目标值，则更新索引，并继续在左半部分查找
        if (arr[mid] >= value) {
            index = mid;
            R = mid - 1;
        } else {
            // 如果中间元素小于目标值，则在右半部分查找
            L = mid + 1;
        }
    }

    // 返回找到的索引，若未找到则返回-1
    return index;
}
