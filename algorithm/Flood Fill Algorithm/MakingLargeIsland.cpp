#include <vector>
#include <iostream>
using namespace std;
// 最大人工岛
// 给你一个大小为 n * n 二进制矩阵 grid 。最多 只能将一格 0 变成 1 。
// 返回执行此操作后，grid 中最大的岛屿面积是多少？
// 岛屿 由一组上、下、左、右四个方向相连的 1 形成
// 测试链接 : https://leetcode.cn/problems/making-a-large-island/
class Solution {
public:
    // 主函数：计算最大人工岛面积
    int largestIsland(vector<vector<int>>& grid) {
        int n = grid.size(); // 获取网格的行数
        if (n == 0) return 0; // 如果网格为空，直接返回0
        int m = grid[0].size(); // 获取网格的列数
        int id = 2; // 岛屿编号从2开始（0是海，1是未标记陆地）

        // **第一步：DFS遍历所有岛屿并标记**
        // 遍历整个网格，对每个未访问的陆地进行DFS，并分配唯一的岛屿编号
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (grid[i][j] == 1) { // 如果当前单元格是未标记的陆地
                    dfs(grid, i, j, id++); // 使用DFS标记该岛屿，并分配唯一编号
                }
            }
        }

        // **第二步：统计每个岛屿的面积**
        vector<int> sizes(id, 0); // 创建一个数组，用于存储每个岛屿的面积
        int ans = 0; // 初始化最大岛屿面积
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (grid[i][j] > 1) { // 如果当前单元格属于某个岛屿
                    ans = max(ans, ++sizes[grid[i][j]]); // 更新该岛屿的面积，并记录最大值
                }
            }
        }

        // **第三步：尝试将每个0变为1，计算合并后的最大面积**
        vector<bool> visited(id, false); // 用于标记相邻岛屿是否已加入计算
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (grid[i][j] == 0) { // 如果当前单元格是海洋
                    // 获取上下左右四个方向的岛屿编号
                    int up = (i > 0) ? grid[i - 1][j] : 0;
                    int down = (i + 1 < n) ? grid[i + 1][j] : 0;
                    int left = (j > 0) ? grid[i][j - 1] : 0;
                    int right = (j + 1 < m) ? grid[i][j + 1] : 0;

                    visited[up] = true; // 标记上方岛屿为已访问
                    int merge = 1 + sizes[up]; // 当前单元格变为1后，面积初始为1

                    // 检查下方岛屿是否已访问，若未访问则将其面积加入合并值
                    if (!visited[down]) {
                        merge += sizes[down];
                        visited[down] = true;
                    }
                    // 检查左方岛屿是否已访问，若未访问则将其面积加入合并值
                    if (!visited[left]) {
                        merge += sizes[left];
                        visited[left] = true;
                    }
                    // 检查右方岛屿是否已访问，若未访问则将其面积加入合并值
                    if (!visited[right]) {
                        merge += sizes[right];
                        visited[right] = true;
                    }

                    ans = max(ans, merge); // 更新最大岛屿面积
                    // 重置visited状态，避免影响后续计算
                    visited[up] = visited[down] = visited[left] = visited[right] = false;
                }
            }
        }
        return ans; // 返回最大岛屿面积
    }

private:
    // 深度优先搜索（DFS）函数：标记岛屿并分配唯一编号
    void dfs(vector<vector<int>>& grid, int i, int j, int id) {
        // 边界条件检查：
        // 如果索引越界，或者当前单元格不是陆地（1），直接返回
        if (i < 0 || i >= grid.size() ||
            j < 0 || j >= grid[0].size() ||
            grid[i][j] != 1) {
            return;
        }
        grid[i][j] = id; // 将当前单元格标记为指定岛屿编号
        // 递归访问上下左右四个方向的相邻单元格
        dfs(grid, i - 1, j, id); // 上方单元格
        dfs(grid, i + 1, j, id); // 下方单元格
        dfs(grid, i, j - 1, id); // 左方单元格
        dfs(grid, i, j + 1, id); // 右方单元格
    }
};
// main函数测试示例
int main() {
    Solution sol;
    vector<vector<int>> grid = {
        {1, 0},
        {0, 1}
    };
    cout << sol.largestIsland(grid) << endl; // 应输出3
    return 0;
}
