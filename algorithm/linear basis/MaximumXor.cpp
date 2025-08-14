#include <iostream>
#include <vector>
#include <algorithm>

// 最大异或和
// 给定一个长度为n的数组arr，arr中都是long类型的非负数，可能有重复值
// 在这些数中选取任意个，使得异或和最大，返回最大的异或和
// 1 <= n <= 50
// 0 <= arr[i] <= 2^50
// 测试链接 : https://www.luogu.com.cn/problem/P3812

using namespace std;

const int MAXN = 51;  // 最大元素个数（题目 n ≤ 50）
const int BIT = 50;   // 最大位数（题目 arr[i] ≤ 2^50）

long long arr[MAXN];       // 存储输入数组
long long basis[BIT + 1];  // 线性基数组，basis[i] 表示最高位是 i 的数
int n;

// 向线性基中插入一个数 num
// 返回值：true 表示线性基新增了一个基向量，false 表示 num 可由已有基向量异或得到
bool insert(long long num) {
    // 从最高位开始判断
    for (int i = BIT; i >= 0; i--) {
        // 如果 num 在第 i 位上是 1
        if ((num >> i) & 1) {
            // 如果该位还没有基向量，则直接放入
            if (basis[i] == 0) {
                basis[i] = num;
                return true; // 新增了一个基向量
            }
            // 否则用该位的基向量消掉 num 的第 i 位
            num ^= basis[i];
        }
    }
    // num 最终变成 0，表示它可以由已有基向量异或得到
    return false;
}

// 计算数组的最大异或和
long long compute() {
    // 1. 构建线性基
    for (int i = 1; i <= n; i++) {
        insert(arr[i]);
    }
    // 2. 贪心法：从最高位往下尝试加上基向量
    long long ans = 0;
    for (int i = BIT; i >= 0; i--) {
        // 如果当前 ans 异或 basis[i] 能变大，就选择它
        ans = max(ans, ans ^ basis[i]);
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false); // 关闭 iostream 与 stdio 同步，加快输入输出
    cin.tie(nullptr);            // 取消 cin 与 cout 的绑定，加快输出

    // 读取 n
    cin >> n;

    // 读取数组
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }

    // 输出最大异或和
    cout << compute() << "\n";

    return 0;
}
