#include <iostream>
#include <vector>
#include <deque>
#include <climits>

using namespace std;

class Solution {
public:
    /**
     * 计算使网格图至少有一条有效路径的最小代价
     * @param grid 二维整数数组，每个格子的值表示移动方向
     * @return 最小代价
     */
    int minCost(vector<vector<int>>& grid) {
        // 定义移动方向数组：右、左、下、上
        vector<vector<int>> move = {
            {}, {0, 1}, {0, -1}, {1, 0}, {-1, 0}
        };
        int m = grid.size();    // 行数
        int n = grid[0].size(); // 列数

        // 初始化距离数组，所有值设为最大值
        vector<vector<int>> distance(m, vector<int>(n, INT_MAX));
        deque<pair<int, int>> q; // 双端队列，用于BFS
        q.emplace_front(0, 0); // 起点入队
        distance[0][0] = 0;               // 起点的距离为0

        // BFS主循环
        while (!q.empty()) {
            auto [x, y] = q.front(); // 获取当前单元格坐标
            q.pop_front();

            // 如果到达终点，返回当前距离
            if (x == m - 1 && y == n - 1) {
                return distance[x][y];
            }

            // 遍历四个方向
            for (int i = 1; i <= 4; i++) {
                int nx = x + move[i][0]; // 新行坐标
                int ny = y + move[i][1]; // 新列坐标
                int weight = (grid[x][y] != i) ? 1 : 0; // 是否需要修改代价

                // 检查新坐标是否合法
                if (nx >= 0 && nx < m && ny >= 0 && ny < n &&
                    distance[x][y] + weight < distance[nx][ny]) {
                    // 更新新坐标的距离
                    distance[nx][ny] = distance[x][y] + weight;

                    // 根据是否需要修改决定插入队列的位置
                    if (weight == 0) {
                        q.emplace_front(nx, ny); // 不需要修改优先处理
                    } else {
                        q.emplace_back(nx, ny); // 需要修改稍后处理
                    }
                }
            }
        }

        return -1; // 如果无法到达终点，返回-1
    }
};
// 使网格图至少有一条有效路径的最小代价
// 给你一个 m * n 的网格图 grid 。 grid 中每个格子都有一个数字
// 对应着从该格子出发下一步走的方向。 grid[i][j] 中的数字可能为以下几种情况：
// 1 ，下一步往右走，也就是你会从 grid[i][j] 走到 grid[i][j + 1]
// 2 ，下一步往左走，也就是你会从 grid[i][j] 走到 grid[i][j - 1]
// 3 ，下一步往下走，也就是你会从 grid[i][j] 走到 grid[i + 1][j]
// 4 ，下一步往上走，也就是你会从 grid[i][j] 走到 grid[i - 1][j]
// 注意网格图中可能会有 无效数字 ，因为它们可能指向 grid 以外的区域
// 一开始，你会从最左上角的格子 (0,0) 出发
// 我们定义一条 有效路径 为从格子 (0,0) 出发，每一步都顺着数字对应方向走
// 最终在最右下角的格子 (m - 1, n - 1) 结束的路径
// 有效路径 不需要是最短路径
// 你可以花费1的代价修改一个格子中的数字，但每个格子中的数字 只能修改一次
// 请你返回让网格图至少有一条有效路径的最小代价
// 测试链接 : https://leetcode.cn/problems/minimum-cost-to-make-at-least-one-valid-path-in-a-grid/
// 测试代码
int main() {
    Solution solution;
    vector<vector<int>> grid = {
        {1, 1, 1, 1},
        {2, 2, 2, 2},
        {1, 1, 1, 1},
        {2, 2, 2, 2}
    };
    cout << "Minimum cost to make at least one valid path: " << solution.minCost(grid) << endl;
    return 0;
}
