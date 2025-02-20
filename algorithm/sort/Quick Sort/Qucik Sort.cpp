#include <bits/stdc++.h>
using namespace std;
// 定义一个分区函数，用于快速排序算法中
// arr: 要排序的数组
// l: 数组的起始索引
// r: 数组的结束索引
auto partition(vector<int> &arr, int l, int r) {
    // less 指向小于基准值的部分的最后一个元素
    int less = l - 1;
    // more 指向大于基准值的部分的第一个元素
    int more = r;
    // 循环直到 l 和 more 相遇
    while (l < more) {
        // 如果当前元素小于基准值
        if (arr[l] < arr[r]) {
            // 将当前元素与 less 的下一个元素交换，并移动 less 指针
            swap(arr[++less], arr[l++]);
        }
        // 如果当前元素大于基准值
        else if (arr[l] > arr[r]) {
            // 将当前元素与 more 的前一个元素交换，并移动 more 指针
            swap(arr[--more], arr[l]);
        }
        // 如果当前元素等于基准值
        else {
            // 只移动 l 指针
            l++;
        }
    }
    // 将基准值放到正确的位置
    swap(arr[more], arr[r]);
    // 返回一个 pair，包含小于基准值部分的结束索引和大于基准值部分的起始索引
    pair<int,int> result = {less + 1, more};
    return result;
}

// 快速排序的递归实现
// arr: 要排序的数组
// l: 数组的起始索引
// r: 数组的结束索引
void quickSort(vector<int>& arr, int l, int r) {
    // 如果起始索引小于结束索引
    if (l < r) {
        // 选择一个随机的基准值，并将其与最后一个元素交换
        int pivotIndex = l + rand() % (r - l + 1);
        swap(arr[pivotIndex], arr[r]);
        // 对数组进行分区
        pair<int,int> p = partition(arr, l, r);
        // 对小于基准值的部分进行递归排序
        quickSort(arr, l, p.first - 1);
        // 对大于基准值的部分进行递归排序
        quickSort(arr, p.second + 1, r);
    }
}

// 快速排序的入口函数
// arr: 要排序的数组
void quickSort(vector<int>& arr) {
    // 如果数组为空或只有一个元素，则不需要排序
    if (arr.empty() || size(arr) < 2) {
        return;
    }
    // 调用递归的快速排序函数
    quickSort(arr, 0, size(arr)-1);
}

