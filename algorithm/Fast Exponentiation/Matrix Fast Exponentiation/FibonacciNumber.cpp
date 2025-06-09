#include <vector>
using namespace std;

// 普通迭代解法 O(n)
// 参数n: 要计算的斐波那契数列项数
// 返回值: 斐波那契数列第n项
int fib1(int n) {
    if (n == 0) return 0;   // 边界条件处理
    if (n == 1) return 1;   // 边界条件处理
    int lastLast = 0, last = 1; // 初始化前两项 F(0) 和 F(1)
    for (int i = 2, cur; i <= n; i++) {
        cur = lastLast + last;  // 计算当前项
        lastLast = last;        // 更新前前项
        last = cur;             // 更新前项
    }
    return last;
}

// 矩阵乘法辅助函数
// 参数a: m×k矩阵，b: k×n矩阵
// 返回值: m×n矩阵乘积结果
vector<vector<int>> multiply(const vector<vector<int>>& a, const vector<vector<int>>& b) {
    int n = a.size();     // 结果矩阵行数 = a的行数
    int m = b[0].size();  // 结果矩阵列数 = b的列数
    int k = a[0].size();  // 乘积计算次数 = a的列数/b的行数
    vector<vector<int>> ans(n, vector<int>(m, 0)); // 初始化结果矩阵

    // 三重循环实现矩阵乘法
    for (int i = 0; i < n; i++) {       // 遍历结果矩阵行
        for (int j = 0; j < m; j++) {   // 遍历结果矩阵列
            for (int c = 0; c < k; c++) { // 累加对应位置乘积
                ans[i][j] += a[i][c] * b[c][j];
            }
        }
    }
    return ans;
}

// 矩阵快速幂辅助函数
// 参数m: 基底矩阵(必须是方阵), p: 指数
// 返回值: m^p 的矩阵计算结果
vector<vector<int>> power(vector<vector<int>> m, int p) {
    int n = m.size();
    vector<vector<int>> ans(n, vector<int>(n, 0)); // 初始化单位矩阵

    // 创建单位矩阵(矩阵乘法的乘法单位元)
    for (int i = 0; i < n; i++) ans[i][i] = 1;

    // 快速幂算法(二进制分解法)
    for (; p != 0; p >>= 1) {       // 遍历指数的每个二进制位
        if ((p & 1) != 0) {         // 当前二进制位为1时
            ans = multiply(ans, m); // 累乘当前矩阵
        }
        m = multiply(m, m);         // 矩阵平方
    }
    return ans;
}

// 矩阵快速幂解法 O(logn)
// 参数n: 要计算的斐波那契数列项数
// 返回值: 斐波那契数列第n项
int fib2(int n) {
    if (n == 0) return 0; // 边界条件处理
    if (n == 1) return 1; // 边界条件处理

    // 初始状态矩阵 [F(2), F(1)] = [1, 0]
    vector<vector<int>> start = {{1, 0}};
    // 状态转移矩阵 [[1,1],[1,0]]
    vector<vector<int>> base = {{1, 1}, {1, 0}};

    // 计算 base^(n-1) 并与初始状态矩阵相乘
    vector<vector<int>> ans = multiply(start, power(base, n - 1));

    // 结果矩阵的第一个元素即为F(n+1)
    return ans[0][0];
}
