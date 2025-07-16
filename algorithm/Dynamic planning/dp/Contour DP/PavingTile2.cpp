#include <iostream>
#include <vector>
#include <cstring>

// 贴瓷砖的方法数(轮廓线dp+空间压缩)
// 给定两个参数n和m，表示n行m列的空白区域
// 有无限多的1*2规格的瓷砖，目标是严丝合缝的铺满所有的空白区域
// 返回有多少种铺满的方法
// 1 <= n, m <= 11
// 测试链接 : http://poj.org/problem?id=2411

using namespace std;

const int MAXN = 11;

// dp[j][s] 表示在当前行，从列j开始到列m-1，状态为s时的方案数
long long dp[MAXN + 1][1 << MAXN];
// prepare[s] 表示上一行处理完后传递给当前行的状态s的方案数
long long prepare[1 << MAXN];

int n, m, maxs;  // n行m列，maxs = 2^m表示状态总数

/**
 * 获取状态s在第j位的值（0或1）
 * @param s 状态值
 * @param j 位置
 * @return 第j位的值
 */
int get(int s, int j) {
    return (s >> j) & 1;
}

/**
 * 设置状态s在第j位的值为v
 * @param s 原状态
 * @param j 位置
 * @param v 要设置的值（0或1）
 * @return 新状态
 */
int set(int s, int j, int v) {
    return v == 0 ? (s & (~(1 << j))) : (s | (1 << j));
}

/**
 * 计算铺瓷砖的方案数
 * 使用轮廓线动态规划，状态压缩
 * @return 总方案数
 */
long long compute() {
    // 初始化prepare数组，表示第n行（虚拟行）的状态都是1（表示可以接受任何状态）
    for (int i = 0; i < maxs; i++) {
        prepare[i] = 1;
    }

    // 从最后一行开始向前处理
    for (int i = n - 1; i >= 0; i--) {
        // 处理第i行

        // 边界情况：j == m（已经处理完当前行的所有列）
        for (int s = 0; s < maxs; s++) {
            dp[m][s] = prepare[s];  // 直接使用上一行传递的状态
        }

        // 从右到左处理每一列
        for (int j = m - 1; j >= 0; j--) {
            for (int s = 0; s < maxs; s++) {
                long long ans = 0;

                // 如果当前位置在轮廓线上方（已经被占用）
                if (get(s, j) == 1) {
                    // 只能选择不放瓷砖，直接转移到下一列
                    ans = dp[j + 1][set(s, j, 0)];
                } else {
                    // 当前位置空闲，有两种选择：

                    // 选择1：放置竖直瓷砖（1*2，占用当前位置和下一行同一列）
                    if (i + 1 < n) {  // 确保下一行存在
                        ans = dp[j + 1][set(s, j, 1)];
                    }

                    // 选择2：放置水平瓷砖（2*1，占用当前位置和右边一列）
                    if (j + 1 < m && get(s, j + 1) == 0) {  // 确保右边存在且空闲
                        ans += dp[j + 2][s];  // 跳过两列，因为水平瓷砖占用两列
                    }
                }

                dp[j][s] = ans;
            }
        }

        // 当前行处理完毕，将结果传递给下一行（实际上是上一行，因为我们是倒序处理）
        for (int s = 0; s < maxs; s++) {
            prepare[s] = dp[0][s];
        }
    }

    // 返回从第0行第0列开始，初始状态为0的方案数
    return dp[0][0];
}

int main() {
    // 持续读入n和m，直到遇到0 0
    while (cin >> n >> m) {
        if (n == 0 && m == 0) {
            break;
        }
        maxs = 1 << m;  // 计算状态总数
        cout << compute() << endl;
    }
    return 0;
}
