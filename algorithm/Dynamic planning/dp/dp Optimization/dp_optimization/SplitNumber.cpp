#include <iostream>
#include <vector>
#include <cstring>

// 数的划分方法
// 将整数m分成n份，且每份不能为空，任意两个方案不相同(不考虑顺序)
// 比如，m=7、n=3，那么(1, 1, 5) (1, 5, 1) (5, 1, 1)认为是同一种方法
// 返回有多少种不同的划分方法
// 测试链接 : https://www.luogu.com.cn/problem/P1025

using namespace std;

// 定义常量：最大的数值范围和最大的份数
const int MAXN = 201;  // 最大数值m的范围
const int MAXK = 7;    // 最大份数n的范围

// 记忆化搜索的dp数组，dp[i][j]表示将数字i分成j份的方案数
int dp[MAXN][MAXK];

/**
 * 递归函数：计算将数字m分成n份的方案数
 * @param m 要分割的数字
 * @param n 要分成的份数
 * @return 返回方案数
 */
int f(int m, int n) {
    // 基础情况1：如果剩余数字为0，说明已经完成分割，返回1种方案
    if (m == 0) {
        return 1;
    }

    // 基础情况2：如果份数为0但还有剩余数字，无法完成分割，返回0种方案
    if (n == 0) {
        return 0;
    }

    // 记忆化搜索：如果已经计算过，直接返回结果
    if (dp[m][n] != -1) {
        return dp[m][n];
    }

    int ans;

    // 如果份数n大于剩余数字m，那么每份最多只能分到1，等价于f(m, m)
    if (n > m) {
        ans = f(m, m);
    } else {
        // 状态转移方程：
        // f(m, n) = f(m, n-1) + f(m-n, n)
        // f(m, n-1)：不使用数字n的方案数（减少一个份数）
        // f(m-n, n)：每份都至少分配1个数字后的方案数
        ans = f(m, n - 1) + f(m - n, n);
    }

    // 将计算结果存入dp数组，实现记忆化
    dp[m][n] = ans;
    return ans;
}

/**
 * 主计算函数：将整数m分成n份的不同方案数
 * @param m 要分割的整数
 * @param n 要分成的份数
 * @return 返回不同的划分方案数
 */
int compute(int m, int n) {
    // 边界情况1：如果m < n，无法将m分成n份（每份至少为1）
    if (m < n) {
        return 0;
    }

    // 边界情况2：如果m == n，只有一种方案（每份都是1）
    if (m == n) {
        return 1;
    }

    // 预处理：由于每份至少为1，先给每份分配1，剩余的数字为m-n
    m -= n;

    // 初始化dp数组为-1，表示还未计算过
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            dp[i][j] = -1;
        }
    }

    // 调用递归函数计算剩余数字m分成n份的方案数
    return f(m, n);
}

/**
 * 主函数：处理输入输出
 * 问题描述：将整数m分成n份，且每份不能为空，任意两个方案不相同(不考虑顺序)
 * 例如：m=7, n=3时，(1,1,5) (1,5,1) (5,1,1)认为是同一种方法
 */
int main() {
    int m, n;

    // 读取输入：m为要分割的数字，n为要分成的份数
    cin >> m >> n;

    // 输出结果：不同的划分方案数
    cout << compute(m, n) << endl;

    return 0;
}
