#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;
// 最大矩形
// 给定一个仅包含 0 和 1 、大小为 rows * cols 的二维二进制矩阵
// 找出只包含 1 的最大矩形，并返回其面积
class Solution {
public:
    // 定义最大行/列数为201
    static const int MAXN = 201;

    // 高度数组，用于记录当前列的高度
    int height[MAXN];
    // 单调栈，用于计算最大矩形面积
    int stack[MAXN];
    // 栈顶指针
    int r;

    // 主函数：计算只包含'1'的最大矩形面积
    int maximalRectangle(vector<vector<char>>& grid) {
        // 如果输入矩阵为空，直接返回0
        if (grid.empty() || grid[0].empty()) return 0;

        int n = grid.size();    // 矩阵的行数
        int m = grid[0].size(); // 矩阵的列数

        // 初始化高度数组为0
        fill(height, height + m, 0);
        int ans = 0; // 最终结果

        // 遍历每一行，更新高度数组并计算当前行的最大矩形面积
        for (int i = 0; i < n; i++) {
            // 更新高度数组：
            // 如果当前格子是'0'，则高度为0；否则在前一行的基础上加1
            for (int j = 0; j < m; j++) {
                height[j] = (grid[i][j] == '0') ? 0 : (height[j] + 1);
            }

            // 调用辅助函数计算当前行的最大矩形面积，并更新答案
            ans = max(ans, largestRectangleArea(m));
        }

        return ans; // 返回最大矩形面积
    }

private:
    // 辅助函数：计算基于当前高度数组的最大矩形面积
    int largestRectangleArea(int m) {
        r = 0; // 初始化栈顶指针
        int ans = 0; // 当前最大面积
        int cur, left; // 当前柱子索引、左侧边界索引

        // 遍历每一列，使用单调栈计算最大矩形面积
        for (int i = 0; i < m; i++) {
            // 维护一个递增的单调栈，当遇到比栈顶小的高度时，弹出栈顶并计算面积
            while (r > 0 && height[stack[r - 1]] >= height[i]) {
                cur = stack[--r]; // 弹出栈顶元素
                // 左侧边界为栈中前一个元素的位置，如果栈为空则为-1
                left = (r == 0) ? -1 : stack[r - 1];
                // 计算当前柱子对应的最大矩形面积
                ans = max(ans, height[cur] * (i - left - 1));
            }
            // 将当前列索引压入栈
            stack[r++] = i;
        }

        // 处理栈中剩余的元素
        while (r > 0) {
            cur = stack[--r]; // 弹出栈顶元素
            // 左侧边界为栈中前一个元素的位置，如果栈为空则为-1
            left = (r == 0) ? -1 : stack[r - 1];
            // 计算当前柱子对应的最大矩形面积（右侧边界为m）
            ans = max(ans, height[cur] * (m - left - 1));
        }

        return ans; // 返回当前行的最大矩形面积
    }
};

int main() {
    // 示例矩阵
    vector<vector<char>> grid = {
        {'1', '0', '1', '0', '0'},
        {'1', '0', '1', '1', '1'},
        {'1', '1', '1', '1', '1'},
        {'1', '0', '0', '1', '0'}
    };

    Solution solution;
    int result = solution.maximalRectangle(grid);
    cout << "最大矩形面积为: " << result << endl;

    return 0;
}
