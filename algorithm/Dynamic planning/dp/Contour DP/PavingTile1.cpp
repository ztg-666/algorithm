#include <iostream>
#include <vector>
#include <cstring>

// 贴瓷砖的方法数(轮廓线dp)
// 给定两个参数n和m，表示n行m列的空白区域
// 有无限多的1*2规格的瓷砖，目标是严丝合缝的铺满所有的空白区域
// 返回有多少种铺满的方法
// 1 <= n, m <= 11
// 测试链接 : http://poj.org/problem?id=2411

using namespace std;

const int MAXN = 11;                                    // 网格最大尺寸
long long dp[MAXN][MAXN][1 << MAXN];                   // dp[i][j][s] 表示第i行第j列轮廓线状态为s时的方案数
int n, m, maxs;                                         // n行m列，maxs是状态总数(2^m)

/**
 * 获取状态s中第j位的值
 * @param s 当前状态（位掩码）
 * @param j 要查询的位置
 * @return 返回第j位的值(0或1)
 */
int get(int s, int j) {
    return (s >> j) & 1;
}

/**
 * 设置状态s中第j位的值
 * @param s 当前状态
 * @param j 要设置的位置
 * @param v 要设置的值(0或1)
 * @return 返回设置后的新状态
 */
int set(int s, int j, int v) {
    return v == 0 ? (s & (~(1 << j))) : (s | (1 << j));
}

/**
 * 轮廓线动态规划主函数
 * @param i 当前处理的行号
 * @param j 当前处理的列号
 * @param s 轮廓线状态：
 *          - 对于第i行第j列，s的第k位表示：
 *            - 如果k < j：表示第i行第k列是否有竖砖向下伸到第i+1行
 *            - 如果k >= j：表示第i-1行第k列是否有竖砖向下伸到第i行
 * @return 返回从当前状态开始的铺设方案数
 */
long long f(int i, int j, int s) {
    // 边界条件：如果已经处理完所有行，说明找到了一种完整的铺设方案
    if (i == n) {
        return 1;
    }

    // 如果当前行已经处理完，转到下一行的第0列
    if (j == m) {
        return f(i + 1, 0, s);
    }

    // 记忆化搜索：如果已经计算过，直接返回结果
    if (dp[i][j][s] != -1) {
        return dp[i][j][s];
    }

    long long ans = 0;

    // 检查上方是否有竖砖伸下来
    if (get(s, j) == 1) {
        // 情况1：上方有竖砖伸下来，当前位置被占用
        // 只能跳过当前位置，继续处理下一列
        // 同时将轮廓线状态中的第j位设为0（表示这个竖砖已经处理完）
        ans = f(i, j + 1, set(s, j, 0));
    } else {
        // 情况2：上方没有竖砖，当前位置空闲，可以选择摆放方式

        // 选择1：在当前位置竖直摆放瓷砖（占用当前位置和下方位置）
        if (i + 1 < n) {  // 确保下方还有空间
            // 在轮廓线状态中标记第j位为1（表示有竖砖向下伸展）
            ans = f(i, j + 1, set(s, j, 1));
        }

        // 选择2：在当前位置水平摆放瓷砖（占用当前位置和右侧位置）
        if (j + 1 < m && get(s, j + 1) == 0) {  // 确保右侧有空间且没有被占用
            // 水平摆放瓷砖，一次占用两个位置，所以j+2
            // 轮廓线状态不变，因为水平瓷砖不会向下伸展
            ans += f(i, j + 2, s);
        }
    }

    // 保存计算结果并返回
    dp[i][j][s] = ans;
    return ans;
}

/**
 * 计算n×m网格的瓷砖铺设方案数
 * @return 返回总的铺设方案数
 */
long long compute() {
    // 初始化dp数组为-1，表示未计算过
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            for (int s = 0; s < maxs; s++) {
                dp[i][j][s] = -1;
            }
        }
    }
    // 从第0行第0列开始，轮廓线状态为0（没有任何竖砖伸下来）
    return f(0, 0, 0);
}

/**
 * 主函数：读取输入并输出结果
 */
int main() {
    // 持续读取输入直到n=0且m=0
    while (cin >> n >> m && (n != 0 || m != 0)) {
        maxs = 1 << m;          // 计算状态总数（2的m次方）
        cout << compute() << endl;  // 输出当前案例的结果
    }
    return 0;
}
