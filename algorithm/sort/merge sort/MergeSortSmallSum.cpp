#include <bits/stdc++.h>
using namespace std;
// 合并两个有序子数组并计算小和
int merge(vector<int>& arr, int l, int m, int r) {
    // 创建一个辅助数组来存储合并后的结果
    vector<int> help(r - l + 1);
    int i = 0; // 辅助数组的索引
    int p1 = l; // 左子数组的起始索引
    int p2 = m + 1; // 右子数组的起始索引
    int res = 0; // 用于存储小和

    // 合并两个子数组，直到其中一个子数组耗尽
    while (p1 <= m && p2 <= r) {
        if (arr[p1] < arr[p2]) {
            // 如果左子数组的当前元素小于右子数组的当前元素
            // 计算小和：arr[p1] 比右子数组中所有剩余元素都小
            res += (r - p2 + 1) * arr[p1];
            // 将 arr[p1] 放入辅助数组，并将 p1 向右移动一位
            help[i++] = arr[p1++];
        } else {
            // 否则，将 arr[p2] 放入辅助数组，并将 p2 向右移动一位
            help[i++] = arr[p2++];
        }
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

    // 返回计算得到的小和
    return res;
}

// 递归归并排序并计算小和
int mergeSort(vector<int>& arr, int l, int r) {
    // 如果左索引等于右索引，说明只有一个元素，返回0，因为没有小和
    if (l == r) {
        return 0;
    }
    // 计算中间索引
    int mid = l + ((r - l) >> 1);
    // 递归地对左子数组进行归并排序并计算小和
    int leftSum = mergeSort(arr, l, mid);
    // 递归地对右子数组进行归并排序并计算小和
    int rightSum = mergeSort(arr, mid + 1, r);
    // 合并两个有序子数组并计算小和
    int mergeSum = merge(arr, l, mid, r);
    // 返回左子数组的小和、右子数组的小和以及合并过程中的小和之和
    return leftSum + rightSum + mergeSum;
}

// 计算小和
int smallSum(vector<int>& arr) {
    // 如果数组为空或只有一个元素，返回0，因为没有小和
    if (arr.empty() || arr.size() < 2) {
        return 0;
    }
    // 调用递归归并排序函数，从整个数组开始排序并计算小和
    return mergeSort(arr, 0, arr.size() - 1);
}
