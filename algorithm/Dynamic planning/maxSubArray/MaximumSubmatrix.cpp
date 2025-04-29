#include <vector>
#include <limits>   // 需使用std::numeric_limits时所需
#include <algorithm> // 需使用std::max时所需（不过在此处并非严格必要）

// 子矩阵最大累加和问题
// 给定一个二维数组grid，找到其中子矩阵的最大累加和
// 返回拥有最大累加和的子矩阵左上角和右下角坐标
// 如果有多个子矩阵都有最大累加和，返回哪一个都可以
// 测试链接 : https://leetcode.cn/problems/max-submatrix-lcci/
class Solution {
public:
    /**
     * @brief 在给定的二维网格中找到和最大的子矩阵。
     *
     * 此函数接受一个表示网格的二维整数向量，并返回一个向量，其中包含和最大的子矩阵的左上角（row1, col1）和右下角（row2, col2）坐标。
     *
     * 该算法遍历所有可能的顶行和底行对，通过对列求和将这些行之间的子网格压缩为一个一维数组，然后对该一维数组应用卡丹算法（Kadane's algorithm），以找到最大子数组和，该和对应于所选行的最大子矩阵和。
     *
     * @param grid 输入的二维整数向量（矩阵）。
     * @return 一个大小为4的std::vector<int>：{row1, col1, row2, col2}。
     * 如果输入网格为空，则返回一个空向量。
     */
    std::vector<int> getMaxMatrix(std::vector<std::vector<int>>& grid) {
        // 处理空网格的边界情况
        if (grid.empty() || grid[0].empty()) {
            return {};
        }

        int n = grid.size();        // 行数
        int m = grid[0].size();     // 列数
        int max_sum = INT_MIN; // 初始化目前找到的最大和
        int r1 = 0, c1 = 0, r2 = 0, c2 = 0; // 最大和子矩阵的坐标 {上边界, 左边界, 下边界, 右边界}

        // 遍历所有可能的顶行
        for (int up = 0; up < n; ++up) {
            // 创建一个临时一维数组，用于存储从“up”行到“down”行的列和
            std::vector<int> nums(m, 0);

            // 遍历所有从“up”行开始的可能的底行
            for (int down = up; down < n; ++down) {
                // 应用卡丹算法，在“nums”中找到最大子数组和
                // 该和表示“up”行和“down”行之间的压缩矩阵。

                int current_start_col = 0; // 当前考虑的子数组的起始列
                // “current_max_ending_here”存储以当前列“r”结尾的最大和
                int current_max_ending_here = INT_MIN;

                // 遍历列，以更新列和并应用
                for (int r = 0; r < m; ++r) {
                    // 累加当前列段（从“up”到“down”）的和
                    nums[r] += grid[down][r];

                    // 卡丹算法逻辑：
                    // 决定是扩展前一个子数组还是开始一个新的子数组
                    if (current_max_ending_here >= 0) {
                        // 扩展现有子数组的和
                        current_max_ending_here += nums[r];
                    } else {
                        // 开始一个新的子数组，因为之前的和为负
                        current_max_ending_here = nums[r];
                        current_start_col = r; // 记录这个新的潜在最大子数组的起始列
                    }

                    // 检查以当前列“r”结尾的最大和
                    // 是否大于目前找到的总体最大和。
                    if (current_max_ending_here > max_sum) {
                        max_sum = current_max_ending_here;
                        r1 = up;                // 更新顶行
                        c1 = current_start_col; // 更新左列
                        r2 = down;              // 更新底行
                        c2 = r;                 // 更新右列
                    }
                }
            }
        }

        // 返回找到的最大和子矩阵的坐标
        return {r1, c1, r2, c2};
    }
};

// 示例用法（可选，用于测试）
#include <iostream>
int main() {
    Solution sol;
    std::vector<std::vector<int>> grid1 = {{1, -2, 3}, {4, 5, -1}, {-3, 2, 6}};
    std::vector<int> result1 = sol.getMaxMatrix(grid1);
    std::cout << "Grid 1 Max Submatrix Coords: {" << result1[0] << "," << result1[1] << "," << result1[2] << "," << result1[3] << "}" << std::endl; // 预期结果: {1,0,2,2} (和为17) 或类似结果，取决于平局情况的处理

    std::vector<std::vector<int>> grid2 = {{-1, -2}, {-3, -4}};
     std::vector<int> result2 = sol.getMaxMatrix(grid2);
    std::cout << "Grid 2 Max Submatrix Coords: {" << result2[0] << "," << result2[1] << "," << result2[2] << "," << result2[3] << "}" << std::endl; // 预期结果: {0,0,0,0} (和为 -1)

     std::vector<std::vector<int>> grid3 = {{0,-2,-7,0},{9,2,-6,2},{-4,1,-4,1},{-1,8,0,-2}};
     std::vector<int> result3 = sol.getMaxMatrix(grid3);
     std::cout << "Grid 3 Max Submatrix Coords: {" << result3[0] << "," << result3[1] << "," << result3[2] << "," << result3[3] << "}" << std::endl; // 预期结果: {1,0,3,1} (和为15)

    return 0;
}