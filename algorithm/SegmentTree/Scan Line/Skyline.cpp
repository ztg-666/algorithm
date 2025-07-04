#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

// 天际线问题(洛谷测试)
// 测试链接 : https://www.luogu.com.cn/problem/P1904
using namespace std;

const int MAXN = 20001;

// 建筑物结构体
struct Building {
    int start, end, height;  // 起始位置、结束位置、高度
};

Building arr[MAXN];        // 存储所有建筑物
int xsort[MAXN * 3];      // 存储所有关键x坐标点（起始、结束-1、结束）
int height[MAXN];         // 存储每个离散化位置的最大高度

// 大根堆节点结构体
struct HeapNode {
    int height, end;       // 建筑物高度和结束位置
};

HeapNode heap[MAXN];      // 手动实现的大根堆
int heapSize = 0;         // 堆的当前大小

// 检查堆是否为空
bool isEmpty() {
    return heapSize == 0;
}

// 获取堆顶的高度（最大高度）
int peekHeight() {
    return heap[0].height;
}

// 获取堆顶的结束位置
int peekEnd() {
    return heap[0].end;
}

// 向上调整堆（插入元素时使用）
void heapInsert(int i) {
    // 当前节点与父节点比较，如果当前节点高度更大则交换
    while (i > 0 && heap[i].height > heap[(i - 1) / 2].height) {
        swap(heap[i], heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

// 向下调整堆（删除元素时使用）
void heapify(int i) {
    int l = i * 2 + 1;  // 左孩子索引
    while (l < heapSize) {
        // 找出左右孩子中高度更大的那个
        int best = (l + 1 < heapSize && heap[l + 1].height > heap[l].height) ? l + 1 : l;
        // 比较父节点和较大孩子的高度
        best = (heap[best].height > heap[i].height) ? best : i;
        if (best == i) {
            break;  // 堆性质已满足
        }
        swap(heap[best], heap[i]);
        i = best;
        l = i * 2 + 1;
    }
}

// 向堆中插入元素
void push(int h, int e) {
    heap[heapSize].height = h;
    heap[heapSize].end = e;
    heapInsert(heapSize++);
}

// 删除堆顶元素
void poll() {
    swap(heap[0], heap[--heapSize]);  // 将堆顶与最后一个元素交换
    heapify(0);                       // 重新调整堆
}

// 二分查找：在排序后的xsort数组中找到第一个大于等于v的位置
int rak(int n, int v) {
    int ans = 0;
    int l = 1, r = n, mid;
    while (l <= r) {
        mid = (l + r) >> 1;
        if (xsort[mid] >= v) {
            ans = mid;
            r = mid - 1;
        } else {
            l = mid + 1;
        }
    }
    return ans;
}

// 预处理：坐标离散化和排序
int prepare(int n) {
    int size = 0;

    // 收集所有关键x坐标点
    for (int i = 0; i < n; i++) {
        xsort[++size] = arr[i].start;      // 建筑物起始位置
        xsort[++size] = arr[i].end - 1;    // 建筑物结束位置前一个点
        xsort[++size] = arr[i].end;        // 建筑物结束位置
    }

    // 对所有x坐标排序
    sort(xsort + 1, xsort + size + 1);

    // 去重：移除重复的x坐标
    int m = 1;
    for (int i = 2; i <= size; i++) {
        if (xsort[m] != xsort[i]) {
            xsort[++m] = xsort[i];
        }
    }

    // 坐标离散化：将原始坐标转换为离散化后的索引
    for (int i = 0; i < n; i++) {
        arr[i].start = rak(m, arr[i].start);
        arr[i].end = rak(m, arr[i].end - 1);
    }

    // 按起始位置排序建筑物
    sort(arr, arr + n, [](const Building& a, const Building& b) {
        return a.start < b.start;
    });

    // 初始化高度数组
    fill(height + 1, height + m + 1, 0);
    return m;  // 返回离散化后的坐标点数量
}

// 计算天际线：扫描线算法
void compute(int n, int m) {
    // 从左到右扫描每个离散化的x坐标
    for (int i = 1, j = 0; i <= m; i++) {
        // 将所有在当前位置开始的建筑物加入堆中
        for (; j < n && arr[j].start <= i; j++) {
            push(arr[j].height, arr[j].end);
        }

        // 移除所有在当前位置之前就结束的建筑物
        while (!isEmpty() && peekEnd() < i) {
            poll();
        }

        // 记录当前位置的最大高度
        if (!isEmpty()) {
            height[i] = peekHeight();
        }
        // 如果堆为空，height[i]保持为0（已在prepare中初始化）
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 读入建筑物数据
    int n = 0;
    while (cin >> arr[n].start >> arr[n].height >> arr[n].end) {
        n++;
    }

    // 预处理：坐标离散化
    int m = prepare(n);

    // 计算天际线
    compute(n, m);

    // 输出结果：只输出高度发生变化的关键点
    cout << xsort[1] << " " << height[1];
    for (int i = 2, pre = height[1]; i <= m; i++) {
        if (pre != height[i]) {  // 高度发生变化
            cout << " " << xsort[i] << " " << height[i];
        }
        pre = height[i];
    }
    cout << endl;

    return 0;
}
