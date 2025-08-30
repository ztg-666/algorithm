#include <bits/stdc++.h>
using namespace std;

// -------------------------
// 完美洗牌算法
// 给定数组 arr，范围 arr[l..r]，长度为偶数 n
// 原始形式: arr[l1,l2,...,lk,r1,r2,...,rk]
// 目标形式: arr[r1,l1,r2,l2,...,rk,lk]
// 要求: 时间复杂度 O(n)，额外空间复杂度 O(1)
// -------------------------

// ---------- 暴力解法 O(n)，用辅助数组（仅用于测试验证） ----------
void shuffle1(vector<int>& arr, int l, int r) {
    int n = r - l + 1;
    vector<int> help(n);   // 辅助数组
    int l1 = l, r1 = (l + r) / 2 + 1;
    int j = 0;
    // 交替拷贝：右 -> 左 -> 右 -> 左
    while (j < n) {
        help[j++] = arr[r1++];
        help[j++] = arr[l1++];
    }
    // 写回 arr
    for (int i = l, k = 0; k < n; i++, k++) {
        arr[i] = help[k];
    }
}

// ---------- 正式解法（完美洗牌算法） ----------
// 辅助数组：记录 3^i 序列
const int MAXN = 20;
int start[MAXN];   // 记录每个环的起始位置
int split_[MAXN];  // 记录每层能处理的长度
int size_;         // 层数

// 数组反转
void reverseVec(vector<int>& arr, int l, int r) {
    while (l < r) swap(arr[l++], arr[r--]);
}

// 旋转函数：把 [l..m] 和 [m+1..r] 的两段对换
void rotate(vector<int>& arr, int l, int m, int r) {
    reverseVec(arr, l, m);     // 翻转左半段
    reverseVec(arr, m + 1, r); // 翻转右半段
    reverseVec(arr, l, r);     // 整体翻转
}

// 下标映射函数：确定当前位置应该去往哪里
int to(int i, int l, int r) {
    if (i <= (l + r) >> 1) {       // 如果在左半区
        return i + (i - l + 1);    // 跳到右边对应位置
    } else {                       // 如果在右半区
        return i - (r - i + 1);    // 跳到左边对应位置
    }
}

// 环置换：把 [l..r] 区间的元素按规则进行置换
void circle(vector<int>& arr, int l, int r, int i) {
    for (int j = 1; j <= i; j++) {         // 处理每一个环
        int init = l + start[j] - 1;       // 当前环的起点
        int cur = init;                    // 当前下标
        int curv = arr[cur];               // 当前值
        int next = to(cur, l, r);          // 下一个位置
        while (next != init) {             // 沿环移动直到回到起点
            int nextv = arr[next];
            arr[next] = curv;              // 把前一个值放到 next
            curv = nextv;                  // 更新当前值
            cur = next;                    // 更新当前位置
            next = to(cur, l, r);          // 找到下一个
        }
        arr[init] = curv;                  // 环结束时写回最后一个值
    }
}

// 构建分割点：split[i] = 3^i*2 - 1 序列
void build(int n) {
    size_ = 0;
    for (int s = 1, p = 2; p <= n; s *= 3, p = s * 3 - 1) {
        start[++size_] = s;   // 环起点位置
        split_[size_] = p;    // 本层能处理的块长度
    }
}

// 完美洗牌主过程
void shuffle2(vector<int>& arr, int l, int r) {
    int n = r - l + 1;   // 当前区间长度
    build(n);            // 构建分割结构
    for (int i = size_; n > 0;) {
        if (split_[i] <= n) { // 如果当前块能处理
            int m = (l + r) / 2;
            // Step1: 旋转，使区间满足置换条件
            rotate(arr, l + split_[i] / 2, m, m + split_[i] / 2);
            // Step2: 做环置换
            circle(arr, l, l + split_[i] - 1, i);
            // 缩小问题规模，继续处理剩余区间
            l += split_[i];
            n -= split_[i];
        } else {
            i--; // 当前块太大，换下一层
        }
    }
}

// ---------- 随机生成与拷贝 ----------
vector<int> randomArray(int n, int v) {
    vector<int> ans(n);
    for (int i = 0; i < n; i++) {
        ans[i] = rand() % v;
    }
    return ans;
}

vector<int> copyArray(const vector<int>& arr) {
    return arr;
}

// ---------- 主函数：对数器 ----------
int main() {
    srand(time(nullptr));
    int n = 10000;    // 数组长度
    int v = 100000;   // 元素最大值
    vector<int> arr1 = randomArray(n, v);
    vector<int> arr2 = copyArray(arr1);
    int test = 50000; // 随机测试次数

    cout << "" << endl;
    for (int i = 1; i <= test; i++) {
        int a = rand() % n;
        int b = rand() % n;
        int l = min(a, b);
        int r = max(a, b);
        if (((r - l + 1) & 1) == 0) { // 必须偶数长度才能洗牌
            shuffle1(arr1, l, r);     // 暴力洗牌
            shuffle2(arr2, l, r);     // 完美洗牌
        }
    }
    for (int i = 0; i < n; i++) {
        if (arr1[i] != arr2[i]) {
            cout << "!" << endl;
            return 0;
        }
    }
    cout << "" << endl;
    return 0;
}
