#include <iostream>
#include <vector>

using namespace std;

class NumMatrix {
public:
    vector<vector<int>> tree; // 树状数组，用于高效更新和查询
    vector<vector<int>> nums; // 原始数组，用于存储实际的数值
    int n, m; // 数组的行数和列数

    // 构造函数，初始化树状数组和原始数组
    NumMatrix(vector<vector<int>>& matrix) {
        n = matrix.size();
        m = matrix[0].size();
        tree = vector<vector<int>>(n + 1, vector<int>(m + 1, 0)); // 初始化树状数组，大小为 (n+1) x (m+1)
        nums = vector<vector<int>>(n + 1, vector<int>(m + 1, 0)); // 初始化原始数组，大小为 (n+1) x (m+1)
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                update(i, j, matrix[i][j]); // 初始化树状数组，将矩阵中的值加入树状数组
            }
        }
    }

private:
    // 计算最低有效位
    int lowbit(int i) {
        return i & -i; // 返回 i 的最低有效位
    }

    // 在树状数组中增加值
    void add(int x, int y, int v) {
        for (int i = x + 1; i <= n; i += lowbit(i)) { // 从 x+1 开始，更新树状数组的行
            for (int j = y + 1; j <= m; j += lowbit(j)) { // 从 y+1 开始，更新树状数组的列
                tree[i][j] += v; // 更新树状数组中的值
            }
        }
    }

    // 从(1,1)到(x,y)这个部分的累加和
    int sum(int x, int y) {
        int ans = 0;
        for (int i = x + 1; i > 0; i -= lowbit(i)) { // 从 x+1 开始，累加树状数组的行
            for (int j = y + 1; j > 0; j -= lowbit(j)) { // 从 y+1 开始，累加树状数组的列
                ans += tree[i][j]; // 累加树状数组中的值
            }
        }
        return ans;
    }

public:
    // 实际二维数组的位置是(x,y)
    // 树状数组上的位置是(x+1, y+1)
    // 题目说的是单点更新，转化成单点增加(老值-新值)即可
    // 不要忘了在nums中把老值改成新值
    void update(int x, int y, int v) {
        add(x, y, v - nums[x + 1][y + 1]); // 更新树状数组，增加差值 (新值 - 老值)
        nums[x + 1][y + 1] = v; // 更新原始数组中的值
    }

    // 实际二维数组的位置是(x,y)
    // 树状数组上的位置是(x+1, y+1)
    int sumRegion(int a, int b, int c, int d) {
        // 计算从 (a,b) 到 (c,d) 的累加和
        return sum(c, d) - sum(a - 1, d) - sum(c, b - 1) + sum(a - 1, b - 1);
    }
};

int main() {
    vector<vector<int>> matrix = {
        {3, 0, 1, 4, 2},
        {5, 6, 3, 2, 1},
        {1, 2, 0, 1, 5},
        {4, 1, 0, 1, 7},
        {1, 0, 3, 0, 5}
    };

    NumMatrix numMatrix(matrix);

    cout << numMatrix.sumRegion(2, 1, 4, 3) << endl; // 输出 8
    numMatrix.update(3, 2, 2);
    cout << numMatrix.sumRegion(2, 1, 4, 3) << endl; // 输出 10

    return 0;
}
