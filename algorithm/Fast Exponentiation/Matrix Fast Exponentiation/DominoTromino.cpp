#include <iostream>
#include <vector>


// 多米诺和托米诺平铺
// 有两种形状的瓷砖，一种是2*1的多米诺形，另一种是形如"L"的托米诺形
// 两种形状都可以旋转，给定整数n，返回可以平铺2*n的面板的方法数量
// 返回对1000000007取模的值
// 测试链接 : https://leetcode.cn/problems/domino-and-tromino-tiling/

using namespace std;

const int MOD = 1000000007; // 定义模数常量

/**
 * 矩阵相乘函数
 * @param a 第一个矩阵，维度为 n x k
 * @param b 第二个矩阵，维度为 k x m
 * @return 矩阵相乘结果，维度为 n x m
 * 每个元素计算时使用 long long 防止乘法溢出
 */
vector<vector<int>> multiply(vector<vector<int>>& a, vector<vector<int>>& b) {
    int n = a.size();
    int m = b[0].size();
    int k = a[0].size();
    vector<vector<int>> ans(n, vector<int>(m)); // 结果矩阵初始化

    // 经典矩阵乘法三重循环实现
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            for (int c = 0; c < k; c++) {
                ans[i][j] = (int)(((long long)a[i][c] * b[c][j] + ans[i][j]) % MOD);
            }
        }
    }
    return ans;
}

/**
 * 矩阵快速幂算法
 * @param m 底数矩阵
 * @param p 指数
 * @return 矩阵的p次幂结果
 * 使用二进制分解法优化计算，时间复杂度O(logp)
 */
vector<vector<int>> matrixPower(vector<vector<int>> m, int p) {
    int n = m.size();
    vector<vector<int>> ans(n, vector<int>(n));
    // 初始化单位矩阵（矩阵乘法中的乘法单位元）
    for (int i = 0; i < n; i++) ans[i][i] = 1;

    // 快速幂核心逻辑：按二进制位分解指数
    while (p > 0) {
        if (p & 1) { // 当前二进制位为1时累乘结果
            ans = multiply(ans, m);
        }
        m = multiply(m, m); // 矩阵平方
        p >>= 1; // 处理下一位
    }
    return ans;
}

/**
 * 计算2xn面板的平铺方法数
 * 使用矩阵快速幂优化递推式：f(n) = 2*f(n-1) + f(n-3)
 */
int numTilings(int n) {
    // 处理前三个基本情况
    if (n == 1) return 1;  // 2x1只能竖放一个多米诺
    if (n == 2) return 2;  // 两个竖放或两个横放
    if (n == 3) return 5;  // 五种组合方式

    // 构造递推矩阵，对应 f(n)=2*f(n-1)+0*f(n-2)+1*f(n-3)
    vector<vector<int>> base = {{2, 1, 0},  // 系数矩阵
                                {0, 0, 1},  // 用于移位保持状态
                                {1, 0, 0}}; // 对应递推关系

    // 初始状态向量 [f(3), f(2), f(1)] = [5, 2, 1]
    vector<vector<int>> start = {{5, 2, 1}};

    // 计算base矩阵的(n-3)次幂
    vector<vector<int>> powerMat = matrixPower(base, n - 3);

    // 初始向量与矩阵幂相乘得到结果
    vector<vector<int>> result = multiply(start, powerMat);

    // 结果矩阵的第一个元素即为所求
    return result[0][0];
}

int main() {
    // 测试1到9的情况，验证算法正确性
    for (int i = 1; i <= 9; i++) {
        cout << "= 2 * " << i << "  : " << numTilings(i) << endl;
    }
    return 0;
}
