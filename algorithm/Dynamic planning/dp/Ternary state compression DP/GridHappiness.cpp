#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

// 最大化网格幸福感
// 给定四个整数m、n、in、ex，表示m*n的网格，以及in个内向的人，ex个外向的人
// 你来决定网格中应当居住多少人，并为每个人分配一个网格单元，不必让所有人都生活在网格中
// 每个人的幸福感计算如下：
// 内向的人开始时有120幸福感，但每存在一个邻居，他都会失去30幸福感
// 外向的人开始时有40幸福感，但每存在一个邻居，他都会得到20幸福感
// 邻居只包含上、下、左、右四个方向
// 网格幸福感是每个人幸福感的总和，返回最大可能的网格幸福感
// 1 <= m、n <= 5
// 1 <= in、ex <= 6
// 测试链接 : https://leetcode.cn/problems/maximize-grid-happiness/

using namespace std;

/**
 * 网格幸福感最大化问题
 *
 * 问题描述：
 * 给定一个m×n的网格，有in个内向的人和ex个外向的人
 * 需要决定在网格中放置多少人，并为每个人分配位置
 *
 * 幸福感计算规则：
 * - 内向的人：基础幸福感120，每个邻居-30
 * - 外向的人：基础幸福感40，每个邻居+20
 * - 邻居只考虑上下左右四个方向
 *
 * 目标：最大化网格总幸福感
 */
class GridHappiness {
public:
    // 常量定义
    static const int MAXN = 5;     // 最大行数
    static const int MAXM = 5;     // 最大列数
    static const int MAXP = 7;     // 最大人数
    static const int MAXS = 243;   // 3^5 = 243，轮廓线状态的最大值

    // 成员变量
    int n, m;           // 实际的行数和列数
    int maxs;           // 轮廓线状态的最大值 = 3^m

    /**
     * 五维动态规划数组
     * dp[i][j][s][a][b] 表示：
     * - 当前处理到第i行第j列
     * - 轮廓线状态为s
     * - 剩余内向人数为a
     * - 剩余外向人数为b
     * 时的最大幸福感
     */
    int dp[MAXN][MAXM][MAXS][MAXP][MAXP];

    /**
     * 主函数：计算最大网格幸福感
     * @param rows 网格行数
     * @param cols 网格列数
     * @param in 内向人数
     * @param ex 外向人数
     * @return 最大幸福感
     */
    int getMaxGridHappiness(int rows, int cols, int in, int ex) {
        // 为了简化轮廓线状态管理，让较大的维度作为行
        n = max(rows, cols);
        m = min(rows, cols);
        maxs = pow(3, m);  // 轮廓线状态数 = 3^列数

        // 初始化dp数组为-1（表示未计算）
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                for (int s = 0; s < maxs; s++) {
                    for (int a = 0; a <= in; a++) {
                        for (int b = 0; b <= ex; b++) {
                            dp[i][j][s][a][b] = -1;
                        }
                    }
                }
            }
        }

        // 从第0行第0列开始，轮廓线状态为0，人数为给定值
        return f(0, 0, 0, in, ex, 1);
    }

    /**
     * 递归函数：计算从当前位置开始的最大幸福感
     *
     * @param i 当前行
     * @param j 当前列
     * @param s 轮廓线状态（用3进制表示每个位置的人员类型）
     * @param a 剩余内向人数
     * @param b 剩余外向人数
     * @param bit 当前位置对应的3进制位权重（3^j）
     * @return 从当前位置开始的最大幸福感
     *
     * 轮廓线状态说明：
     * - 0: 空位
     * - 1: 内向的人
     * - 2: 外向的人
     */
    int f(int i, int j, int s, int a, int b, int bit) {
        // 边界条件：超出网格范围
        if (i == n) {
            return 0;  // 所有位置都处理完了
        }

        // 到达行末，转到下一行开始
        if (j == m) {
            return f(i + 1, 0, s, a, b, 1);  // bit重新从1开始（3^0）
        }

        // 记忆化搜索：如果已经计算过，直接返回
        if (dp[i][j][s][a][b] != -1) {
            return dp[i][j][s][a][b];
        }

        // 选择1：当前格子不放人
        int ans = f(i, j + 1, set(s, bit, 0), a, b, bit * 3);

        // 获取邻居状态
        int up = get(s, bit);                    // 上方邻居状态
        int left = (j == 0) ? 0 : get(s, bit / 3);  // 左方邻居状态

        int neighbor = 0;  // 邻居人数
        int pre = 0;       // 因为放置当前人员，邻居幸福感的变化

        // 检查上方邻居
        if (up != 0) {
            neighbor++;
            // 上邻居是内向的人，幸福感减30；是外向的人，幸福感加20
            pre += (up == 1) ? -30 : 20;
        }

        // 检查左方邻居
        if (left != 0) {
            neighbor++;
            // 左邻居是内向的人，幸福感减30；是外向的人，幸福感加20
            pre += (left == 1) ? -30 : 20;
        }

        // 选择2：当前格子放内向的人
        if (a > 0) {
            // 内向的人：基础幸福感120 - 邻居数×30 + 邻居幸福感变化
            int introvertHappiness = pre + 120 - neighbor * 30;
            ans = max(ans, introvertHappiness + f(i, j + 1, set(s, bit, 1), a - 1, b, bit * 3));
        }

        // 选择3：当前格子放外向的人
        if (b > 0) {
            // 外向的人：基础幸福感40 + 邻居数×20 + 邻居幸福感变化
            int extrovertHappiness = pre + 40 + neighbor * 20;
            ans = max(ans, extrovertHappiness + f(i, j + 1, set(s, bit, 2), a, b - 1, bit * 3));
        }

        // 记忆化存储结果
        dp[i][j][s][a][b] = ans;
        return ans;
    }

    /**
     * 获取轮廓线状态中第j位的值
     *
     * @param s 轮廓线状态
     * @param bit 位权重（3^j）
     * @return 第j位的值（0/1/2）
     *
     * 例如：s=19，bit=9（3^2）
     * 19 / 9 % 3 = 2 % 3 = 2
     * 表示第2位是外向的人
     */
    int get(int s, int bit) {
        return s / bit % 3;
    }

    /**
     * 设置轮廓线状态中第j位的值
     *
     * @param s 原轮廓线状态
     * @param bit 位权重（3^j）
     * @param v 新的值（0/1/2）
     * @return 更新后的轮廓线状态
     *
     * 算法：先减去原值，再加上新值
     * 例如：s=19，bit=9，v=1
     * 原第2位值：19/9%3=2
     * 新状态：19-2*9+1*9=19-18+9=10
     */
    int set(int s, int bit, int v) {
        return s - get(s, bit) * bit + v * bit;
    }
};

// 使用示例和测试
int main() {
    GridHappiness solution;

    // 测试用例1：2×3网格，1个内向人，2个外向人
    cout << "===  ===" << endl;
    int rows1 = 2, cols1 = 3, in1 = 1, ex1 = 2;
    int result1 = solution.getMaxGridHappiness(rows1, cols1, in1, ex1);
    cout << "" << rows1 << " " << cols1 << ", : " << in1 << ", : " << ex1 << endl;
    cout << ": " << result1 << endl << endl;

    // 测试用例2：3×1网格，2个内向人，1个外向人
    cout << "===  ===" << endl;
    int rows2 = 3, cols2 = 1, in2 = 2, ex2 = 1;
    int result2 = solution.getMaxGridHappiness(rows2, cols2, in2, ex2);
    cout << "" << rows2 << " " << cols2 << ", : " << in2 << ", : " << ex2 << endl;
    cout << ": " << result2 << endl << endl;

    // 测试用例3：1×1网格，1个内向人，1个外向人
    cout << "===  ===" << endl;
    int rows3 = 1, cols3 = 1, in3 = 1, ex3 = 1;
    int result3 = solution.getMaxGridHappiness(rows3, cols3, in3, ex3);
    cout << ": " << rows3 << " " << cols3 << ", : " << in3 << ", : " << ex3 << endl;
    cout << ": " << result3 << endl;
    
    return 0;
}
