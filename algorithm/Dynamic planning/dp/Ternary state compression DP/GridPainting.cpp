#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

// 用三种不同颜色为网格涂色
// 给你两个整数m和n，表示m*n的网格，其中每个单元格最开始是白色
// 请你用红、绿、蓝三种颜色为每个单元格涂色，所有单元格都需要被涂色
// 要求相邻单元格的颜色一定要不同
// 返回网格涂色的方法数，答案对 1000000007 取模
// 1 <= m <= 5
// 1 <= n <= 1000
// 测试链接 : https://leetcode.cn/problems/painting-a-grid-with-three-different-colors/

using namespace std;

class GridPainting {
private:
    static const int MAXN = 1001;     // 最大行数
    static const int MAXM = 5;        // 最大列数
    static const int MAXS = 243;      // 最大状态数 (3^5 = 243)
    static const int MOD = 1000000007; // 取模数

    int n, m, maxs;                    // n: 较大维度, m: 较小维度, maxs: 最大状态数
    vector<vector<vector<int>>> dp;    // 动态规划数组 dp[行][列][状态]
    vector<int> first;                 // 存储第一行所有有效的涂色状态
    int size;                          // 有效状态的数量

public:
    /**
     * 主函数：计算网格涂色的方法数
     * @param rows 网格行数
     * @param cols 网格列数
     * @return 涂色方法数（对MOD取模）
     */
    int colorTheGrid(int rows, int cols) {
        build(rows, cols);             // 初始化并生成第一行的有效状态
        int ans = 0;
        // 遍历所有第一行的有效状态，累加每种状态的方法数
        for (int i = 0; i < size; i++) {
            ans = (ans + f(1, 0, first[i], 1)) % MOD;
        }
        return ans;
    }

private:
    /**
     * 初始化函数：设置参数并生成第一行的有效状态
     * @param rows 网格行数
     * @param cols 网格列数
     */
    void build(int rows, int cols) {
        // 为了优化，让较大的维度作为行数，较小的维度作为列数
        n = max(rows, cols);           // 行数（较大维度）
        m = min(rows, cols);           // 列数（较小维度）
        maxs = (int)pow(3, m);         // 状态总数 = 3^列数

        // 初始化三维DP数组，-1表示未计算
        dp = vector<vector<vector<int>>>(n, vector<vector<int>>(m, vector<int>(maxs, -1)));

        // 初始化存储第一行有效状态的数组
        first = vector<int>(maxs);
        size = 0;

        // 通过DFS生成第一行所有有效的涂色状态
        dfs(0, 0, 1);
    }

    /**
     * DFS生成第一行的有效状态
     * 使用三进制表示颜色状态：0=红色，1=绿色，2=蓝色
     * @param j 当前处理的列
     * @param s 当前状态（三进制数）
     * @param bit 当前位的权重（3的幂）
     */
    void dfs(int j, int s, int bit) {
        if (j == m) {
            // 已经处理完所有列，保存这个有效状态
            first[size++] = s;
        } else {
            // 获取左边单元格的颜色（如果是第一列则为-1）
            int left = (j == 0) ? -1 : get(s, bit / 3);

            // 尝试三种颜色，但不能与左边相同
            if (left != 0) {  // 红色（0）
                dfs(j + 1, set(s, bit, 0), bit * 3);
            }
            if (left != 1) {  // 绿色（1）
                dfs(j + 1, set(s, bit, 1), bit * 3);
            }
            if (left != 2) {  // 蓝色（2）
                dfs(j + 1, set(s, bit, 2), bit * 3);
            }
        }
    }

    /**
     * 动态规划函数：计算从当前位置开始的涂色方法数
     * @param i 当前行
     * @param j 当前列
     * @param s 上一行的状态
     * @param bit 当前位的权重
     * @return 方法数
     */
    int f(int i, int j, int s, int bit) {
        // 边界条件：已经处理完所有行
        if (i == n) {
            return 1;
        }

        // 当前行处理完毕，进入下一行
        if (j == m) {
            return f(i + 1, 0, s, 1);
        }

        // 记忆化：如果已经计算过，直接返回
        if (dp[i][j][s] != -1) {
            return dp[i][j][s];
        }

        // 获取约束条件
        int up = get(s, bit);                    // 上方单元格的颜色
        int left = (j == 0) ? -1 : get(s, bit / 3); // 左边单元格的颜色
        int ans = 0;

        // 尝试三种颜色，但不能与上方和左边相同
        if (up != 0 && left != 0) {  // 红色（0）
            ans = (ans + f(i, j + 1, set(s, bit, 0), bit * 3)) % MOD;
        }
        if (up != 1 && left != 1) {  // 绿色（1）
            ans = (ans + f(i, j + 1, set(s, bit, 1), bit * 3)) % MOD;
        }
        if (up != 2 && left != 2) {  // 蓝色（2）
            ans = (ans + f(i, j + 1, set(s, bit, 2), bit * 3)) % MOD;
        }

        // 记忆化存储结果
        dp[i][j][s] = ans;
        return ans;
    }

    /**
     * 获取状态s在指定位置的颜色值
     * @param s 状态（三进制数）
     * @param bit 位权重
     * @return 该位置的颜色值（0、1、2）
     */
    int get(int s, int bit) {
        return s / bit % 3;
    }

    /**
     * 设置状态s在指定位置的颜色值
     * @param s 原状态
     * @param bit 位权重
     * @param v 新的颜色值
     * @return 更新后的状态
     */
    int set(int s, int bit, int v) {
        return s - get(s, bit) * bit + v * bit;
    }
};

// 使用示例
int main() {
    GridPainting solution;

    // 测试用例
    cout << "1x1 : " << solution.colorTheGrid(1, 1) << endl;  // 应该输出 3
    cout << "1x2 : " << solution.colorTheGrid(1, 2) << endl;  // 应该输出 6
    cout << "2x2 : " << solution.colorTheGrid(2, 2) << endl;  // 应该输出 6
    
    return 0;
}
