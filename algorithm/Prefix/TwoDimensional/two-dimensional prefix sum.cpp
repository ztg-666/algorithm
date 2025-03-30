#include <vector>

class NumMatrix {
public:
    // 用于存储二维前缀和的矩阵
    std::vector<std::vector<int>> sum;

    // 构造函数，初始化前缀和矩阵
    NumMatrix(std::vector<std::vector<int>>& matrix) {
        int n = matrix.size();  // 输入矩阵的行数
        int m = matrix[0].size();  // 输入矩阵的列数
        // 初始化前缀和矩阵，大小为 (n+1) x (m+1)，初始值为 0
        sum = std::vector<std::vector<int>>(n + 1, std::vector<int>(m + 1, 0));

        // 将输入矩阵的值复制到前缀和矩阵的对应位置
        for (int a = 1, c = 0; c < n; a++, c++) {
            for (int b = 1, d = 0; d < m; b++, d++) {
                sum[a][b] = matrix[c][d];
            }
        }

        // 计算前缀和矩阵
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                // 前缀和计算公式
                sum[i][j] += sum[i][j - 1] + sum[i - 1][j] - sum[i - 1][j - 1];
            }
        }
    }

    // 计算子矩阵 (a, b) 到 (c, d) 的元素和
    int sumRegion(int a, int b, int c, int d) {
        c++;  // 调整索引以适应前缀和矩阵的偏移
        d++;  // 调整索引以适应前缀和矩阵的偏移
        // 使用前缀和矩阵计算子矩阵的和
        return sum[c][d] - sum[c][b] - sum[a][d] + sum[a][b];
    }
};
