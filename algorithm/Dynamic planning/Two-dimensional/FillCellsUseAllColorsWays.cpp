#include <iostream>
#include <vector>
#include <cstring>
#include <chrono>  // 用于时间测量

using namespace std;

// 最大格子和颜色数量
const int MAXN = 5001;
// 取模数，防止结果溢出
const int MOD = 1000000007;
// 有效涂色问题
// 给定n、m两个参数
// 一共有n个格子，每个格子可以涂上一种颜色，颜色在m种里选
// 当涂满n个格子，并且m种颜色都使用了，叫一种有效方法
// 求一共有多少种有效的涂色方法
// 1 <= n, m <= 5000
// 结果比较大请 % 1000000007 之后返回
// 对数器验证
// 主类：包含暴力解和动态规划解
class FillCellsUseAllColorsWays {
public:
    // 暴力方法：枚举所有填色方案并验证是否有效（适用于小范围）
    static int ways1(int n, int m) {
        vector<int> path(n);            // 当前填色路径
        vector<bool> used(m + 1, false); // 用于统计是否用到所有颜色
        return dfs(path, used, 0, n, m); // 递归枚举
    }

    // 递归方法：path 表示当前涂色方案；used 用于判断颜色是否都使用
    static int dfs(vector<int>& path, vector<bool>& used, int idx, int n, int m) {
        if (idx == n) {  // 所有格子都涂色完成
            fill(used.begin(), used.end(), false);  // 重置使用记录
            int colors = 0;
            for (int c : path) {
                if (!used[c]) {
                    used[c] = true;
                    colors++;
                }
            }
            return colors == m ? 1 : 0;  // 如果恰好用了 m 种颜色，算有效方案
        } else {
            int ans = 0;
            for (int color = 1; color <= m; color++) {
                path[idx] = color;                        // 尝试给当前格子涂上 color
                ans += dfs(path, used, idx + 1, n, m);    // 递归到下一个格子
            }
            return ans;
        }
    }

    // 动态规划方法：适用于大规模 n, m，时间复杂度 O(n * m)
    static int ways2(int n, int m) {
        // dp[i][j] 表示：i 个格子，使用了 j 种颜色的有效方案数
        static int dp[MAXN][MAXN] = {0};

        // 初始化：i 个格子，只用 1 种颜色，有 m 种选择（任选一种）
        for (int i = 1; i <= n; i++) {
            dp[i][1] = m;
        }

        // 填表过程
        for (int i = 2; i <= n; i++) {
            for (int j = 2; j <= m; j++) {
                // 第一部分：第 i 个格子使用已有的 j 种颜色之一
                long sameColor = 1LL * dp[i - 1][j] * j % MOD;
                // 第二部分：第 i 个格子引入第 j 种新颜色
                long newColor = 1LL * dp[i - 1][j - 1] * (m - j + 1) % MOD;
                dp[i][j] = (sameColor + newColor) % MOD;
            }
        }

        return dp[n][m];
    }
};
#include<iostream>
#include <chrono>
int main() {
    // 小规模功能测试：暴力验证 + 正式方法
    cout << "start" << endl;
    int N = 9;
    int M = 9;
    for (int n = 1; n <= N; n++) {
        for (int m = 1; m <= M; m++) {
            int ans1 = FillCellsUseAllColorsWays::ways1(n, m);
            int ans2 = FillCellsUseAllColorsWays::ways2(n, m);
            if (ans1 != ans2) {
                cout << "error! n = " << n << ", m = " << m << endl;
            }
        }
    }
    cout << "end" << endl;

    // 大规模性能测试
    cout << "start" << endl;
    int n = 5000;
    int m = 4877;
    cout << "n: " << n << ", m: " << m << endl;
    auto start = chrono::steady_clock::now();
    int ans = FillCellsUseAllColorsWays::ways2(n, m);
    auto end = chrono::steady_clock::now();
    cout << "result: " << ans << endl;
    cout << "time: "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count()
         << " mm" << endl;
    cout << "end " << endl;

    return 0;
}
