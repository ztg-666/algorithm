#include <bits/stdc++.h>

// 摇摆排序ii(满足全部进阶要求)
// 给定一个数组arr，重新排列数组，确保满足：arr[0] < arr[1] > arr[2] < arr[3] > ...
// 题目保证输入的数组一定有解，要求时间复杂度O(n)，额外空间复杂度O(1)
// 测试链接 : https://leetcode.cn/problems/wiggle-sort-ii/

using namespace std;

// -------------------- 全局变量 --------------------
int firstIdx, lastIdx;         // 用于 partition（三向切分）边界
const int MAXN = 20;           // 最大支持的分解次数（足够大）
int startArr[MAXN], splitArr[MAXN], sz;  // 洗牌用的参数

// -------------------- 基础工具函数 --------------------
// 交换两个数
void swapVal(int &a, int &b) {
    int tmp = a;
    a = b;
    b = tmp;
}

// 反转数组某个区间
void reverseArr(vector<int>& arr, int l, int r) {
    while (l < r) {
        swapVal(arr[l++], arr[r--]);
    }
}

// -------------------- 随机选择算法 --------------------
/*
   randomizedSelect:
   在无序数组 arr 中找到第 i 位的数（如果排好序）。
   同时对数组进行三向切分：
   - 左边小于 pivot
   - 中间等于 pivot
   - 右边大于 pivot
   时间复杂度 O(n)，空间复杂度 O(1)
*/
int randomizedSelect(vector<int>& arr, int n, int i) {
    int ans = 0;
    for (int l = 0, r = n - 1; l <= r;) {
        // 随机选择一个 pivot（避免退化）
        int pivot = arr[l + rand() % (r - l + 1)];

        // 三向切分
        firstIdx = l;
        lastIdx = r;
        int idx = l;
        while (idx <= lastIdx) {
            if (arr[idx] == pivot) {
                idx++;
            } else if (arr[idx] < pivot) {
                swapVal(arr[firstIdx++], arr[idx++]);
            } else {
                swapVal(arr[idx], arr[lastIdx--]);
            }
        }

        // 根据目标位置 i，缩小搜索范围
        if (i < firstIdx) {
            r = firstIdx - 1;
        } else if (i > lastIdx) {
            l = lastIdx + 1;
        } else {
            ans = arr[i];
            break;
        }
    }
    return ans;
}

// -------------------- 完美洗牌算法 --------------------
/*
   用来把数组重新排列成 wiggle sort 需要的顺序。
   原理：
   - 先通过 build() 找到能够进行完美循环置换的区间
   - 使用 rotate + circle 实现位置映射
*/

// 构造 startArr 和 splitArr，用于后续分解
void build(int n) {
    sz = 0;
    for (int s = 1, p = 2; p <= n; s *= 3, p = s * 3 - 1) {
        startArr[++sz] = s;    // 每个循环的起点
        splitArr[sz] = p;      // 每个循环能处理的长度
    }
}

// 旋转区间 [l..m] 和 [m+1..r]
void rotateArr(vector<int>& arr, int l, int m, int r) {
    reverseArr(arr, l, m);
    reverseArr(arr, m + 1, r);
    reverseArr(arr, l, r);
}

// 计算下一个位置的映射
int to(int i, int l, int r) {
    if (i <= (l + r) >> 1) {
        return i + (i - l + 1);
    } else {
        return i - (r - i + 1);
    }
}

// 把区间 [l..r] 内的数进行循环置换
void circle(vector<int>& arr, int l, int r, int i) {
    for (int j = 1; j <= i; j++) {
        int init = l + startArr[j] - 1;  // 起始点
        int cur = init;
        int next = to(cur, l, r);        // 下一个位置
        int curv = arr[cur];             // 当前值

        // 循环移动
        while (next != init) {
            int nextv = arr[next];
            arr[next] = curv;
            curv = nextv;
            cur = next;
            next = to(cur, l, r);
        }
        arr[init] = curv;
    }
}

// 完美洗牌：递归处理区间
void shuffle(vector<int>& arr, int l, int r) {
    int n = r - l + 1;
    build(n);
    for (int i = sz, m; n > 0;) {
        if (splitArr[i] <= n) {
            m = (l + r) / 2;
            // 先做旋转
            rotateArr(arr, l + splitArr[i] / 2, m, m + splitArr[i] / 2);
            // 再做循环置换
            circle(arr, l, l + splitArr[i] - 1, i);
            // 缩小范围
            l += splitArr[i];
            n -= splitArr[i];
        } else {
            i--;
        }
    }
}

// -------------------- 主函数：Wiggle Sort II --------------------
/*
   思路：
   1. 用 randomizedSelect 找到中位数，把数组三向切分。
   2. 根据数组长度的奇偶性，决定如何洗牌：
      - 偶数：整体 shuffle，然后 reverse
      - 奇数：从索引 1 开始 shuffle
   3. 结果即 wiggle 排序
*/
void wiggleSort(vector<int>& arr) {
    int n = arr.size();
    randomizedSelect(arr, n, n / 2);
    if ((n & 1) == 0) {
        shuffle(arr, 0, n - 1);
        reverseArr(arr, 0, n - 1);
    } else {
        shuffle(arr, 1, n - 1);
    }
}

// -------------------- 测试 --------------------
int main() {
    srand(time(0));
    vector<int> arr = {1, 5, 1, 1, 6, 4};
    wiggleSort(arr);

    // 输出结果
    for (int x : arr) cout << x << " ";
    cout << endl;
    return 0;
}
