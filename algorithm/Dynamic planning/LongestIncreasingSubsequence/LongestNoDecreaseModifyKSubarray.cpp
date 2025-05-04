#include <iostream>
#include <vector>
#include <algorithm>
// 有一次修改机会的最长不下降子序列
// 给定一个长度为n的数组arr，和一个整数k
// 只有一次机会可以将其中连续的k个数全修改成任意一个值
// 这次机会你可以用也可以不用，请返回最长不下降子序列长度
// 1 <= k, n <= 10^5
// 1 <= arr[i] <= 10^6
// 测试链接 : https://www.luogu.com.cn/problem/P8776
using namespace std;

const int MAXN = 100001; // 定义数组最大长度

int arr[MAXN];          // 存储输入的原始序列
int right_[MAXN];       // right_[i] 表示从 i 到末尾的最长不上升子序列长度
int ends_[MAXN];        // 辅助数组，用于 LIS（最长递增子序列）/LDS（最长递减子序列） 的二分查找优化

int n, k;               // n 表示序列长度，k 表示可以连续保留的元素个数

// 求最长不上升子序列长度的二分查找函数
int bs1(int len, int num) {
    int l = 0, r = len - 1, m, ans = -1;
    while (l <= r) {
        m = (l + r) / 2;
        if (ends_[m] < num) {   // 如果当前值小于 num，则尝试更小的位置
            ans = m;
            r = m - 1;
        } else {
            l = m + 1;
        }
    }
    return ans;
}

// 求最长不下降子序列长度的二分查找函数
int bs2(int len, int num) {
    int l = 0, r = len - 1, m, ans = -1;
    while (l <= r) {
        m = (l + r) / 2;
        if (ends_[m] > num) {   // 如果当前值大于 num，则尝试更小的位置
            ans = m;
            r = m - 1;
        } else {
            l = m + 1;
        }
    }
    return ans;
}

// 构建 `right_` 数组：从右往左构建最长不上升子序列
void right() {
    int len = 0;
    for (int i = n - 1; i >= 0; i--) {
        int find = bs1(len, arr[i]);
        if (find == -1) {
            ends_[len++] = arr[i];
            right_[i] = len;
        } else {
            ends_[find] = arr[i];
            right_[i] = find + 1;
        }
    }
}

// 主逻辑函数
int compute() {
    right();  // 预处理 right_ 数组

    int len = 0;
    int ans = 0;

    // 遍历窗口 [i, j)，其中 j = i + k
    for (int i = 0, j = k; j < n; i++, j++) {
        int find = bs2(len, arr[j]);
        int left = find == -1 ? len : find;
        ans = max(ans, left + k + right_[j]);  // 计算可能的最大子序列长度

        // 更新 ends_ 数组，维护最长不下降子序列
        find = bs2(len, arr[i]);
        if (find == -1) {
            ends_[len++] = arr[i];
        } else {
            ends_[find] = arr[i];
        }
    }

    ans = max(ans, len + k);  // 最终答案可能是完全由前面部分构成的最长子序列
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    while (cin >> n >> k) {
        for (int i = 0; i < n; ++i) {
            cin >> arr[i];  // 输入序列
        }

        if (k >= n) {
            cout << n << "\n";  // 特殊情况处理
        } else {
            cout << compute() << "\n";  // 输出计算结果
        }
    }

    return 0;
}
