#include <vector>
#include <iostream>
using namespace std;

const int MAXSIZE = 100001; // 定义最大网格点数量（一维化后的索引范围）
int father[MAXSIZE];       // 并查集的父节点数组，用于记录每个节点的父节点
int cols;                  // 网格的列数，用于二维坐标到一维索引的转换
int sets;                  // 当前连通分量的数量（岛屿数量）

// 将二维坐标 (a, b) 转换为一维索引
int index(int a, int b) {
    return a * cols + b; // 通过行号乘以列数加上列号得到一维索引
}

// 查找节点 i 的根节点，并进行路径压缩
int find(int i) {
    if (i != father[i]) {
        father[i] = find(father[i]); // 路径压缩优化查找效率
    }
    return father[i]; // 返回根节点
}

// 合并两个陆地所在的集合
void unionSet(int a, int b, int c, int d) {
    int fx = find(index(a, b)); // 找到第一个陆地的根节点
    int fy = find(index(c, d)); // 找到第二个陆地的根节点
    if (fx != fy) {             // 如果两个陆地不在同一个集合中
        father[fx] = fy;        // 合并两个集合
        sets--;                 // 连通分量数量减一
    }
}

// 初始化并查集
void build(int n, int m, vector<vector<char>>& board) {
    cols = m; // 设置列数
    sets = 0; // 初始化连通分量数量为 0
    for (int a = 0; a < n; ++a) {
        for (int b = 0; b < m; ++b) {
            if (board[a][b] == '1') { // 如果当前网格点是陆地
                int idx = index(a, b); // 计算其一维索引
                father[idx] = idx;     // 初始化父节点为其自身
                sets++;                // 增加一个连通分量（初始时每个陆地是一个独立的岛屿）
            }
        }
    }
}

// 计算岛屿数量
int numIslands(vector<vector<char>>& board) {
    if (board.empty() || board[0].empty()) return 0; // 如果网格为空，直接返回 0
    int n = board.size();    // 网格的行数
    int m = board[0].size(); // 网格的列数
    build(n, m, board);       // 初始化并查集

    // 遍历每个网格点
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (board[i][j] == '1') { // 如果当前网格点是陆地
                // 检查左边的网格点是否也是陆地
                if (j > 0 && board[i][j - 1] == '1') {
                    unionSet(i, j, i, j - 1); // 合并当前陆地与左边的陆地
                }
                // 检查上方的网格点是否也是陆地
                if (i > 0 && board[i - 1][j] == '1') {
                    unionSet(i, j, i - 1, j); // 合并当前陆地与上方的陆地
                }
            }
        }
    }
    return sets; // 返回最终的岛屿数量
}
// 岛屿数量
// 给你一个由 '1'（陆地）和 '0'（水）组成的的二维网格，请你计算网格中岛屿的数量
// 岛屿总是被水包围，并且每座岛屿只能由水平方向和/或竖直方向上相邻的陆地连接形成
// 此外，你可以假设该网格的四条边均被水包围
// 测试链接 : https://leetcode.cn/problems/number-of-islands/
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    // 测试用例 1：包含一个岛屿
    vector<vector<char>> grid1 = {
        {'1', '1', '1', '1', '0'},
        {'1', '1', '0', '1', '0'},
        {'1', '1', '0', '0', '0'},
        {'0', '0', '0', '0', '0'}
    }; // 输出 1

    // 测试用例 2：包含三个岛屿
    vector<vector<char>> grid2 = {
        {'1', '1', '0', '0', '0'},
        {'1', '1', '0', '0', '0'},
        {'0', '0', '1', '0', '0'},
        {'0', '0', '0', '1', '1'}
    }; // 输出 3

    // 输出测试结果
    cout << numIslands(grid1) << endl; // 输出 1
    cout << numIslands(grid2) << endl; // 输出 3

    return 0;
}
