#include <iostream>
#include <vector>
#include <cstring>

// 苹果和盘子
// 有m个苹果，认为苹果之间无差别，有n个盘子，认为盘子之间无差别
// 比如5个苹果如果放进3个盘子，那么(1, 3, 1) (1, 1, 3) (3, 1, 1)认为是同一种方法
// 允许有些盘子是空的，返回有多少种放置方法
// 测试链接 : https://www.nowcoder.com/practice/bfd8234bb5e84be0b493656e390bdebf

using namespace std;

// 定义最大苹果数和盘子数的常量
const int MAXM = 11;
const int MAXN = 11;

// 动态规划数组，dp[i][j]表示i个苹果放入j个盘子的方案数
int dp[MAXM][MAXN];

/**
 * 递归函数：计算m个苹果放入n个盘子的方案数
 * @param m 苹果数量
 * @param n 盘子数量
 * @return 放置方案数
 */
int f(int m, int n) {
    // 基础情况1：没有苹果了，只有一种方法（全部盘子为空）
    if (m == 0) {
        return 1;
    }

    // 基础情况2：没有盘子了，但还有苹果，无法放置
    if (n == 0) {
        return 0;
    }

    // 如果已经计算过，直接返回缓存结果
    if (dp[m][n] != -1) {
        return dp[m][n];
    }

    int ans;

    // 如果盘子数量大于苹果数量，多余的盘子必然为空
    // 等价于m个苹果放入m个盘子的方案数
    if (n > m) {
        ans = f(m, m);
    } else {
        // 核心递推关系：分两种情况
        // 1. f(m, n-1): 至少有一个盘子为空的方案数
        // 2. f(m-n, n): 每个盘子至少放一个苹果的方案数
        //    (先每个盘子放一个，剩下m-n个苹果再分配)
        ans = f(m, n - 1) + f(m - n, n);
    }

    // 缓存结果
    dp[m][n] = ans;
    return ans;
}

/**
 * 主计算函数：初始化dp数组并调用递归函数
 * @param m 苹果数量
 * @param n 盘子数量
 * @return 放置方案数
 */
int compute(int m, int n) {
    // 初始化dp数组为-1，表示未计算过
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            dp[i][j] = -1;
        }
    }
    return f(m, n);
}

/**
 * 主函数：读取输入并输出结果
 */
int main() {
    int m, n;  // m个苹果，n个盘子
    cin >> m >> n;
    cout << compute(m, n) << endl;
    return 0;
}
