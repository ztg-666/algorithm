#include <bits/stdc++.h>
using namespace std;
// 用于调整堆的函数
void heapify(std::vector<int>& arr, int index, int size) {
    int left = index * 2 + 1;  // 计算左子节点的索引
    while (left < size) {
        // 找到左右子节点中较大的那个
        int largest = left + 1 < size && arr[left + 1] > arr[left] ? left + 1 : left;
        // 比较较大的子节点和当前节点，确定最大值的索引
        largest = arr[largest] > arr[index] ? largest : index;
        // 如果最大值的索引是当前节点，说明已经满足堆的性质，退出循环
        if (largest == index) {
            break;
        }
        // 交换当前节点和最大值节点
        std::swap(arr[largest], arr[index]);
        // 更新当前节点索引为最大值节点的索引
        index = largest;
        // 计算新的左子节点索引
        left = index * 2 + 1;
    }
}

// 用于插入元素到堆中的函数
void heapInsert(std::vector<int>& arr, int index) {
    // 当前节点大于其父节点时，交换它们的位置
    while (arr[index] > arr[(index - 1) / 2]) {
        std::swap(arr[index], arr[(index - 1) / 2]);
        // 更新当前节点索引为其父节点的索引
        index = (index - 1) / 2;
    }
}

// 堆排序函数
void heapSort(std::vector<int>& arr) {
    int n = arr.size();  // 获取数组的大小
    if (arr.empty() || n < 2) {
        return;  // 如果数组为空或只有一个元素，直接返回
    }
    // 将数组构建成大根堆
    for (int i = 0; i < n; i++) {
        heapInsert(arr, i);
    }
    int size = n;  // 初始化堆的大小为数组的大小
    // 交换堆顶元素和最后一个元素，并减少堆的大小
    std::swap(arr[0], arr[--size]);
    // 重复调整堆的过程，直到堆的大小为0
    while (size > 0) {
        heapify(arr, 0, size);  // 调整堆，使堆顶元素下沉到正确位置
        std::swap(arr[0], arr[--size]);  // 交换堆顶元素和最后一个元素，并减少堆的大小
    }
}
