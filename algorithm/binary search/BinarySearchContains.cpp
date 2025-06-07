#include <bits/stdc++.h>
using namespace std;
bool exist(const vector<int>& sortedArr, int num) {
    // 如果输入的数组为空，直接返回 false
    if (sortedArr.empty()) {
        return false;
    }

    // 初始化左右指针，L 指向数组的起始位置，R 指向数组的结束位置
    int L = 0;
    int R = sortedArr.size() - 1;
    int mid = 0;

    // 使用二分查找算法，在数组中查找目标值 num
    while (L < R) {
        // 计算中间位置 mid，避免整数溢出
        mid = L + ((R - L) >> 1);

        // 如果中间位置的值等于目标值 num，直接返回 true
        if (sortedArr[mid] == num) {
            return true;
        }

        // 如果中间位置的值大于目标值 num，调整右指针 R 到 mid - 1
        if (sortedArr[mid] > num) {
            R = mid - 1;
        }
        // 如果中间位置的值小于目标值 num，调整左指针 L 到 mid + 1
        else {
            L = mid + 1;
        }
    }

    // 当 L == R 时，跳出循环，检查 sortedArr[L] 是否等于 num
    return sortedArr[L] == num;
}
