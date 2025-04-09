#include <vector>
#include <iostream>
using namespace std;
// 被围绕的区域
// 给你一个 m x n 的矩阵 board ，由若干字符 'X' 和 'O' ，找到所有被 'X' 围绕的区域
// 并将这些区域里所有的 'O' 用 'X' 填充。
// 测试链接 : https://leetcode.cn/problems/surrounded-regions/
class Solution {
public:
    // 主函数：解决被'X'围绕的区域问题
    void solve(vector<vector<char>>& board) {
        if (board.empty()) return; // 如果矩阵为空，直接返回
        int n = board.size();      // 获取矩阵的行数
        int m = board[0].size();   // 获取矩阵的列数

        // **第一步：处理边界上的'O'及其相连的'O'**
        // 遍历矩阵的上下边界
        for (int j = 0; j < m; j++) {
            // 如果第一行的某个位置是'O'，进行DFS标记
            if (board[0][j] == 'O') dfs(board, 0, j, n, m);
            // 如果最后一行的某个位置是'O'，进行DFS标记
            if (board[n - 1][j] == 'O') dfs(board, n - 1, j, n, m);
        }

        // 遍历矩阵的左右边界（排除已经处理过的四个角）
        for (int i = 1; i < n - 1; i++) {
            // 如果第一列的某个位置是'O'，进行DFS标记
            if (board[i][0] == 'O') dfs(board, i, 0, n, m);
            // 如果最后一列的某个位置是'O'，进行DFS标记
            if (board[i][m - 1] == 'O') dfs(board, i, m - 1, n, m);
        }

        // **第二步：遍历整个矩阵，进行最终替换**
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                // 将所有未被标记的'O'替换为'X'
                if (board[i][j] == 'O') board[i][j] = 'X';
                // 将所有标记为'F'的位置恢复为'O'
                else if (board[i][j] == 'F') board[i][j] = 'O';
            }
        }
    }

private:
    // 深度优先搜索（DFS）函数：标记与边界相连的'O'
    void dfs(vector<vector<char>>& board, int i, int j, int n, int m) {
        // 边界条件检查：
        // 如果索引越界，或者当前单元格不是'O'，直接返回
        if (i < 0 || i >= n || j < 0 || j >= m || board[i][j] != 'O') {
            return;
        }

        // 将当前单元格标记为'F'，表示它与边界相连
        board[i][j] = 'F';

        // 递归访问上下左右四个方向的相邻单元格
        dfs(board, i + 1, j, n, m); // 下方单元格
        dfs(board, i - 1, j, n, m); // 上方单元格
        dfs(board, i, j + 1, n, m); // 右方单元格
        dfs(board, i, j - 1, n, m); // 左方单元格
    }
};

// main函数：测试solve方法
int main() {
    // 定义一个二维网格（矩阵）
    vector<vector<char>> board = {
        {'X', 'X', 'X', 'X'},
        {'X', 'O', 'O', 'X'},
        {'X', 'X', 'O', 'X'},
        {'X', 'O', 'X', 'X'}
    };

    // 打印原始矩阵
    cout << "Original Board:" << endl;
    for (const auto& row : board) {
        for (char cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }

    // 创建Solution对象并调用solve方法
    Solution solution;
    solution.solve(board);

    // 打印修改后的矩阵
    cout << "\nModified Board:" << endl;
    for (const auto& row : board) {
        for (char cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }

    return 0;
}
