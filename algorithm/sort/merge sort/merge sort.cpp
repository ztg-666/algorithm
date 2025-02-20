#include <bits/stdc++.h>
using namespace std;
// 合并两个有序子数组
void merge(vector<int>& arr, int l, int m, int r) {
    // 创建一个辅助数组来存储合并后的结果
    vector<int> help(r - l + 1);
    int i = 0; // 辅助数组的索引
    int p1 = l; // 左子数组的起始索引
    int p2 = m + 1; // 右子数组的起始索引

    // 合并两个子数组，直到其中一个子数组耗尽
    while (p1 <= m && p2 <= r) {
        // 比较两个子数组的当前元素，将较小的元素放入辅助数组
        help[i++] = arr[p1] < arr[p2] ? arr[p1++] : arr[p2++];
    }

    // 如果左子数组还有剩余元素，将其全部放入辅助数组
    while (p1 <= m) {
        help[i++] = arr[p1++];
    }

    // 如果右子数组还有剩余元素，将其全部放入辅助数组
    while (p2 <= r) {
        help[i++] = arr[p2++];
    }

    // 将辅助数组中的元素复制回原数组
    for (i = 0; i < help.size(); i++) {
        arr[l + i] = help[i];
    }
}

// 递归归并排序
void mergeSort(vector<int>& arr, int l, int r) {
    // 如果左索引等于右索引，说明只有一个元素，不需要排序
    if (l == r) {
        return;
    }
    // 计算中间索引
    int mid = l + ((r - l) >> 1);
    // 对左子数组进行递归排序
    mergeSort(arr, l, mid);
    // 对右子数组进行递归排序
    mergeSort(arr, mid + 1, r);
    // 合并两个有序子数组
    merge(arr, l, mid, r);
}

// 归并排序主函数
void mergeSort(vector<int>& arr) {
    // 如果数组为空或只有一个元素，不需要排序
    if (arr.empty() || arr.size() < 2) {
        return;
    }
    // 调用递归归并排序函数，从整个数组开始排序
    mergeSort(arr, 0, arr.size() - 1);
}
