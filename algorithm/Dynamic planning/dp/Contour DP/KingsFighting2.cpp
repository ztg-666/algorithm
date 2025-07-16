#include <iostream>
#include <vector>
#include <cstring>

// 摆放国王的方法数(轮廓线dp+空间压缩)
// 给定两个参数n和k，表示n*n的区域内要摆放k个国王
// 国王可以攻击临近的8个方向，所以摆放时不能让任何两个国王打架
// 返回摆放的方法数
// 1 <= n <= 9
// 1 <= k <= n*n
// 测试链接 : https://www.luogu.com.cn/problem/P1896

using namespace std;

const int MAXN = 9;      // 棋盘最大大小
const int MAXK = 82;     // 最大国王数量 (9*9=81，实际使用82)

// dp[列][上一行状态][左上角状态][已放置国王数量] = 方案数
long long dp[MAXN + 1][1 << MAXN][2][MAXK];
// prepare[状态][国王数量] = 临时存储当前行的方案数
long long prepare[1 << MAXN][MAXK];

int n, kings, maxs;      // n: 棋盘大小, kings: 国王数量, maxs: 状态总数(2^n)

// 获取状态s中第j位的值（0或1）
int getBit(int s, int j) {
    return (s >> j) & 1;
}

// 设置状态s中第j位的值为v（0或1）
int setBit(int s, int j, int v) {
    return v == 0 ? (s & (~(1 << j))) : (s | (1 << j));
}

long long compute() {
    // 初始化prepare数组：只有k=0时方案数为1，其他都是0
    // 这表示初始状态（没有任何国王）只有一种方案
    for (int s = 0; s < maxs; s++) {
        for (int k = 0; k <= kings; k++) {
            prepare[s][k] = (k == 0) ? 1 : 0;
        }
    }

    // 轮廓线DP：从最后一行开始向前处理
    // 每处理完一行，就更新prepare数组作为下一行的初始状态
    for (int i = n - 1; i >= 0; i--) {

        // 边界情况：j == n（超出棋盘右边界）
        // 此时直接使用prepare数组的值
        for (int s = 0; s < maxs; s++) {
            for (int k = 0; k <= kings; k++) {
                dp[n][s][0][k] = prepare[s][k];
                dp[n][s][1][k] = prepare[s][k];
            }
        }

        // 从右到左处理当前行的每一列
        for (int j = n - 1; j >= 0; j--) {
            // 遍历所有可能的上一行状态
            for (int s = 0; s < maxs; s++) {
                // 遍历左上角位置的状态（0或1）
                for (int leftup = 0; leftup <= 1; leftup++) {
                    // 遍历已放置的国王数量
                    for (int k = 0; k <= kings; k++) {
                        // 获取相邻位置的状态
                        int left = (j == 0) ? 0 : getBit(s, j - 1);    // 左边位置
                        int up = getBit(s, j);                          // 上边位置
                        int rightup = getBit(s, j + 1);                 // 右上位置

                        // 方案1：当前位置不放国王
                        long long ans = dp[j + 1][setBit(s, j, 0)][up][k];

                        // 方案2：当前位置放国王
                        // 条件：还有国王可放 && 左、左上、上、右上四个位置都没有国王
                        if (k > 0 && left == 0 && leftup == 0 && up == 0 && rightup == 0) {
                            ans += dp[j + 1][setBit(s, j, 1)][up][k - 1];
                        }

                        dp[j][s][leftup][k] = ans;
                    }
                }
            }
        }

        // 当前行处理完毕，更新prepare数组作为下一行的初始状态
        for (int s = 0; s < maxs; s++) {
            for (int k = 0; k <= kings; k++) {
                prepare[s][k] = dp[0][s][0][k];
            }
        }
    }

    // 返回最终答案：处理完所有行，上一行状态为0，左上角为0，放置了kings个国王的方案数
    return dp[0][0][0][kings];
}

int main() {
    // 加速输入输出
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 读取输入
    cin >> n >> kings;
    maxs = 1 << n;  // 计算状态总数：2^n

    // 输出结果
    cout << compute() << endl;
    
    return 0;
}
