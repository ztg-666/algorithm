#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// 炮兵阵地(空间压缩优化)
// 给定一个n * m的二维数组grid，其中的1代表可以摆放炮兵，0代表不可以摆放
// 任何炮兵攻击范围是一个"十字型"的区域，具体是上下左右两个格子的区域
// 你的目的是在gird里摆尽量多的炮兵，但要保证任何两个炮兵之间无法互相攻击
// 返回最多能摆几个炮兵
// 1 <= n <= 100
// 1 <= m <= 10
// 0 <= grid[i][j] <= 1
// 测试链接 : https://www.luogu.com.cn/problem/P2704

using namespace std;

// 常量定义
const int MAXN = 100;  // 最大行数
const int MAXM = 10;   // 最大列数
const int MAXS = 60;   // 最大状态数

// 全局变量
int grid[MAXN][MAXM];  // 存储地图，1表示可放置炮兵，0表示不可放置
int memo[MAXS][MAXS];  // 记忆化数组，memo[a][b]表示当前行状态a，前一行状态b的最大炮兵数
int dp[MAXS][MAXS];    // 临时DP数组，用于计算下一行的状态
int sta[MAXS];         // 存储所有合法的状态（位掩码）
int n, m, k;           // n行数，m列数，k状态总数

/**
 * 预处理所有合法状态
 * j: 当前处理的列
 * s: 当前状态的位掩码
 *
 * 合法状态要求：任意两个炮兵之间至少间隔2个位置（因为攻击范围是十字型，左右各2格）
 */
void prepare(int j, int s) {
    if (j >= m) {
        // 已处理完所有列，将当前状态保存
        sta[k++] = s;
    } else {
        // 当前位置不放炮兵
        prepare(j + 1, s);
        // 当前位置放炮兵（跳过下2个位置，确保不冲突）
        prepare(j + 3, s | (1 << j));
    }
}

/**
 * 计算在第i行按状态s放置炮兵时的实际炮兵数量
 * i: 行号
 * s: 状态位掩码
 * 返回: 实际能放置的炮兵数量
 */
int cnt(int i, int s) {
    int ans = 0;
    for (int j = 0; j < m; j++) {
        // 如果状态s在第j位为1，且地图上该位置可以放置炮兵
        if (((s >> j) & 1) == 1 && grid[i][j] == 1) {
            ans++;
        }
    }
    return ans;
}

/**
 * 主要的动态规划计算函数
 * 使用状态压缩DP，考虑相邻三行之间的约束关系
 */
int compute() {
    // 初始化第一行（第0行）
    for (int a = 0; a < k; a++) {
        int count = cnt(0, sta[a]);  // 计算第0行状态a的炮兵数量
        memo[a][0] = count;          // memo[a][0]表示第0行状态a，"前一行"为空状态的情况
    }

    // 处理第1行到第n-1行
    for (int i = 1; i < n; i++) {
        // 遍历当前行的所有可能状态
        for (int a = 0; a < k; a++) {
            int count = cnt(i, sta[a]);  // 计算当前行状态a的炮兵数量

            // 遍历前一行的所有可能状态
            for (int b = 0; b < k; b++) {
                dp[a][b] = 0;  // 初始化

                // 检查当前行状态a和前一行状态b是否冲突
                // 因为炮兵攻击范围包括上下2格，所以相邻行不能有冲突
                if ((sta[a] & sta[b]) == 0) {
                    // 遍历前前行的所有可能状态
                    for (int c = 0; c < k; c++) {
                        // 检查三行之间是否都不冲突
                        // sta[b] & sta[c] == 0: 前一行和前前行不冲突
                        // sta[a] & sta[c] == 0: 当前行和前前行不冲突（因为攻击范围上下2格）
                        if ((sta[b] & sta[c]) == 0 && (sta[a] & sta[c]) == 0) {
                            // 更新最大值：前前行状态c到前一行状态b的最优解 + 当前行的炮兵数
                            dp[a][b] = max(dp[a][b], memo[b][c] + count);
                        }
                    }
                }
            }
        }

        // 将dp数组的值复制到memo数组，为下一轮迭代做准备
        for (int a = 0; a < k; a++) {
            for (int b = 0; b < k; b++) {
                memo[a][b] = dp[a][b];
            }
        }
    }

    // 找到所有状态中的最大值
    int ans = 0;
    for (int a = 0; a < k; a++) {
        for (int b = 0; b < k; b++) {
            ans = max(ans, memo[a][b]);
        }
    }
    return ans;
}

int main() {
    // 读取输入
    cin >> n >> m;
    k = 0;  // 初始化状态计数器

    // 读取地图
    for (int i = 0; i < n; i++) {
        string line;
        cin >> line;
        for (int j = 0; j < m; j++) {
            // 'H'表示不能放置（山地），其他字符表示可以放置（平原）
            grid[i][j] = (line[j] == 'H') ? 0 : 1;
        }
    }

    // 预处理所有合法状态
    prepare(0, 0);

    // 计算并输出结果
    cout << compute() << endl;

    return 0;
}
