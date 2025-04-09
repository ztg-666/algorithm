#include <vector>
#include <iostream>
using namespace std;

// 打砖块
// 有一个 m * n 的二元网格 grid ，其中 1 表示砖块，0 表示空白
// 砖块 稳定（不会掉落）的前提是：
// 一块砖直接连接到网格的顶部，或者
// 至少有一块相邻（4 个方向之一）砖块 稳定 不会掉落时
// 给你一个数组 hits ，这是需要依次消除砖块的位置
// 每当消除 hits[i] = (rowi, coli) 位置上的砖块时，对应位置的砖块（若存在）会消失
// 然后其他的砖块可能因为这一消除操作而 掉落
// 一旦砖块掉落，它会 立即 从网格 grid 中消失（即，它不会落在其他稳定的砖块上）
// 返回一个数组 result ，其中 result[i] 表示第 i 次消除操作对应掉落的砖块数目。
// 注意，消除可能指向是没有砖块的空白位置，如果发生这种情况，则没有砖块掉落。
// 测试链接 : https://leetcode.cn/problems/bricks-falling-when-hit/
class Solution {
private:
    int n, m; // 网格的行数和列数
    vector<vector<int>> grid; // 网格数据
    const int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // 定义四个方向（上、下、左、右）

    // 深度优先搜索（DFS）函数：标记稳定砖块并统计新增的稳定砖块数量
    int dfs(int i, int j) {
        // 边界条件检查：
        // 如果索引越界，或者当前单元格不是未稳定的砖块（值为1），直接返回0
        if (i < 0 || i >= n || j < 0 || j >= m || grid[i][j] != 1)
            return 0;
        grid[i][j] = 2; // 将当前砖块标记为稳定（值为2）
        int count = 1; // 初始化计数器，包含当前砖块本身
        // 遍历四个方向，递归访问相邻砖块
        for (const auto& dir : directions) {
            count += dfs(i + dir[0], j + dir[1]);
        }
        return count; // 返回新增的稳定砖块数量
    }

    // 判断砖块是否可能成为稳定砖块
    bool isStable(int row, int col) {
        // 如果当前单元格不是未稳定的砖块（值为1），直接返回false
        if (grid[row][col] != 1) return false;
        // 如果砖块位于顶部，则直接稳定
        if (row == 0) return true;
        // 检查四个方向，判断是否有相邻的稳定砖块（值为2）
        for (const auto& dir : directions) {
            int x = row + dir[0], y = col + dir[1];
            if (x >= 0 && x < n && y >= 0 && y < m && grid[x][y] == 2)
                return true;
        }
        return false; // 如果没有相邻的稳定砖块，返回false
    }

public:
    // 主函数：计算每次消除操作后掉落的砖块数量
    vector<int> hitBricks(vector<vector<int>>& g, vector<vector<int>>& hits) {
        n = g.size(); // 获取网格的行数
        if (n == 0) return {}; // 如果网格为空，直接返回空结果
        m = g[0].size(); // 获取网格的列数
        grid = g; // 初始化网格
        vector<int> result(hits.size(), 0); // 结果数组，用于存储每次消除操作后掉落的砖块数量

        // **第一步：预处理，模拟所有击打操作**
        // 将所有被击中的砖块标记为0（已消除）或-1（无效击打位置）
        for (const auto& hit : hits) {
            if (grid[hit[0]][hit[1]] == 1) // 如果当前位置是砖块
                grid[hit[0]][hit[1]] = 0; // 标记为已消除
            else
                grid[hit[0]][hit[1]] = -1; // 标记为无效击打位置
        }

        // **第二步：标记最终稳定砖块**
        // 从顶部开始，使用DFS标记所有最终稳定的砖块（值为2）
        for (int j = 0; j < m; ++j) {
            dfs(0, j); // 对第一行的每个单元格进行DFS
        }

        // **第三步：逆向处理击打操作**
        // 从最后一个击打操作开始逆向恢复砖块，并计算掉落的砖块数量
        for (int i = hits.size() - 1; i >= 0; --i) {
            int row = hits[i][0], col = hits[i][1]; // 当前击打位置
            if (grid[row][col] == -1) { // 如果原位置没有砖块（无效击打）
                grid[row][col] = 0; // 恢复为0
                continue; // 跳过本次循环
            }

            grid[row][col] = 1; // 恢复砖块（值为1）
            if (isStable(row, col)) { // 如果该砖块可能成为稳定砖块
                int added = dfs(row, col) - 1; // 使用DFS标记新增的稳定砖块，减去恢复的砖块本身
                result[i] = added; // 记录掉落的砖块数量
            }
        }

        return result; // 返回结果数组
    }
};


// 测试主函数
int main() {
    Solution sol;
    vector<vector<int>> grid = {{1,0,1}, {1,1,1}};
    vector<vector<int>> hits = {{1,0}, {1,1}, {1,2}};
    vector<int> res = sol.hitBricks(grid, hits);
    // 预期输出：[0, 3, 0]
    for (int num : res) cout << num << " ";
    return 0;
}