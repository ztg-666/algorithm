#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

class Solution {
public:
    /**
     * 计算二维接雨水的最大体积
     * @param height 二维高度图，每个单元的高度
     * @return 最大接雨水体积
     */
    int trapRainWater(vector<vector<int>>& height) {
        // 定义移动方向数组：上、右、下、左
        // 每次移动由两个连续元素表示，例如 {-1, 0} 表示向上移动
        vector<int> move = {-1, 0, 1, 0, -1};
        int n = height.size();    // 矩阵的行数
        int m = height[0].size(); // 矩阵的列数

        // 自定义比较结构体，用于实现小顶堆（优先队列）
        struct Compare {
            bool operator()(const vector<int>& a, const vector<int>& b) const {
                return a[2] > b[2]; // 按水线高度排序，a[2] > b[2] 表示小顶堆
            }
        };

        // 定义优先队列，存储三元组 {行号, 列号, 水线高度}
        priority_queue<vector<int>, vector<vector<int>>, Compare> heap;

        // 访问标记数组，记录每个单元格是否已被访问
        vector<vector<bool>> visited(n, vector<bool>(m, false));

        // 将矩阵的边界单元格加入优先队列，并标记为已访问
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (i == 0 || i == n - 1 || j == 0 || j == m - 1) {
                    // 边界单元格入队，存储格式为 {行号, 列号, 高度}
                    heap.push({i, j, height[i][j]});
                    visited[i][j] = true; // 标记为已访问
                }
            }
        }

        int ans = 0; // 接雨水总量
        while (!heap.empty()) {
            auto record = heap.top(); // 获取当前最小水线高度的单元格
            heap.pop();
            int r = record[0]; // 当前单元格的行号
            int c = record[1]; // 当前单元格的列号
            int w = record[2]; // 当前单元格的水线高度

            // 遍历四个方向（上、右、下、左）
            for (int i = 0; i < 4; i++) {
                int nr = r + move[i];      // 新行号
                int nc = c + move[i + 1];  // 新列号

                // 检查新坐标是否合法且未访问
                if (nr >= 0 && nr < n && nc >= 0 && nc < m && !visited[nr][nc]) {
                    // 更新接雨水量
                    // 如果当前水线高度高于邻居单元格的高度，则可以接雨水
                    ans += max(0, w - height[nr][nc]);

                    // 将邻居单元格加入优先队列，更新水线高度
                    // 新的水线高度为 max(height[nr][nc], w)，即邻居高度和当前水线高度的较大值
                    heap.push({nr, nc, max(height[nr][nc], w)});
                    visited[nr][nc] = true; // 标记邻居单元格为已访问
                }
            }
        }

        return ans; // 返回接雨水总量
    }
};
// 二维接雨水
// 给你一个 m * n 的矩阵，其中的值均为非负整数，代表二维高度图每个单元的高度
// 请计算图中形状最多能接多少体积的雨水。
// 测试链接 : https://leetcode.cn/problems/trapping-rain-water-ii/
// 测试代码
int main() {
    Solution solution;
    // 输入二维高度图
    vector<vector<int>> height = {
        {1, 4, 3, 1, 3, 2},
        {3, 2, 1, 3, 2, 4},
        {2, 3, 3, 2, 3, 1}
    };
    // 输出最大接雨水量
    cout << "Maximum trapped water: " << solution.trapRainWater(height) << endl;
    return 0;
}
