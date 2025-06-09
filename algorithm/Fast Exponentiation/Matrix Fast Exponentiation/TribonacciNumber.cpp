#include <vector>
using namespace std;

// 第n个泰波那契数
// t(0) = 0, t(1) = 1, t(2) = 1
// t(i) = t(i-1) + t(i-2) + t(i-3)
// 求t(n)
// 测试链接 : https://leetcode.cn/problems/n-th-tribonacci-number/

using ll = long long; // 使用长整型防止大数计算溢出

/**
 * 矩阵乘法函数
 * @param a 第一个矩阵，维度为 n x k
 * @param b 第二个矩阵，维度为 k x m
 * @return 矩阵乘积结果，维度为 n x m
 * 时间复杂度：O(n*k*m)，对于3x3矩阵为O(1)常数时间
 */
vector<vector<ll>> multiply(const vector<vector<ll>>& a, const vector<vector<ll>>& b) {
    int n = a.size();
    int m = b[0].size();
    int k = a[0].size();
    vector<vector<ll>> ans(n, vector<ll>(m, 0));
    // 三重循环实现标准矩阵乘法
    for (int i = 0; i < n; i++) {      // 遍历第一个矩阵的行
        for (int j = 0; j < m; j++) {  // 遍历第二个矩阵的列
            for (int c = 0; c < k; c++) { // 计算点积
                ans[i][j] += a[i][c] * b[c][j];
            }
        }
    }
    return ans;
}

/**
 * 矩阵快速幂算法
 * @param m 基底矩阵，必须是方阵
 * @param p 指数
 * @return 矩阵的p次幂结果
 * 时间复杂度：O(log p) 次矩阵乘法
 */
vector<vector<ll>> power(vector<vector<ll>> m, int p) {
    int n = m.size();
    vector<vector<ll>> ans(n, vector<ll>(n, 0));
    // 初始化单位矩阵（矩阵乘法的乘法单位元）
    for (int i = 0; i < n; i++) {
        ans[i][i] = 1;
    }
    // 快速幂分解：将p分解为二进制位，每次处理一位
    while (p != 0) {
        if ((p & 1) != 0) {  // 当前二进制位为1时累积结果
            ans = multiply(ans, m);
        }
        m = multiply(m, m);  // 矩阵平方
        p >>= 1;             // 右移处理下一位
    }
    return ans;
}

/**
 * 使用矩阵快速幂计算第n个泰波那契数
 * 泰波那契数列定义：
 * t(0) = 0, t(1) = 1, t(2) = 1
 * t(n) = t(n-1) + t(n-2) + t(n-3) (n >= 3)
 * 时间复杂度：O(log n)
 * 空间复杂度：O(1)（固定大小的矩阵运算）
 * @param n 要计算的数列位置
 * @return 第n个泰波那契数
 */
ll tribonacci(int n) {
    // 处理边界条件
    if (n == 0) return 0;
    if (n == 1 || n == 2) return 1;

    // 初始状态向量 [t(2), t(1), t(0)] = [1, 1, 0]
    vector<vector<ll>> start = {{1, 1, 0}};

    /* 递推矩阵设计（关键部分）
     * 矩阵构造原理：
     * [ t(n)   ]   = [[1,1,0]] [t(n-1)]
     * [ t(n-1) ]     [[1,0,1]] [t(n-2)]
     * [ t(n-2) ]     [[1,0,0]] [t(n-3)]
     * 该矩阵可以将状态向量向前推进一位
     */
    vector<vector<ll>> base = {
        {1, 1, 0},  // 计算新的t(n)
        {1, 0, 1},  // 保存前一个t(n-1)
        {1, 0, 0}   // 保存前前一个t(n-2)
    };

    // 计算矩阵幂：start * (base)^(n-2)
    vector<vector<ll>> ans = multiply(start, power(base, n - 2));

    // 结果矩阵的第一个元素即为t(n)
    return ans[0][0];
}
