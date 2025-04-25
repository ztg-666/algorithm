#include <vector>
#include <algorithm> // 包含 std::max
#include <iostream>  // 可选：用于测试/调试

// 使用标准命名空间以简化代码（可选）
using namespace std;

// 矩阵中的最长递增路径
// 给定一个 m x n 整数矩阵 matrix ，找出其中 最长递增路径 的长度
// 对于每个单元格，你可以往上，下，左，右四个方向移动
// 你 不能 在 对角线 方向上移动或移动到 边界外（即不允许环绕）
// 测试链接 : https://leetcode.cn/problems/longest-increasing-path-in-a-matrix/
class Solution {
private:
    // --- 方法 1: 纯递归 ---
    // 从(i,j)出发，能走出来多长的递增路径，返回最长长度
    int f1(const vector<vector<int>>& grid, int i, int j) {
        int n = grid.size();
        int m = grid[0].size();
        int next_len = 0; // 存储从邻居出发的最长路径长度

        // 检查四个方向的邻居
        // 向上
        if (i > 0 && grid[i][j] < grid[i - 1][j]) {
            next_len = max(next_len, f1(grid, i - 1, j));
        }
        // 向下
        if (i + 1 < n && grid[i][j] < grid[i + 1][j]) {
            next_len = max(next_len, f1(grid, i + 1, j));
        }
        // 向左
        if (j > 0 && grid[i][j] < grid[i][j - 1]) {
            next_len = max(next_len, f1(grid, i, j - 1));
        }
        // 向右
        if (j + 1 < m && grid[i][j] < grid[i][j + 1]) {
            next_len = max(next_len, f1(grid, i, j + 1));
        }

        // 当前路径长度 = 1 (当前格子) + 从满足条件的邻居出发的最长路径
        return next_len + 1;
    }

    // --- 方法 2: 记忆化搜索 ---
    // 使用 dp 表存储从 (i,j) 出发的最长递增路径长度
    // dp[i][j] = 0 表示未计算，> 0 表示已计算的结果
    int f2(const vector<vector<int>>& grid, int i, int j, vector<vector<int>>& dp) {
        // 如果已经计算过，直接返回结果
        if (dp[i][j] != 0) {
            return dp[i][j];
        }

        int n = grid.size();
        int m = grid[0].size();
        int next_len = 0; // 存储从邻居出发的最长路径长度

        // 检查四个方向的邻居
        // 向上
        if (i > 0 && grid[i][j] < grid[i - 1][j]) {
            next_len = max(next_len, f2(grid, i - 1, j, dp));
        }
        // 向下
        if (i + 1 < n && grid[i][j] < grid[i + 1][j]) {
            next_len = max(next_len, f2(grid, i + 1, j, dp));
        }
        // 向左
        if (j > 0 && grid[i][j] < grid[i][j - 1]) {
            next_len = max(next_len, f2(grid, i, j - 1, dp));
        }
        // 向右
        if (j + 1 < m && grid[i][j] < grid[i][j + 1]) {
            next_len = max(next_len, f2(grid, i, j + 1, dp));
        }

        // 计算结果并存入 dp 表
        dp[i][j] = next_len + 1;
        return dp[i][j];
    }

public:
    // 方法 1 的入口函数
    int longestIncreasingPath1(const vector<vector<int>>& grid) {
        if (grid.empty() || grid[0].empty()) {
            return 0; // 处理空网格或无效网格
        }
        int n = grid.size();
        int m = grid[0].size();
        int ans = 0;

        // 遍历每个单元格作为起点
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                ans = max(ans, f1(grid, i, j));
            }
        }
        return ans;
    }

    // 方法 2 的入口函数
    int longestIncreasingPath2(const vector<vector<int>>& grid) {
        if (grid.empty() || grid[0].empty()) {
            return 0; // 处理空网格或无效网格
        }
        int n = grid.size();
        int m = grid[0].size();
        // 初始化 dp 表，大小为 n x m，所有值为 0 (表示未计算)
        vector<vector<int>> dp(n, vector<int>(m, 0));
        int ans = 0;

        // 遍历每个单元格作为起点，进行记忆化搜索
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                ans = max(ans, f2(grid, i, j, dp));
            }
        }
        return ans;
    }
};

// 示例用法（可选）
int main() {
    Solution sol;
    vector<vector<int>> matrix1 = {{9, 9, 4},
                                   {6, 6, 8},
                                   {2, 1, 1}};
    vector<vector<int>> matrix2 = {{3, 4, 5},
                                   {3, 2, 6},
                                   {2, 2, 1}};
     vector<vector<int>> matrix3 = {{1}};

    cout << "矩阵 1:" << endl;
    cout << "方法 1 (递归): " << sol.longestIncreasingPath1(matrix1) << endl; // 4
    cout << "方法 2 (记忆化): " << sol.longestIncreasingPath2(matrix1) << endl; // 4

    cout << "\n矩阵 2:" << endl;
    // cout << "方法 1 (递归): " << sol.longestIncreasingPath1(matrix2) << endl; // 可能较慢
    cout << "方法 2 (记忆化): " << sol.longestIncreasingPath2(matrix2) << endl; // 4

     cout << "\n矩阵 3:" << endl;
    cout << "方法 2 (记忆化): " << sol.longestIncreasingPath2(matrix3) << endl; // 1


    return 0;
}
