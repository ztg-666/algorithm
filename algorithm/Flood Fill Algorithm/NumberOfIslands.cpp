#include <vector>
#include <iostream>
using namespace std;
// 岛屿数量
// 给你一个由 '1'（陆地）和 '0'（水）组成的的二维网格，请你计算网格中岛屿的数量
// 岛屿总是被水包围，并且每座岛屿只能由水平方向和/或竖直方向上相邻的陆地连接形成
// 此外，你可以假设该网格的四条边均被水包围
// 测试链接 : https://leetcode.cn/problems/number-of-islands/
class Solution {
public:
    // 主函数：计算二维网格中的岛屿数量
    int numIslands(vector<vector<char>>& board) {
        int n = board.size(); // 获取网格的行数
        if (n == 0) return 0; // 如果网格为空，直接返回0
        int m = board[0].size(); // 获取网格的列数
        int islands = 0; // 初始化岛屿计数器

        // 遍历网格中的每个单元格
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                // 如果当前单元格是陆地（'1'）
                if (board[i][j] == '1') {
                    islands++; // 岛屿计数加1
                    dfs(board, i, j, n, m); // 使用DFS将与该单元格相连的所有陆地标记为已访问
                }
            }
        }
        return islands; // 返回岛屿总数
    }

private:
    // 深度优先搜索（DFS）函数：标记与当前单元格相连的所有陆地
    void dfs(vector<vector<char>>& board, int i, int j, int n, int m) {
        // 边界条件检查：
        // 如果索引越界，或者当前单元格不是陆地（'1'），直接返回
        if (i < 0 || i >= n || j < 0 || j >= m || board[i][j] != '1') {
            return;
        }

        // 将当前单元格标记为已访问（设置为0）
        board[i][j] = 0;

        // 递归访问上下左右四个方向的相邻单元格
        dfs(board, i - 1, j, n, m); // 上方单元格
        dfs(board, i + 1, j, n, m); // 下方单元格
        dfs(board, i, j - 1, n, m); // 左方单元格
        dfs(board, i, j + 1, n, m); // 右方单元格
    }
};


// main函数：测试numIslands方法
int main() {
    // 定义一个二维网格（岛屿地图）
    vector<vector<char>> grid = {
        {'1', '1', '0', '0', '0'},
        {'1', '1', '0', '0', '0'},
        {'0', '0', '1', '0', '0'},
        {'0', '0', '0', '1', '1'}
    };

    // 创建Solution对象并调用numIslands方法
    Solution solution;
    int result = solution.numIslands(grid);

    // 输出结果
    cout << "Number of islands: " << result << endl;

    return 0;
}