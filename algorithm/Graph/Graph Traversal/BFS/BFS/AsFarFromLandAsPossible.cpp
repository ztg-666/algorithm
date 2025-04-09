#include <vector>
#include <queue>
using namespace std;
// 地图分析
// 你现在手里有一份大小为 n x n 的 网格 grid
// 上面的每个 单元格 都用 0 和 1 标记好了其中 0 代表海洋，1 代表陆地。
// 请你找出一个海洋单元格，这个海洋单元格到离它最近的陆地单元格的距离是最大的
// 并返回该距离。如果网格上只有陆地或者海洋，请返回 -1。
// 我们这里说的距离是「曼哈顿距离」（ Manhattan Distance）：
// (x0, y0) 和 (x1, y1) 这两个单元格之间的距离是 |x0 - x1| + |y0 - y1| 。
// 测试链接 : https://leetcode.cn/problems/as-far-from-land-as-possible/
int maxDistance(vector<vector<int>>& grid) {
    int n = grid.size(); // 网格的行数
    int m = grid[0].size(); // 网格的列数
    queue<pair<int, int>> q; // 用于BFS的队列，存储坐标 (x, y)
    vector<vector<bool>> visited(n, vector<bool>(m, false)); // 标记是否访问过
    int seas = 0; // 统计海洋单元格的数量

    // 遍历整个网格
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (grid[i][j] == 1) { // 如果是陆地
                q.push({i, j}); // 将陆地坐标加入队列
                visited[i][j] = true; // 标记为已访问
            } else { // 如果是海洋
                ++seas; // 统计海洋数量
            }
        }
    }

    // 如果全是陆地或全是海洋，则直接返回 -1
    if (seas == 0 || seas == n * m) return -1;

    int level = 0; // 记录当前扩展的层数（距离）
    vector<int> dirs = {-1, 0, 1, 0, -1}; // 方向数组：上、右、下、左

    // 开始多源BFS扩展
    while (!q.empty()) {
        int size = q.size(); // 当前层的节点数量
        ++level; // 每处理完一层，距离+1
        for (int k = 0; k < size; ++k) { // 遍历当前层的所有节点
            auto [x, y] = q.front(); q.pop(); // 取出队首节点
            for (int i = 0; i < 4; ++i) { // 遍历四个方向
                int nx = x + dirs[i]; // 新的行坐标
                int ny = y + dirs[i + 1]; // 新的列坐标
                // 检查新坐标是否合法且未访问
                if (nx >= 0 && nx < n && ny >= 0 && ny < m && !visited[nx][ny]) {
                    visited[nx][ny] = true; // 标记为已访问
                    q.push({nx, ny}); // 将新坐标加入队列
                }
            }
        }
    }

    return level - 1; // 最后一层没有扩展，因此需要减去1
}
#include<iostream>
// main 函数
int main() {
    // 示例输入网格
    vector<vector<int>> grid = {
        {1, 0, 1},
        {0, 0, 0},
        {1, 0, 1}
    };

    // 调用 maxDistance 函数并输出结果
    int result = maxDistance(grid);
    cout << "maxDistance: " << result << endl;

    // 测试其他案例
    vector<vector<int>> grid2 = {
        {1, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    cout << "maxDistance: " << maxDistance(grid2) << endl;

    vector<vector<int>> grid3 = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    };
    cout << "maxDistance: " << maxDistance(grid3) << endl;

    vector<vector<int>> grid4 = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };
    cout << "maxDistance: " << maxDistance(grid4) << endl;

    return 0;
}