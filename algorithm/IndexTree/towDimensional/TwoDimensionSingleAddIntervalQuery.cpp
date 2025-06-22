#include <vector>
using namespace std;

// 这是一个使用二维树状数组（Fenwick Tree / Binary Indexed Tree, BIT）解决二维矩阵区域和问题的模板。
// 它支持对矩阵中单个元素值的修改，并能高效地查询任意矩形区域内元素的和。
// 主要应用场景：需要频繁更新单个点的值，并频繁查询一个矩形区域的和。
// 测试链接 : https://leetcode.cn/problems/range-sum-query-2d-mutable/

class NumMatrix {
private:
    vector<vector<int>> tree;  // 树状数组，用于存储前缀和信息。tree[i][j] 存储的是一个特定子矩形的和。
    vector<vector<int>> nums;  // 用于存储矩阵的原始数值，方便在 update 时计算增量。
    int n, m;                  // 矩阵的行数和列数。

    /**
     * @brief 计算 x 的 lowbit。
     * @param x 输入的整数。
     * @return 返回 x 的二进制表示中，最低位的1所代表的值。例如 lowbit(6) = lowbit(0110) = 2(0010)。
     * 这是树状数组的核心操作，用于在树状结构中找到父节点或子节点。
     */
    int lowbit(int x) {
        return x & -x;
    }

    /**
     * @brief 在树状数组的 (x, y) 位置增加值 v。
     * 这个操作会沿着树状数组的结构向右上方向更新所有相关的父节点。
     * @param x 坐标x（1-based）。
     * @param y 坐标y（1-based）。
     * @param v 要增加的值（增量）。
     */
    void add(int x, int y, int v) {
        // 从 (x, y) 开始，不断向父节点移动并更新
        for (int i = x; i <= n; i += lowbit(i)) {
            for (int j = y; j <= m; j += lowbit(j)) {
                tree[i][j] += v;
            }
        }
    }

    /**
     * @brief 查询从左上角 (1, 1) 到 (x, y) 的矩形区域内所有元素的和。
     * 这个操作会沿着树状数组的结构向左下方向累加所有相关的子节点的值。
     * @param x 坐标x（1-based）。
     * @param y 坐标y（1-based）。
     * @return 返回该区域的和。
     */
    int query(int x, int y) {
        int ans = 0;
        // 从 (x, y) 开始，不断向子节点移动并累加
        for (int i = x; i > 0; i -= lowbit(i)) {
            for (int j = y; j > 0; j -= lowbit(j)) {
                ans += tree[i][j];
            }
        }
        return ans;
    }

public:
    /**
     * @brief 构造函数，根据输入的初始矩阵 matrix 初始化树状数组。
     * @param matrix 初始的二维整数矩阵。
     */
    NumMatrix(vector<vector<int>>& matrix) {
        if (matrix.empty() || matrix[0].empty()) {
            n = 0;
            m = 0;
            return;
        }
        n = matrix.size();
        m = matrix[0].size();
        // 初始化树状数组和原始数值数组。大小为 (n+1)x(m+1) 是因为树状数组的下标通常从 1 开始。
        tree.assign(n + 1, vector<int>(m + 1, 0));
        nums.assign(n + 1, vector<int>(m + 1, 0));
        // 遍历初始矩阵，通过调用 update 方法来填充树状数组。
        // 这里的 update 行为等同于对一个全零矩阵进行逐个赋值，从而构建出初始状态的树状数组。
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                update(i, j, matrix[i][j]);
            }
        }
    }

    /**
     * @brief 更新矩阵中 (row, col) 位置的元素值为 val。
     * LeetCode 的接口使用的是 0-based 索引。
     * @param row 元素所在的行（0-based）。
     * @param col 元素所在的列（0-based）。
     * @param val 新的数值。
     */
    void update(int row, int col, int val) {
        // 将 0-based 坐标转换为 1-based 坐标以适应树状数组。
        int x = row + 1;
        int y = col + 1;
        // 计算新值与旧值的差（增量）。
        int delta = val - nums[x][y];
        // 将这个增量应用到树状数组上。
        add(x, y, delta);
        // 更新我们自己维护的原始数值数组。
        nums[x][y] = val;
    }

    /**
     * @brief 计算从 (row1, col1) 到 (row2, col2) 的矩形区域内所有元素的和。
     * LeetCode 的接口使用的是 0-based 索引。
     * @param row1 矩形左上角的行（0-based）。
     * @param col1 矩形左上角的列（0-based）。
     * @param row2 矩形右下角的行（0-based）。
     * @param col2 矩形右下角的列（0-based）。
     * @return 返回该矩形区域的和。
     */
    int sumRegion(int row1, int col1, int row2, int col2) {
        // 将 0-based 坐标转换为 1-based 坐标。
        int r1 = row1 + 1, c1 = col1 + 1;
        int r2 = row2 + 1, c2 = col2 + 1;

        // 使用二维前缀和的经典容斥原理来计算区域和。
        // sum(ABCD) = sum(OD) - sum(OB) - sum(OC) + sum(OA)
        // 其中 O 是原点(0,0)，A=(r1-1,c1-1), B=(r1-1,c2), C=(r2,c1-1), D=(r2,c2)
        return query(r2, c2) - query(r1 - 1, c2) - query(r2, c1 - 1) + query(r1 - 1, c1 - 1);
    }
};
