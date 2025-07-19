#include <iostream>
#include <algorithm>
#include <cstring>

// 方伯伯的玉米田
// 给定一个长度为n的数组arr
// 每次可以选择一个区间[l,r]，区间内的数字都+1，最多执行k次
// 返回执行完成后，最长的不下降子序列长度
// 1 <= n <= 10^4
// 1 <= arr[i] <= 5000
// 2 <= k <= 500
// 测试链接 : https://www.luogu.com.cn/problem/P3287

using namespace std;

// 常量定义
const int MAXN = 10001;  // 数组最大长度
const int MAXK = 501;    // 最大操作次数
const int MAXH = 5500;   // 最大高度值（原始值5000 + 最大操作次数500）

int arr[MAXN];                    // 存储输入数组
int tree[MAXH + 1][MAXK + 1];     // 二维树状数组，用于维护区间最大值
int n, k;                         // n为数组长度，k为最大操作次数

/**
 * 二维树状数组的更新操作
 * @param x 第一维坐标（对应数值大小）
 * @param y 第二维坐标（对应使用的操作次数+1）
 * @param v 要更新的值
 */
void update(int x, int y, int v) {
    // 二维树状数组的更新，使用lowbit操作
    for (int i = x; i <= MAXH; i += i & -i) {           // 第一维：数值维度
        for (int j = y; j <= k + 1; j += j & -j) {      // 第二维：操作次数维度
            tree[i][j] = max(tree[i][j], v);           // 维护最大值
        }
    }
}

/**
 * 二维树状数组的查询操作
 * @param x 第一维查询上界（查询≤x的范围）
 * @param y 第二维查询上界（查询≤y的范围）
 * @return 查询范围内的最大值
 */
int query_max(int x, int y) {
    int ans = 0;
    // 二维树状数组的查询，使用lowbit操作
    for (int i = x; i > 0; i -= i & -i) {              // 第一维：数值维度
        for (int j = y; j > 0; j -= j & -j) {          // 第二维：操作次数维度
            ans = max(ans, tree[i][j]);                // 取最大值
        }
    }
    return ans;
}

/**
 * 核心计算函数
 * 使用动态规划 + 二维树状数组优化
 * dp[i][j] 表示考虑前i个元素，使用j次操作后的最长不下降子序列长度
 * 由于状态转移需要查询前缀最大值，使用树状数组优化查询效率
 */
int compute() {
    int v, dp;

    // 遍历每个位置
    for (int i = 1; i <= n; i++) {
        // 注意：j必须从k到0枚举，防止同一个位置i的不同状态相互影响
        // 这是因为我们在同一轮中既要查询又要更新树状数组
        for (int j = k; j >= 0; j--) {
            v = arr[i] + j;  // 当前元素经过j次操作后的值

            // 查询所有数值≤v且操作次数≤j的状态中的最大长度
            // j+1是因为树状数组下标从1开始，而j从0开始
            dp = query_max(v, j + 1) + 1;

            // 更新树状数组：在位置(v, j+1)处更新值dp
            update(v, j + 1, dp);
        }
    }

    // 返回使用≤k次操作，数值≤MAXH的所有状态中的最大长度
    return query_max(MAXH, k + 1);
}

int main() {
    // C++输入输出优化
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 初始化树状数组为0
    memset(tree, 0, sizeof(tree));

    // 读入数组长度n和最大操作次数k
    cin >> n >> k;

    // 读入数组元素（下标从1开始）
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }

    // 输出结果
    cout << compute() << endl;

    return 0;
}
