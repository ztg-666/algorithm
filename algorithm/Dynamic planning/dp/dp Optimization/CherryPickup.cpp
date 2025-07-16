#include <vector>
#include <algorithm>

// 摘樱桃
// 给定一个n*n的正方形矩阵grid，每个格子值只有三种-1、0、1
// -1表示格子不能走、0表示格子可以走但是没有樱桃、1表示格子可以走且有一颗樱桃
// 你的目标是从左上角走到右下角，每一步只能 向下 或者 向右
// 然后从右下角走回左上角，每一步只能 向上 或者 向左
// 这个过程中，想尽量多的获得樱桃，但是有樱桃的格子，只能捡一次
// 返回最多能获得多少樱桃，如果不存在通路返回0
// 测试链接 : https://leetcode.cn/problems/cherry-pickup/

using namespace std;

class Solution {
public:
    /**
     * 摘樱桃问题的主函数
     * @param grid n*n的正方形矩阵，-1表示不能走，0表示可以走但没有樱桃，1表示有樱桃
     * @return 返回最多能获得的樱桃数量，如果不存在通路返回0
     */
    int cherryPickup(vector<vector<int>>& grid) {
        int n = grid.size();

        // 创建三维dp数组进行记忆化搜索
        // dp[a][b][c] 表示第一个人在位置(a,b)，第二个人在位置(c,d)时能获得的最大樱桃数
        // 其中 d = a + b - c（因为两个人走的步数相同）
        // 初始化为-2表示该状态未计算过
        vector<vector<vector<int>>> dp(n, vector<vector<int>>(n, vector<int>(n, -2)));

        // 开始递归搜索，从(0,0)开始
        int ans = f(grid, n, 0, 0, 0, dp);

        // 如果返回-1表示不存在通路，返回0；否则返回实际的樱桃数量
        return ans == -1 ? 0 : ans;
    }

private:
    /**
     * 递归函数，计算从当前状态到终点能获得的最大樱桃数
     * @param grid 原始网格
     * @param n 网格大小
     * @param a 第一个人的行坐标
     * @param b 第一个人的列坐标
     * @param c 第二个人的行坐标
     * @param dp 记忆化数组
     * @return 从当前状态到终点能获得的最大樱桃数，-1表示无法到达终点
     */
    int f(vector<vector<int>>& grid, int n, int a, int b, int c, vector<vector<vector<int>>>& dp) {
        // 根据步数相等的约束计算第二个人的列坐标
        // 因为两个人同时出发，走的步数相同：a + b = c + d
        int d = a + b - c;

        // 边界检查：
        // 1. 检查是否越界（任何坐标超出网格范围）
        // 2. 检查是否遇到障碍物（grid值为-1的位置）
        if (a == n || b == n || c == n || d == n ||
            grid[a][b] == -1 || grid[c][d] == -1) {
            return -1;  // 无法继续前进，返回-1
        }

        // 递归终止条件：两个人都到达终点(n-1, n-1)
        if (a == n - 1 && b == n - 1) {
            return grid[a][b];  // 返回终点的樱桃数（0或1）
        }

        // 记忆化：如果该状态已经计算过，直接返回结果
        if (dp[a][b][c] != -2) {
            return dp[a][b][c];
        }

        // 计算当前位置能获得的樱桃数
        // 关键点：如果两个人在同一位置，樱桃只能取一次
        int get;
        if (a == c && b == d) {
            // 两个人在同一位置，只能取一次樱桃
            get = grid[a][b];
        } else {
            // 两个人在不同位置，可以分别取各自位置的樱桃
            get = grid[a][b] + grid[c][d];
        }

        // 尝试所有可能的移动方向组合（每个人只能向下或向右移动）
        int next = -1;

        // 情况1：第一个人向下移动，第二个人向下移动
        next = f(grid, n, a + 1, b, c + 1, dp);

        // 情况2：第一个人向下移动，第二个人向右移动
        next = max(next, f(grid, n, a + 1, b, c, dp));

        // 情况3：第一个人向右移动，第二个人向下移动
        next = max(next, f(grid, n, a, b + 1, c + 1, dp));

        // 情况4：第一个人向右移动，第二个人向右移动
        next = max(next, f(grid, n, a, b + 1, c, dp));

        // 计算最终答案
        int ans = -1;
        if (next != -1) {
            // 如果存在可行的后续路径，当前获得的樱桃数加上后续路径的樱桃数
            ans = get + next;
        }
        // 如果next为-1，说明从当前状态无法到达终点，ans保持为-1

        // 记忆化：将计算结果存储到dp数组中
        dp[a][b][c] = ans;
        return ans;
    }
};

