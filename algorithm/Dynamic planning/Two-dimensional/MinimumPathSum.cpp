#include <vector>
#include <algorithm> // 包含 std::min
#include <limits>    // 包含 numeric_limits
#include<bits/stdc++.h>
// 最小路径和
// 给定一个包含非负整数的 m x n 网格 grid
// 请找出一条从左上角到右下角的路径，使得路径上的数字总和为最小。
// 说明：每次只能向下或者向右移动一步。
// 测试链接 : https://leetcode.cn/problems/minimum-path-sum/
using namespace std;
using ll = long long;
class Solution {

private:
    // 递归方法的辅助函数 (等价于 f1)
    // 计算从 (0,0) 到 (i,j) 的最小路径和
    int solveRecursive(const vector<vector<int>>& grid, int i, int j) {
        // 基本情况：左上角
        if (i == 0 && j == 0) {
            return grid[0][0];
        }

        // 初始化来自上方和左方的路径和为最大值
        // 使用 ll 防止后面加上 grid[i][j] 时发生溢出（如果直接用 INT_MAX 的话）
        ll up_path = INT_MAX;
        ll left_path = INT_MAX;

        // 检查是否可以从上方移动过来
        if (i > 0) {
            up_path = solveRecursive(grid, i - 1, j);
        }
        // 检查是否可以从左方移动过来
        if (j > 0) {
            left_path = solveRecursive(grid, i, j - 1);
        }

        // 当前单元格的值加上来自上方或左方的路径中的较小者
        return grid[i][j] + static_cast<int>(min(up_path, left_path));
    }

    // 带备忘录的递归方法的辅助函数 (等价于 f2)
    // 使用 dp 表存储子问题的结果
    int solveMemoized(const vector<vector<int>>& grid, int i, int j, vector<vector<int>>& dp) {
        // 如果 (i, j) 的结果已经被计算过，直接返回
        if (dp[i][j] != -1) {
            return dp[i][j];
        }

        int ans;
        // 基本情况：左上角
        if (i == 0 && j == 0) {
            ans = grid[0][0];
        } else {
            // 初始化来自上方和左方的路径和为最大值
            ll up_path = INT_MAX;
            ll left_path = INT_MAX;

            // 检查是否可以从上方移动过来
            if (i > 0) {
                up_path = solveMemoized(grid, i - 1, j, dp);
            }
            // 检查是否可以从左方移动过来
            if (j > 0) {
                left_path = solveMemoized(grid, i, j - 1, dp);
            }

            // 计算当前单元格的最小路径和
            ans = grid[i][j] + static_cast<int>(min(up_path, left_path));
        }

        // 将计算结果存入 dp 表并返回
        dp[i][j] = ans;
        return ans;
    }


public:
    // 1. 暴力递归

    int minPathSum1(vector<vector<int>>& grid) {
        if (grid.empty() || grid[0].empty()) {
            return 0; // 或者进行适当的错误处理
        }
        int n = grid.size();
        int m = grid[0].size();
        return solveRecursive(grid, n - 1, m - 1);
    }

    // 2. 记忆化搜索

    int minPathSum2(vector<vector<int>>& grid) {
        if (grid.empty() || grid[0].empty()) {
            return 0;
        }
        int n = grid.size();
        int m = grid[0].size();
        // 初始化 dp 表，所有值为 -1 (表示未计算)
        vector<vector<int>> dp(n, vector<int>(m, -1));
        return solveMemoized(grid, n - 1, m - 1, dp);
    }

    // 3. 严格位置依赖的动态规划
    int minPathSum3(vector<vector<int>>& grid) {
        if (grid.empty() || grid[0].empty()) {
            return 0;
        }
        int n = grid.size();
        int m = grid[0].size();
        // 创建 dp 表
        vector<vector<int>> dp(n, vector<int>(m));

        // 初始化起始单元格
        dp[0][0] = grid[0][0];

        // 初始化第一列 (只能从上方到达)
        for (int i = 1; i < n; ++i) {
            dp[i][0] = dp[i - 1][0] + grid[i][0];
        }

        // 初始化第一行 (只能从左方到达)
        for (int j = 1; j < m; ++j) {
            dp[0][j] = dp[0][j - 1] + grid[0][j];
        }

        // 填充 dp 表的剩余部分
        // dp[i][j] = 到达单元格 (i, j) 的最小路径和
        for (int i = 1; i < n; ++i) {
            for (int j = 1; j < m; ++j) {
                // 从上方 (dp[i-1][j]) 或 左方 (dp[i][j-1]) 到达的路径中的较小值
                dp[i][j] = min(dp[i - 1][j], dp[i][j - 1]) + grid[i][j];
            }
        }

        // 右下角的单元格包含最终的最小路径和
        return dp[n - 1][m - 1];
    }

    // 4. 严格位置依赖的动态规划 + 空间压缩技巧
    int minPathSum4(vector<vector<int>>& grid) {
        if (grid.empty() || grid[0].empty()) {
            return 0;
        }
        int n = grid.size();
        int m = grid[0].size();
        // 使用一个一维 dp 数组，表示当前正在计算的行
        vector<int> dp(m);

        // 根据 grid 的第一行初始化 dp 数组
        dp[0] = grid[0][0];
        for (int j = 1; j < m; ++j) {
            dp[j] = dp[j - 1] + grid[0][j];
        }

        // 遍历剩余的行 (从第二行开始, i=1)
        for (int i = 1; i < n; ++i) {
            // 更新 dp 数组的第一个元素 (dp[0] 代表 dp[i][0])
            // 它只能从上方 (dp[i-1][0], 即旧的 dp[0]) 到达
            dp[0] += grid[i][0];

            // 更新 dp 数组中的其余元素 (j 从 1 到 m-1)
            for (int j = 1; j < m; ++j) {
                // dp[j] 当前存储的是来自上方单元格的值 (dp[i-1][j])
                // dp[j-1] 存储的是来自左方单元格的值 (dp[i][j-1]), 已经是为第 i 行更新过的值
                dp[j] = min(dp[j - 1], dp[j]) + grid[i][j];
            }
        }

        // dp 数组的最后一个元素包含到达右下角单元格的最小路径和
        return dp[m - 1];
    }
};

#include <iostream> // 如果需要输出
int main() {
    Solution sol;
    vector<vector<int>> grid = {{1, 3, 1}, {1, 5, 1}, {4, 2, 1}};
    int minSum1 = sol.minPathSum1(grid); // 递归
    int minSum2 = sol.minPathSum2(grid); // 记忆化搜索
    int minSum3 = sol.minPathSum3(grid); // 二维 DP
    int minSum4 = sol.minPathSum4(grid); // 一维 DP (优化)
    // 输出结果... (例如使用 std::cout)
    std::cout << "递归: " << minSum1 << std::endl;
    std::cout << "记忆化: " << minSum2 << std::endl;
    std::cout << "二维DP: " << minSum3 << std::endl;
    std::cout << "一维DP: " << minSum4 << std::endl;
    return 0;
}
