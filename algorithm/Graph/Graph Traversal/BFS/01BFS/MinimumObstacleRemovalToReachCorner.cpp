#include <iostream>
#include <vector>
#include <deque>
#include <climits>

using namespace std;

class Solution {
public:
    /**
     * 计算从左上角到右下角需要移除的障碍物的最小数目
     * @param grid 二维整数数组，0 表示空单元格，1 表示障碍物
     * @return 需要移除的障碍物的最小数目
     */
    int minimumObstacles(vector<vector<int>>& grid) {
        // 定义移动方向数组：上、右、下、左
        int move[5] = {-1, 0, 1, 0, -1};
        int m = grid.size();    // 行数
        int n = grid[0].size(); // 列数

        // 初始化距离数组，所有值设为最大值
        vector<vector<int>> distance(m, vector<int>(n, INT_MAX));
        deque<pair<int, int>> deque; // 双端队列，用于BFS
        deque.emplace_front(0, 0);    // 起点入队
        distance[0][0] = 0;          // 起点的距离为0

        // BFS主循环
        while (!deque.empty()) {
            auto [x, y] = deque.front(); // 获取当前单元格坐标
            deque.pop_front();

            // 如果到达终点，返回当前距离
            if (x == m - 1 && y == n - 1) {
                return distance[x][y];
            }

            // 遍历四个方向
            for (int i = 0; i < 4; i++) {
                int nx = x + move[i]; // 新行坐标
                int ny = y + move[i + 1]; // 新列坐标

                // 检查新坐标是否合法
                if (nx >= 0 && nx < m && ny >= 0 && ny < n &&
                    distance[x][y] + grid[nx][ny] < distance[nx][ny]) {
                    // 更新新坐标的距离
                    distance[nx][ny] = distance[x][y] + grid[nx][ny];

                    // 根据是否有障碍物决定插入队列的位置
                    if (grid[nx][ny] == 0) {
                        deque.emplace_front(nx, ny); // 空单元格优先处理
                    } else {
                        deque.emplace_back(nx, ny); // 障碍物稍后处理
                    }
                    }
            }
        }

        return -1; // 如果无法到达终点，返回-1
    }
};
// 到达角落需要移除障碍物的最小数目
// 给你一个下标从 0 开始的二维整数数组 grid ，数组大小为 m x n
// 每个单元格都是两个值之一：
// 0 表示一个 空 单元格，
// 1 表示一个可以移除的 障碍物
// 你可以向上、下、左、右移动，从一个空单元格移动到另一个空单元格。
// 现在你需要从左上角 (0, 0) 移动到右下角 (m - 1, n - 1) 
// 返回需要移除的障碍物的最小数目
// 测试链接 : https://leetcode.cn/problems/minimum-obstacle-removal-to-reach-corner/
// 测试代码
int main() {
    Solution solution;
    vector<vector<int>> grid = {
        {0, 1, 1},
        {1, 1, 0},
        {1, 1, 0}
    };
    cout << "Minimum obstacles to remove: " << solution.minimumObstacles(grid) << endl;
    return 0;
}
