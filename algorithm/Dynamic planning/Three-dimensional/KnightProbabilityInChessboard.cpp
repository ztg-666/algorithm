#include <vector>
#include <iostream>

// 骑士在棋盘上的概率
// n * n的国际象棋棋盘上，一个骑士从单元格(row, col)开始，并尝试进行 k 次移动
// 行和列从0开始，所以左上单元格是 (0,0)，右下单元格是 (n-1, n-1)
// 象棋骑士有8种可能的走法。每次移动在基本方向上是两个单元格，然后在正交方向上是一个单元格
// 每次骑士要移动时，它都会随机从8种可能的移动中选择一种，然后移动到那里
// 骑士继续移动，直到它走了 k 步或离开了棋盘
// 返回 骑士在棋盘停止移动后仍留在棋盘上的概率
// 测试链接 : https://leetcode.cn/problems/knight-probability-in-chessboard/

class Solution {
private:
    // 动态规划表（记忆化）的成员变量
    std::vector<std::vector<std::vector<double>>> dp;
    int board_size; // 存储棋盘维度 n

    // 骑士的移动偏移量：8 种可能的移动
    const int dr[8] = {-2, -1, 1, 2, 2, 1, -1, -2};
    const int dc[8] = {1, 2, 2, 1, -1, -2, -2, -1};

    // 带记忆化的递归函数
    // 计算从 (r, c) 出发，剩余 k 步时仍留在棋盘上的概率。
    double solve(int r, int c, int k) {
        // 基本情况 1：骑士移出棋盘
        if (r < 0 || r >= board_size || c < 0 || c >= board_size) {
            return 0.0; // 如果已经移出棋盘，留在棋盘上的概率为 0
        }

        // 基本情况 2：骑士已经完成了所有 k 步移动
        if (k == 0) {
            // 如果在剩余 0 步时仍在棋盘上，概率为 1
            return 1.0;
        }

        // 记忆化检查：如果已经计算过，返回存储的结果
        // 使用 -1.0 作为未计算的标记
        if (dp[r][c][k] != -1.0) {
            return dp[r][c][k];
        }

        // 递归步骤：通过将所有可能的下一步移动的概率相加来计算概率
        double probability = 0.0;
        for (int i = 0; i < 8; ++i) {
            // 递归调用下一个位置 (r + dr[i], c + dc[i])，剩余 k - 1 步
            probability += solve(r + dr[i], c + dc[i], k - 1);
        }

        // 每一步移动的概率是 1/8，所以将下一步的总概率除以 8
        probability /= 8.0;

        // 在返回之前将计算结果存储在动态规划表中
        dp[r][c][k] = probability;
        return probability;
    }

public:
    // 启动计算的主函数
    double knightProbability(int n, int k, int row, int col) {
        board_size = n; // 存储棋盘大小

        // 用 -1.0 初始化动态规划表（未计算的状态）
        // 维度：n x n x (k + 1)
        dp.assign(n, std::vector<std::vector<double>>(n, std::vector<double>(k + 1, -1.0)));

        // 从初始位置 (row, col) 开始，剩余 k 步，启动递归计算
        return solve(row, col, k);
    }
};

// 示例用法（可选）
int main() {
    Solution sol;
    int n1 = 3, k1 = 2, row1 = 0, col1 = 0;
    std::cout << "Probability 1: " << sol.knightProbability(n1, k1, row1, col1) << std::endl; // 输出: 0.0625

    int n2 = 1, k2 = 0, row2 = 0, col2 = 0;
    std::cout << "Probability 2: " << sol.knightProbability(n2, k2, row2, col2) << std::endl; // 输出: 1.0

    int n3 = 8, k3 = 30, row3 = 6, col3 = 4;
    // 由于 k 值较大，这可能需要一些时间来计算
    std::cout << "Probability 3: " << sol.knightProbability(n3, k3, row3, col3) << std::endl; // 输出: 某个概率值

    return 0;
}
