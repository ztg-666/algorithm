#include<bits/stdc++.h>
using namespace std;
// 构建前缀和数组
void build(vector<vector<int>>& m) {
    for (int i = 1; i < m.size(); i++) {
        for (int j = 1; j < m[0].size(); j++) {
            m[i][j] += m[i - 1][j] + m[i][j - 1] - m[i - 1][j - 1];
        }
    }
}
// 计算二维数组中矩形区域的和
int sumRegion(vector<vector<int>>& sum, int a, int b, int c, int d) {
    return sum[c][d] - sum[c][b - 1] - sum[a - 1][d] + sum[a - 1][b - 1];
}
// 在指定范围内增加值
void add(vector<vector<int>>& diff, int a, int b, int c, int d) {
    diff[a][b] += 1;
    diff[c + 1][d + 1] += 1;
    diff[c + 1][b] -= 1;
    diff[a][d + 1] -= 1;
}

// 时间复杂度 O(n*m)，额外空间复杂度 O(n*m)
bool possibleToStamp(vector<vector<int>>& grid, int h, int w) {
    int n = grid.size();
    int m = grid[0].size();

    // sum 是前缀和数组，用于快速查询原始矩阵中的某个范围的累加和
    vector<vector<int>> sum(n + 1, vector<int>(m + 1, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            sum[i + 1][j + 1] = grid[i][j];
        }
    }
    build(sum);

    // diff 是差分矩阵，用于记录邮票的放置情况
    vector<vector<int>> diff(n + 2, vector<int>(m + 2, 0));
    for (int a = 1, c = a + h - 1; c <= n; a++, c++) {
        for (int b = 1, d = b + w - 1; d <= m; b++, d++) {
            // 原始矩阵中 (a, b) 左上角点
            // 根据邮票规格，h、w，算出右下角点 (c, d)
            // 这个区域彻底都是 0，那么 sumRegion(sum, a, b, c, d) == 0
            // 那么此时这个区域可以贴邮票
            if (sumRegion(sum, a, b, c, d) == 0) {
                add(diff, a, b, c, d);
            }
        }
    }
    build(diff);

    // 检查所有的格子
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            // 原始矩阵里：grid[i][j] == 0，说明是个洞
            // 差分矩阵里：diff[i + 1][j + 1] == 0，说明洞上并没有邮票
            // 此时返回 false
            if (grid[i][j] == 0 && diff[i + 1][j + 1] == 0) {
                return false;
            }
        }
    }
    return true;
}



int main() {
    // 示例网格
    vector<vector<int>> grid = {
        {1, 0, 0, 0},
        {1, 0, 0, 0},
        {1, 0, 1, 0},
        {1, 0, 0, 0}
    };

    // 邮票尺寸
    int stampHeight = 3;
    int stampWidth = 2;

    // 检查是否可以贴邮票
    bool result = possibleToStamp(grid, stampHeight, stampWidth);

    // 输出结果
    if (result) {
        cout << "1" << endl;
    } else {
        cout << "0" << endl;
    }

    return 0;
}