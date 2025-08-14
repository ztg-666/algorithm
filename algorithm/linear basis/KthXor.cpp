#include <bits/stdc++.h>
using namespace std;

const int MAXN = 100001; // 最大数组长度（1e5）
const int BIT = 50;      // 最大二进制位数（0 <= arr[i] <= 2^50）

long long arr[MAXN]; // 存储输入的数组
int len;             // 线性基的长度（消元后的基向量个数）
bool zero;           // 是否存在 0 异或和
int n;               // 数组的元素个数

// 交换两个元素的函数
inline void swapLL(int a, int b) {
    long long tmp = arr[a];
    arr[a] = arr[b];
    arr[b] = tmp;
}

// 高斯消元法构建线性基
void compute() {
    len = 1; // 初始时线性基的长度为 1
    // 从高位到低位依次处理每一位
    for (long long i = BIT; i >= 0; i--) {
        // 找到第一个当前位不为 0 的数，交换到当前的 len 位置
        for (int j = len; j <= n; j++) {
            if ((arr[j] & (1LL << i)) != 0) {
                swapLL(j, len); // 交换
                break;
            }
        }

        // 如果当前 len 位置在该位上有 1，则进行消元操作
        if ((arr[len] & (1LL << i)) != 0) {
            // 用当前 len 位置的数消去其他数相同位上的 1
            for (int j = 1; j <= n; j++) {
                if (j != len && (arr[j] & (1LL << i)) != 0) {
                    arr[j] ^= arr[len]; // 异或消元
                }
            }
            len++; // 线性基长度增加
        }
    }
    len--;                 // 最终的线性基长度
    zero = (len != n);     // 如果 len < n，说明可以得到 0 异或和
}

// 查询第 k 小的异或和
long long query(long long k) {
    // 如果 0 存在且 k=1，返回 0（即第 1 小的异或和是 0）
    if (zero && k == 1) {
        return 0;
    }

    // 如果有 0 异或和，那么实际的 k 小应该减去 1
    if (zero) {
        k--;
    }

    // 如果 k 超出了最大可能的异或和的数量，则返回 -1
    if (k >= (1LL << len)) {
        return -1;
    }

    // 根据 k 的二进制表示，组合线性基中合适的数得到第 k 小的异或和
    long long ans = 0;
    for (int i = len, j = 0; i >= 1; i--, j++) {
        // 如果 k 的二进制表示的第 j 位是 1，则选择 arr[i] 对 ans 进行异或
        if ((k & (1LL << j)) != 0) {
            ans ^= arr[i];
        }
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false); // 提高输入输出效率
    cin.tie(nullptr);            // 解除 cin 和 cout 的绑定，进一步提高效率

    // 读取数组长度 n
    cin >> n;

    // 读取 n 个数
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }

    // 通过高斯消元法构建线性基
    compute();

    // 读取查询次数 q
    int q;
    cin >> q;

    // 进行 q 次查询
    while (q--) {
        long long k;
        cin >> k;
        cout << query(k) << "\n"; // 输出第 k 小的异或和
    }

    return 0;
}
