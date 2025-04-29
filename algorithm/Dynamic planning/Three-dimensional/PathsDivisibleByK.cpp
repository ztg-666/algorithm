#include <vector>
// 矩阵中和能被 K 整除的路径
// 给一个下标从0开始的 n * m 整数矩阵 grid 和一个整数 k
// 从起点(0,0)出发，每步只能往下或者往右，你想要到达终点(m-1, n-1)
// 请你返回路径和能被 k 整除的路径数目
// 答案对 1000000007 取模
// 测试链接 : https://leetcode.cn/problems/paths-in-matrix-whose-sum-is-divisible-by-k/
class Solution {
private:
    const int MOD = 1000000007;
    int rows, cols, mod_k; // 存储网格维度和 k 值，供辅助函数使用

    // --- 方法 1：普通递归法 ---
    // 递归方法的辅助函数
    // i, j：当前单元格的坐标
    // r：从 (i, j) 到 (n - 1, m - 1) 的路径和所需的余数
    int solve_recursive(const std::vector<std::vector<int>>& grid, int i, int j, int r) {
        // 基本情况：到达目的地
        if (i == rows - 1 && j == cols - 1) {
            // 检查最后一个单元格的值是否满足所需余数 r
            return (grid[i][j] % mod_k == r) ? 1 : 0;
        }

        // 计算从 *下一步* 开始所需的余数
        // 以实现从 (i, j) 开始的路径达到目标余数 'r'。
        // need = (r - grid[i][j]) % k （调整为正结果）
        int current_val_mod = grid[i][j] % mod_k;
        int need = (r - current_val_mod + mod_k) % mod_k; // 确保余数为正

        long long count = 0; // 使用 long long 存储中间和

        // 尝试向下移动
        if (i + 1 < rows) {
            count = (count + solve_recursive(grid, i + 1, j, need)) % MOD;
        }

        // 尝试向右移动
        if (j + 1 < cols) {
            count = (count + solve_recursive(grid, i, j + 1, need)) % MOD;
        }

        return static_cast<int>(count);
    }

    // --- 方法 2：记忆化搜索法（自顶向下动态规划） ---
    std::vector<std::vector<std::vector<int>>> memo;

    // 记忆化搜索方法的辅助函数
    int solve_memo(const std::vector<std::vector<int>>& grid, int i, int j, int r) {
        // 基本情况：到达目的地
        if (i == rows - 1 && j == cols - 1) {
            return (grid[i][j] % mod_k == r) ? 1 : 0;
        }

        // 检查记忆化表
        if (memo[i][j][r] != -1) {
            return memo[i][j][r];
        }

        // 计算下一步所需的余数
        int current_val_mod = grid[i][j] % mod_k;
        int need = (r - current_val_mod + mod_k) % mod_k;

        long long count = 0; // 使用 long long 存储中间和

        // 尝试向下移动
        if (i + 1 < rows) {
            count = (count + solve_memo(grid, i + 1, j, need)) % MOD;
        }

        // 尝试向右移动
        if (j + 1 < cols) {
            count = (count + solve_memo(grid, i, j + 1, need)) % MOD;
        }

        // 在返回之前将结果存储在记忆化表中
        memo[i][j][r] = static_cast<int>(count);
        return memo[i][j][r];
    }

public:
    // --- 递归方法的接口 ---
    int numberOfPaths1(const std::vector<std::vector<int>>& grid, int k) {
        rows = grid.size();
        if (rows == 0) return 0;
        cols = grid[0].size();
        if (cols == 0) return 0;
        mod_k = k;

        // 从 (0, 0) 开始，目标是路径总和的余数为 0
        return solve_recursive(grid, 0, 0, 0);
    }

    // --- 记忆化搜索方法的接口（等同于 Java 中的 numberOfPaths2） ---
    int numberOfPaths2(const std::vector<std::vector<int>>& grid, int k) {
        rows = grid.size();
        if (rows == 0) return 0;
        cols = grid[0].size();
        if (cols == 0) return 0;
        mod_k = k;

        // 用 -1 初始化记忆化表
        memo.assign(rows, std::vector<std::vector<int>>(cols, std::vector<int>(k, -1)));

        // 从 (0, 0) 开始，目标是路径总和的余数为 0
        return solve_memo(grid, 0, 0, 0);
    }

    // --- 方法 3：迭代的自底向上动态规划法（等同于 Java 中的 numberOfPaths3） ---
    int numberOfPaths(const std::vector<std::vector<int>>& grid, int k) {
        int n = grid.size();
        if (n == 0) return 0;
        int m = grid[0].size();
        if (m == 0) return 0;

        // dp[i][j][r]：从 (i, j) 到 (n - 1, m - 1) 的路径数量
        // 使得该路径段的和对 k 取余的余数为 r。
        std::vector<std::vector<std::vector<int>>> dp(
            n, std::vector<std::vector<int>>(m, std::vector<int>(k, 0))
        );

        // 基本情况：目标单元格 (n - 1, m - 1)
        // 从这里到自身有 1 条路径。其和的余数为 grid[n - 1][m - 1] % k。
        dp[n - 1][m - 1][grid[n - 1][m - 1] % k] = 1;

        // 填充最后一列（从下往上，不包括角落）
        for (int i = n - 2; i >= 0; --i) {
            int current_val_mod = grid[i][m - 1] % k;
            for (int r = 0; r < k; ++r) {
                // 要从 (i, m - 1) 开始得到余数 'r'，
                // 从 (i + 1, m - 1) 开始的路径必须有余数 'need'。
                int need = (r - current_val_mod + k) % k;
                dp[i][m - 1][r] = dp[i + 1][m - 1][need];
            }
        }

        // 填充最后一行（从右往左，不包括角落）
        for (int j = m - 2; j >= 0; --j) {
            int current_val_mod = grid[n - 1][j] % k;
            for (int r = 0; r < k; ++r) {
                // 要从 (n - 1, j) 开始得到余数 'r'，
                // 从 (n - 1, j + 1) 开始的路径必须有余数 'need'。
                int need = (r - current_val_mod + k) % k;
                dp[n - 1][j][r] = dp[n - 1][j + 1][need];
            }
        }

        // 填充网格的其余部分（从右下角向左上角）
        for (int i = n - 2; i >= 0; --i) {
            for (int j = m - 2; j >= 0; --j) {
                int current_val_mod = grid[i][j] % k;
                for (int r = 0; r < k; ++r) {
                    // 计算下一步（向下或向右）所需的余数
                    int need = (r - current_val_mod + k) % k;

                    // 获取从下方 (i + 1, j) 来且需要余数 'need' 的路径数量
                    long long paths_down = dp[i + 1][j][need];
                    // 获取从右方 (i, j + 1) 来且需要余数 'need' 的路径数量
                    long long paths_right = dp[i][j + 1][need];

                    // 对路径数量求和并取模
                    dp[i][j][r] = static_cast<int>((paths_down + paths_right) % MOD);
                }
            }
        }

        // 最终答案是从 (0, 0) 到 (n - 1, m - 1) 的路径数量
        // 其总和除以 k 的余数为 0。
        return dp[0][0][0];
    }
};

// 示例用法（可选）

#include <iostream>

int main() {
    Solution sol;
    std::vector<std::vector<int>> grid1 = {{5, 2, 4}, {3, 0, 5}, {0, 7, 2}};
    int k1 = 3;
    std::cout << "Result 1 (DP): " << sol.numberOfPaths(grid1, k1) << std::endl;    // 输出: 2
    std::cout << "Result 1 (Memo): " << sol.numberOfPaths2(grid1, k1) << std::endl;   // 输出: 2
    // std::cout << "Result 1 (Rec): " << sol.numberOfPaths1(grid1, k1) << std::endl;    // 输出: 2（可能较慢）

    std::vector<std::vector<int>> grid2 = {{0, 0}};
    int k2 = 5;
    std::cout << "Result 2 (DP): " << sol.numberOfPaths(grid2, k2) << std::endl;    // 输出: 1
    std::cout << "Result 2 (Memo): " << sol.numberOfPaths2(grid2, k2) << std::endl;   // 输出: 1
    // std::cout << "Result 2 (Rec): " << sol.numberOfPaths1(grid2, k2) << std::endl;    // 输出: 1

    std::vector<std::vector<int>> grid3 = {{7, 3, 4, 9}, {2, 3, 6, 2}, {2, 3, 7, 0}};
    int k3 = 1;
    std::cout << "Result 3 (DP): " << sol.numberOfPaths(grid3, k3) << std::endl;   // 输出: 10（因为 k = 1，所有路径都满足条件）
    std::cout << "Result 3 (Memo): " << sol.numberOfPaths2(grid3, k3) << std::endl;  // 输出: 10
    // std::cout << "Result 3 (Rec): " << sol.numberOfPaths1(grid3, k3) << std::endl;   // 输出: 10

    return 0;
}

