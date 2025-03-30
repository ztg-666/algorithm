#include <iostream>
#include <vector>

using namespace std;

// 边框为1的最大正方形
// 给定一个由若干 0 和 1 组成的二维网格 grid，
// 找出边界全部由 1 组成的最大正方形子网格，并返回该子网格中的元素数量。
// 如果不存在这样的正方形，则返回 0。

// 时间复杂度 O(n * m * min(n, m))，额外空间复杂度 O(1)
// 复杂度指标上绝对是最优解

// 获取二维数组中指定位置的值，如果超出边界则返回 0
int get(vector<vector<int>>& g, int i, int j) {
    return (i < 0 || j < 0) ? 0 : g[i][j];
}

// 计算二维数组中矩形区域的和
// 参数 a, b 表示左上角坐标，c, d 表示右下角坐标
int sum(vector<vector<int>>& g, int a, int b, int c, int d) {
    // 如果左上角坐标大于右下角坐标，则返回 0
    return a > c ? 0 : (g[c][d] - get(g, c, b - 1) - get(g, a - 1, d) + get(g, a - 1, b - 1));
}

// 将原始二维数组 g 转换为其前缀和数组（复用原数组）
// 参数 n 表示行数，m 表示列数
void build(int n, int m, vector<vector<int>>& g) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            // 当前位置的前缀和等于当前值加上上方和左侧的前缀和，减去重复计算的部分
            g[i][j] += get(g, i, j - 1) + get(g, i - 1, j) - get(g, i - 1, j - 1);
        }
    }
}

// 主函数：寻找边框全为 1 的最大正方形
int largest1BorderedSquare(vector<vector<int>>& g) {
    int n = g.size();  // 网格的行数
    int m = g[0].size();  // 网格的列数
    build(n, m, g);  // 构建前缀和数组

    // 如果整个网格的和为 0，说明没有 1 存在，直接返回 0
    if (sum(g, 0, 0, n - 1, m - 1) == 0) {
        return 0;
    }

    // 初始化答案为 1（最小可能的正方形边长）
    int ans = 1;

    // 遍历所有可能的左上角点 (a, b)
    for (int a = 0; a < n; a++) {
        for (int b = 0; b < m; b++) {
            // 对于每个左上角点，尝试更大的边长 k
            // c, d 表示当前尝试的右下角点
            for (int c = a + ans, d = b + ans, k = ans + 1; c < n && d < m; c++, d++, k++) {
                // 检查当前正方形是否满足条件
                // 正方形的边框和等于 (k - 1) * 4
                if (sum(g, a, b, c, d) - sum(g, a + 1, b + 1, c - 1, d - 1) == (k - 1) << 2) {
                    ans = k;  // 更新最大边长
                }
            }
        }
    }

    // 返回最大正方形的面积
    return ans * ans;
}


int main() {
    // 示例网格
    vector<vector<int>> grid = {
        {1, 1, 1, 1},
        {1, 0, 1, 1},
        {1, 1, 1, 1},
        {1, 1, 1, 0}
    };

    // 调用 largest1BorderedSquare 函数并输出结果
    int result = largest1BorderedSquare(grid);
    cout << "The area of the largest 1-bordered square is: " << result << endl;

    return 0;
}
