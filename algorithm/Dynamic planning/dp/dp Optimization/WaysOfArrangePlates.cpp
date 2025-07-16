#include <iostream>
#include <vector>
#include <ctime>
using namespace std;

/**
 * 摆盘子问题解决方案
 * 问题描述：有n个盘子，k种菜，所有盘子排成一排，每个盘子只能放一种菜
 * 约束条件：最多连续两个盘子菜品一样，不允许连续三个或更多相同菜品
 * 目标：计算符合条件的摆盘方法数，结果对1000000007取模
 */
class WaysOfArrangePlates {
public:
    static const int MOD = 1000000007;  // 取模常数

    /**
     * 方法一：递归解法（会超时，仅用于理解思路）
     * 时间复杂度：O(2^n)
     * 空间复杂度：O(n)
     */
    static int dp1(int n, int k) {
        if (n == 1) {
            return k;  // 只有一个盘子，可以放k种菜中的任意一种
        }
        // 最后一个位置可以放k种菜中的任意一种
        // f1(n-1, k)表示前n-1个位置最后一个位置与第n个位置不同的方案数
        // f1(n-2, k)表示前n-2个位置最后一个位置与第n个位置不同的方案数
        return (int)(((long long)(f1(n - 1, k) + f1(n - 2, k)) * k) % MOD);
    }

    /**
     * 辅助递归函数：计算i个位置中最后一个位置与第i+1个位置不同的方案数
     * @param i 位置数量
     * @param k 菜品种类数
     * @return 方案数
     */
    static int f1(int i, int k) {
        if (i == 0) {
            return 1;  // 0个位置，只有一种方案（空方案）
        }
        if (i == 1) {
            return k - 1;  // 1个位置，有k-1种选择（不能与下一个位置相同）
        }
        long long p1 = f1(i - 1, k);  // 第i个位置与第i-1个位置不同
        long long p2 = f1(i - 2, k);  // 第i个位置与第i-1个位置相同，但与第i-2个位置不同
        return (int)(((p1 + p2) * (k - 1)) % MOD);
    }

    /**
     * 方法二：动态规划解法
     * 时间复杂度：O(n)
     * 空间复杂度：O(n)
     *
     * 状态定义：dp[i]表示前i+1个位置中，最后一个位置与第i+2个位置不同的方案数
     */
    static int dp2(int n, int k) {
        if (n == 1) {
            return k;  // 只有一个盘子的情况
        }

        vector<int> dp(n);
        dp[0] = 1;      // 第1个位置，只有1种方案
        dp[1] = k - 1;  // 前2个位置，第2个位置有k-1种选择

        // 动态规划递推
        for (int i = 2; i < n; i++) {
            long long p1 = dp[i - 1];  // 第i+1个位置与第i个位置不同
            long long p2 = dp[i - 2];  // 第i+1个位置与第i个位置相同，但与第i-1个位置不同
            dp[i] = (int)(((p1 + p2) * (k - 1)) % MOD);
        }

        // 最终答案：最后一个位置可以放k种菜中的任意一种
        return (int)(((long long)(dp[n - 1] + dp[n - 2]) * k) % MOD);
    }

    /**
     * 方法三：矩阵快速幂优化
     * 时间复杂度：O(log n)
     * 空间复杂度：O(1)
     *
     * 核心思想：将递推关系表示为矩阵乘法，利用快速幂加速计算
     */
    static int dp3(int n, int k) {
        if (n == 1) {
            return k;
        }

        int s = k - 1;  // 不同选择的数量

        // 初始状态矩阵：[dp[0], dp[1]] = [1, k-1]
        vector<vector<int>> start = {{1, s}};

        // 转移矩阵：将dp[i-2], dp[i-1]转换为dp[i-1], dp[i]
        // dp[i] = (dp[i-1] + dp[i-2]) * (k-1)
        vector<vector<int>> base = {{0, s}, {1, s}};

        // 计算base^(n-2)，然后与start相乘
        vector<vector<int>> ans = multiply(start, power(base, n - 2));

        // 最终答案
        return (int)(((long long)(ans[0][0] + ans[0][1]) * k) % MOD);
    }

    /**
     * 矩阵乘法
     * @param a 左矩阵
     * @param b 右矩阵
     * @return 乘积矩阵
     */
    static vector<vector<int>> multiply(const vector<vector<int>>& a, const vector<vector<int>>& b) {
        int n = a.size();       // 左矩阵行数
        int m = b[0].size();    // 右矩阵列数
        int k = a[0].size();    // 左矩阵列数 = 右矩阵行数

        vector<vector<int>> ans(n, vector<int>(m, 0));

        // 标准矩阵乘法
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                for (int c = 0; c < k; c++) {
                    ans[i][j] = (int)(((long long)ans[i][j] + (long long)a[i][c] * b[c][j]) % MOD);
                }
            }
        }
        return ans;
    }

    /**
     * 矩阵快速幂
     * @param m 底数矩阵
     * @param p 指数
     * @return m^p
     */
    static vector<vector<int>> power(vector<vector<int>> m, int p) {
        int n = m.size();

        // 初始化单位矩阵
        vector<vector<int>> ans(n, vector<int>(n, 0));
        for (int i = 0; i < n; i++) {
            ans[i][i] = 1;
        }

        // 快速幂算法
        for (; p != 0; p >>= 1) {
            if ((p & 1) != 0) {  // 如果当前位为1
                ans = multiply(ans, m);
            }
            m = multiply(m, m);  // m = m^2
        }
        return ans;
    }

    /**
     * 暴力方法：用于验证结果的正确性
     * 时间复杂度：O(k^n)
     * 空间复杂度：O(n)
     */
    static int right(int n, int k) {
        vector<int> path(n);  // 存储当前摆盘方案
        return dfs(path, 0, k);
    }

    /**
     * 深度优先搜索：枚举所有可能的摆盘方案
     * @param path 当前摆盘方案
     * @param i 当前处理的位置
     * @param k 菜品种类数
     * @return 从位置i开始的合法方案数
     */
    static int dfs(vector<int>& path, int i, int k) {
        if (i == path.size()) {
            // 到达终点，检查当前方案是否合法
            int len = 1;  // 当前连续相同菜品的长度
            for (int j = 1; j < path.size(); j++) {
                if (path[j - 1] == path[j]) {
                    len++;  // 相同菜品，长度加1
                } else {
                    len = 1;  // 不同菜品，重置长度
                }
                if (len > 2) {
                    return 0;  // 连续相同菜品超过2个，方案不合法
                }
            }
            return len > 2 ? 0 : 1;  // 返回方案是否合法
        } else {
            // 尚未到达终点，继续递归
            int ans = 0;
            for (int j = 0; j < k; j++) {
                path[i] = j;  // 在位置i放置菜品j
                ans += dfs(path, i + 1, k);  // 递归处理下一个位置
            }
            return ans;
        }
    }
};

// 对数器
// 为了验证
int main() {
    cout << "" << endl;
    for (int n = 1; n <= 8; n++) {
        for (int k = 1; k <= 8; k++) {
            int ans1 = WaysOfArrangePlates::dp1(n, k);
            int ans2 = WaysOfArrangePlates::dp2(n, k);
            int ans3 = WaysOfArrangePlates::dp3(n, k);
            int ans4 = WaysOfArrangePlates::right(n, k);
            if (ans1 != ans2 || ans1 != ans3 || ans1 != ans4) {
                cout << "出错了!" << endl;
            }
        }
    }
    cout << "" << endl;
    cout << endl;

    cout << "" << endl;
    int n = 505060123;
    int k = 303060123;
    cout << " : n = " << n << ", k = " << k << endl;

    clock_t start, end;
    start = clock();
    end = clock();
    cout << "() : " << (double)(end - start) / CLOCKS_PER_SEC * 1000 << endl;

    start = clock();
    end = clock();
    cout << "() : " << (double)(end - start) / CLOCKS_PER_SEC * 1000 << endl;
    cout << "" << endl;

    return 0;
}
